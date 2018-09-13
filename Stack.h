#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef double Data;

struct LNode {
	Data data;
	struct LNode* next;
};

class Stack {
public:
	Stack();

	void destroy();

	LNode* Top();

	bool isEmpty();

	void Push(Data data);

	Data Pop();

private:
	LNode* stack;
};

LNode* Copy(LNode* list);

#endif // !STACK_H