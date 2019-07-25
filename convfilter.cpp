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

    // Set initial original image
    originalImageFilename = "C:/Users/Damn/Documents/ConvFilters/ExamplePhotos/liver1.pgm";
    // Set filterSelected() to no
    filterSelected = false;

    // Connect signals to slots
    // Signal for when openSubimageButton QPushButton is released
    connect(ui->openSubimageButton, SIGNAL(released()), this, SLOT(openFilter()));
    // Signal for when openImageButton QPushButton is released
    connect(ui->openImageButton, SIGNAL(released()), this, SLOT(openImage()));
    // Signal for when openImageButton QPushButton is released
    connect(ui->filterImageButton, SIGNAL(released()), this, SLOT(filterImage()));
}

ConvFilter::~ConvFilter()
{
    delete ui;
}

// Open an already existing text file that describes a filter
// Format needs to be specified either as popup message, in help, or both
void ConvFilter::openFilter(){

    currentFilterFilename = QFileDialog::getOpenFileName(this, "Open a subimage");

    // The annoying thing is that my input for the reading functions in the FilterImage
    // class take the <char *> type for the input filename and the Qt getOpenFileName()
    // function returns the filename as a QString.
    // I got a little stuck so had to use a work around I found on stack overflow:
    // https://stackoverflow.com/questions/17936160/clean-way-to-convert-qstring-to-char-not-const-char
    QByteArray filterFilenameArray = currentFilterFilename.toLocal8Bit();
    char* filterFilename = filterFilenameArray.data();

    // Use functions from FilterImage module to read filter file
    unsigned int filterSize = 0;

    // read the header on the filter file to get the dimensions
    // Filters are expected to be square and odd, i.e. 3 X 3, 5 X 5
    readFilterHeader(filterFilename, filterSize);

    // Reset currentFilter
    currentFilter.resetFilter(filterSize);
    // Read the values into the currentFilter FilterImage instance
    currentFilter.readFilterImage(filterFilename);

    // Open Qfile stream object
    QFile filterFile(currentFilterFilename);
    filterFile.open(QIODevice::ReadOnly);
    QTextStream inStream(&filterFile);
    QString filterFileContent = inStream.readAll();
    filterFile.close();
    // Display the contents of the string in the text display on the UI
    ui->subimageValuesTextBox->setPlainText(filterFileContent);

    // Set bool filterSelected to true
    filterSelected = true;
}

// Function/Slot to open up a new image, display it, and update
// originalImageFilename
void ConvFilter::openImage(){

    // Get filename from getOpenFileName() as a QString
    originalImageFilename = QFileDialog::getOpenFileName(this, "Open a subimage");

    // Display the .pgm image in the left label area
    // Practice displaying an image
    QPixmap originalImagePGM(originalImageFilename);
    ui->originalImageLabel->setPixmap(originalImagePGM.scaled
        (ui->originalImageLabel->width(),ui->originalImageLabel->height(),
        Qt::KeepAspectRatio));
}

// Filter the opened image with the selected filter
void ConvFilter::filterImage(){
    // Get original image sizes and required padding
    unsigned int r;
    unsigned int c;
    unsigned int levs;
    unsigned int p;

    // Needed to convert QString to char* in order to use FloatImage modules writePGM function.
    QByteArray origFilenameArray = originalImageFilename.toLocal8Bit();
    char* origImageFilename = origFilenameArray.data();

    // Check if there is a filter loaded into
    if (filterSelected == false){
        // If not tell user to choose a filter
    }
    // Get header info



    // Save a temporary copy of the filtered image
    QString tempFilename ="workingImage.pgm";
    // Needed to convert QString to char* in order to use FloatImage modules writePGM function.
    QByteArray tempFilenameArray = tempFilename.toLocal8Bit();
    char* tempImageFilename = tempFilenameArray.data();
    // Write PGM. writePGM handles file i/o from here.

    // STOPPED HERE

//    //Display the filtered image
//    // Now display the .pgm image in the right label area
//    QFile openTempFile(tempFilename);
//    QPixmap tempImagePGM(tempFilename);
//    ui->filteredImageLabel->setPixmap(tempImagePGM.scaled(ui->filteredImageLabel->width(),ui->filteredImageLabel->height(),Qt::KeepAspectRatio));

//   openTempFile.close();

}
