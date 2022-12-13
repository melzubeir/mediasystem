#ifndef SQLDialog_H
#define SQLDialog_H

#include <QtGui>
#include <QtSql>


namespace Ui
{
    class SQLDialog;
}

class SQLDialog : public QDialog
{
    Q_OBJECT

public:
    SQLDialog(QWidget *parent = 0);
    ~SQLDialog();

    void readSettings();
    void connectToDB();
private:
    Ui::SQLDialog *ui;

    QString fileName;
    QString fileContent;

    QString m_masterDatabaseHost;
    QString m_masterDatabaseName;
    QString m_masterDatabaseUser;
    QString m_masterDatabasePassword;

    int m_masterDatabasePort;

    QSqlDatabase m_masterDatabase;

private slots:
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // SQLDialog_H
