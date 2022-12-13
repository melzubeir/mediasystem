#ifndef CLIPPINGWINDOW_H
#define CLIPPINGWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class ClippingWindow;
}


class QCompleter;

class ClippingWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClippingWindow(QWidget *parent = 0);
    ~ClippingWindow();

private:
    Ui::ClippingWindow *ui;
};

#endif // CLIPPINGWINDOW_H
