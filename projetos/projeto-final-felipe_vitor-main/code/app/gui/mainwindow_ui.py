# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.9.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QComboBox, QDateTimeEdit, QFrame,
    QHBoxLayout, QHeaderView, QLabel, QMainWindow,
    QMenuBar, QPushButton, QSizePolicy, QSpacerItem,
    QStackedWidget, QStatusBar, QTableView, QVBoxLayout,
    QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(759, 370)
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.verticalLayout_4 = QVBoxLayout(self.centralwidget)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.main_container = QFrame(self.centralwidget)
        self.main_container.setObjectName(u"main_container")
        self.main_container.setFrameShape(QFrame.Shape.StyledPanel)
        self.main_container.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_2 = QHBoxLayout(self.main_container)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.pages = QStackedWidget(self.main_container)
        self.pages.setObjectName(u"pages")
        self.main_page = QWidget()
        self.main_page.setObjectName(u"main_page")
        self.horizontalLayout_4 = QHBoxLayout(self.main_page)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.horizontalLayout_3 = QHBoxLayout()
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.bpm_container = QFrame(self.main_page)
        self.bpm_container.setObjectName(u"bpm_container")
        self.bpm_container.setFrameShape(QFrame.Shape.StyledPanel)
        self.bpm_container.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_3 = QVBoxLayout(self.bpm_container)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.horizontalLayout_7 = QHBoxLayout()
        self.horizontalLayout_7.setObjectName(u"horizontalLayout_7")
        self.bpm_logo = QLabel(self.bpm_container)
        self.bpm_logo.setObjectName(u"bpm_logo")
        self.bpm_logo.setMaximumSize(QSize(100, 100))
        self.bpm_logo.setScaledContents(True)

        self.horizontalLayout_7.addWidget(self.bpm_logo)

        self.label_3 = QLabel(self.bpm_container)
        self.label_3.setObjectName(u"label_3")

        self.horizontalLayout_7.addWidget(self.label_3)


        self.verticalLayout_3.addLayout(self.horizontalLayout_7)

        self.bpm_value = QLabel(self.bpm_container)
        self.bpm_value.setObjectName(u"bpm_value")

        self.verticalLayout_3.addWidget(self.bpm_value)

        self.verticalSpacer = QSpacerItem(20, 40, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Preferred)

        self.verticalLayout_3.addItem(self.verticalSpacer)


        self.horizontalLayout_3.addWidget(self.bpm_container)

        self.spo2_container = QFrame(self.main_page)
        self.spo2_container.setObjectName(u"spo2_container")
        self.spo2_container.setFrameShape(QFrame.Shape.StyledPanel)
        self.spo2_container.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_2 = QVBoxLayout(self.spo2_container)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.horizontalLayout_6 = QHBoxLayout()
        self.horizontalLayout_6.setObjectName(u"horizontalLayout_6")
        self.spo2_logo = QLabel(self.spo2_container)
        self.spo2_logo.setObjectName(u"spo2_logo")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(0)
        sizePolicy1.setHeightForWidth(self.spo2_logo.sizePolicy().hasHeightForWidth())
        self.spo2_logo.setSizePolicy(sizePolicy1)
        self.spo2_logo.setMaximumSize(QSize(50, 70))
        self.spo2_logo.setLineWidth(-3)
        self.spo2_logo.setTextFormat(Qt.TextFormat.RichText)
        self.spo2_logo.setPixmap(QPixmap(u"assets/blood.png"))
        self.spo2_logo.setScaledContents(True)

        self.horizontalLayout_6.addWidget(self.spo2_logo)

        self.label_2 = QLabel(self.spo2_container)
        self.label_2.setObjectName(u"label_2")

        self.horizontalLayout_6.addWidget(self.label_2)


        self.verticalLayout_2.addLayout(self.horizontalLayout_6)

        self.spo2_value = QLabel(self.spo2_container)
        self.spo2_value.setObjectName(u"spo2_value")

        self.verticalLayout_2.addWidget(self.spo2_value)

        self.verticalSpacer_2 = QSpacerItem(20, 20, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Preferred)

        self.verticalLayout_2.addItem(self.verticalSpacer_2)


        self.horizontalLayout_3.addWidget(self.spo2_container)


        self.verticalLayout.addLayout(self.horizontalLayout_3)

        self.quedas_container = QFrame(self.main_page)
        self.quedas_container.setObjectName(u"quedas_container")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Fixed)
        sizePolicy2.setHorizontalStretch(0)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.quedas_container.sizePolicy().hasHeightForWidth())
        self.quedas_container.setSizePolicy(sizePolicy2)
        self.quedas_container.setFrameShape(QFrame.Shape.StyledPanel)
        self.quedas_container.setFrameShadow(QFrame.Shadow.Raised)
        self.horizontalLayout_5 = QHBoxLayout(self.quedas_container)
        self.horizontalLayout_5.setObjectName(u"horizontalLayout_5")
        self.quedas_value = QLabel(self.quedas_container)
        self.quedas_value.setObjectName(u"quedas_value")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)
        sizePolicy3.setHorizontalStretch(0)
        sizePolicy3.setVerticalStretch(0)
        sizePolicy3.setHeightForWidth(self.quedas_value.sizePolicy().hasHeightForWidth())
        self.quedas_value.setSizePolicy(sizePolicy3)

        self.horizontalLayout_5.addWidget(self.quedas_value)

        self.btnOpenHistory = QPushButton(self.quedas_container)
        self.btnOpenHistory.setObjectName(u"btnOpenHistory")

        self.horizontalLayout_5.addWidget(self.btnOpenHistory)


        self.verticalLayout.addWidget(self.quedas_container)


        self.horizontalLayout_4.addLayout(self.verticalLayout)

        self.pages.addWidget(self.main_page)
        self.alert_page = QWidget()
        self.alert_page.setObjectName(u"alert_page")
        self.verticalLayout_5 = QVBoxLayout(self.alert_page)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.horizontalLayout_8 = QHBoxLayout()
        self.horizontalLayout_8.setObjectName(u"horizontalLayout_8")
        self.alert_logo = QLabel(self.alert_page)
        self.alert_logo.setObjectName(u"alert_logo")
        self.alert_logo.setMaximumSize(QSize(200, 250))
        self.alert_logo.setScaledContents(True)

        self.horizontalLayout_8.addWidget(self.alert_logo)

        self.lbl_problema = QLabel(self.alert_page)
        self.lbl_problema.setObjectName(u"lbl_problema")
        self.lbl_problema.setTextFormat(Qt.TextFormat.AutoText)

        self.horizontalLayout_8.addWidget(self.lbl_problema)


        self.verticalLayout_5.addLayout(self.horizontalLayout_8)

        self.lbl_subproblema = QLabel(self.alert_page)
        self.lbl_subproblema.setObjectName(u"lbl_subproblema")
        self.lbl_subproblema.setTextFormat(Qt.TextFormat.AutoText)

        self.verticalLayout_5.addWidget(self.lbl_subproblema)

        self.pages.addWidget(self.alert_page)
        self.history_page = QWidget()
        self.history_page.setObjectName(u"history_page")
        self.horizontalLayout_9 = QHBoxLayout(self.history_page)
        self.horizontalLayout_9.setObjectName(u"horizontalLayout_9")
        self.frame = QFrame(self.history_page)
        self.frame.setObjectName(u"frame")
        self.frame.setFrameShape(QFrame.Shape.StyledPanel)
        self.frame.setFrameShadow(QFrame.Shadow.Raised)
        self.verticalLayout_6 = QVBoxLayout(self.frame)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.horizontalLayout = QHBoxLayout()
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.label = QLabel(self.frame)
        self.label.setObjectName(u"label")

        self.horizontalLayout.addWidget(self.label)

        self.dateTimeFromEdit = QDateTimeEdit(self.frame)
        self.dateTimeFromEdit.setObjectName(u"dateTimeFromEdit")
        self.dateTimeFromEdit.setCalendarPopup(True)

        self.horizontalLayout.addWidget(self.dateTimeFromEdit)

        self.label_4 = QLabel(self.frame)
        self.label_4.setObjectName(u"label_4")

        self.horizontalLayout.addWidget(self.label_4)

        self.dateTimeToEdit = QDateTimeEdit(self.frame)
        self.dateTimeToEdit.setObjectName(u"dateTimeToEdit")
        self.dateTimeToEdit.setCalendarPopup(True)

        self.horizontalLayout.addWidget(self.dateTimeToEdit)

        self.label_5 = QLabel(self.frame)
        self.label_5.setObjectName(u"label_5")

        self.horizontalLayout.addWidget(self.label_5)

        self.typeCombo = QComboBox(self.frame)
        self.typeCombo.setObjectName(u"typeCombo")

        self.horizontalLayout.addWidget(self.typeCombo)


        self.verticalLayout_6.addLayout(self.horizontalLayout)

        self.historyTable = QTableView(self.frame)
        self.historyTable.setObjectName(u"historyTable")

        self.verticalLayout_6.addWidget(self.historyTable)

        self.btnBackToMain = QPushButton(self.frame)
        self.btnBackToMain.setObjectName(u"btnBackToMain")

        self.verticalLayout_6.addWidget(self.btnBackToMain)


        self.horizontalLayout_9.addWidget(self.frame)

        self.pages.addWidget(self.history_page)

        self.horizontalLayout_2.addWidget(self.pages)


        self.verticalLayout_4.addWidget(self.main_container)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 759, 20))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.bpm_logo.setText("")
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:28pt; font-weight:700;\">BATIMENTOS</span></p></body></html>", None))
        self.bpm_value.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">70 BPM</span></p></body></html>", None))
        self.spo2_logo.setText("")
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:28pt; font-weight:700;\">OXIGENA\u00c7\u00c3O</span></p></body></html>", None))
        self.spo2_value.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:20pt;\">90% S</span><span style=\" font-size:20pt; vertical-align:sub;\">p</span><span style=\" font-size:20pt;\">O</span><span style=\" font-size:20pt; vertical-align:super;\">2</span></p></body></html>", None))
        self.quedas_value.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p><span style=\" font-size:20pt;\">Sem quedas hoje</span></p></body></html>", None))
        self.btnOpenHistory.setText(QCoreApplication.translate("MainWindow", u"Hist\u00f3rico", None))
        self.alert_logo.setText("")
        self.lbl_problema.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:48pt;\">QUEDA</span></p><p align=\"center\"><span style=\" font-size:48pt;\">IDENTIFICADA</span></p></body></html>", None))
        self.lbl_subproblema.setText(QCoreApplication.translate("MainWindow", u"<html><head/><body><p align=\"center\"><span style=\" font-size:28pt;\">RESTAM XX SEGUNDOS PARA O PACIENTE</span></p><p align=\"center\"><span style=\" font-size:28pt;\"> APERTAR O BOT\u00c3O</span></p></body></html>", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"De:", None))
        self.dateTimeFromEdit.setDisplayFormat(QCoreApplication.translate("MainWindow", u"dd/MM/yyyy HH:mm", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"At\u00e9:", None))
        self.dateTimeToEdit.setDisplayFormat(QCoreApplication.translate("MainWindow", u"dd/MM/yyyy HH:mm", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"Tipo:", None))
        self.btnBackToMain.setText(QCoreApplication.translate("MainWindow", u"Voltar", None))
    # retranslateUi

