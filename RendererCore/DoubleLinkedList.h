#pragma once
#include <initializer_list>


#define CRASH { int x = *(int *)0; }


template<typename T>
class DoubleLinkedList
{
public:
	struct Node
	{
	public:
		T _data;
		Node* _prevNode;
		Node* _nextNode;

		Node() : _prevNode(nullptr), _nextNode(nullptr) {}
		Node(T data) : _data(data), _prevNode(nullptr), _nextNode(nullptr) {}
	};

public:
	class Iterator
	{
	public:
		Iterator(Node* node) : _pNode(node) {};
		//typedef forward_iterator_tag iterator_category;
		//typedef int difference_type;

		Iterator& Next();

		Iterator& operator++();
		Iterator operator++(int);

		bool operator!=(const Iterator& rhs);
		bool operator==(const Iterator& rhs);

		int operator-(const Iterator& rhs) const;

		T& operator*();
		T* operator->();
		T& operator[](int index);

	public:
		Node* _pNode;
	};


	class ReverseIterator
	{
	public:
		ReverseIterator(Node* node) : _pNode(node) {};

		ReverseIterator& Next();

		ReverseIterator& operator++();
		ReverseIterator operator++(int);

		bool operator!=(const ReverseIterator& rhs);
		bool operator==(const ReverseIterator& rhs);

		int operator-(const ReverseIterator& rhs) const;

		T& operator*();
		T* operator->();
		T& operator[](int index);

	private:
		Node* _pNode;
	};

public:
	Iterator begin();
	Iterator end();

	ReverseIterator rbegin();
	ReverseIterator rend();

public:
	DoubleLinkedList();
	DoubleLinkedList(std::initializer_list<T> list);
	~DoubleLinkedList();
	Node* Find(const T& data);
	Node* ReverseFind(const T& data);
	void Insert(const T& data, Node* nextNode = &_tail);
	void Insert(const T& data, Iterator& nextIter);
	void Insert(const T& data, ReverseIterator& nextrIter);
	void PushFront(const T& data);
	void PushBack(const T& data);
	Node* PopFront();
	Node* PopBack();
	void Erase(Node* node);
	Iterator& Erase(Iterator& iter);
	void Erase(ReverseIterator riter);
	void Clear();

private:
	Node _head;
	Node _tail;
};

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList()
{
	memset(&_head, 0, sizeof(_head));
	memset(&_tail, 0, sizeof(_tail));

	_head._nextNode = &_tail;
	_head._prevNode = nullptr;
	_tail._nextNode = nullptr;
	_tail._prevNode = &_head;
}

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(std::initializer_list<T> list)
{
	memset(&_head, 0, sizeof(_head));
	memset(&_tail, 0, sizeof(_tail));

	Node* nowNode = &_head;
	Node* node = nullptr;
	for (const T& item : list)
	{
		node = new Node(item);
		nowNode->_nextNode = node;
		node->_prevNode = nowNode;
		nowNode = node;
	}
	node->_nextNode = &_tail;
	_tail._prevNode = node;
}

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
	Clear();
}

template<typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::Find(const T& data)
{
	Node* nowNode = _head._nextNode;

	while (nowNode != &_tail)
	{
		if (nowNode->_data == data)
		{
			return nowNode;
		}
		nowNode = nowNode->_nextNode;
	}
	return nullptr;
}

template<typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::ReverseFind(const T& data)
{
	Node* nowNode = _tail._prevNode;

	while (nowNode != &_head)
	{
		if (nowNode->_data == data)
		{
			return nowNode;
		}
		nowNode = nowNode->_prevNode;
	}
	return nullptr;
}

template<typename T>
void DoubleLinkedList<T>::Insert(const T& data, Node* nextNode /*= &_tail*/)
{
	if (nextNode == &_head)
		nextNode = _head._nextNode;

	Node* node = new Node(data);
	node->_nextNode = nextNode;
	node->_prevNode = nextNode->_prevNode;
	nextNode->_prevNode = node;
	node->_prevNode->_nextNode = node;
}

template<typename T>
void DoubleLinkedList<T>::Insert(const T& data, Iterator& nextIter)
{
	Insert(data, reinterpret_cast<Node*>(&(*nextIter)));
}

template<typename T>
void DoubleLinkedList<T>::Insert(const T& data, ReverseIterator& nextrIter)
{
	Node* nextNode = reinterpret_cast<Node*>(&(*nextrIter));
}

template<typename T>
void DoubleLinkedList<T>::PushFront(const T& data)
{
	Node* node = new Node(data);
	node->_nextNode = _head._nextNode;
	_head._nextNode = node;
	node->_prevNode = &_head;
}

template<typename T>
void DoubleLinkedList<T>::PushBack(const T& data)
{
	Node* node = new Node(data);
	node->_prevNode = _tail._prevNode;
	node->_nextNode = &_tail;
	_tail._prevNode = node;
	node->_prevNode->_nextNode = node;
}

template<typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::PopFront()
{
	
}

template<typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::PopBack()
{

}

template<typename T>
void DoubleLinkedList<T>::Erase(Node* node)
{
	if (node->_prevNode == nullptr || node->_nextNode == nullptr)
	{
		return;
	}

	node->_prevNode->_nextNode = node->_nextNode;
	node->_nextNode->_prevNode = node->_prevNode;
	delete node;
}

