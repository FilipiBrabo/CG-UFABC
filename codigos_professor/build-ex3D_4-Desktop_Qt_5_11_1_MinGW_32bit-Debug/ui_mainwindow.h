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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionOpen_Texture;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    OpenGLWidget *openGLWidget;
    QComboBox *comboBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTexture;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(614, 526);
        MainWindow->setFocusPolicy(Qt::TabFocus);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen_Texture = new QAction(MainWindow);
        actionOpen_Texture->setObjectName(QStringLiteral("actionOpen_Texture"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        openGLWidget = new OpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setEnabled(true);
        openGLWidget->setFocusPolicy(Qt::StrongFocus);

        verticalLayout->addWidget(openGLWidget);

        comboBox = new QComboBox(centralWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setEnabled(false);
        comboBox->setFocusPolicy(Qt::TabFocus);

        verticalLayout->addWidget(comboBox);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 614, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuTexture = new QMenu(menuBar);
        menuTexture->setObjectName(QStringLiteral("menuTexture"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTexture->menuAction());
        menuFile->addAction(actionOpen);
        menuTexture->addAction(actionOpen_Texture);

        retranslateUi(MainWindow);
        QObject::connect(actionOpen, SIGNAL(triggered()), openGLWidget, SLOT(showFileOpenDialog()));
        QObject::connect(openGLWidget, SIGNAL(statusBarMessage(QString)), statusBar, SLOT(showMessage(QString)));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)), openGLWidget, SLOT(changeShader(int)));
        QObject::connect(openGLWidget, SIGNAL(enableComboShaders(bool)), comboBox, SLOT(setEnabled(bool)));
        QObject::connect(actionOpen_Texture, SIGNAL(triggered()), openGLWidget, SLOT(loadTexture()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
        actionOpen_Texture->setText(QApplication::translate("MainWindow", "Open Texture", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "0 - vShader1", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "1 - Flat", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "2 - Gouraud", nullptr));
        comboBox->setItemText(3, QApplication::translate("MainWindow", "3 - Phong", nullptr));
        comboBox->setItemText(4, QApplication::translate("MainWindow", "4 - Normal", nullptr));
        comboBox->setItemText(5, QApplication::translate("MainWindow", "5 - Texture", nullptr));

        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuTexture->setTitle(QApplication::translate("MainWindow", "Texture", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
