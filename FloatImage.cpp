/*-------------------------------------------------------------------------*/
/*----------------- F l o a t I m a g e  .  c p p -------------------------*/
/*-------------------------------------------------------------------------*/

/*
Name: Daniel Lynch
Date: April 12, 2019
Description: This file contains functions used to process 2D images and per-
form spatial domain analysis on 2D images. It was created to be used for Lab
3 of the EECE4840 Computer Vision and Digital Image Processing course in 
the Spring of 2019 at UMass Lowell. I used notes from the following websites
for making classes and making an image class in C++:
http://pages.cs.wisc.edu/~hasti/cs368/CppTutorial/NOTES/CLASSES-INTRO.html
https://www.dreamincode.net/forums/topic/76816-image-processing-tutorial/
http://www.cplusplus.com/reference/fstream/ifstream/open/
https://www.geeksforgeeks.org/reinterpret_cast-in-c-type-casting-operators/
http://www.cplusplus.com/doc/tutorial/files/
http://www.cplusplus.com/reference/istream/istream/read/
http://www.cplusplus.com/reference/string/to_string/

Log: 
04/12/2019: DML I started planning out what the class object should look like
and what functions I might need.
should do.
04/14/2019: I started the writing the functions for reading and writing a PGM
file.
04/20/2019: I continued working on the project going. I was able to make a 
function for reading the header info. Now I'm working on getting it to read
the rest of the file with the intensity values. I think I'm just going to make
it so that the subimages (tiles used for filtering) are also just very small
PGM images. Then when the user calls the program it will generate the PGM's 
for the subimages if it hasn't already. Also in the future it would be nice
to write a program that would input a PGM and a filter PGM and then just apply
it. A user could write their own filters and see what it does. 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "FloatImage.h"
#include "FilterImage.h"

using namespace std;

/*-------------------------------------------------------------------------*/
/*------------ F l o a t I m a g e  C o n s t r u c t o r s ---------------*/
/*-------------------------------------------------------------------------*/

// Default constructor creates an empty FloatImage of 0 X 0 size with and an
// of values that is just a null pointer
FloatImage::FloatImage()
{
	rows = 0;
	cols = 0;
	levels = 0;
    values = nullptr;
}

// This constructor loads an image according to the sizes specified in the 
// arguments and loads a 2D array of zeroes by default, but you can specify 
// a different fill as the fourth argument.
FloatImage::FloatImage(unsigned int r, unsigned int c, unsigned int l)
{
	rows = r;
	cols = c;
	levels = l;
	// Load 2D array with fill values
	values = new float *[rows];
	for (unsigned int i  = 0; i < rows; i++)
	{
		values[i] = new float [cols];
		for (unsigned int j = 0; j < cols; j ++)
		{
			values[i][j] = 0.0;
		}
	}
}

// This is a destructor so you don't have to keep deleting FloatImage ins-
// tances in main()
FloatImage::~FloatImage(void)
{

	for (unsigned int i = 0; i < rows; i++)
	{
		delete values[i];
	}
	delete values;

	// Print message so we know the instance was deleted
	cout << "Deleting FloatImage object instance to free memory!\n" << endl;
}

/*-------------------------------------------------------------------------*/
/*--------------- F l o a t I m a g e  F u n c t i o n s ------------------*/
/*-------------------------------------------------------------------------*/

// For caller to get value of Float Image instance's width
unsigned int FloatImage::getHeight()
{
	return rows;
}

// For caller to get value of Float Image instance's height
unsigned int FloatImage::getWidth()
{
	return cols;
}

// For caller to get value of Float Image instance's maximum level of intensity
unsigned int FloatImage::getLevels()
{
	return levels;
}

void FloatImage::setSize(unsigned int &headerRows, unsigned int &headerCols, unsigned int &headerLevels){
    // Set the size info from the input arguments
    rows = headerRows;
    cols = headerCols;
    levels = headerLevels;

    // Load 2D array with fill values
    values = new float *[rows];
    for (unsigned int i  = 0; i < rows; i++)
    {
        values[i] = new float [cols];
        for (unsigned int j = 0; j < cols; j ++)
        {
            values[i][j] = 0.0;
        }
    }

}

