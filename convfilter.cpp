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
    QString subimageFilename = QFileDialog::getSaveFileName(this, "Save this subimage");
    // Open a QFile object to handle file I/O
    QFile subimageFile(subimageFilename);
    // Error handling for opening a write only file.
    if (!subimageFile.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Warning", "Couldn't save subimage as text file" + subimageFile.errorString());
        return;
    }
    // Read text in the plain text input fields.
    // outFile is a new instance of a QTextStream object. It's kind of like the iostream objects in the stdio library
    QTextStream outFile(&subimageFile);
    // Placeholder QString variable for holding what was written in the text field of the ui
    QString newFilterValueString = ui->subimageValuesTextBox->toPlainText();
    // Dump the string into the new file. QTextStream objects also have the overloaded << operator
    outFile << newFilterValueString;
    // Close the file
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
