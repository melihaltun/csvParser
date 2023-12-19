/**
* @file csvParser.cpp
* @author Melih Altun @2015
**/

#include "csvParser.h"

//reads csv file contents into a double array and returns number of rows
//parameters: (output) parsed array, (inputs) pointer to file, max number of rows, max number of columns to be read
int csv2floatArrayWithHeader(double dataOut[], FILE* fp, int maxRows, int maxCols)
{
	int retVal;
	char c;

	do {
		c = fgetc(fp);
		if (c == '\n' || c == '\r')
			break;
	} while (c != EOF);

	retVal = csv2floatArray(dataOut, fp, maxRows, maxCols);

	return retVal;
}

//reads csv file contents into a double array and returns number of rows
//parameters: (output) parsed array, (inputs) pointer to file, max number of rows, max number of columns to be read
int csv2floatArray(double dataOut[], FILE *fp, int maxRows, int maxCols)
{
	char c = ' ', prev_c = ' ';
	int num=0, dec_digit=1, expNum = 0;
	double dec = 0;
	int row=0, col=0;
	bool decimal=false, negative = false, exponent = false, negativeExp = false;

	do{  //Parse CSV to int array line by line;
		c = fgetc(fp);
		if ((c == ' ' && prev_c == ',') || (c == ' ' && prev_c == ' ')) {
			prev_c = c;
			continue;  //ignore spaces after a coma or another space
		}
		if ((c == '\n' && prev_c == '\n') || (c == '\r' && prev_c == '\r') || (c == '\r' && prev_c == '\n') || (c == '\n' && prev_c == '\r')) {
			prev_c = c;
			continue;  //ignore consecutive return characters. This is usually the case for files encoded in windows machines. Lines terminate with both carriage return and new line characters.
		}
		if (c > 47 && c < 58) {
			if (!decimal && !exponent)
				num = num * 10 + c - 48; //add digit to number
			else if (!exponent) {
				dec += (double)((int)c - 48)*pow(10.0, -dec_digit); //get digits after decimal point
				dec_digit++;
			}
			else {
				expNum = expNum * 10 + c - 48; //add digit to number
			}
		}
		else if (c == '-') {
			if (!exponent)
				negative = true; // remember if there's a negative sign
			else if (exponent && (prev_c == 'e' || prev_c == 'E'))
				negativeExp = true; // remember if there's a negative sign for exp
		}
		else if (c == '.')
			decimal = true; //remember if there's a decimal point
		else if (c == 'e' || c == 'E')
			exponent = true; //remember if there's an exponent E for scientific notation
		else if (c == ' ' || c == ',') { //numbers can be delimated by commas, spaces or both: 1, 2, 4, ...
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			if (exponent && negativeExp)
				expNum = -expNum;
			dataOut[lin_index(row,col,maxCols)] = pow(10,expNum)*(num + dec);  //number = integer + mantissa
			col++;
			if (col > maxCols)
				printf("Columns exceed allocated array size!\n");
			negative = false;
			decimal = false;
			expNum = 0;
			exponent = false;
			negativeExp = false;
			dec_digit = 1;
			dec = 0;
			num = 0;
		}
		else if (c == '\n' || c == '\r') {  //reached end of line
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[lin_index(row, col, maxCols)] = num + dec;
			col = 0;
			row++;
			negative = false;
			decimal = false;
			dec_digit = 1;
			num = 0;
			dec = 0;
		}
		prev_c = c;
	} while (row < maxRows && c != EOF);
	return row;
}


//reads csv file contents into a double array and returns number of rows
//parameters: (output) parsed array, (inputs) pointer to file, max number of rows, max number of columns to be read
int csv2intArray(int dataOut[], FILE *fp, int maxRows, int maxCols)
{
	char c = ' ', prev_c = ' ';
	int num = 0, dec_digit = 1;
	double dec = 0;
	int row = 0, col = 0;
	bool decimal = false, negative = false;

	do{  //Parse CSV to int array line by line;
		c = fgetc(fp);
		if ((c == ' ' && prev_c == ',') || (c == ' ' && prev_c == ' ')) {
			prev_c = c;
			continue;  //ignore spaces after a coma or another space
		}
		if ((c == '\n' && prev_c == '\n') || (c == '\r' && prev_c == '\r') || (c == '\r' && prev_c == '\n') || (c == '\n' && prev_c == '\r')) {
			prev_c = c;
			continue;  //ignore consecutive return characters. This is usually the case for files encoded in windows machines. Lines terminate with both carriage return and new line characters.
		}
		if (c > 47 && c < 58) {
			if (!decimal)
				num = num * 10 + c - 48; //add digit to number
			else {
				dec += (double)((int)c - 48)*pow(10.0, -dec_digit); //get digits after decimal point
				dec_digit++;
			}
		}
		else if (c == '-')
			negative = true; // remember if there's a negative sign
		else if (c == '.')
			decimal = true; //remember if there's a decimal point
		else if (c == ' ' || c == ',') { //numbers can be delimated by commas, spaces or both: 1, 2, 4, ...
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[lin_index(row, col, maxCols)] = (int) (num + dec);
			col++;
			if (col > maxCols)
				printf("Columns exceed allocated array size!\n");
			negative = false;
			decimal = false;
			dec_digit = 1;
			dec = 0;
			num = 0;
		}
		else if ( c == '\r'||c == '\n') {  //reached end of line
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[lin_index(row, col, maxCols)] = (int) (num + dec);
			col = 0;
			row++;
			negative = false;
			decimal = false;
			dec_digit = 1;
			num = 0;
			dec = 0;
		}
		prev_c = c;
	} while (row < maxRows && c != EOF);
	return row;
}


