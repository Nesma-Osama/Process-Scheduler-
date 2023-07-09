#pragma once
#include"Node.h"
#include"Process.h"

#ifndef SL_PRIORITY_QUEUE_
#define SL_PRIORITY_QUEUE_
template<class T>
class SL_PriorityQueue 
{
private:
	Node<T>*headptr; // Pointer to sorted list of
	// items in the priority queue
	Node<T>* getNodeBefore(const T& anEntry) const;
public:
	SL_PriorityQueue();
	SL_PriorityQueue(const SL_PriorityQueue<T>& pq);
	~SL_PriorityQueue();
	bool isEmpty() const;
	bool add(const T& newEntry);
	bool remove(T&Ptr);
	/** @throw PrecondViolatedExcep if priority queue is empty. */
	bool peek(T&ptr) const;
	void print() const;
}; // end SL_PriorityQueue


template<class T>
Node<T>* SL_PriorityQueue<T>::getNodeBefore(const T& anEntry) const
{
	Node<T>* curPtr = headptr;
	Node<T>* prevPtr = nullptr;
	while ((curPtr != nullptr) && anEntry && (*anEntry) > *(curPtr->getItem()))
	{
		prevPtr = curPtr;
		curPtr = curPtr->getNext();
	} // end while
	return prevPtr;
}
template<class T>
SL_PriorityQueue<T>::SL_PriorityQueue()
{
	headptr = nullptr;
}
template<class T>
bool SL_PriorityQueue<T>::isEmpty() const
{
	return (headptr == nullptr);
}
template<class T>
bool SL_PriorityQueue<T>::peek(T& ptr) const
{
	if (isEmpty())
		return false;

	ptr = headptr->getItem();
	return true;

}

template<class T>
void SL_PriorityQueue<T>::print() const
{
    Node<T>* ptr = headptr;
	if (!ptr)
		return;
	while (ptr)
	{
		cout << ptr->getItem();
		if (ptr->getNext())
			cout << ", ";
		ptr = ptr->getNext();
	}
}

template<class T>
SL_PriorityQueue<T>::SL_PriorityQueue(const SL_PriorityQueue<T>& LQ)
{
	Node<T>* NodePtr = LQ.headptr;
	if (!NodePtr) //LQ is empty
	{
		headptr = nullptr;
		return;
	}

	//insert the first node
	Node<T>* ptr = new Node<T>(NodePtr->getItem());
	headptr = ptr;
	NodePtr = NodePtr->getNext();

	//insert remaining nodes
	while (NodePtr)
	{
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		NodePtr = NodePtr->getNext();
	}
}

template<class T>
bool SL_PriorityQueue<T>::remove(T& Ptr)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = headptr;
	Ptr = headptr->getItem();
	headptr = headptr->getNext();
	// Queue is not empty; remove front


	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	nodeToDeletePtr = nullptr;
	return true;

}

template<class T>
SL_PriorityQueue<T>::~SL_PriorityQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (remove(temp));
}
template<class T>
bool SL_PriorityQueue<T>::add(const T& newEntry)
{

	Node<T>* newNodePtr = new Node<T>(newEntry);
	Node<T>* prevPtr = getNodeBefore(newEntry);
	if (isEmpty() || (prevPtr == nullptr)) // Add at beginning
	{
		newNodePtr->setNext(headptr);
		headptr = newNodePtr;
	}
	else // Add after node before
	{
		Node<T>* aftPtr = prevPtr->getNext();
		newNodePtr->setNext(aftPtr);
		prevPtr->setNext(newNodePtr);
	} // end if
	return true;
}

#endif