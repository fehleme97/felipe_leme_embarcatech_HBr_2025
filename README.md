# PROJETOS DESENVOLVIDOS NO CURSO EMBARCATECH 2025 

Autor: FELIPE LEME CORREA DA SILVA 

Instituição: EmbarcaTech - HBr - Campinas

## SOBRE O REPOSITÓRIO: 

AO DECORRER DESSE REPOSITÓRIO SERÁ APRESENTADO PROJETOS QUE FORAM DESENVOLVIDOS DURANTE O PROCESSO DE APRENDIZAGEM NO CURSO DE RESIDÊNCIA TECNOLÓGICA EM SISTEMAS EMBARCADOS.

## 📂 Lista de Projetos

```
/projetos
│──projeto1
    │──assets          # Imagens, vídeos, esquemas
    │──docs            # Documentação técnica complementar
    │──src             # Código-fonte
    │── README.md      # Descrição específica do projeto
│──LICENSE             # Licença do repositório (MIT)
```

## 🔗 Projetos
Abaixo está a lista de projetos desenvolvidos. Cada um possui um diretório específico dentro do repositório:

- [Projeto 1 - Nome do Projeto](./projetos/projeto1/)
- [Projeto 2 - Nome do Projeto](./projetos/projeto2/)
- [Projeto 3 - Nome do Projeto](./projetos/projeto3/)

## 🖼️ Setup do Hardware
Sugestão: incluir aqui uma **foto geral do kit** ou do **setup utilizado** nos projetos. Isso pode ajudar na reprodução dos experimentos.

## 🛠️ Requisitos e Configuração
Para compilar e rodar os projetos no **Raspberry Pi Pico W**, siga os passos abaixo:

1. **Instale o SDK do Pico** seguindo as instruções oficiais da Raspberry Pi.
2. Clone este repositório:
   ```bash
   git clone https://github.com/seu_usuario/seu_repositorio.git
   cd meu_projeto
   ```
3. **Crie o diretório de build e compile:**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
4. **Carregue o firmware no Raspberry Pi Pico W.**