//#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;
class Images {

public:

    struct Header
    {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
    };
    struct Pixel
    {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };

    void GetDataFromFile(string filePath, Header& header, vector<Pixel>& pixelsVector);
    void MultiplyBlend(vector<Pixel>& Part1, vector<Pixel>& A, vector<Pixel>& B);
    void WriteDataInFile(string filePath, vector<Pixel>& pixelVector, Header& header);

    void SubtractBlend(vector <Pixel>& Part2, vector <Pixel>& A, vector <Pixel>& B);
    void ScreenBlend(vector <Pixel>& Part3, vector <Pixel>& B, vector <Pixel>& A);
    void OverlayBlend(vector <Pixel>& Part5, vector <Pixel>& B, vector <Pixel>& A);
    void Green(vector <Pixel>& Part6, vector <Pixel>& A);
    void RedBlue(vector <Pixel>& Part7, vector <Pixel>& A);
    void Split(vector <Pixel> A, vector <Pixel>& B, vector <Pixel>& G, vector <Pixel>& R);
    void Combine(vector <Pixel>& combined, vector <Pixel>& B, vector <Pixel>& G, vector <Pixel>& R);
    void Flip(vector <Pixel>& flipped, vector <Pixel>& A);
    void TestOne();
    void TestTwo();
    void TestThree();
    void TestFour();
    void TestFive();
    void TestSix();
    void TestSeven();
    void TestEight();
    void TestNine();
    void TestTen();

    bool test_equal(string filenameOne, string filenameTwo);
    bool sameexactbits(string filenameOne, string filenameTwo);
};

