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
    ui->originalImageLabel->setPixmap(testImage.scaled(ui->originalImageLabel->width(),ui->originalImageLabel->height(),Qt::KeepAspectRatio));
}

ConvFilter::~ConvFilter()
{
    delete ui;
}

// Open an already existing text file that describes a filter
// Format needs to be specified either as popup message, in help, or both
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
    // Update the filtervalues member usint the FilterImage function getValues()
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

// Function / Slot to open up a new image, display it, and then created a FloatImage Object of it
void ConvFilter::openImage(){
    // Get filename from getOpenFileName() as a QString and convert it to a char array
    QString openImageFilename = QFileDialog::getOpenFileName(this, "Open a subimage");
    QByteArray filenameArray = openImageFilename.toLocal8Bit();
    char* filename = filenameArray.data();
    // Open a QFile filestream
    QFile subimageFile(openImageFilename);

    // Initialize some variables for saving header info
    unsigned int imageRows = 0;
    unsigned int imageCols = 0;
    unsigned int imageLevels = 0;
    // read the header on the filter file to get the dimensions
    // Images are expected to be in the PGM format, i.e. "P5"
    readPGMHeader(filename, imageRows, imageCols, imageLevels);


    // Create an instance of a FloatImage of the right size
    FloatImage openedImage(imageRows, imageCols, imageLevels);

    // Read the values into the FloatImage instance
    openedImage.readInPGMImage(filename);

    // Update the filtervalues member usint the FilterImage function getValues()
    // Need to add a member to convfilter to hold the FloatImage object.
    //
    // filterValues = *openedFilter.getValues();
    // origFloatImage = *openedImage();
}
