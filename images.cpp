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
			inFile.read((char*)&pixelC.blue, 1);
			inFile.read((char*)&pixelC.green, 1);
			inFile.read((char*)&pixelC.red, 1);
			pixelsVector.push_back(pixelC);
		}
		inFile.close();
	}
	
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
		//if (pixelsVector != '')
		for (int i = 0; i < totalPixels; i++)
		{
			inFile.write((char*)&pixelsVector.at(i).blue, 1);
			inFile.write((char*)&pixelsVector.at(i).green, 1);
			inFile.write((char*)&pixelsVector.at(i).red, 1);
		}
		inFile.close();
	}


}

void Images::MultiplyBlend(vector <Pixel>& Part1, vector <Pixel>& A, vector <Pixel>& B) //Multiply and "blend" the images A and B together to create an output of C. 
{
	int counter = 0;
	int maximum = A.size();
	while (counter < maximum)
	{
		Pixel pixelC;
		pixelC.red = ((float)(A.at(counter).red) * (float)(B.at(counter).red)) / 255.0f + .5f;
		pixelC.green = ((float)(A.at(counter).green) * (float)(B.at(counter).green)) / 255.0f + .5f;
		pixelC.blue = ((float)(A.at(counter).blue) * (float)(B.at(counter).blue)) / 255.0f + .5f;
		Part1.push_back(pixelC);
		counter++;
	}
}

void Images::SubtractBlend(vector <Pixel>& Part2, vector <Pixel>& B, vector <Pixel>& A)
{
	int maximum = A.size();
	int counter = 0;
	while (counter < maximum)
	{
		Pixel pixelC;
		float red = (A.at(counter).red - B.at(counter).red);
		if (red < 0) { red = 0; }

		float green = (A.at(counter).green - B.at(counter).green);
		if (green < 0) { green = 0; }

		float blue = (A.at(counter).blue - B.at(counter).blue);
		if (blue < 0) { blue = 0;  }

		pixelC.red = red;
		pixelC.green = green;
		pixelC.blue = blue;
		Part2.push_back(pixelC);
		counter++;
	}
}

void Images::ScreenBlend(vector <Pixel>& Part3, vector <Pixel>& B, vector <Pixel>& A) //combination of screen and multiply, depending on the 
{
	int maximum = A.size();
	int count = 0;
	while (count < maximum)
	{
		Pixel pixelC;
		pixelC.red = ((1.0f - ((1.0f - (A.at(count).red / 255.0f)) * (1.0f - (B.at(count).red / 255.0f)))) * 255) + .5f;
		pixelC.green = ((1.0f - ((1.0f - (A.at(count).green / 255.0f)) * (1.0f - (B.at(count).green / 255.0f)))) * 255) + .5f;
		pixelC.blue = ((1.0f - ((1.0f - (A.at(count).blue / 255.0f)) * (1.0f - (B.at(count).blue / 255.0f)))) * 255) + .5f;
		Part3.push_back(pixelC);
		count++;
	}
}

void Images::OverlayBlend(vector <Pixel>& Part5, vector <Pixel>& A, vector <Pixel>& B)
{
	int maximum = A.size();
	int count = 0;
	float floatRedC, floatGreenC, floatBlueC;
	for (int i = 0; i < maximum; i++)
	{
		Pixel pixelC;

		//Ternerary operators (I need the practice plus it looks cool) to determine what RedC, GreenC, and BlueC are equated to
		pixelC.red = (((float)(B.at(count).red) / 255.0f) <= .5) ? ((2.0f * ((float)A.at(count).red / 255.0f) * ((float)B.at(count).red / 255.0f)) * 255) + .5f :
			((1.0f - (2.0f * (1.0f - ((float)A.at(count).red / 255.0f)) * (1.0f - (float)B.at(count).red / 255.0f))) * 255) + .5f;

		pixelC.green = (((float)(B.at(count).green) / 255.0f) <= .5) ? ((2.0f * ((float)A.at(count).green / 255.0f) * ((float)B.at(count).green / 255.0f)) * 255) + .5f :
			((1.0f - (2.0f * (1.0f - ((float)A.at(count).green / 255.0f)) * (1.0f - (float)B.at(count).green / 255.0f))) * 255) + .5f;

		pixelC.blue = (((float)(B.at(count).blue) / 255.0f) <= .5) ? ((2.0f * ((float)A.at(count).blue / 255.0f) * ((float)B.at(count).blue / 255.0f)) * 255) + .5f :
			((1.0f - (2.0f * (1.0f - ((float)A.at(count).blue / 255.0f)) * (1.0f - (float)B.at(count).blue / 255.0f))) * 255) + .5f;

		Part5.push_back(pixelC);
		count++;
	}
}