int readLine2floatArray(double dataOut[], FILE* fp, int maxCols)
{
	char c = ' ', prev_c = ' ';
	int num = 0, dec_digit = 1, expNum = 0;
	double dec = 0;
	int col = 0;
	bool decimal = false, negative = false, exponent = false, negativeExp = false;

	do {  //Parse CSV to int array line by line;
		c = fgetc(fp);
		if ((c == ' ' && prev_c == ',') || (c == ' ' && prev_c == ' ')) {
			prev_c = c;
			continue;  //ignore spaces after a coma or another space
		}
		if ((c == '\n' && prev_c == '\n') || (c == '\r' && prev_c == '\r') || (c == '\r' && prev_c == '\n') || (c == '\n' && prev_c == '\r')) {
			prev_c = c;
			continue;  //ignore consecutive return characters. This is usually the case for files encoded in windows machines. Lines terminate with both carriage return and new line characters.
		}
		if (c > 47 && c < 58) {
			if (!decimal && !exponent)
				num = num * 10 + c - 48; //add digit to number
			else if (!exponent) {
				dec += (double)((int)c - 48) * pow(10.0, -dec_digit); //get digits after decimal point
				dec_digit++;
			}
			else {
				expNum = expNum * 10 + c - 48; //add digit to number
			}
		}
		else if (c == '-') {
			if (!exponent)
				negative = true; // remember if there's a negative sign
			else if (exponent && (prev_c == 'e' || prev_c == 'E'))
				negativeExp = true; // remember if there's a negative sign for exp
		}
		else if (c == '.')
			decimal = true; //remember if there's a decimal point
		else if (c == 'e' || c == 'E')
			exponent = true; //remember if there's an exponent E for scientific notation
		else if (c == ' ' || c == ',') { //numbers can be delimated by commas, spaces or both: 1, 2, 4, ...
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			if (exponent && negativeExp)
				expNum = -expNum;
			dataOut[col] = pow(10, expNum) * (num + dec);  //number = integer + mantissa
			col++;
			if (col > maxCols)
				printf("Columns exceed allocated array size!\n");
			negative = false;
			decimal = false;
			expNum = 0;
			exponent = false;
			negativeExp = false;
			dec_digit = 1;
			dec = 0;
			num = 0;
		}
		else if (c == '\n' || c == '\r') {  //reached end of line
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[col] = num + dec;
			col = 0;
			negative = false;
			decimal = false;
			dec_digit = 1;
			num = 0;
			dec = 0;
		}
		prev_c = c;
	} while (c != '\n' && c != '\r' && c != EOF);
	return 0;
}

int readLine2intArray(int dataOut[], FILE* fp, int maxCols)
{
	char c = ' ', prev_c = ' ';
	int num = 0, dec_digit = 1;
	double dec = 0;
	int col = 0;
	bool decimal = false, negative = false;

	do {  //Parse CSV to int array line by line;
		c = fgetc(fp);
		if ((c == ' ' && prev_c == ',') || (c == ' ' && prev_c == ' ')) {
			prev_c = c;
			continue;  //ignore spaces after a coma or another space
		}
		if ((c == '\n' && prev_c == '\n') || (c == '\r' && prev_c == '\r') || (c == '\r' && prev_c == '\n') || (c == '\n' && prev_c == '\r')) {
			prev_c = c;
			continue;  //ignore consecutive return characters. This is usually the case for files encoded in windows machines. Lines terminate with both carriage return and new line characters.
		}
		if (c > 47 && c < 58) {
			if (!decimal)
				num = num * 10 + c - 48; //add digit to number
			else {
				dec += (double)((int)c - 48) * pow(10.0, -dec_digit); //get digits after decimal point
				dec_digit++;
			}
		}
		else if (c == '-')
			negative = true; // remember if there's a negative sign
		else if (c == '.')
			decimal = true; //remember if there's a decimal point
		else if (c == ' ' || c == ',') { //numbers can be delimated by commas, spaces or both: 1, 2, 4, ...
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[col] = (int)(num + dec);
			col++;
			if (col > maxCols)
				printf("Columns exceed allocated array size!\n");
			negative = false;
			decimal = false;
			dec_digit = 1;
			dec = 0;
			num = 0;
		}
		else if (c == '\r' || c == '\n') {  //reached end of line
			if (negative) {
				num *= -1;
				dec *= -1;
			}
			dataOut[col] = (int)(num + dec);
			col = 0;
			negative = false;
			decimal = false;
			dec_digit = 1;
			num = 0;
			dec = 0;
		}
		prev_c = c;
	} while(c != '\n' && c != '\r' && c != EOF);
	return 0;
}


void getRowColCount(int *RowMax,int *ColMax, FILE *fp)
{
	char ch;
	int col=1,row=0;

	while(1)
	{
		ch = getc(fp);
		if(ch==EOF)
		{
			break;
		}
		if(ch == ',' && row<1)
		{
			col++;
			continue;
		}
		if (ch == '\r'||ch == '\n')
		{
			row++;
		}
	}

	*RowMax = row;
	*ColMax = col;
}
