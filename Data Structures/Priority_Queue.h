#pragma once
#include "PNode.h"

//Linked List implementation of Priority Queue

template<class T>

class PriorityQueue
{
	PNode <T>* Head;


public:

	PriorityQueue() {
		Head = nullptr;
	}

	PriorityQueue( T item,int priority) {
		PNode <T> * Node = new PNode <T> (item, priority);
		Head = Node;
	}


	void Enqueue(T newitem, int itm_priority) {
		
		PNode <T> * AddedNode = new PNode <T> (newitem, itm_priority);
		
		if (!Head) {
			Head = AddedNode;
			return;
		}



		if (Head->getPriority() <= itm_priority) {
				
			AddedNode->setNext(Head);
			Head = AddedNode;
			return;
		}

		PNode <T> * Current = Head;
		PNode <T>* Next= Current->getNext();

		// since the P-Q is sorted descindingly we only need to look 
		while (Next) {
			if (Next->getPriority() <= itm_priority) {
				Current->setNext(AddedNode);
				AddedNode->setNext(Next);
				return;
			}
			Current = Next;
			Next = Next->getNext();

		}
		//if we get here, this means we reached the end of the list so
		Current->setNext(AddedNode);
	}


	bool  Dequeue(T &item) {

		//corner case: if list is empty
		if (isEmpty()) { return false; }

		item = Head->getItem();
		PNode<T>* temp = Head->getNext();

		delete Head;
		Head = temp;
		
		return true;
	}

	bool Peek(T & item) {
		if (isEmpty()) { return 0; }
		
		item = Head->getItem();
		
		return 1;
	}

	bool isEmpty() {
		return Head == nullptr;
	}

	~PriorityQueue() {
		//dummy item
		T item;

		//note that dequeue frees the allocated memory
		while (Head)
		{
			Dequeue(item);
			Head = Head->getNext();
		}

	}

};

