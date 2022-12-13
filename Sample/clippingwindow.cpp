 #include <QtGui>
#include "clippingwindow.h"
#include "ui_clippingwindow.h"



ClippingWindow::ClippingWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClippingWindow)
{
    ui->setupUi(this);
/*
    QStringList wordlist;
    wordlist << " News paper1" << " Newspaper2" << " Aljazeera" << " xyz" << " Abc" << " Sample";

    completer = new QCompleter(wordlist, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_2->setCompleter(completer);

*/

    QStringListModel * model = new QStringListModel;

    for (int x = 0; x < 1000; x++)
    {
        model->insertRow(model->rowCount());
        model->setData(model->index(model->rowCount() - 1, 0), QString("hello%1").arg(x), Qt::DisplayRole);
    }

    QSortFilterProxyModel * proxy = new QSortFilterProxyModel;
    proxy->setSourceModel(model);

    ui->comboBox->setModel(proxy);
    ui->comboBox->setEditable(true);
    ui->comboBox->setCompleter(0);

    // When the edit text changes, use it to filter the proxy model.
    connect(ui->comboBox, SIGNAL(editTextChanged(QString)), proxy, SLOT(setFilterWildcard(QString)));

}

ClippingWindow::~ClippingWindow()
{
    delete ui;
}
