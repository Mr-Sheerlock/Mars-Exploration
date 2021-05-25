#pragma once
#include"Node.h"
template<class T>
class Queue
{
	Node<T>* Head;
	Node<T>* Rear;
public:

	Queue()
	{
		Head = nullptr;
		Rear = nullptr;
	}

	Queue(T item)
	{
		Node<T>* node = new Node<T>(item);
		Head = node;
		Rear = Head;
	}

	bool IsEmpty()
	{
		return Head == nullptr;
	}

	bool Enqueue(T item)
	{
		if (!Head)
		{
			Node<T>* node = new Node<T>(item);
			Head = node;
			Rear = Head;
			return true;
		}
		else
		{
			Node<T>* node = new Node<T>(item);
			Rear->setNext(node);
			Rear = Rear->getNext();
			return true;
		}
	}

	bool Dequeue(T& item)
	{
		if (IsEmpty())
			return false;

		item = Head->getitem();
		Node<T>* temp = Head->getNext();
		delete Head;
		Head = temp;
		return true;
	}

	T Peek() {
		if (IsEmpty())
		{
			return NULL;
		}
		else
		{
			return Head->getitem();
		}

	}
	~Queue()
	{
		T temp;

		while (Dequeue(temp));
	}
};



#pragma region Retrieve
	/*bool Retrieve(T item)
	{
		Node<T>* L, * N;
		L = Head;
		N = L->getNext();
		if (Head->getitem() == item)
		{
			delete Head;
			Head = N;
			return true;
		}
		Node<T>* temp;
		while (N)
		{
			if (N->getitem() == item)
			{
				temp = N->getNext();
				L->setNext(temp);
				delete N;
				return true;
			}
			else
			{
				L = L->getNext();
				N = N->getNext();
			}
		}
		return false;
	}*/
#pragma endregion