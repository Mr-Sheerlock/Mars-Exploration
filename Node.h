template <class T>
class Node
{
private:
	T Item;
	Node<T>* Next;
public:
	Node()
	{
		Next = NULL;
	}
	Node(T nItem)
	{
		Item = nItem;
		Next = NULL;
	}
	void setitem(T nItem)
	{
		Item = nItem;
	}
	T getitem()
	{
		return Item;
	}
	Node<T>* getNext()
	{
		return Next;
	}
	void setNext(Node<T>* NextNode)
	{
		Next = NextNode;
	}


	

};