void FloatImage::readInPGMImage(char *fileName)
{
	// Open file and use as a stream 	
  	ifstream inFile(fileName);
	if (inFile) 
	{	
		cout << "Opened the file " << fileName << "." << endl;

	}
	// The file couldn't be opened so tell the user.
	else
	{
		cout << "Cannot open the file " << fileName << "." << endl;
		cout << "Exiting program. Try again with a different file" << endl;
		exit(EXIT_FAILURE);
	}
	int pixelSize = 1;
	if (levels > 255)
	{
		pixelSize = 2;
	}
	// Get ImageSize in bytes
	int imageSize  = rows * cols * pixelSize; 
	// Calculate size of block of data
	int dataSize = imageSize * sizeof(unsigned char);
	// For debugging pring image size
	cout << "Image size is: " << imageSize << endl;
	
	// Go to the end of file
	inFile.seekg(0, inFile.end);
	// Save how big the file stream is
	int fileLength = inFile.tellg();
	// Find the start of the raster data
	int imageStart = fileLength - imageSize;
	// For debuging
	cout << "Starting position in size units is: " << imageStart << endl;
	// Go to the begining of the raster
	inFile.seekg(imageStart);
	// Placeholders for the loop
	unsigned char* placeHolder;
	unsigned int tempValue;
 	float tempFloat; 
	// Allocate space for a placeholder array of the chars read in
	placeHolder = (unsigned char *) new char[imageSize];
	// Read in the image data as a block
	// Have to use reinterpret_cast because of the way the istream.read()
	// function works.
	inFile.read(reinterpret_cast<char *>(placeHolder), dataSize);
	// Loop through the 2D image pixel by pixel
	for(unsigned int y = 0; y < rows; y+= pixelSize)
	{
		for(unsigned int x = 0; x < cols; x+= pixelSize)
		{
			// Cast the char value in placeHolder at this x, y position 
			// into an int.
			tempValue = (unsigned int) placeHolder[(y * cols) + x];
			// Cast the int into a float
			tempFloat = (float) tempValue;
			values [y][x] = tempFloat;
			// // For debugging
			// cout << tempFloat << " " ;
		}
	}

	// Get rid of the placeHolder array and free up memory
	delete(placeHolder);

	// Close the input file. 
	inFile.close();
	cout << "Closing the file " << fileName << ".\n" << endl;

	// // For debugging
	// cout << "The pixel in positon (4, 4) is " << values[4][4] << endl; 

}

// This function loads an makes a FloatImageInstance that is a copy of
// an already existing FloatImage instance.
void FloatImage::copyAndPadImage(const FloatImage &refImage, unsigned int pad)
{
	// If this float image instance isn't a proper dimension
	// for the given padding and reference image, then error
	if ((rows != refImage.rows + (2 * pad)) || 
		(cols != refImage.cols + (2 * pad))  ||
		(levels != refImage.levels))
	{
	cout << "Cannot copy this image with this padding.\n" << endl;
		exit(EXIT_FAILURE);
	}
	// Load 2D array with the values from original image values
	// Load 2D array with fill values
	else
	{
		// Fill in the value if it is needed
		for (unsigned int y  = pad; y < (rows - pad); y++)
		{	
			for (unsigned int x = pad; x < (cols - pad); x ++)
			{
				values[y][x] = refImage.values[y - pad][x-pad];
			}
		}
		cout << "Copied and padded image.\n" << endl;
	}

}

// Apply a threshold to the image in a look-up table
// It assumes the image in the FloatImage instance has already been normalized
void FloatImage::applyLookUp(float *lookupTable, unsigned int tableSize)
{

// Make sure there are enough entries in the look-up table to 
// use for the levels on the image

	unsigned int index = 0;

	if (levels == tableSize)
	{
		for (unsigned int r = 0; r < rows; r ++)
		{
			for (unsigned int c = 0; c < cols; c++)
			{
				index = (unsigned int) values[r][c];
				values[r][c] = lookupTable[index];
				// // For debugging
				// cout << values[r][c] << " ";
			}	
			// // For debugging
			// cout << endl;
		}
		cout << "Applied a look-up table to image.\n" << endl;
	}

	// If the size of the lookup table and levels of the image in the 
	// FloatImage instance do not match, then exit with error.
	else
	{
		cout << "Cannot apply a look-up table of size "<< tableSize;
		cout << " to an image with " << levels << " levels.\n" << endl;
		exit(EXIT_FAILURE);
	}
}


