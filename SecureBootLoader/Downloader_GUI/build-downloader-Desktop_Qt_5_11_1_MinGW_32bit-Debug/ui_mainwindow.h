/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFormLayout *formLayout;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *comboBox;
    QPushButton *SelectComPort;
    QPushButton *SelectComPort_Disconnect;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBoxBaudrate;
    QSpacerItem *verticalSpacer_3;
    QGridLayout *gridLayout;
    QLabel *label_rd;
    QLabel *label_rd1;
    QLabel *label_wr_lbs;
    QLabel *label_rd2;
    QLabel *label_wr_ofst;
    QLabel *label_rd_3;
    QLabel *label_wr_apcount_2;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_2;
    QLineEdit *fileLocation;
    QPushButton *selecFile;
    QPushButton *downloadImage;
    QProgressBar *downloadProgressBar;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_3;
    QLabel *fileSizeWR;
    QLabel *downloadStatus;
    QLabel *fileSizeRD;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(338, 390);
        QFont font;
        font.setFamily(QStringLiteral("Arial Unicode MS"));
        font.setPointSize(9);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral("../../Desktop/tools-icon-200x200.jpg"), QSize(), QIcon::Normal, QIcon::On);
        MainWindow->setWindowIcon(icon);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        formLayout = new QFormLayout(centralWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 4, 1, 1);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout_4->addWidget(comboBox, 0, 0, 1, 1);

        SelectComPort = new QPushButton(centralWidget);
        SelectComPort->setObjectName(QStringLiteral("SelectComPort"));

        gridLayout_4->addWidget(SelectComPort, 0, 3, 1, 1);

        SelectComPort_Disconnect = new QPushButton(centralWidget);
        SelectComPort_Disconnect->setObjectName(QStringLiteral("SelectComPort_Disconnect"));

        gridLayout_4->addWidget(SelectComPort_Disconnect, 0, 5, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 0, 2, 1, 1);

        comboBoxBaudrate = new QComboBox(centralWidget);
        comboBoxBaudrate->setObjectName(QStringLiteral("comboBoxBaudrate"));

        gridLayout_4->addWidget(comboBoxBaudrate, 0, 1, 1, 1);


        formLayout->setLayout(0, QFormLayout::SpanningRole, gridLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(1, QFormLayout::LabelRole, verticalSpacer_3);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_rd = new QLabel(centralWidget);
        label_rd->setObjectName(QStringLiteral("label_rd"));

        gridLayout->addWidget(label_rd, 0, 0, 1, 1);

        label_rd1 = new QLabel(centralWidget);
        label_rd1->setObjectName(QStringLiteral("label_rd1"));

        gridLayout->addWidget(label_rd1, 1, 0, 1, 1);

        label_wr_lbs = new QLabel(centralWidget);
        label_wr_lbs->setObjectName(QStringLiteral("label_wr_lbs"));

        gridLayout->addWidget(label_wr_lbs, 1, 1, 1, 1);

        label_rd2 = new QLabel(centralWidget);
        label_rd2->setObjectName(QStringLiteral("label_rd2"));

        gridLayout->addWidget(label_rd2, 2, 0, 1, 1);

        label_wr_ofst = new QLabel(centralWidget);
        label_wr_ofst->setObjectName(QStringLiteral("label_wr_ofst"));

        gridLayout->addWidget(label_wr_ofst, 2, 1, 1, 1);

        label_rd_3 = new QLabel(centralWidget);
        label_rd_3->setObjectName(QStringLiteral("label_rd_3"));

        gridLayout->addWidget(label_rd_3, 3, 0, 1, 1);

        label_wr_apcount_2 = new QLabel(centralWidget);
        label_wr_apcount_2->setObjectName(QStringLiteral("label_wr_apcount_2"));

        gridLayout->addWidget(label_wr_apcount_2, 3, 1, 1, 1);


        formLayout->setLayout(2, QFormLayout::SpanningRole, gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(3, QFormLayout::LabelRole, verticalSpacer_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        fileLocation = new QLineEdit(centralWidget);
        fileLocation->setObjectName(QStringLiteral("fileLocation"));

        gridLayout_2->addWidget(fileLocation, 0, 0, 1, 1);

        selecFile = new QPushButton(centralWidget);
        selecFile->setObjectName(QStringLiteral("selecFile"));

        gridLayout_2->addWidget(selecFile, 0, 1, 1, 1);

        downloadImage = new QPushButton(centralWidget);
        downloadImage->setObjectName(QStringLiteral("downloadImage"));

        gridLayout_2->addWidget(downloadImage, 2, 1, 1, 1);

        downloadProgressBar = new QProgressBar(centralWidget);
        downloadProgressBar->setObjectName(QStringLiteral("downloadProgressBar"));
        downloadProgressBar->setValue(24);

        gridLayout_2->addWidget(downloadProgressBar, 2, 0, 1, 1);


        formLayout->setLayout(4, QFormLayout::SpanningRole, gridLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(5, QFormLayout::LabelRole, verticalSpacer);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(4);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        fileSizeWR = new QLabel(centralWidget);
        fileSizeWR->setObjectName(QStringLiteral("fileSizeWR"));

        gridLayout_3->addWidget(fileSizeWR, 1, 1, 1, 1);

        downloadStatus = new QLabel(centralWidget);
        downloadStatus->setObjectName(QStringLiteral("downloadStatus"));

        gridLayout_3->addWidget(downloadStatus, 0, 0, 1, 2);

        fileSizeRD = new QLabel(centralWidget);
        fileSizeRD->setObjectName(QStringLiteral("fileSizeRD"));

        gridLayout_3->addWidget(fileSizeRD, 1, 0, 1, 1);


        formLayout->setLayout(6, QFormLayout::LabelRole, gridLayout_3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 338, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "G1 Boot Manager", nullptr));
        SelectComPort->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        SelectComPort_Disconnect->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
        label_rd->setText(QApplication::translate("MainWindow", "Status", nullptr));
        label_rd1->setText(QApplication::translate("MainWindow", "Last Boot Status", nullptr));
        label_wr_lbs->setText(QApplication::translate("MainWindow", "None", nullptr));
        label_rd2->setText(QApplication::translate("MainWindow", "Running App offset", nullptr));
        label_wr_ofst->setText(QApplication::translate("MainWindow", "None", nullptr));
        label_rd_3->setText(QApplication::translate("MainWindow", "Application in system", nullptr));
        label_wr_apcount_2->setText(QApplication::translate("MainWindow", "None", nullptr));
        selecFile->setText(QApplication::translate("MainWindow", "Select .bin", nullptr));
        downloadImage->setText(QApplication::translate("MainWindow", "Download App", nullptr));
        fileSizeWR->setText(QApplication::translate("MainWindow", "unknown", nullptr));
        downloadStatus->setText(QApplication::translate("MainWindow", "Status : idle", nullptr));
        fileSizeRD->setText(QApplication::translate("MainWindow", "File Size : ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
