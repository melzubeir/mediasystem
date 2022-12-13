/********************************************************************************
** Form generated from reading ui file 'emsocrdialog.ui'
**
** Created: Mon 8. Mar 12:01:37 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EMSOCRDIALOG_H
#define UI_EMSOCRDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_emsOCRDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *startButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *currentLineEdit;
    QSpacerItem *verticalSpacer_3;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QListWidget *queueWidget;
    QSpacerItem *horizontalSpacer_2;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QTableWidget *processedTableWidget;
    QSpacerItem *verticalSpacer_4;
    QLabel *totalPagesLabel;

    void setupUi(QDialog *emsOCRDialog)
    {
        if (emsOCRDialog->objectName().isEmpty())
            emsOCRDialog->setObjectName(QString::fromUtf8("emsOCRDialog"));
        emsOCRDialog->setWindowModality(Qt::WindowModal);
        emsOCRDialog->resize(541, 717);
        gridLayout = new QGridLayout(emsOCRDialog);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        startButton = new QPushButton(emsOCRDialog);
        startButton->setObjectName(QString::fromUtf8("startButton"));

        horizontalLayout->addWidget(startButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(emsOCRDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        currentLineEdit = new QLineEdit(emsOCRDialog);
        currentLineEdit->setObjectName(QString::fromUtf8("currentLineEdit"));
        currentLineEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(currentLineEdit);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(17, 6, QSizePolicy::Minimum, QSizePolicy::Minimum);

        verticalLayout_3->addItem(verticalSpacer_3);


        gridLayout->addLayout(verticalLayout_3, 1, 0, 1, 1);

        splitter = new QSplitter(emsOCRDialog);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setPointSize(10);
        label_4->setFont(font);

        verticalLayout->addWidget(label_4);

        queueWidget = new QListWidget(layoutWidget);
        queueWidget->setObjectName(QString::fromUtf8("queueWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(queueWidget->sizePolicy().hasHeightForWidth());
        queueWidget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(queueWidget);

        horizontalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(horizontalSpacer_2);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        processedTableWidget = new QTableWidget(layoutWidget1);
        if (processedTableWidget->columnCount() < 4)
            processedTableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        processedTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        processedTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        processedTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        processedTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        processedTableWidget->setObjectName(QString::fromUtf8("processedTableWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(processedTableWidget->sizePolicy().hasHeightForWidth());
        processedTableWidget->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(processedTableWidget);

        splitter->addWidget(layoutWidget1);

        gridLayout->addWidget(splitter, 2, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Minimum);

        gridLayout->addItem(verticalSpacer_4, 3, 0, 1, 1);

        totalPagesLabel = new QLabel(emsOCRDialog);
        totalPagesLabel->setObjectName(QString::fromUtf8("totalPagesLabel"));
        totalPagesLabel->setFont(font);

        gridLayout->addWidget(totalPagesLabel, 4, 0, 1, 1);


        retranslateUi(emsOCRDialog);

        QMetaObject::connectSlotsByName(emsOCRDialog);
    } // setupUi

    void retranslateUi(QDialog *emsOCRDialog)
    {
        emsOCRDialog->setWindowTitle(QApplication::translate("emsOCRDialog", "socialhose OCR Engine", 0, QApplication::UnicodeUTF8));
        startButton->setText(QApplication::translate("emsOCRDialog", "Start", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("emsOCRDialog", "Current Page", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("emsOCRDialog", "Pages in Queue:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("emsOCRDialog", "Pages Processed", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = processedTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("emsOCRDialog", "Date/Time", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = processedTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("emsOCRDialog", "Page", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = processedTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("emsOCRDialog", "Time of processing", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = processedTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("emsOCRDialog", "# of keywords", 0, QApplication::UnicodeUTF8));
        totalPagesLabel->setText(QApplication::translate("emsOCRDialog", "Total Pages Processed", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(emsOCRDialog);
    } // retranslateUi

};

namespace Ui {
    class emsOCRDialog: public Ui_emsOCRDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMSOCRDIALOG_H
