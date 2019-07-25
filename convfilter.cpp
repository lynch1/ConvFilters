#include "convfilter.h"
#include "ui_convfilter.h"

ConvFilter::ConvFilter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConvFilter)
{
    ui->setupUi(this);

    //    // Practice displaying an image
    //    QPixmap testImage("C:/Users/Damn/Documents/ConvFilters/ExamplePhotos/liver1.pgm");
    //    ui->originalImageLabel->setPixmap(testImage.scaled(ui->originalImageLabel->width(),ui->originalImageLabel->height(),Qt::KeepAspectRatio));

    // Set initial original image
    originalImageFilename = "C:/Users/Damn/Documents/ConvFilters/ExamplePhotos/liver1.pgm";
    // Set filterSelected() to no
    filterSelected = false;
    imageSelected = false;

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

    // Copy the values of filter to filtervalues
    filterValues = currentFilter.getValues();

    // Display filter
    QString filterFileContent = "";
    float tempFloat = 0;
    for(unsigned int i = 0; i < currentFilter.getSize(); i ++){
        for(unsigned int j = 0; j < currentFilter.getSize(); j++){
            tempFloat = filterValues[i][j];
            filterFileContent.append(QString::number(tempFloat));
            filterFileContent.append(" ");
            }
        filterFileContent.append("\n");
    }
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

    imageSelected = true;
}

// Filter the opened image with the selected filter
void ConvFilter::filterImage(){

    // Needed to convert QString to char* in order to use FloatImage modules writePGM function.
    QByteArray origFilenameArray = originalImageFilename.toLocal8Bit();
    char* origImageFilename = origFilenameArray.data();

    // Check if there is a filter loaded into
    if (filterSelected == false){
        // If not tell user to choose a filter
        qDebug("You didn't choose a filter.");
    }
    else if (imageSelected == false){
        // If not tell user to choose an image
        qDebug("You didn't choose an image.");
    }
    else{
    // Get header info
    // STOPPED HERE
    // Get original image sizes and required padding
    unsigned int r;
    unsigned int c;
    unsigned int levs;
    unsigned int p;

    readPGMHeader(origImageFilename, r, c, levs);
    p = currentFilter.getReqPad();

    // Read in original image as a float image
    FloatImage originalImage(r, c, levs);
    // In debugging r, c, and levs are zero here. p is 1.
    originalImage.readInPGMImage(origImageFilename);

    // Make an appropriately sized padded image
    FloatImage paddedImage((r + (p*2)), (c + (p*2)), levs);
    paddedImage.copyAndPadImage(originalImage, p);

    FloatImage filteredImage(r, c, levs);

    // Filter the image and normalize
    filteredImage.applyFilter(paddedImage, currentFilter.getSize(), p, filterValues);
    filteredImage.normalizeImage();

    // Write image aas temporary file
    QByteArray tempFilenameArray = tempFilteredImageFilename.toLocal8Bit();
    char* tempImageFilename = tempFilenameArray.data();
    originalImage.writePGM(tempImageFilename);

    // Display filtered image
    QPixmap displayedImage(tempFilteredImageFilename);
    ui->filteredImageLabel->setPixmap(displayedImage.scaled
        (ui->filteredImageLabel->width(),ui->filteredImageLabel->height(),
        Qt::KeepAspectRatio));
    }
}


//void saveFilter(){
//    QFile tempFilterFile(tempFilterFilename);
//    if (tempFilterFile.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
//        QTextStream outStream(&tempFilterFile);
//        outStream << "FS\n" << currentFilter.getSize() << "\n";
//        for(unsigned int i = 0; i < currentFilter.getSize(); i ++){
//            for(unsigned int j = 0; j < currentFilter.getSize(); j++){
//                outStream << filterValues[i][j] << " ";
//                }
//        outStream << "\n";
//    }