void Images::Green(vector <Pixel>& Part6, vector <Pixel>& A)
{
	float greenC;
	int counter = 0;
	int maximum = A.size();
	while (counter < maximum)
	{
		Pixel pixelC;
		pixelC.red = A.at(counter).red;
		greenC = A.at(counter).green + 200.0f; //gotta add in the greenC variable because pixelC.green = A.at(i).green and if (pixelC.green > 255) pixelC.green = 255 doesnt work
		pixelC.green = greenC;
		if (greenC > 255.0f)
		{
			pixelC.green = 255.0f;
		}
		pixelC.blue = A.at(counter).blue;
		greenC = 0;
		Part6.push_back(pixelC); 
		counter = counter + 1;
	}
} 

void Images::RedBlue(vector <Pixel>& Part7, vector <Pixel>& A) //multiply red by 4, and blue by 0. 
{
	int counter = 0;
	int maximum = A.size();
	float blueC = 0;
	float redC;
	while (counter < maximum)
	{
		Pixel pixelC;
		//red
		redC = A.at(counter).red * 4.0f;
		pixelC.red = redC;
		if (redC > 255.0f)
		{
			pixelC.red = 255.0f;
		}
		//green
		pixelC.green = A.at(counter).green;
		//blue
		pixelC.blue = 0;
		Part7.push_back(pixelC);
		counter = counter + 1;
	}
}

void Images::Split(vector <Pixel> A, vector <Pixel>& B, vector <Pixel>& G, vector <Pixel>& R) //load car.tga and seperate each color into a seperate file and write
{
	int counter = 0;
	int maximum = A.size();
	while (counter < maximum) 
	{
		Pixel pixelB;
		Pixel pixelG;
		Pixel pixelR;
		pixelB.blue = A.at(counter).blue;
		pixelB.green = A.at(counter).blue;
		pixelB.red = A.at(counter).blue;

		pixelG.blue = A.at(counter).green;
		pixelG.green = A.at(counter).green;
		pixelG.red = A.at(counter).green;

		pixelR.blue = A.at(counter).red;
		pixelR.green = A.at(counter).red;
		pixelR.red = A.at(counter).red;

		B.push_back(pixelB);
		G.push_back(pixelG);
		R.push_back(pixelR);
		counter = counter + 1;
	}	
}

void Images::Combine(vector <Pixel>& combined, vector <Pixel>& B, vector <Pixel>& G, vector <Pixel>& R)
{
	int counter = 0;
	int maximum = B.size();
	while (counter < maximum)
	{
		Pixel pixelC;
		pixelC.blue = B.at(counter).blue;
		pixelC.green = G.at(counter).green;
		pixelC.red = R.at(counter).red;
		combined.push_back(pixelC);
		counter = counter + 1;
	}
}

void Images::Flip(vector <Pixel>& flipped, vector <Pixel>& A)
{
	int counter = A.size()-1;
	while (counter >= 0)
	{
		Pixel pixelC;
		pixelC.blue = A.at(counter).blue;
		pixelC.green = A.at(counter).green;
		pixelC.red = A.at(counter).red;
		flipped.push_back(pixelC);
		counter = counter - 1;
	}
}


void Images::TestOne() //WORKS!
{
	cout << "Starting Test One" << endl;
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer1.tga", headerA, A);
	GetDataFromFile("input/pattern1.tga", headerB, B);

	vector <Pixel> Part1;
	MultiplyBlend(Part1, A, B);
	//vector <Pixel> C = MultiplyBlend(A, B);
	WriteDataInFile("output/part1.tga", Part1, headerA); //Might not work with headerA? 
	cout << "Test One Complete" << endl;
	cout << endl;
}

void Images::TestTwo() //WORKS!
{
	cout << "Starting Test Two" << endl;
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer2.tga", headerA, A);
	GetDataFromFile("input/car.tga", headerB, B);
	vector<Pixel> Part2;
	SubtractBlend(Part2, A, B);
	WriteDataInFile("output/part2.tga", Part2, headerA);
	cout << "Test Two Complete" << endl;
	cout << endl;
}

void Images::TestThree() // WORKS!
{
	cout << "Starting Test Three" << endl;
	//Take A(layer1) & B(pattern2), and multiply them to get C. Take C and screen it with D(text) to get a result of E. Write E. 
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer1.tga", headerA, A);
	GetDataFromFile("input/pattern2.tga", headerB, B);
	vector<Pixel> C;
	MultiplyBlend(C, A, B);
	vector<Pixel> D; 
	Header headerD;
	GetDataFromFile("input/text.tga", headerD, D);
	vector<Pixel> E;
	ScreenBlend(E, D, C); // C - D
	WriteDataInFile("output/part3.tga", E, headerA);
	cout << "Test Three Complete" << endl;
	cout << endl;
}

