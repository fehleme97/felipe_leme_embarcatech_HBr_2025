from gui.mainwindow_ui import Ui_MainWindow
from PySide6.QtCore import QTimer
from PySide6.QtWidgets import QMainWindow
from PySide6.QtGui import QMovie, QPixmap
import os
from PySide6.QtCore import (
    QAbstractTableModel, Qt, QModelIndex, QDateTime, QSortFilterProxyModel
)
from PySide6.QtWidgets import QHeaderView
from datetime import datetime, date
import csv

base_dir = os.path.dirname(os.path.abspath(__file__))

# modelo de tabela (formato longo: Data/Hora | Tipo | Valor)
class CsvLongTableModel(QAbstractTableModel):
    def __init__(self, rows=None, parent=None):
        super().__init__(parent)
        self._headers = ["Data/Hora", "Tipo", "Valor"]
        self._rows = rows or []  # cada row = (datetime, str, float/int)

    def setRows(self, rows):
        self.beginResetModel()
        self._rows = rows
        self.endResetModel()

    def rowCount(self, parent=QModelIndex()):
        return 0 if parent.isValid() else len(self._rows)

    def columnCount(self, parent=QModelIndex()):
        return 0 if parent.isValid() else 3

    def data(self, index, role=Qt.DisplayRole):
        if not index.isValid():
            return None
        r = index.row()
        c = index.column()
        dt_obj, tipo, valor = self._rows[r]

        if role == Qt.DisplayRole:
            if c == 0:
                # formata a data/hora
                return dt_obj.strftime("%d/%m/%Y %H:%M:%S")
            elif c == 1:
                return tipo
            elif c == 2:
                return str(valor)
        elif role == Qt.UserRole:
            return self._rows[r]  # usado pelo proxy
        return None

    def headerData(self, section, orientation, role=Qt.DisplayRole):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Horizontal:
            return self._headers[section]
        else:
            return section + 1

    def sort(self, column, order):
        reverse = (order == Qt.DescendingOrder)
        key_funcs = [
            lambda row: row[0],  # datetime
            lambda row: row[1],  # tipo
            lambda row: row[2],  # valor
        ]
        key = key_funcs[column]
        self.layoutAboutToBeChanged.emit()
        self._rows.sort(key=key, reverse=reverse)
        self.layoutChanged.emit()


