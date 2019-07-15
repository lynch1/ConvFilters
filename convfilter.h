#ifndef CONVFILTER_H
#define CONVFILTER_H

#define MAXFILTERSIZE 10

#include <QMainWindow>
// For opening files
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QFileDialog>
// For giving user error messages
#include <QMessageBox>
#include <QPixmap>

// Some classes from sobel edge detection program
// I wrote for a class assignent last spring.
#include "FilterImage.h"
#include "FloatImage.h"

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
    void openFilter();

private:
    // The UI Form
    Ui::ConvFilter *ui;
    QString currentFilter = "";
    // Might not need
    float *filterValues;
    FilterImage workingFilter;
    FloatImage origFloatImage;
    FloatImage filtFloatImage;
};

#endif//CONVFILTER_H
