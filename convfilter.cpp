#include "convfilter.h"
#include "ui_convfilter.h"

ConvFilter::ConvFilter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConvFilter)
{
    ui->setupUi(this);
}

ConvFilter::~ConvFilter()
{
    delete ui;
}



void ConvFilter::on_saveSubimageButton_released()
{
    QString subimageFilename = QFileDialog::getOpenFileName(this, "Save a subimage");
    QFile subimageFile(subimageFilename);

    // Error handling for opening a sub image file.
    if (!subimageFile.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Couldn't save filter" + subimageFile.errorString());
        return;
    }
    // Read text in the plain text input fields.
//    QTextStream outFile(&subimageFile);
//    QString newFilterValues = "test";
//    QString filterValues = inFile.readAll();
//    ui->subimageValuesTextBox->setPlainText(filterValues);
    subimageFile.close();
}

void ConvFilter::on_loadSubimageButton_released()
{
    QString subimageFilename = QFileDialog::getOpenFileName(this, "Open a subimage");
    QFile subimageFile(subimageFilename);
    currentFilter = subimageFilename;
    // Error handling for opening a sub image file.
    if (!subimageFile.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Couldn't load filter" + subimageFile.errorString());
        return;
    }
    // Read in file contents
    QTextStream inFile(&subimageFile);
    QString filterValues = inFile.readAll();
    ui->subimageValuesTextBox->setPlainText(filterValues);
    subimageFile.close();
}
