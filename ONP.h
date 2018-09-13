#ifndef ONP_H
#define ONP_H

#include "Stack.h"
#include <malloc.h>
#include <math.h>
#include <iostream>

using namespace std;

class ONP {
public:
	ONP();
	~ONP();
	double CalculateFunction(double* arguments, const int argumentCount);
	void ReadOperations();
private:
	void parseOperation(char* operation);
	bool isDigit(char* text);
	int variableNameToNumber(char name);
	char variableNumberToName(int number);
	Stack* stack;
	char* operations;
	int operationsLength = 0;
};

// how many elements in array
int CountArray(char array[], int n, int count);

#endif // !ONP_H