template<typename T>
typename DoubleLinkedList<T>::Iterator& DoubleLinkedList<T>::Erase(Iterator& iter)
{
	Node* node = *reinterpret_cast<Node**>(&iter);
	iter.Next();
	Erase(node);
	return iter;
}

template<typename T>
void DoubleLinkedList<T>::Erase(ReverseIterator iter)
{
	Node* node = *reinterpret_cast<Node**>(&iter);
	iter->Next();
	Erase(node);
	return iter;
}

template<typename T>
void DoubleLinkedList<T>::Clear()
{
	Node* nowNode = _head._nextNode;
	_head._nextNode = &_tail;
	_tail._prevNode = &_head;
	while (nowNode != &_tail)
	{
		Node* nextNode = nowNode->_nextNode;
		delete nowNode;
		nowNode = nextNode;
	}
}


template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::begin()
{
	Iterator iter(_head._nextNode);
	return iter;
}

template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::end()
{
	return Iterator(&_tail);
}

template<typename T>
typename DoubleLinkedList<T>::ReverseIterator DoubleLinkedList<T>::rbegin()
{
	return ReverseIterator(_tail._prevNode);
}

template<typename T>
typename DoubleLinkedList<T>::ReverseIterator DoubleLinkedList<T>::rend()
{
	return ReverseIterator(&_head);
}

/*-------------------------------
			 Iterator
-------------------------------*/

template<typename T>
typename DoubleLinkedList<T>::Iterator& DoubleLinkedList<T>::Iterator::Next()
{
	_pNode = _pNode->_nextNode;
	return *this;
}

template<typename T>
typename DoubleLinkedList<T>::Iterator& DoubleLinkedList<T>::Iterator::operator++()
{
	return this->Next();
}

template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::Iterator::operator++(int)
{
	Iterator old = *this;
	this->_pNode = _pNode->_nextNode;
	return old;
}

template<typename T>
bool DoubleLinkedList<T>::Iterator::operator!=(const Iterator& rhs)
{
	return (this->_pNode != rhs._pNode);
}

//template<typename T>
//bool DoubleLinkedList<T>::Iterator::operator==(const Iterator& rhs)
//{
//
//}
//
//template<typename T>
//int DoubleLinkedList<T>::Iterator::operator-(const Iterator& rhs) const
//{
//
//}

template<typename T>
T& DoubleLinkedList<T>::Iterator::operator*()
{
	return this->_pNode->_data;
}

template<typename T>
T* DoubleLinkedList<T>::Iterator::operator->()
{
	return &(this->_pNode->_data);
}

template<typename T>
T& DoubleLinkedList<T>::Iterator::operator[](int index)
{
	Node* nowNode = this->_pNode;
	
	unsigned __int64 offset = reinterpret_cast<char*>(&(nowNode->_nextNode)) - reinterpret_cast<char*>(nowNode);
	//node포인터에서 _nextNode, _prevNode까지의 거리를 저장
	if (index < 0)
	{
		offset = reinterpret_cast<char*>(&(nowNode->_prevNode)) - reinterpret_cast<char*>(nowNode);
		index *= -1;
	}

	for (int i = 0; i < index; ++i)
	{
		//node부터 offset만큼 떨어져 있는 _nextNode, _prevNode값을 대입.
		nowNode = *(reinterpret_cast<Node**>(reinterpret_cast<__int64>(nowNode) + offset));

		if (nowNode->_nextNode == nullptr || nowNode->_prevNode == nullptr)
		{
			CRASH;
		}
	}
	return nowNode->_data;
}

/*-------------------------------
		  ReverseIterator
-------------------------------*/

template<typename T>
typename DoubleLinkedList<T>::ReverseIterator& DoubleLinkedList<T>::ReverseIterator::Next()
{
	_pNode = _pNode->_prevNode;
	return *this;
}

template<typename T>
typename DoubleLinkedList<T>::ReverseIterator& DoubleLinkedList<T>::ReverseIterator::operator++()
{
	return this->Next();
}

template<typename T>
typename DoubleLinkedList<T>::ReverseIterator DoubleLinkedList<T>::ReverseIterator::operator++(int)
{
	Iterator old = *this;
	this->_pNode = _pNode->_prevNode;
	return old;
}

template<typename T>
bool DoubleLinkedList<T>::ReverseIterator::operator!=(const ReverseIterator& rhs)
{
	return (this->_pNode != rhs._pNode);
}

template<typename T>
T& DoubleLinkedList<T>::ReverseIterator::operator*()
{
	return this->_pNode->_data;
}

template<typename T>
T* DoubleLinkedList<T>::ReverseIterator::operator->()
{
	return &(this->_pNode->_data);
}

template<typename T>
T& DoubleLinkedList<T>::ReverseIterator::operator[](int index)
{
	Node* nowNode = this->_pNode;
	int dir = 1;

	if (index < 0)
	{
		dir *= -1;
		index *= -1;
	}

	for (int i = 0; i < index; ++i)
	{
		if (dir == 1)
		{
			nowNode = nowNode->_prevNode;
		}
		else
		{
			nowNode = nowNode->_nextNode;
		}

		if (nowNode == &_tail || nowNode == &_head)
		{
			return nowNode->_data;
		}
	}
	return nowNode->_data;
}

#undef CRASH