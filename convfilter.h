#ifndef CONVFILTER_H
#define CONVFILTER_H

#include <QMainWindow>

namespace Ui {
class ConvFilter;
}

class ConvFilter : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConvFilter(QWidget *parent = nullptr);
    ~ConvFilter();

private:
    Ui::ConvFilter *ui;
};

#endif // CONVFILTER_H
