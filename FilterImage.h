/*-------------------------------------------------------------------------*/
/*-------------------- F i l t e r  I m a g e  .  h -----------------------*/
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

#ifndef FILTERIMAGE_H
#define FILTERIMAGE_H

static const unsigned int MAXFILTERHEADERSIZE = 100;

/*-------------------------------------------------------------------------*/
/*------ F i l t e r  I m a g e   C l a s s   D e f i n i t i o n ---------*/
/*------------- a n d   F u n c t i o n   P r o t o t y p e s -------------*/
/*-------------------------------------------------------------------------*/

class FilterImage
{
	public:
		// Constructors
		FilterImage();
		FilterImage(unsigned int s);

		// Destructor
		~FilterImage();

		unsigned int const getSize();

		unsigned int const getReqPad();

		float **getValues();

		void readFilterImage(char *fileName);

		void printFilterImage();

	private:
		// Number of rows
		unsigned int size;
		// Required padding to use filter
		unsigned int reqPad;
		// 2D array of float values to serve as filter sub image
		float **values;

};

/*-------------------------------------------------------------------------*/
/*------- S O M E   F U N C T I O N   P R O T O T Y P E S  F O R ----------*/
/*------------- F I L T E R    I M A G E    F U N C T I O N S -------------*/
/*-------------------------------------------------------------------------*/

// Read First line of PGM file for info about size of image 
void readFilterHeader(char *fileName, unsigned int &size);

#endif