#include "convfilter.h"
#include "ui_convfilter.h"

ConvFilter::ConvFilter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConvFilter)
{
    ui->setupUi(this);

    // Connect signals to slots
    // Signal for when openSubimageButton QPushButton is released
    connect(ui->openSubimageButton, SIGNAL(released()), this, SLOT(openFilter()));

    // Practice displaying an image
    QPixmap testImage("C:/Users/Damn/Documents/ConvFilters/ExamplePhotos/liver1.pgm");
    ui->originalImageLabel->setPixmap(testImage);
}

ConvFilter::~ConvFilter()
{
    delete ui;
}


void ConvFilter::openFilter()
{
    QString subimageFilename = QFileDialog::getOpenFileName(this, "Open a subimage");
    // Error handling for opening a sub image file.

    // Use functions from FilterImage module to read the filter file.
    // The annoying thing is that my input for the reading functions in the FilterImage
    // class take the <char *> type for the input filename and the Qt getOpenFileName()
    // function returns the filename as a QString.
    // I got a little stuck so had to use a work around I found on stack overflow:
    // https://stackoverflow.com/questions/17936160/clean-way-to-convert-qstring-to-char-not-const-char
    QByteArray filenameArray = subimageFilename.toLocal8Bit();
    char* filename = filenameArray.data();
    QFile subimageFile(subimageFilename);
    // Use functions from FilterImage module to read filter file
    unsigned int filterSize = 0;

    // read the header on the filter file to get the dimensions
    // Filters are expected to be square and odd, i.e. 3 X 3, 5 X 5
    readFilterHeader(filename, filterSize);

    // Create an instance of a FilterImage of the right size
    FilterImage openedFilter(filterSize);

    // Read the values into the FilterImage instance
    openedFilter.readFilterImage(filename);
    // Update the filtervalues member
    filterValues = *openedFilter.getValues();

    // Display the filter values in the text area
    // First convert the array of floats into a text formatted string
    QString valuesFromFilter = "";
    QString placeHolderString = "";
    for (unsigned int i = 0; i < filterSize ; i ++){
        for (unsigned int j = 0; j < filterSize ; j ++){
        placeHolderString = QString::number(filterValues[i]);
        valuesFromFilter.append(placeHolderString + " ");
        }
        valuesFromFilter.append("\n");
    }
    // Display the contents of the string in the text display on the UI
    ui->subimageValuesTextBox->setPlainText(valuesFromFilter);
}
