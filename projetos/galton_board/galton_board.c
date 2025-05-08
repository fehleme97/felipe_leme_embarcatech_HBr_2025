#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"

// Definições de hardware e configurações dos pinos e tela
#define I2C_SDA 14
#define I2C_SCL 15
#define BUTTON_A 5
#define BUTTON_B 6

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FRAME_SIZE 1024

// Tamanhos dos elementos gráficos
#define BALL_RADIUS 2
#define OBSTACLE_RADIUS 2

// Configurações do simulador de bolinhas e canaletas
#define MAX_BALLS 100
#define NUM_CANALS 8
#define CANAL_WIDTH (SCREEN_HEIGHT / NUM_CANALS)

int canaletas[NUM_CANALS] = {0};
int total_balls = 0;
bool pending_add_50 = false;

// Gerador de número pseudoaleatório com semente única por bolinha
int pseudo_rand(uint32_t *seed) {
    *seed = (*seed) * 1103515245 + 12345;
    return (*seed >> 16) & 0x7FFF;
}

// Estrutura que representa uma bolinha
typedef struct {
    int x, y;
    bool active;
    uint32_t seed;  // Semente para aleatoriedade individual
} Ball;

// Coordenadas fixas dos obstáculos da Galton Board
int obstacle_centers[][2] = {
    {77, 20}, {57, 24}, {37, 29}, {77, 29}, {16, 34},
    {57, 34}, {37, 38}, {78, 38}, {58, 43}, {78, 48}
};
int num_obstacles = sizeof(obstacle_centers) / sizeof(obstacle_centers[0]);

// Desenha um círculo preenchido em uma posição do buffer da tela OLED
void draw_filled_circle(uint8_t *buffer, int cx, int cy, int r) {
    for (int y_off = -r; y_off <= r; y_off++) {
        for (int x_off = -r; x_off <= r; x_off++) {
            if ((x_off * x_off + y_off * y_off) <= r * r) {
                int px = cx + x_off;
                int py = cy + y_off;
                if (px < 0 || px >= SCREEN_WIDTH || py < 0 || py >= SCREEN_HEIGHT) continue;
                int page = py / 8;
                int bit = py % 8;
                int idx = page * SCREEN_WIDTH + px;
                buffer[idx] &= ~(1 << bit);
            }
        }
    }
}

// Desenha todos os obstáculos fixos no display
void draw_obstacles(uint8_t *frame) {
    for (int i = 0; i < num_obstacles; i++) {
        int x = obstacle_centers[i][0];
        int y = obstacle_centers[i][1];
        draw_filled_circle(frame, x, y, OBSTACLE_RADIUS);
    }
}

// Verifica se uma posição (x,y) está ocupada por um obstáculo no buffer
bool is_obstacle(uint8_t *frame, int x, int y) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return true;
    int page = y / 8;
    int bit = y % 8;
    int idx = page * SCREEN_WIDTH + x;
    return !(frame[idx] & (1 << bit));
}

Ball balls[MAX_BALLS];  // Vetor que armazena todas as bolinhas possíveis

// Atualiza as bolinhas em movimento e registra colisões com obstáculos
void update_balls(uint8_t *frame) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) continue;

        bool over_obstacle = false;

        for (int j = 0; j < num_obstacles; j++) {
            int dx = balls[i].x - obstacle_centers[j][0];
            int dy = balls[i].y - obstacle_centers[j][1];
            if (dx * dx + dy * dy <= (OBSTACLE_RADIUS + BALL_RADIUS) * (OBSTACLE_RADIUS + BALL_RADIUS)) {
                over_obstacle = true;
                break;
            }
        }

        // Desvio com peso de probabilidade ao colidir com obstáculo
        if (over_obstacle) {
            int r = pseudo_rand(&balls[i].seed) % 100;
            int offset;
            if (r < 10) offset = -3;
            else if (r < 35) offset = -2;
            else if (r < 50) offset = -1;
            else if (r < 65) offset = 1;
            else if (r < 90) offset = 2;
            else offset = 3;

            balls[i].y += offset;
            if (balls[i].y < BALL_RADIUS) balls[i].y = BALL_RADIUS;
            if (balls[i].y > SCREEN_HEIGHT - BALL_RADIUS) balls[i].y = SCREEN_HEIGHT - BALL_RADIUS;
        }

        balls[i].x++;

        // Verifica se a bolinha chegou ao fim e registra na canaleta
        if (balls[i].x >= SCREEN_WIDTH - BALL_RADIUS) {
            int canal_idx = balls[i].y / CANAL_WIDTH;
            if (canal_idx >= 0 && canal_idx < NUM_CANALS) {
                canaletas[canal_idx]++;
            }
            balls[i].active = false;
        } else {
            draw_filled_circle(frame, balls[i].x, balls[i].y, BALL_RADIUS);
        }
    }
}

