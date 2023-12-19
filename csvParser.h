/**
* @file csvParser.h
* @author Melih Altun @2015
**/

#include <stdio.h>
#include <math.h>

#define lin_index(i, j, numCol)  ( ((i)*(numCol))+(j) )   //2D to 1D array

int csv2floatArrayWithHeader(double dataOut[], FILE* fp, int maxRows, int maxCols);

int csv2floatArray(double dataOut[], FILE *fp, int maxRows, int maxCols);

int csv2intArray(int dataOut[], FILE *fp, int maxRows, int maxCols);

int readLine2floatArray(double dataOut[], FILE* fp, int maxCols);

int readLine2intArray(int dataOut[], FILE* fp, int maxCols);

void getRowColCount(int *RowMax,int *ColMax, FILE *fp);
