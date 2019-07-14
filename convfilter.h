#ifndef CONVFILTER_H
#define CONVFILTER_H

#include <QMainWindow>
// For opening files
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QFileDialog>
// For giving user error messages
#include <QMessageBox>

namespace Ui {
class ConvFilter;
}

// ConvFilter's base class is a Q_OBJECT
class ConvFilter : public QMainWindow
{
    Q_OBJECT

public:
    // Default constructore with no parent
    explicit ConvFilter(QWidget *parent = nullptr);
    // Destructor
    ~ConvFilter();

private slots:
    void on_saveSubimageButton_released();

    void on_loadSubimageButton_released();

private:
    // The UI Form
    Ui::ConvFilter *ui;
    QString currentFilter = "";
    double *filterValues[9];
};

#endif//CONVFILTER_H