// Desenha as canaletas com bolinhas empilhadas
void draw_canaletas(uint8_t *frame) {
    for (int c = 0; c < NUM_CANALS; c++) {
        int base_y = c * CANAL_WIDTH + CANAL_WIDTH / 2;
        for (int h = 0; h < canaletas[c] && h < 6; h++) {
            draw_filled_circle(frame, SCREEN_WIDTH - 3 - h * 6, base_y, 1);
        }
    }
}

// Mostra histograma de resultados e o total de bolinhas lançadas
void draw_histogram(uint8_t *frame) {
    memset(frame, 0xFF, FRAME_SIZE);
    for (int i = 0; i < NUM_CANALS; i++) {
        int height = canaletas[i];
        if (height > SCREEN_WIDTH - 10) height = SCREEN_WIDTH - 10;
        for (int w = 0; w < height; w++) {
            for (int dy = 0; dy < CANAL_WIDTH - 2; dy++) {
                int x = 10 + w;
                int y = i * CANAL_WIDTH + 1 + dy;
                int page = y / 8;
                int bit = y % 8;
                frame[page * SCREEN_WIDTH + x] &= ~(1 << bit);
            }
        }
    }
    // Escreve contador de bolas no canto superior direito
    char buf[20];
    snprintf(buf, sizeof(buf), "Total: %d", total_balls);
    int len = strlen(buf);
    int x = SCREEN_WIDTH - 6 * len - 2;
    for (int i = 0; i < len; i++) {
        ssd1306_draw_char(frame, x + i * 6, 0, buf[i]);
    }
}

// Cria uma única bolinha se houver espaço no vetor
bool spawn_ball() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!balls[i].active) {
            balls[i].x = 0;
            balls[i].y = SCREEN_HEIGHT / 2;
            balls[i].seed = to_us_since_boot(get_absolute_time()) + i * 17;
            balls[i].active = true;
            return true;
        }
    }
    return false;
}

// Cria múltiplas bolinhas e marca para atualizar o total posteriormente
void spawn_multiple_balls(int count) {
    for (int c = 0; c < count; c++) {
        spawn_ball();
    }
    pending_add_50 = true;
}

// Loop principal da simulação e controle
int main() {
    stdio_init_all();
    i2c_init(i2c1, 1000000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Configuração dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    ssd1306_init();

     // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = SCREEN_WIDTH - 1,
        .start_page = 0,
        .end_page = (SCREEN_HEIGHT / 8) - 1
    };
    calculate_render_area_buffer_length(&frame_area);

    uint8_t frame[FRAME_SIZE];
    uint32_t last_spawn_time = 0;
    uint32_t last_button_time = 0;
    uint32_t last_button_b_time = 0;
    bool show_histogram = false;

    memset(balls, 0, sizeof(balls));

    while (1) {
        uint64_t now = to_ms_since_boot(get_absolute_time());

        // Alterna exibição do histograma com botão A
        if (!gpio_get(BUTTON_A) && (now - last_button_time > 300)) {
            show_histogram = !show_histogram;
            last_button_time = now;
        }

        // Gera 50 bolas se botão B for pressionado
        if (!gpio_get(BUTTON_B) && (now - last_button_b_time > 300)) {
            spawn_multiple_balls(50);
            last_button_b_time = now;
        }

        // Atualiza contador total de bolas geradas por botão B
        if (pending_add_50) {
            total_balls += 50;
            pending_add_50 = false;
        }

        // Se modo histograma estiver ativado, exibe
        if (show_histogram) {
            draw_histogram(frame);
            render_on_display(frame, &frame_area);
            sleep_ms(100);
            continue;
        }

        // Gera uma bolinha automaticamente a cada 2 segundos
        if (now - last_spawn_time >= 2000) {
            if (spawn_ball()) {
                total_balls++;
            }
            last_spawn_time = now;
        }

        // Redesenha o frame da simulação
        memset(frame, 0xFF, sizeof(frame));
        draw_obstacles(frame);
        update_balls(frame);
        draw_canaletas(frame);
        render_on_display(frame, &frame_area);
        sleep_ms(30);
    }

    return 0;
}
