/*-------------------------------------------------------------------------*/
/*----------------------- F l o a t I m a g e  .  h -----------------------*/
/*-------------------------------------------------------------------------*/

/*
Name: Daniel Lynch
Date: April 12, 2019
Description: This header file contains a class for a 2D grayscale image and
function prototypes used to process 2D images and perform spatial domain 
analysis on 2D images. It was created to be used for Lab 3 of the EECE4840 
Computer Vision and Digital Image Processing course at UMass Lowell.

Log: 
04/12/2019: DML I started planning out what the class object should look like
and what functions I might need.
should do.
04/14/2019: I started the writing the functions for reading and writing a PGM
file.
*/

#ifndef FLOATIMAGE_H
#define FLOATIMAGE_H

/*-------------------------------------------------------------------------*/
/*--------- F l o a t I m a g e  C l a s s   D e f i n i t i o n ----------*/
/*------------- a n d   F u n c t i o n   P r o t o t y p e s -------------*/
/*-------------------------------------------------------------------------*/

static const unsigned int MAXBUFFERSIZE = 200;
static const unsigned int MAXSTRINGSIZE = 128;
static const unsigned int NUMBERHEADERARGS = 4;
static const unsigned int MAXFILENAMESIZE = 100;
static const unsigned int NUMBEROFDIMENSIONS = 3;
static const unsigned int CUTOFFVALUE = 127;
static const unsigned int RANDOMMODULUS = 10;
static const unsigned int DEFAULTRANDOMFACTOR = 0;

class FloatImage
{
	public:
		// Constructors
		FloatImage();
		FloatImage(unsigned int r, unsigned int c, unsigned int l);

		// Destructor
		~FloatImage();

        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getLevels();

        void setSize(unsigned int &headerRows, unsigned int &headerCols, unsigned int &headerLevels);

		//Functions
		void readInPGMImage(char *fileName);
		void writePGM(char *fileName);
		void copyAndPadImage(const FloatImage &originalImage, unsigned int pad);
		void applyLookUp(float *lookupTable, unsigned int tableSize);
		void normalizeImage();
		void addNoise(unsigned int noiseFactor);
		void applyFilter(const FloatImage &paddedImage, const unsigned int &size, const unsigned int &pad, float **filterVals);
		float compareImage(const FloatImage &refImage);

	private:
		// Number of rows
		unsigned int rows;
		// Number of columns
		unsigned int cols;
		// Number of levels
		unsigned int levels;
		// 2D array of float values to serve as image
		float **values;

};

/*-------------------------------------------------------------------------*/
/*------- S O M E   F U N C T I O N   P R O T O T Y P E S  F O R ----------*/
/*------------- P G M   F I L E   I / O  F U N C T I O N S ----------------*/
/*---------- A N D   P O S S I B L Y   O T H E R   N E E D E D-------------*/
/*------------------------- F U N C T I O N S -----------------------------*/
/*-------------------------------------------------------------------------*/

// Read First line of PGM file for info about size of image 
void readPGMHeader(char *fileName, unsigned int &rows, unsigned int &cols, unsigned int &levels);

#endif
