#pragma once
#include <iostream>
#include "Node.h"
using namespace std;

template <typename T>
class LinkedCircularQueue
{
private:
    Node<T>* front;
    Node<T>* rear;

public:
    LinkedCircularQueue();
    bool enqueue(const T& process);
    bool dequeue(T& process);
    bool isEmpty() const;
    bool peek(T FrontEntry) const;
    void print() const;
};


template<typename T>
inline LinkedCircularQueue<T>::LinkedCircularQueue()
{
    front = rear = nullptr;
}

template<typename T>
bool LinkedCircularQueue<T>::enqueue(const T& process)
{
    Node<T>* newNode = new Node<T>();
    if (!newNode)
        return false;
    newNode->setItem(process);
    newNode->setNext(nullptr);

    if (front == nullptr)
    {
        front = rear = newNode;
        rear->setNext(front);
    }
    else
    {
        rear->setNext(newNode);
        rear = newNode;
        rear->setNext(front);
    }
    return true;
}

template<typename T>
bool LinkedCircularQueue<T>::dequeue(T& process)
{
    if (front == nullptr)
    {
        return false;
    }
    else if (front == rear)
    {
        Node<T>* temp = front;
        process = front;
        front = rear = nullptr;
        delete temp;
    }
    else
    {
        Node<T>* temp = front;
        front = front->getNext();
        rear->setNext(front);
        delete temp;
    }
    return true;
}

template<typename T>
bool LinkedCircularQueue<T>::isEmpty() const
{
    if (front == rear && front == nullptr)
        return true;
    return false;
}

template<typename T>
bool LinkedCircularQueue<T>::peek(T FrontEntry) const
{
    if (isEmpty())
        return false;

    FrontEntry = front->getItem();
    return true;
}

template<typename T>
void LinkedCircularQueue<T>::print() const
{
    Node<T>* ptr = front;
   cout<<" RDY:";
   if (!ptr)
       return;
    do {
        cout << ptr->getItem();
        if (ptr->getNext() != front)
            cout << ", ";
        ptr = ptr->getNext();
    } while (ptr != front);

}