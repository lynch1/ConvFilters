/*-------------------------------------------------------------------------*/
/*------------------- F i l t e r  I m a g e  .  cpp ----------------------*/
/*-------------------------------------------------------------------------*/

/*
Name: Daniel Lynch
Date: April 25, 2019
Description: This header file contains a class for 2D subImages to be used
for filtering via convolution. The 2D subimages can be described in text 
files. It was created to be used for Lab 3 of the EECE4840 Computer Vision 
and Digital Image Processing course at UMass Lowell. I decided to make this 
a class so that in the future I can use it more easily to apply arbitrary 
filters to images in future programs. 

Log: 
04/25/2019: DML I started planning out what the class object should look like
and what functions I might need.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <stdlib.h>

#include "FilterImage.h"

using namespace std;


/*-------------------------------------------------------------------------*/
/*------ F i l t e r  I m a g e   C l a s s   F u n c t i o n s -----------*/
/*-------------------------------------------------------------------------*/

// Constructors
// Default constructor
FilterImage::FilterImage()
{
	size = 0;
	reqPad = 0;
    values = nullptr;
}

// Initializing constructor
FilterImage::FilterImage(unsigned int s)
{
	size = s;
	// Is the filter image a suported one?
	if ((size % 2) == 0)
	{
		cout << "Even dimensioned filter subimages are not supported.\n";
		cout << "Only sqaure subimages with an odd side size are ";
		cout << "supported right now.\n" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		// Padding required around an image to be convoluted with this subimage
		reqPad = size / 2;

		// Initialize the values array with zeros
		values = new float *[size];

		for (unsigned int y  = 0; y < size; y++)
		{
			values[y] = new float [size];
			for (unsigned int x = 0; x < size; x ++)
			{
				values[y][x] = 0.0;
			}
		}
	}
}

// Destructor
FilterImage::~FilterImage(void)
{

	for (unsigned int i = 0; i < size; i++)
	{
        delete[] values[i];
	}
    delete[] values;

	// Print message so we know the instance was deleted
	cout << "Deleting FilterImage object instance to free memory!\n" << endl;
}

unsigned int FilterImage::getSize()
{
	return size;
}
unsigned int FilterImage::getReqPad()
{
	return reqPad;
}
float **FilterImage::getValues()
{
	return values;
}

// Reads in filter values from a file specified by argument
// and puts them in the values in the FilterImage object
void FilterImage::readFilterImage(char *fileName)
{
	// Start a stream of the PGM file to read
  	ifstream inFile(fileName);

	// Placeholder for reading in strings
	char infoBuffer[MAXFILTERHEADERSIZE];
	// For checking file is suitable for instance
	unsigned int dimSize;
	// Placeholder for reading floats from file
	float tempFloat = 0.0;

	// If you can open the file as an input stream
	if (inFile.is_open())
	{
		inFile >> infoBuffer;
		// Is it the write type of file?
		if ((infoBuffer[0] == 'F') || (infoBuffer[1] == 'S'))
		{
			// Since it is the right type of file
			inFile >> infoBuffer;
            dimSize = static_cast<unsigned int>(stoi(infoBuffer));
			// If the dimensions don't match for some reason
			if (dimSize != size)
			{
				cout << "The dimensions (" << dimSize << "X" << dimSize;
				cout << ") of the filter specified in the file\n ";
				cout << fileName << " are different than their expected ";
				cout << "value: (" << dimSize << "X" << dimSize << ").\n";
				cout << endl;
				exit(EXIT_FAILURE);
			}
			for (unsigned int y = 0; y < size; y ++)
			{
				for (unsigned int x = 0; x < size; x ++)
				{
					inFile >> infoBuffer;
                    tempFloat = strtof(infoBuffer, nullptr);
					values[y][x] = tempFloat;
				}
			}
		}
		else
		{
			cout << "The file " << fileName << " is not in the";
			cout << " expected format of a filtering subimage.\n" <<endl;			
		}

	}
	else
	{
		cout << "Could not open the filter subimage file ";
		cout << fileName << ".\n Exiting program. Try again.\n" << endl;
	}
}

// Resets size and reqPad to zero, clears **values[][] if nec.
void FilterImage::resetFilter(unsigned int s){
    //Free up values
    // I think it makes sense to delete values[][] itself and
    // make a new values[][] so that its a contiguous block
    if (values != nullptr){
        for (unsigned int i = 0; i < size; i++){
            delete values[i];
        }
        delete values;
        values = nullptr;
    }

    //
    size = s;
    reqPad = s % 2;
    // Initialize the values array with zeros
    values = new float *[size];
    for (unsigned int y  = 0; y < size; y++)
    {
        values[y] = new float [size];
        for (unsigned int x = 0; x < size; x ++)
        {
            values[y][x] = 0.0;
        }
    }

}

// This is mostly for debugging
// It's a simple print function.
void FilterImage::printFilterImage()
{
	cout << "Printing filtering subimage to output\n" << endl;
	cout << "FS" << endl;
	cout << size << endl;
	for (unsigned int y = 0; y < size; y ++)
	{
		for (unsigned int x = 0; x < size; x ++)
		{
			cout << values[y][x] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

/*-------------------------------------------------------------------------*/
/*------- S O M E   F U N C T I O N   P R O T O T Y P E S  F O R ----------*/
/*------------- F I L T E R    I M A G E    F U N C T I O N S -------------*/
/*-------------------------------------------------------------------------*/

void readFilterHeader(char *fileName, unsigned int &size)
{
	// Placeholder for reading in strings
	char infoBuffer[MAXFILTERHEADERSIZE];
	int filterDim = 0;

  	// Start a stream of the FILET file to read
  	ifstream inFile(fileName);

  	// If you were able to open the file to read as a stream
	if (inFile) 
	{	
		cout << "Opened the file " << fileName << "." << endl;

		// Read first word for info about size of image 
		inFile >> infoBuffer;
		// // For debugging
		// cout << "First ?? of filter file is "<< infoBuffer << ".\n" << endl;

		// Is it using your filter subimage file format?
		if((infoBuffer[0] == 'F') || (infoBuffer[1] == 'S'))
		{
			// Read in the size of the square subimage. 
			// It can be expected that it is adhering to specifictation
			inFile >> infoBuffer;
			filterDim = atoi(infoBuffer);
            size = static_cast<unsigned int>(filterDim);
		}
		else
		{
			cout << "The file " << fileName << " is not in the expected";
			cout << "file format." << endl;
			cout << "Please refer to README.txt for the filter image ";
			cout << "file specification.\n" << endl;
		}

		// Close the input file. 
		inFile.close();
		cout << "Closing the file " << fileName << ".\n" << endl;
	}
 
	else
	{
	  	cout << "Cannot open the file " << fileName << "." << endl;
	}
}
