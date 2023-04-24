#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "images.h"
using namespace std;

void Images::GetDataFromFile(string filePath, Header& header, vector<Pixel>& pixelsVector) //Pass the filepath and the blank header file |  GetDataFromFile("/input/pattern1.tga/", headerB)
{
	ifstream inFile(filePath, ios_base::binary);
	if (inFile.is_open())
	{
		cout << "File is opened " << endl;
		//Read the header data for the file
		inFile.read(&header.idLength, sizeof(header.idLength));
		inFile.read(&header.colorMapType, sizeof(header.colorMapType));
		inFile.read(&header.dataTypeCode, sizeof(header.dataTypeCode));
		inFile.read((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		inFile.read((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		inFile.read((char*)&header.colorMapDepth, sizeof(header.colorMapDepth));
		inFile.read((char*)&header.xOrigin, sizeof(header.xOrigin));
		inFile.read((char*)&header.yOrigin, sizeof(header.yOrigin));
		inFile.read((char*)&header.width, sizeof(header.width));
		inFile.read((char*)&header.height, sizeof(header.height));
		inFile.read((char*)&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		inFile.read((char*)&header.imageDescriptor, sizeof(header.imageDescriptor));
		int totalPixels = header.width * header.height;
		//Read the image
		//Create Pixel structs and push them into a vector of pixels
		for (int i = 0; i < totalPixels; i++)
		{
			Pixel pixelC;
			inFile.read((char*)&pixelC.red, 1);
			inFile.read((char*)&pixelC.green, 1);
			inFile.read((char*)&pixelC.blue, 1);
			pixelsVector.push_back(pixelC);
		}
		inFile.close();
	}

}

vector <Images::Pixel> Images::MultiplyBlend(vector <Pixel>& A, vector <Pixel>& B) //Multiply and "blend" the images A and B together to create an output of C. 
{
	vector <Pixel> C;
	int maximum = A.size();
	for (int i = 0; i < maximum; i++)
	{
		Pixel pixelC;
		pixelC.red = ((float)(A.at(i).red) * (float)(B.at(i).red)) / 255.0f + .5f;
		pixelC.green = ((float)(A.at(i).green) * (float)(B.at(i).green)) / 255.0f + .5f;
		pixelC.blue = ((float)(A.at(i).blue) * (float)(B.at(i).blue)) / 255.0f + .5f;
		C.push_back(pixelC);
	}
	return C;
}

//WriteDataInFile(HA, C, "/output/part1.tga");
void Images::WriteDataInFile(string filePath, vector<Pixel>& pixelsVector, Header& header) //Write the output image, C, into a new file. 
{
	fstream inFile(filePath, ios_base::binary | ios_base::out); //Posibly include ios_base::trunc? 
	if (inFile.is_open())
	{
		inFile.write(&header.idLength, sizeof(header.idLength));
		inFile.write(&header.colorMapType, sizeof(header.colorMapType));
		inFile.write(&header.dataTypeCode, sizeof(header.dataTypeCode));
		inFile.write((char*)&header.colorMapOrigin, sizeof(header.colorMapOrigin));
		inFile.write((char*)&header.colorMapLength, sizeof(header.colorMapLength));
		inFile.write((char*)&header.colorMapDepth, sizeof(header.colorMapDepth));
		inFile.write((char*)&header.xOrigin, sizeof(header.xOrigin));
		inFile.write((char*)&header.yOrigin, sizeof(header.yOrigin));
		inFile.write((char*)&header.width, sizeof(header.width));
		inFile.write((char*)&header.height, sizeof(header.height));
		inFile.write((char*)&header.bitsPerPixel, sizeof(header.bitsPerPixel));
		inFile.write((char*)&header.imageDescriptor, sizeof(header.imageDescriptor));
		int totalPixels = header.width * header.height;
		for (int i = 0; i < totalPixels; i++)
		{
			inFile.write((char*)&pixelsVector.at(i).red, 1);
			inFile.write((char*)&pixelsVector.at(i).green, 1);
			inFile.write((char*)&pixelsVector.at(i).blue, 1);
		}
		inFile.close();
	}

}


void Images::TestOne()
{
	cout << "Starting Test One" << endl;
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer1.tga", headerA, A);
	GetDataFromFile("input/pattern1.tga", headerB, A);
	cout << headerA.width << endl;
	cout << headerB.width << endl;
	vector <Pixel> C = MultiplyBlend(A, B);
	WriteDataInFile("output/part1.tga", C, headerA); //Might not work with headerA? 
	cout << "Test One Complete" << endl;
	cout << endl;
}

int main()
{
	//Part 1
	Images test;
	test.TestOne();

	return 0;
}