void FloatImage::normalizeImage()
{
	// Find current minumum and maximum in the FloatImage instance
	float xMin = 0;
	float xMax = 0;
	float temp = 0;
	for (unsigned int r = 0; r < rows; r ++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			temp = values[r][c];
			if (temp < xMin)
			{
				xMin = temp;
			}
			if (temp > xMax)
			{
				xMax = temp;
			}
		}
	}
	// Use the normalization equation to normalize the image. 
	// Also round using floor
	float newVal = 0;
	float range = xMax - xMin;
	for (unsigned int r = 0; r < rows; r ++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			temp = values[r][c];
			newVal = floor((levels * (temp - xMin)) / (range));
			// // For debugging
			// cout << newVal << " ";
			values[r][c] = newVal;
		
		}
		// // For debugging
		// cout << endl;
	}
}


// This function applies the filter to the float image by sliding it over every pixel 
// and summing the product of each overlapping pixel, i.e. discrete 2D convolution 
// size parameter refers to the size of the filter.
void FloatImage::applyFilter(const FloatImage &paddedImage, const unsigned int &size, const unsigned int &pad, float **filterVals)
{
	float tempFloat = 0;
	float tempSum = 0;
	// Check that the call to filter is do-able
	unsigned int filtSize = size;
	unsigned int padding = pad;
	if ((rows == (paddedImage.rows - (2 * padding))) && 
		(cols == (paddedImage.cols - (2 * padding))))
	{
		// If that condition is satisfied, we can do the convolution
		for (unsigned int r = 0; r < rows; r ++)
		{
			for (unsigned int c = 0; c < cols; c++)
			{
				tempSum = 0;

				for (unsigned int y = 0; y < filtSize; y++)
				{
					for (unsigned int x = 0; x < filtSize; x++)
					{
					tempFloat = (paddedImage.values[r + y][c + x]) * filterVals[y][x];
					tempSum = tempSum + tempFloat;
					}
				}
				values[r][c] = tempSum ;
			}
		}
	}
	else
	{
		cout << "There is a problem with the image size, original image \n";
		cout << " size and the filter size." << endl;
		exit(EXIT_FAILURE);
	}

}


void FloatImage::addNoise(unsigned int noiseFactor)
{
	// Seed rand() using time()
	srand(time(0));
	// Find current minumum and maximum in the FloatImage instance
	int noise = 0;
	
	// Go through image intensity values and add noise
	for (unsigned int r = 0; r < rows; r ++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			// Noise will be between -5 and 5 * noiseFactor
			noise = ((rand() % RANDOMMODULUS ) - (RANDOMMODULUS / 2)) * noiseFactor;
			values[r][c] = values[r][c] + ((float) noise);
		}
	}
	// Let's normalize the image in addNoise so we don't have to remember
	// to in the caller's scope
	normalizeImage();
}


// This function writes a PGM file for a FloatImageInstance to the
// filename specified in the argument. 
void FloatImage::writePGM(char *fileName)
{
	// Start a stream of the PGM file to read
  	ofstream outFile(fileName);

  	// If you can open the file as a stream, write to it
	if (outFile.is_open()) 
	{
		// Write header that has magic number, dimensions, and 
		// intensities
		outFile << "P5" << endl;
		outFile << rows << ' ' << cols<< endl;
		outFile << levels << endl;

		// Write data chunk of intensity values
		unsigned int imageSize = rows * cols;
		unsigned int pixelSize = 1;
		if (levels > 255)
		{
			pixelSize = 2;
		}
		unsigned int dataSize  = imageSize * pixelSize;

		// Load intensities into placeholder array

		unsigned char *placeHolder;
		placeHolder = (unsigned char *) new char[dataSize];

		for (unsigned int y  = 0; y < rows; y++)
		{		
			for (unsigned int x = 0; x < cols; x ++)
			{
				placeHolder[(y * cols) + x] = (unsigned char) (unsigned int) values[y][x];
			}
		}
		// ofstream::write requires the first argument to be a char *
		outFile.write(reinterpret_cast<char *>(placeHolder), dataSize);
		
		cout << "Wrote PGM image " << fileName << ".\n" << endl;
		
		delete placeHolder;
		
		// Otherwise wasn't able to open a file.
		outFile.close();
	}	
	else
	{
		cout << "Could not open file " << fileName << ".\n" <<endl;
	}
}


