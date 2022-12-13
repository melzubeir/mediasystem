/********************************************************************************
** Form generated from reading ui file 'clippingwindow.ui'
**
** Created: Mon 10. Aug 10:13:43 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CLIPPINGWINDOW_H
#define UI_CLIPPINGWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClippingWindow
{
public:
    QAction *actionImporter;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionRorate_left;
    QAction *actionRotate_right;
    QAction *actionZoom;
    QAction *actionRotate;
    QAction *actionZoom_2;
    QAction *actionRotate_90;
    QAction *actionRotate_180;
    QAction *actionRotate_3;
    QAction *actionAbout;
    QAction *actionHelp_Topics;
    QAction *actionLock;
    QAction *actionUnlock;
    QAction *actionDone_2;
    QAction *actionView;
    QAction *actionSearch_Box;
    QAction *actionMark;
    QAction *actionNext_Page;
    QAction *actionPrevious_Page;
    QAction *actionLast_Page;
    QAction *actionFirst_Page;
    QAction *actionMain_toolbar;
    QAction *actionOperations_toolbar;
    QAction *actionBrowse_toolbar;
    QAction *actionPre_clip;
    QAction *actionAutoArrange;
    QAction *actionDrawer_Operation;
    QAction *actionAdd_to_clip;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QSplitter *splitter_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *label;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QLabel *label_2;
    QPushButton *pushButton_5;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOptions;
    QMenu *menuImage;
    QMenu *menuHelp;
    QMenu *menuToolBox;
    QMenu *menuToolBars;
    QToolBar *fileToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QComboBox *comboBox_2;
    QSpacerItem *horizontalSpacer_3;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *treeWidget;
    QListWidget *listWidget;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QListView *listView_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_2;
    QToolBar *operationToolBar;
    QToolBar *scrollToolBar;
    QToolBar *manipulationToolBar;

    void setupUi(QMainWindow *ClippingWindow)
    {
        if (ClippingWindow->objectName().isEmpty())
            ClippingWindow->setObjectName(QString::fromUtf8("ClippingWindow"));
        ClippingWindow->resize(1020, 694);
        actionImporter = new QAction(ClippingWindow);
        actionImporter->setObjectName(QString::fromUtf8("actionImporter"));
        actionSave = new QAction(ClippingWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(ClippingWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionZoom_In = new QAction(ClippingWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(ClippingWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionRorate_left = new QAction(ClippingWindow);
        actionRorate_left->setObjectName(QString::fromUtf8("actionRorate_left"));
        actionRotate_right = new QAction(ClippingWindow);
        actionRotate_right->setObjectName(QString::fromUtf8("actionRotate_right"));
        actionZoom = new QAction(ClippingWindow);
        actionZoom->setObjectName(QString::fromUtf8("actionZoom"));
        actionRotate = new QAction(ClippingWindow);
        actionRotate->setObjectName(QString::fromUtf8("actionRotate"));
        actionZoom_2 = new QAction(ClippingWindow);
        actionZoom_2->setObjectName(QString::fromUtf8("actionZoom_2"));
        actionRotate_90 = new QAction(ClippingWindow);
        actionRotate_90->setObjectName(QString::fromUtf8("actionRotate_90"));
        actionRotate_180 = new QAction(ClippingWindow);
        actionRotate_180->setObjectName(QString::fromUtf8("actionRotate_180"));
        actionRotate_3 = new QAction(ClippingWindow);
        actionRotate_3->setObjectName(QString::fromUtf8("actionRotate_3"));
        actionAbout = new QAction(ClippingWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionHelp_Topics = new QAction(ClippingWindow);
        actionHelp_Topics->setObjectName(QString::fromUtf8("actionHelp_Topics"));
        actionLock = new QAction(ClippingWindow);
        actionLock->setObjectName(QString::fromUtf8("actionLock"));
        actionUnlock = new QAction(ClippingWindow);
        actionUnlock->setObjectName(QString::fromUtf8("actionUnlock"));
        actionDone_2 = new QAction(ClippingWindow);
        actionDone_2->setObjectName(QString::fromUtf8("actionDone_2"));
        actionView = new QAction(ClippingWindow);
        actionView->setObjectName(QString::fromUtf8("actionView"));
        actionSearch_Box = new QAction(ClippingWindow);
        actionSearch_Box->setObjectName(QString::fromUtf8("actionSearch_Box"));
        actionMark = new QAction(ClippingWindow);
        actionMark->setObjectName(QString::fromUtf8("actionMark"));
        actionNext_Page = new QAction(ClippingWindow);
        actionNext_Page->setObjectName(QString::fromUtf8("actionNext_Page"));
        actionPrevious_Page = new QAction(ClippingWindow);
        actionPrevious_Page->setObjectName(QString::fromUtf8("actionPrevious_Page"));
        actionLast_Page = new QAction(ClippingWindow);
        actionLast_Page->setObjectName(QString::fromUtf8("actionLast_Page"));
        actionFirst_Page = new QAction(ClippingWindow);
        actionFirst_Page->setObjectName(QString::fromUtf8("actionFirst_Page"));
        actionMain_toolbar = new QAction(ClippingWindow);
        actionMain_toolbar->setObjectName(QString::fromUtf8("actionMain_toolbar"));
        actionOperations_toolbar = new QAction(ClippingWindow);
        actionOperations_toolbar->setObjectName(QString::fromUtf8("actionOperations_toolbar"));
        actionBrowse_toolbar = new QAction(ClippingWindow);
        actionBrowse_toolbar->setObjectName(QString::fromUtf8("actionBrowse_toolbar"));
        actionPre_clip = new QAction(ClippingWindow);
        actionPre_clip->setObjectName(QString::fromUtf8("actionPre_clip"));
        actionAutoArrange = new QAction(ClippingWindow);
        actionAutoArrange->setObjectName(QString::fromUtf8("actionAutoArrange"));
        actionDrawer_Operation = new QAction(ClippingWindow);
        actionDrawer_Operation->setObjectName(QString::fromUtf8("actionDrawer_Operation"));
        actionAdd_to_clip = new QAction(ClippingWindow);
        actionAdd_to_clip->setObjectName(QString::fromUtf8("actionAdd_to_clip"));
        centralWidget = new QWidget(ClippingWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(85, 255, 127, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(213, 255, 223, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(149, 255, 175, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(42, 127, 63, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(56, 170, 84, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush7(QColor(170, 255, 191, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        widget->setPalette(palette);
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setMargin(11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        splitter_2 = new QSplitter(widget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        scrollArea = new QScrollArea(splitter_2);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 217, 544));
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 9, 101, 16));
        scrollArea->setWidget(scrollAreaWidgetContents);
        splitter_2->addWidget(scrollArea);
        scrollArea_2 = new QScrollArea(splitter_2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 216, 544));
        pushButton_3 = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(190, 10, 75, 23));
        pushButton_4 = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(270, 10, 75, 23));
        label_2 = new QLabel(scrollAreaWidgetContents_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 130, 281, 16));
        pushButton_5 = new QPushButton(scrollAreaWidgetContents_2);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(180, 60, 21, 23));
        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        splitter_2->addWidget(scrollArea_2);

        gridLayout_4->addWidget(splitter_2, 0, 0, 1, 1);


        gridLayout_2->addWidget(widget, 0, 0, 1, 1);

        ClippingWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ClippingWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1020, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QString::fromUtf8("menuOptions"));
        menuImage = new QMenu(menuOptions);
        menuImage->setObjectName(QString::fromUtf8("menuImage"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuToolBox = new QMenu(menuBar);
        menuToolBox->setObjectName(QString::fromUtf8("menuToolBox"));
        menuToolBars = new QMenu(menuBar);
        menuToolBars->setObjectName(QString::fromUtf8("menuToolBars"));
        ClippingWindow->setMenuBar(menuBar);
        fileToolBar = new QToolBar(ClippingWindow);
        fileToolBar->setObjectName(QString::fromUtf8("fileToolBar"));
        ClippingWindow->addToolBar(Qt::TopToolBarArea, fileToolBar);
        statusBar = new QStatusBar(ClippingWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ClippingWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(ClippingWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(226, 300));
        dockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox_2 = new QComboBox(dockWidgetContents);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setEditable(true);
        comboBox_2->setMaxVisibleItems(15);
        comboBox_2->setInsertPolicy(QComboBox::NoInsert);
        comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        comboBox_2->setFrame(true);

        gridLayout->addWidget(comboBox_2, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(186, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 1, 1, 1);

        comboBox = new QComboBox(dockWidgetContents);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(true);
        comboBox->setMaxVisibleItems(15);
        comboBox->setInsertPolicy(QComboBox::NoInsert);
        comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        comboBox->setFrame(true);

        gridLayout->addWidget(comboBox, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

        treeWidget = new QTreeWidget(dockWidgetContents);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        gridLayout->addWidget(treeWidget, 2, 0, 1, 1);

        listWidget = new QListWidget(dockWidgetContents);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        gridLayout->addWidget(listWidget, 2, 1, 1, 1);

        dockWidget->setWidget(dockWidgetContents);
        ClippingWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_3 = new QDockWidget(ClippingWindow);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        dockWidget_3->setMinimumSize(QSize(175, 238));
        dockWidget_3->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setMargin(11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tableWidget = new QTableWidget(dockWidgetContents_3);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget->rowCount() < 3)
            tableWidget->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setItem(1, 0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setItem(1, 1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(2, 0, __qtablewidgetitem9);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setDragEnabled(true);
        tableWidget->setSortingEnabled(true);

        verticalLayout_2->addWidget(tableWidget);

        verticalSpacer = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(dockWidgetContents_3);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(dockWidgetContents_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);

        listView_2 = new QListView(dockWidgetContents_3);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));

        verticalLayout->addWidget(listView_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_2 = new QPushButton(dockWidgetContents_3);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        dockWidget_3->setWidget(dockWidgetContents_3);
        ClippingWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_3);
        operationToolBar = new QToolBar(ClippingWindow);
        operationToolBar->setObjectName(QString::fromUtf8("operationToolBar"));
        ClippingWindow->addToolBar(Qt::TopToolBarArea, operationToolBar);
        scrollToolBar = new QToolBar(ClippingWindow);
        scrollToolBar->setObjectName(QString::fromUtf8("scrollToolBar"));
        ClippingWindow->addToolBar(Qt::TopToolBarArea, scrollToolBar);
        ClippingWindow->insertToolBarBreak(scrollToolBar);
        manipulationToolBar = new QToolBar(ClippingWindow);
        manipulationToolBar->setObjectName(QString::fromUtf8("manipulationToolBar"));
        ClippingWindow->addToolBar(Qt::TopToolBarArea, manipulationToolBar);
        ClippingWindow->insertToolBarBreak(manipulationToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuToolBox->menuAction());
        menuBar->addAction(menuToolBars->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionImporter);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionDone_2);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuOptions->addAction(menuImage->menuAction());
        menuOptions->addAction(actionRotate_3);
        menuOptions->addSeparator();
        menuOptions->addAction(actionLock);
        menuOptions->addAction(actionUnlock);
        menuOptions->addSeparator();
        menuOptions->addSeparator();
        menuOptions->addAction(actionFirst_Page);
        menuOptions->addAction(actionNext_Page);
        menuOptions->addAction(actionPrevious_Page);
        menuOptions->addAction(actionLast_Page);
        menuOptions->addSeparator();
        menuOptions->addAction(actionPre_clip);
        menuOptions->addAction(actionAdd_to_clip);
        menuOptions->addAction(actionAutoArrange);
        menuOptions->addSeparator();
        menuOptions->addAction(actionMark);
        menuImage->addAction(actionZoom);
        menuImage->addAction(actionRotate);
        menuImage->addAction(actionZoom_2);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionHelp_Topics);
        menuToolBox->addAction(actionSearch_Box);
        menuToolBars->addAction(actionMain_toolbar);
        menuToolBars->addAction(actionOperations_toolbar);
        menuToolBars->addAction(actionBrowse_toolbar);
        fileToolBar->addAction(actionImporter);
        fileToolBar->addAction(actionSave);
        fileToolBar->addSeparator();
        operationToolBar->addAction(actionZoom_In);
        operationToolBar->addAction(actionZoom_Out);
        operationToolBar->addAction(actionRorate_left);
        operationToolBar->addAction(actionRotate_right);
        operationToolBar->addSeparator();
        operationToolBar->addAction(actionLock);
        operationToolBar->addAction(actionUnlock);
        operationToolBar->addSeparator();
        operationToolBar->addAction(actionDone_2);
        scrollToolBar->addAction(actionFirst_Page);
        scrollToolBar->addAction(actionNext_Page);
        scrollToolBar->addAction(actionPrevious_Page);
        scrollToolBar->addAction(actionLast_Page);
        manipulationToolBar->addAction(actionPre_clip);
        manipulationToolBar->addAction(actionAdd_to_clip);
        manipulationToolBar->addAction(actionAutoArrange);

        retranslateUi(ClippingWindow);

        QMetaObject::connectSlotsByName(ClippingWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ClippingWindow)
    {
        ClippingWindow->setWindowTitle(QApplication::translate("ClippingWindow", "ClippingWindow", 0, QApplication::UnicodeUTF8));
        actionImporter->setText(QApplication::translate("ClippingWindow", "Importer", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("ClippingWindow", "Save Image", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("ClippingWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("ClippingWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("ClippingWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionRorate_left->setText(QApplication::translate("ClippingWindow", "Rorate left", 0, QApplication::UnicodeUTF8));
        actionRotate_right->setText(QApplication::translate("ClippingWindow", "Rotate right", 0, QApplication::UnicodeUTF8));
        actionZoom->setText(QApplication::translate("ClippingWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionRotate->setText(QApplication::translate("ClippingWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_2->setText(QApplication::translate("ClippingWindow", "Custom...", 0, QApplication::UnicodeUTF8));
        actionRotate_90->setText(QApplication::translate("ClippingWindow", "Rotate 90", 0, QApplication::UnicodeUTF8));
        actionRotate_180->setText(QApplication::translate("ClippingWindow", "Rotate 180", 0, QApplication::UnicodeUTF8));
        actionRotate_3->setText(QApplication::translate("ClippingWindow", "Rotate...", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("ClippingWindow", "About", 0, QApplication::UnicodeUTF8));
        actionHelp_Topics->setText(QApplication::translate("ClippingWindow", "Help Topics", 0, QApplication::UnicodeUTF8));
        actionLock->setText(QApplication::translate("ClippingWindow", "Lock", 0, QApplication::UnicodeUTF8));
        actionUnlock->setText(QApplication::translate("ClippingWindow", "Unlock", 0, QApplication::UnicodeUTF8));
        actionDone_2->setText(QApplication::translate("ClippingWindow", "Done!", 0, QApplication::UnicodeUTF8));
        actionView->setText(QApplication::translate("ClippingWindow", "View", 0, QApplication::UnicodeUTF8));
        actionSearch_Box->setText(QApplication::translate("ClippingWindow", "Search Box", 0, QApplication::UnicodeUTF8));
        actionMark->setText(QApplication::translate("ClippingWindow", "Mark", 0, QApplication::UnicodeUTF8));
        actionNext_Page->setText(QApplication::translate("ClippingWindow", "Next Page", 0, QApplication::UnicodeUTF8));
        actionPrevious_Page->setText(QApplication::translate("ClippingWindow", "Previous Page", 0, QApplication::UnicodeUTF8));
        actionLast_Page->setText(QApplication::translate("ClippingWindow", "Last Page", 0, QApplication::UnicodeUTF8));
        actionFirst_Page->setText(QApplication::translate("ClippingWindow", "First Page", 0, QApplication::UnicodeUTF8));
        actionMain_toolbar->setText(QApplication::translate("ClippingWindow", "Main toolbar", 0, QApplication::UnicodeUTF8));
        actionOperations_toolbar->setText(QApplication::translate("ClippingWindow", "Operations toolbar", 0, QApplication::UnicodeUTF8));
        actionBrowse_toolbar->setText(QApplication::translate("ClippingWindow", "Browse toolbar", 0, QApplication::UnicodeUTF8));
        actionPre_clip->setText(QApplication::translate("ClippingWindow", "Pre Clip", 0, QApplication::UnicodeUTF8));
        actionAutoArrange->setText(QApplication::translate("ClippingWindow", "AutoArrange", 0, QApplication::UnicodeUTF8));
        actionDrawer_Operation->setText(QApplication::translate("ClippingWindow", "Drawer Operation", 0, QApplication::UnicodeUTF8));
        actionAdd_to_clip->setText(QApplication::translate("ClippingWindow", "Add to clip", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ClippingWindow", "Image will show Here", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("ClippingWindow", "Clip", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("ClippingWindow", "Reject", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ClippingWindow", "Clipped Images will be shown here", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("ClippingWindow", "+", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("ClippingWindow", "File", 0, QApplication::UnicodeUTF8));
        menuOptions->setTitle(QApplication::translate("ClippingWindow", "Operation", 0, QApplication::UnicodeUTF8));
        menuImage->setTitle(QApplication::translate("ClippingWindow", "Zoom", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("ClippingWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuToolBox->setTitle(QApplication::translate("ClippingWindow", "View", 0, QApplication::UnicodeUTF8));
        menuToolBars->setTitle(QApplication::translate("ClippingWindow", "ToolBars", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("ClippingWindow", "Filter1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ClippingWindow", "Filter2", 0, QApplication::UnicodeUTF8)
        );
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("ClippingWindow", "Filter1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ClippingWindow", "Filter2", 0, QApplication::UnicodeUTF8)
        );
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("ClippingWindow", "Issues", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("ClippingWindow", "Date1", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(0, QApplication::translate("ClippingWindow", "Date2", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget->topLevelItem(2);
        ___qtreewidgetitem3->setText(0, QApplication::translate("ClippingWindow", "Date3", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(3);
        ___qtreewidgetitem4->setText(0, QApplication::translate("ClippingWindow", "Date4", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
        ___qtreewidgetitem5->setText(0, QApplication::translate("ClippingWindow", "Section1", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem4->child(1);
        ___qtreewidgetitem6->setText(0, QApplication::translate("ClippingWindow", "Section2", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem4->child(2);
        ___qtreewidgetitem7->setText(0, QApplication::translate("ClippingWindow", "Section3", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem7->child(0);
        ___qtreewidgetitem8->setText(0, QApplication::translate("ClippingWindow", "Page1", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem7->child(1);
        ___qtreewidgetitem9->setText(0, QApplication::translate("ClippingWindow", "Page2", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem7->child(2);
        ___qtreewidgetitem10->setText(0, QApplication::translate("ClippingWindow", "Page3", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem11 = ___qtreewidgetitem7->child(3);
        ___qtreewidgetitem11->setText(0, QApplication::translate("ClippingWindow", "Page4", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem12 = ___qtreewidgetitem7->child(4);
        ___qtreewidgetitem12->setText(0, QApplication::translate("ClippingWindow", "Page5", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem13 = ___qtreewidgetitem7->child(5);
        ___qtreewidgetitem13->setText(0, QApplication::translate("ClippingWindow", "Page6", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem14 = ___qtreewidgetitem4->child(3);
        ___qtreewidgetitem14->setText(0, QApplication::translate("ClippingWindow", "Section4", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem15 = treeWidget->topLevelItem(4);
        ___qtreewidgetitem15->setText(0, QApplication::translate("ClippingWindow", "Date5", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem16 = treeWidget->topLevelItem(5);
        ___qtreewidgetitem16->setText(0, QApplication::translate("ClippingWindow", "Date6", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem17 = treeWidget->topLevelItem(6);
        ___qtreewidgetitem17->setText(0, QApplication::translate("ClippingWindow", "Date7", 0, QApplication::UnicodeUTF8));
        treeWidget->setSortingEnabled(__sortingEnabled);


        const bool __sortingEnabled1 = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("ClippingWindow", "Clipped Article 1", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled1);

        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ClippingWindow", "Keywords", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ClippingWindow", "Comments", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("ClippingWindow", "1", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("ClippingWindow", "2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("ClippingWindow", "3", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled2 = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->item(0, 0);
        ___qtablewidgetitem5->setText(QApplication::translate("ClippingWindow", "Keyword", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->item(0, 1);
        ___qtablewidgetitem6->setText(QApplication::translate("ClippingWindow", "This is a comment", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->item(1, 0);
        ___qtablewidgetitem7->setText(QApplication::translate("ClippingWindow", "Keyword 2", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->item(1, 1);
        ___qtablewidgetitem8->setText(QApplication::translate("ClippingWindow", "no comment is here", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(2, 0);
        ___qtablewidgetitem9->setText(QApplication::translate("ClippingWindow", "Keyword 3 & 4", 0, QApplication::UnicodeUTF8));
        tableWidget->setSortingEnabled(__sortingEnabled2);

        pushButton->setText(QApplication::translate("ClippingWindow", "Search", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("ClippingWindow", "PushButton", 0, QApplication::UnicodeUTF8));
        operationToolBar->setWindowTitle(QApplication::translate("ClippingWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        scrollToolBar->setWindowTitle(QApplication::translate("ClippingWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        manipulationToolBar->setWindowTitle(QApplication::translate("ClippingWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ClippingWindow: public Ui_ClippingWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIPPINGWINDOW_H