# proxy de filtro por data e tipo
class HistoryFilterProxy(QSortFilterProxyModel):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._date_from = None
        self._date_to = None
        self._type = None        # "Todos" | "Batimentos" | "Oxigenação" | "Quedas"

    def setDateRange(self, date_from: QDateTime, date_to: QDateTime):
        self._date_from = date_from
        self._date_to = date_to
        self.invalidateFilter()

    def setType(self, t: str):
        self._type = t
        self.invalidateFilter()

    def filterAcceptsRow(self, source_row, source_parent):
        idx = self.sourceModel().index(source_row, 0, source_parent)
        raw = self.sourceModel().data(idx, Qt.UserRole)
        if not raw:
            return True
        dt_obj, tipo, valor = raw

        # filtro de tipo
        if self._type and self._type != "Todos":
            if tipo != self._type:
                return False

        # filtro por data/hora
        if isinstance(dt_obj, datetime):
            qdt = QDateTime(dt_obj)
            if self._date_from and qdt < self._date_from:
                return False
            if self._date_to and qdt > self._date_to:
                return False

        return True


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.setWindowTitle("TCC - embarcatech")
        self.setFixedSize(self.size())  # fixa o tamanho da tela

        self.black_heart_path = os.path.join(base_dir, "gui", "assets", "black_heart_rate.gif")
        self.blood_path = os.path.join(base_dir, "gui", "assets", "blood.png")

        # imagens da tela principal
        heart_mv = QMovie(self.black_heart_path)
        blood_pix = QPixmap(self.blood_path)

        if heart_mv.isValid():
            self.bpm_logo.setMovie(heart_mv)
            heart_mv.start()
        else:
            self.bpm_logo.setText("error")

        self.spo2_logo.setPixmap(blood_pix)
        self.spo2_logo.setScaledContents(True)
        
        # configurações do csv
        self.csv_file = 'dados.csv'
        self.last_size = 0
        self.last_values = {}

        # estruturas do histórico
        self.history_model = CsvLongTableModel()
        self.history_proxy = HistoryFilterProxy(parent=self)
        self.history_proxy.setSourceModel(self.history_model)

        # timer
        self.monitor_timer = QTimer()
        self.monitor_timer.timeout.connect(self.check_file_update)
        self.monitor_timer.start(1000)

        # preparar a página de histórico
        self.setup_history_page()

        # carga inicial
        self.update_last_values()
        self.load_history_data()
        self.set_params()

    def check_file_update(self):
        """verifica se o arquivo .csv foi modificado e atualiza os valores"""
        try:
            if not os.path.exists(self.csv_file):
                print("file not found")
                return
            current_size = os.path.getsize(self.csv_file)
            if current_size != self.last_size:
                self.last_size = current_size
                self.update_last_values()
                self.set_params()
                self.load_history_data()
        except Exception as e:
            print(f"Erro na verificação: {str(e)}")

    def update_last_values(self):
        try:
            if not os.path.exists(self.csv_file):
                return
            file_size = os.path.getsize(self.csv_file)
            if file_size == 0:
                return
            with open(self.csv_file, 'r') as f:
                headers_line = f.readline().strip()
                if not headers_line:
                    return
                headers = headers_line.split(',')
                f.seek(0, os.SEEK_END)
                pos = f.tell()
                line_found = False
                while pos > 0:
                    pos -= 1
                    f.seek(pos)
                    char = f.read(1)
                    if char == '\n':
                        current_pos = f.tell()
                        if current_pos < file_size:
                            line_found = True
                            break
                    elif pos == 0:
                        f.seek(0)
                        line_found = True
                        break
                if line_found:
                    last_line = f.readline().strip()
                    values = last_line.split(',')
                    for i, header in enumerate(headers):
                        if i < len(values):
                            value = values[i].strip()
                            self.last_values[header] = value if value != '' else '--'
        except Exception as e:
            print(f"Erro na leitura do arquivo: {str(e)}")
            self.last_values = {
                'batimentos': '--',
                'oxigenação': '--',
                'horário': '--:--',
                'queda': '0',
                'quedas': '0',
                'tempo': '00:00'
            }

    def set_params(self):
        '''modifica os valores dos parâmetros (labels) conforme leitura do .csv'''
        try: 
            def safe_get(key, default="--", conversion=int):
                value = self.last_values.get(key)
                if value is None or value == '':
                    return default
                try:
                    return conversion(value)
                except (ValueError, TypeError):
                    return default
                
            batimentos = safe_get('batimentos', '--', float)
            oxigenacao = safe_get('oxigenação', '--', float)
            horario = safe_get('horário', '--:--')
            queda = safe_get('queda', '0', int)
            quedas = safe_get('quedas', '0', int)
            emergencia = safe_get('emergencia', '0', int)

            self.bpm_value.setText(f"{batimentos} BPM")
            self.bpm_value.setStyleSheet("""
                QLabel{
                        font-size: 20pt;
                        qproperty-alignment: 'AlignCenter';                    
                    }
            """)
            self.spo2_value.setText(f"{oxigenacao}% S<sub>p</sub>O\u00B2")
            self.spo2_value.setStyleSheet("""
                QLabel{
                        font-size: 20pt;
                        qproperty-alignment: 'AlignCenter';                    
                    }
            """)

            if  quedas == 0:
                self.quedas_value.setText('Sem quedas hoje')
            elif quedas == 1:
                self.quedas_value.setText('O paciente caiu uma vez')
            else:
                self.quedas_value.setText(f'O paciente caiu {quedas} vezes')
            self.quedas_value.setStyleSheet("""
                QLabel{
                        font-size: 20pt;
                    }
            """)

            if  queda == 1:
                self.pages.setCurrentWidget(self.alert_page)
                self.set_alert(0, "QUEDA\nIDENTIFICADA",
                                   "VERIFIQUE A SITUAÇÃO DO PACIENTE\nURGENTEMENTE")
            elif emergencia == 1:
                self.pages.setCurrentWidget(self.alert_page)
                self.set_alert(1, "EMERGÊNCIA",
                                   f"O PACIENTE SOLICITOU\n AJUDA")
            else:
                self.pages.setCurrentWidget(self.main_page)

        except Exception as e:
            print(f"Erro na atualização: {str(e)}")

    def set_alert(self, alert = bool, top_msg = str, sub_msg = str):
        '''modifica a página de alerta'''

        self.one_ex_path = os.path.join(base_dir, "gui", "assets", "one_exclamation.gif")
        self.dual_ex_path = os.path.join(base_dir, "gui", "assets", "dual_exclamation.gif")
        
        one_ex_mv = QMovie(self.one_ex_path)
        two_ex_mv = QMovie(self.dual_ex_path)

        if alert:
            if one_ex_mv.isValid():
                self.alert_logo.setMovie(one_ex_mv)
                one_ex_mv.start()
            else:
                self.alert_logo.setText("error")
        else:    
            if two_ex_mv.isValid():
                self.alert_logo.setMovie(two_ex_mv)
                two_ex_mv.start()
            else:
                self.alert_logo.setText("error")

        self.lbl_problema.setText(top_msg)
        self.lbl_problema.setStyleSheet("""
            QLabel{
                    font-size: 48pt;
                    qproperty-alignment: 'AlignCenter';                    
                }
        """)
        self.lbl_subproblema.setText(sub_msg)
        self.lbl_subproblema.setStyleSheet("""
            QLabel{
                    font-size: 27pt;
                    qproperty-alignment: 'AlignCenter';                    
                }
        """)

    # página histórico
    def setup_history_page(self):
        if hasattr(self, "historyTable"):
            self.historyTable.setModel(self.history_proxy)
            self.historyTable.setSortingEnabled(True)
            self.historyTable.horizontalHeader().setStretchLastSection(True)
            self.historyTable.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)

        if hasattr(self, "typeCombo"):
            self.typeCombo.clear()
            self.typeCombo.addItems(["Todos", "Batimentos", "Oxigenação", "Quedas"])
            self.typeCombo.currentTextChanged.connect(self.on_type_changed)

        now = QDateTime.currentDateTime()
        if hasattr(self, "dateTimeFromEdit"):
            self.dateTimeFromEdit.setDateTime(now.addDays(-7))
            self.dateTimeFromEdit.dateTimeChanged.connect(self.on_date_changed)
        if hasattr(self, "dateTimeToEdit"):
            self.dateTimeToEdit.setDateTime(now)
            self.dateTimeToEdit.dateTimeChanged.connect(self.on_date_changed)

        if hasattr(self, "btnBackToMain"):
            self.btnBackToMain.clicked.connect(self.back_to_main)
        if hasattr(self, "btnOpenHistory"):
            self.btnOpenHistory.clicked.connect(self.go_to_history)

        self.apply_filters()

    def on_type_changed(self, text):
        self.history_proxy.setType(text)

    def on_date_changed(self, _):
        self.apply_filters()

    def apply_filters(self):
        df = self.dateTimeFromEdit.dateTime() if hasattr(self, "dateTimeFromEdit") else None
        dt = self.dateTimeToEdit.dateTime() if hasattr(self, "dateTimeToEdit") else None
        self.history_proxy.setDateRange(df, dt)

    def go_to_history(self):
        self.pages.setCurrentWidget(self.history_page)

    def back_to_main(self):
        self.pages.setCurrentWidget(self.main_page)

    def load_history_data(self):
        rows = []
        if not os.path.exists(self.csv_file) or os.path.getsize(self.csv_file) == 0:
            self.history_model.setRows(rows)
            return

        try:
            with open(self.csv_file, newline='', encoding="utf-8") as f:
                reader = csv.DictReader(f)
                for rec in reader:
                    h_raw = rec.get('horário') or rec.get('horario') or rec.get('data_hora')
                    dt_obj = self._parse_datetime(h_raw)

                    bat = rec.get('batimentos')
                    if bat not in (None, ""):
                        v = self._to_number(bat)
                        if v is not None:
                            rows.append((dt_obj, "Batimentos", v))

                    spo = rec.get('oxigenação') or rec.get('oxigenacao') or rec.get('spo2')
                    if spo not in (None, ""):
                        v = self._to_number(spo)
                        if v is not None:
                            rows.append((dt_obj, "Oxigenação", v))

                    qds = rec.get('quedas')
                    if qds not in (None, ""):
                        v = self._to_number(qds)
                        if v is not None:
                            rows.append((dt_obj, "Quedas", v))

            self.history_model.setRows(rows)
            self.apply_filters()

            if hasattr(self, "historyTable"):
                self.historyTable.sortByColumn(0, Qt.DescendingOrder)

        except Exception as e:
            print(f"Erro lendo histórico: {e}")
            self.history_model.setRows([])

    def _parse_datetime(self, raw):
        if not raw:
            today = date.today()
            return datetime(today.year, today.month, today.day, 0, 0, 0)
        s = str(raw).strip()
        try:
            return datetime.fromisoformat(s.replace("Z", "+00:00"))
        except Exception:
            pass
        fmts = [
            "%d/%m/%Y %H:%M:%S",
            "%d/%m/%Y %H:%M",
            "%Y-%m-%d %H:%M:%S",
            "%Y-%m-%d %H:%M",
            "%d-%m-%Y %H:%M:%S",
            "%d-%m-%Y %H:%M",
            "%H:%M:%S",
            "%H:%M",
        ]
        for fmt in fmts:
            try:
                return datetime.strptime(s, fmt)
            except Exception:
                continue
        today = date.today()
        return datetime(today.year, today.month, today.day, 0, 0, 0)

    def _to_number(self, value):
        if value is None:
            return None
        s = str(value).strip().replace(",", ".")
        try:
            if "." in s:
                return float(s)
            return int(s)
        except Exception:
            return None