// This function is to take another FloatImage Instance compare it to this
// instance and return a score between 0 and 100 that represents the
// percentage of pixels at the same indices that are the same intensity. 
// If the images aren't the same size, it throws an error.
float FloatImage::compareImage(const FloatImage &refImage)
{
	unsigned int count = 0;
	// Are the dimensions the same
	if ((rows == refImage.rows)&&(cols == refImage.cols))
	{
		for (unsigned int r = 0; r < rows; r++)
		{
			for (unsigned int c = 0; c < cols; c++)
			{
				if (values[r][c] == refImage.values[r][c])
				{
					count++;
				}			
			}
		}
	}
	else
	{
		cout << "Cannot compare the two images because they are not the";
		cout << " same dimensions." << endl;
		exit(EXIT_FAILURE);
	}
	unsigned int numPixels = rows * cols;
	float percent = (((float) count) / ((float)numPixels)) * 100;
	return percent;

}
/*-------------------------------------------------------------------------*/
/*----- F U N C T I O N S   F O R   P G M   F I L E   I / O ---------------*/
/*-------------------------------------------------------------------------*/


// Read First line of PGM file for info about size of image 
void readPGMHeader(char *fileName, unsigned int &rows, unsigned int &cols, unsigned int &levels)
{
	char wordBuffer[MAXSTRINGSIZE];
	// Number or arguments we need to get from the PGM file defined in the 
  	// PGM specification
  	unsigned int headerArgCount = 0;
    bool magicNumberFlag = false;
    // I changed this from int to unsigned int on 7-15-2019
    unsigned int headerArgs[NUMBERHEADERARGS - 1];

  	// Start a stream of the PGM file to read
  	ifstream inFile(fileName);

	if (inFile) 
	{	
		cout << "Opened the file " << fileName << "." << endl;

		// While there is still more header information to read.
		while (headerArgCount < NUMBERHEADERARGS)
		{
			// Read in a string from the stream
			inFile >> wordBuffer;

			// If the string doesn't start with a '#' which would
			// indicate a comment based on the PGM file format
			// specification
			if (wordBuffer[0] != '#')
			{
				// If we haven't read in the file's "magic number" yet
				if (magicNumberFlag == false)
				{

					// If the "magic number" is "P5"
					if ((wordBuffer[0] == 'P') && (wordBuffer[1] == '5'))
					{
						// Increase the header argument count by 1 since
						// we've read in one of the four header strings
						// we need and set the magicNumberFlag to true
						// to indicate we've read in the magic number
						headerArgCount++;
						magicNumberFlag = true;
						cout << "The file " << fileName << " is correctly ";
						cout << "formatted." << endl;
					}

					// If the first uncommented string is not "P5"
					else
					{
						// Report that the file is not formatted correctly 
						cout << "The file " << fileName << " is not in the";
						cout <<  " correct format." << endl;
						cout << "Exiting the program.";
						cout <<  "Try again with a different .pgm file.";
						cout << endl;
						exit(EXIT_FAILURE);
					}
				}

				// Otherwise if we have read in the magic number already, 
				// then read in the next three uncommented strings as
				// the number of rows, number of columns, and max intensity.
				// This header information is read as strings then casted as
				// integers and stored in an array.
				else
				{
					headerArgs[headerArgCount - 1 ] = atoi(wordBuffer);
					headerArgCount++;
				}
			}
	  	}
	  }
	  // The file couldn't be opened so tell the user.
	  else
	  {
	  	cout << "Cannot open the file " << fileName << "." << endl;
	  	cout << "Exiting program. Try again with a different file.\n" << endl;
	  	exit(EXIT_FAILURE);
	  }
	
	// Return the useful header information.
	rows = headerArgs[0];
    cout << "rows:" << rows <<endl;
	cols = headerArgs[1];
	levels = headerArgs[2];

	// Close the input file. 
	inFile.close();
	cout << "Closing the file " << fileName << ".\n" << endl;
}
