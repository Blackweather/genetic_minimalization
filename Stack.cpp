#include "Stack.h"

Stack::Stack() {
	stack = NULL;
}

void Stack::destroy() {
	if (isEmpty())
		return;
	Pop();
	return destroy();
}

LNode* Stack::Top() {
	return stack;
}

bool Stack::isEmpty() {
	return (stack == NULL);
}

void Stack::Push(Data data) {
	LNode* node = new LNode;
	node->data = data;
	node->next = stack;
	stack = node;
}

Data Stack::Pop() {
	Data poppedData;

	// guard from popping empty stack
	if (!isEmpty()) {
		LNode* node = stack;
		// save popped data for return
		poppedData = stack->data;

		// switch first element to next
		stack = stack->next;
		delete node;
	}
	else {
		int x = 0;
	}

	return poppedData;
}