#include "ONP.h"

ONP::ONP() {
	stack = new Stack();
}

ONP::~ONP() {
	stack->destroy();
	operations = NULL;
	free(operations);
}

double ONP::CalculateFunction(double* arguments, const int argumentCount) {
	// evaluate operations using stack
	// replace each variable by argument value
	// use math function strings by applying specific functions

	// copy operations to tmp c-string
	char* tmpOperations = (char*)malloc(sizeof(char)*(operationsLength + 1));
	int i = 0;
	while (*(operations + i) != '\0') {
		*(tmpOperations + i) = *(operations + i);
		i++;
	}
	*(tmpOperations + i) = '\0';

	// split tmp operations
	char* split = strtok(tmpOperations, " ");

	i = 0;

	while (split != NULL) {
		char* operation = split;
		// for each operation check if variable else parseOperation
		if (strcmp(operation, "x") == 0 || strcmp(operation, "y") == 0
			|| strcmp(operation, "z") == 0 || strcmp(operation, "a") == 0
			|| strcmp(operation, "b") == 0 || strcmp(operation, "c") == 0
			|| strcmp(operation, "d") == 0 || strcmp(operation, "e") == 0
			|| strcmp(operation, "f") == 0 || strcmp(operation, "g") == 0) {
			// push corresponding variable onto stack
			stack->Push(*(arguments + variableNameToNumber(*operation)));
		}
		else {
			//parse
			parseOperation(operation);
		}

		split = strtok(NULL, " ");
	}
	free(tmpOperations);

	Data x = stack->Pop();
	// to avoid floating point errors
	if (x < 0.000001) {
		x = 0;
	}
	return x;
}

void ONP::ReadOperations() {
	// read operations from input
	// TODO:
	// correct to read from multiple lines
	size_t lineSize = 1000;
	operations = (char*)malloc(lineSize * sizeof(char));
	cin.getline(operations, lineSize, EOF);

	operationsLength = CountArray(operations, 0, 0);
	for (int i = 0; i < operationsLength; i++) {
		if (*(operations+i) == '\n') {
			*(operations + i) = ' ';
		}
	}

	// find out operations length
	if (*operations != NULL) {
		int i = 0;
		do {
			operationsLength++;
			i++;
		} while (*(operations + i) != '\0');
	}
}

void ONP::parseOperation(char* operation) {

	if (isDigit(operation)) {
		stack->Push(atof(operation));
	}
	else if (strcmp(operation, "+") == 0) {
		Data x = stack->Pop();
		Data y = stack->Pop();
		stack->Push(x + y);
	}
	else if (strcmp(operation, "-") == 0) {
		Data x = stack->Pop();
		Data y = stack->Pop();
		stack->Push(y - x);
	}
	else if (strcmp(operation, "*") == 0) {
		Data x = stack->Pop();
		Data y = stack->Pop();
		stack->Push(x*y);
	}
	else if (strcmp(operation, "/") == 0) {
		Data x = stack->Pop();
		Data y = stack->Pop();
		stack->Push(y / x);
	}
	else if (strcmp(operation, "pow") == 0) {
		Data x = stack->Pop();
		Data y = stack->Pop();
		stack->Push(pow(y, x));
	}
	else if (strcmp(operation, "neg") == 0) {
		Data x = stack->Pop();
		x = -x;
		stack->Push(x);
	}
	else if (strcmp(operation, "abs") == 0) {
		Data x = stack->Pop();
		x = abs(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "sqr") == 0) {
		Data x = stack->Pop();
		x *= x;
		stack->Push(x);
	}
	else if (strcmp(operation, "sqrt") == 0) {
		Data x = stack->Pop();
		x = sqrt(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "exp") == 0) {
		Data x = stack->Pop();
		x = exp(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "log") == 0) {
		Data x = stack->Pop();
		x = log(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "sin") == 0) {
		Data x = stack->Pop();
		x = sin(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "cos") == 0) {
		Data x = stack->Pop();
		x = cos(x);
		stack->Push(x);
	}
	else if (strcmp(operation, "pi") == 0) {
		stack->Push(3.14);
	}
}

bool ONP::isDigit(char* text) {
	int i = 0;
	if (*(text + i) == '-') {
		i++;
		if (*(text + i) == '\0') {
			return false;
		}
	}
	while (*(text + i) != '\0') {
		if ((*(text + i) < '0' || *(text + i) > '9') && *(text + i) != '.') {
			return false;
		}
		i++;
	}
	return true;
}

int ONP::variableNameToNumber(char name) {
	if (name >= 'x' && name <= 'z') {
		return name - 'x';
	}
	else if (name >= 'a' && name <= 'g') {
		return name - 'a' + 3;
	}
	return 0;
}

// needed for writing out results
char ONP::variableNumberToName(int number) {
	if (number >= 0 && number <= 2) {
		return number + 'x';
	}
	else if (number >= 3) {
		return number - 3 + 'a';
	}
	return 'x';
}

int CountArray(char array[], int n, int count) {
	if (array[n] != '\0') {
		return CountArray(array, n + 1, count + 1);
	}

	return count;
}