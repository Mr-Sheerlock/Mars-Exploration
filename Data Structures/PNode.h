#pragma once

//Priority Node, just like the normal node but has an extra data member: int priority

template <class T>
class PNode
{
	T item;
	int priority;
	PNode <T> * Next;


public:

	PNode() {
		Next = nullptr;
	}

	PNode(T newitem, int newpriority) {
		item = newitem;
		priority = newpriority;
		Next =	nullptr;
	}

	T getItem() {
		return item;
	}

	int getPriority() {
		return priority;
	}

	PNode<T> * getNext() {
		return Next;
	}

	void setItem(T newitem) {
		item = newitem;
	}
	
	void setPriority(int newpriority) {
		priority = newpriority;
	}

	void setNext(PNode<T>* NextNodeptr) {
		Next = NextNodeptr;
	}
};