void Images::TestFour() // WORKS!
{
	cout << "Starting Test Four" << endl;
	//Take A(layer2) & B(circles), and multiply them to get C. Take C and subtract it with D(pattern2) to get a result of E. Write E. C - D 
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer2.tga", headerA, A);
	GetDataFromFile("input/circles.tga", headerB, B);

	vector<Pixel> C; 
	MultiplyBlend(C, A, B);
	vector<Pixel> D;
	Header headerD;
	GetDataFromFile("input/pattern2.tga", headerD, D);
	vector<Pixel> E;
	SubtractBlend(E, D, C);
	WriteDataInFile("output/part4.tga", E, headerA); //Breaks here 
	cout << "Test Four Complete" << endl;
	cout << endl;
}

void Images::TestFive() // WORKS!
{
	cout << "Starting Test Five" << endl;
	//Take top layer A(layer1) and combine it with B(pattern1) using the Overlay blending mode.
	Header headerA;
	Header headerB;
	vector<Pixel> A;
	vector<Pixel> B;
	GetDataFromFile("input/layer1.tga", headerA, A);
	GetDataFromFile("input/pattern1.tga", headerB, B);
	vector<Pixel> C;
	OverlayBlend(C, A, B);
	WriteDataInFile("output/part5.tga", C, headerA);
	cout << "Test Five Complete" << endl;
	cout << endl;
}

void Images::TestSix() //WORKS! 
{
	cout << "Starting Test Six" << endl;
	Header headerA;
	vector<Pixel> A;
	GetDataFromFile("input/car.tga", headerA, A);
	vector<Pixel> B;
	Green(B, A);
	WriteDataInFile("output/part6.tga", B, headerA);
	cout << "Test Six Complete" << endl;
	cout << endl;
}

void Images::TestSeven() //WORKS!
{
	cout << "Starting Test Seven" << endl;
	Header headerA;
	vector<Pixel> A;
	GetDataFromFile("input/car.tga", headerA, A);
	vector <Pixel> B;
	RedBlue(B, A);
	WriteDataInFile("output/part7.tga", B, headerA);
	cout << "Test Seven Complete" << endl;
	cout << endl;
}

void Images::TestEight() //WORKS!
{
	cout << "Starting Test Eight" << endl;
	Header headerA;
	vector <Pixel> A;
	GetDataFromFile("input/car.tga", headerA, A);
	vector <Pixel> part8_b;
	vector <Pixel> part8_g;
	vector <Pixel> part8_r;
	Split(A, part8_b, part8_g, part8_r);
	WriteDataInFile("output/part8_b.tga", part8_b, headerA);
	WriteDataInFile("output/part8_g.tga", part8_g, headerA);
	WriteDataInFile("output/part8_r.tga", part8_r, headerA);
	cout << "Test Eight Complete" << endl;
	cout << endl;
}

void Images::TestNine() //WORKS!
{
	cout << "Starting Test Nine" << endl;
	Header headerA;
	vector <Pixel> A;
	Header headerB;
	vector <Pixel> B;
	Header headerC;
	vector <Pixel> C;
	GetDataFromFile("input/layer_blue.tga", headerC, A);
	GetDataFromFile("input/layer_green.tga", headerB, B);
	GetDataFromFile("input/layer_red.tga", headerA, C);
	vector <Pixel> combined;
	Combine(combined, A, B, C);
	WriteDataInFile("output/part9.tga", combined, headerA);
	cout << "Test Nine Complete" << endl;
	cout << endl;
}

void Images::TestTen() //WORKS!!
{
	cout << "Starting Test Ten" << endl;
	Header headerA;
	vector <Pixel> A;
	GetDataFromFile("input/text2.tga", headerA, A);
	vector <Pixel> flipped;
	Flip(flipped, A);
	WriteDataInFile("output/part10.tga", flipped, headerA);
	cout << "Test Ten Complete" << endl;
	cout << endl;
}

int main()
{
	//Part 1
	Images test; //good
	test.TestOne();
	
	//Part 2 
	Images test2;  //good
	test2.TestTwo();
	
	//Part 3
	Images test3; //good
	test3.TestThree();
	
	//Part 4
	Images test4; //good
	test4.TestFour();
	
	//Part 5
	Images test5; //good
	test5.TestFive(); 
	
	//Part 6 
	Images test6; //good
	test6.TestSix();
	
	//Part 7
	Images test7; //good 
	test7.TestSeven();
	
	//Part 8
	Images test8; //good
	test8.TestEight();
	
	//Part 9
	Images test9; //good
	test9.TestNine();
	
	//Part 10
	Images test10; //good
	test10.TestTen();

	return 0;
}