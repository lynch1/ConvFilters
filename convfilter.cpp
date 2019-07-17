#include "convfilter.h"
#include "ui_convfilter.h"

ConvFilter::ConvFilter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConvFilter)
{
    ui->setupUi(this);

    // Practice displaying an image
    QPixmap testImage("C:/Users/Damn/Documents/ConvFilters/ExamplePhotos/liver1.pgm");
    ui->originalImageLabel->setPixmap(testImage.scaled(ui->originalImageLabel->width(),ui->originalImageLabel->height(),Qt::KeepAspectRatio));

    // Connect signals to slots
    // Signal for when openSubimageButton QPushButton is released
    connect(ui->openSubimageButton, SIGNAL(released()), this, SLOT(openFilter()));
    // Signal for when openImageButton QPushButton is released
    connect(ui->openImageButton, SIGNAL(released()), this, SLOT(openImage()));
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
    subimageFile.close();
}

// Function / Slot to open up a new image, display it, and then created a FloatImage Object of it
void ConvFilter::openImage(){

    // Get filename from getOpenFileName() as a QString and convert it to a char array
    QString openImageFilename = QFileDialog::getOpenFileName(this, "Open a subimage");
    QByteArray filenameArray = openImageFilename.toLocal8Bit();
    char* filename = filenameArray.data();
    // Open a QFile filestream
    QFile openImageFile(openImageFilename);

    // Read PGM header and set originalImage based on this info
    // Initialize some variables for saving header info
    unsigned int imageRows = 0;
    unsigned int imageCols = 0;
    unsigned int imageLevels = 0;
    // read the header on the filter file to get the dimensions
    // Images are expected to be in the PGM format, i.e. "P5"
    readPGMHeader(filename, imageRows, imageCols, imageLevels);
    // Use setSize() to put .pgm file contents into origImage.
    // Don't forget to free any memory from an older values[]
    origFloatImage.setSize(imageRows, imageCols, imageLevels);
    // Read the values into the FloatImage instance
    origFloatImage.readInPGMImage(filename);

    // Now display the .pgm image in the left label area
    // Practice displaying an image
    QPixmap originalImagePGM(filename);
    ui->originalImageLabel->setPixmap(originalImagePGM.scaled(ui->originalImageLabel->width(),ui->originalImageLabel->height(),Qt::KeepAspectRatio));

    openImageFile.close();
}

// Filter the opened image with the selected filter
void ConvFilter::filterImage()
{
    // Get original image sizes and required padding
    unsigned int w = origFloatImage.getWidth();
    unsigned int h = origFloatImage.getHeight();
    unsigned int l = origFloatImage.getLevels();
    unsigned int p = workingFilter.getReqPad();

    FloatImage paddedImage((h + 2 * p),(w + 2 * p), l );
    FloatImage workingImage((h + 2 * p),(w + 2 * p), l );
    paddedImage.copyAndPadImage(origFloatImage, p);
    workingImage.applyFilter(paddedImage, 8, p, workingFilter.getValues());


}
