#pragma once
#pragma once
#include "LinkedQueue.h"
class FCFSQueue :
	public LinkedQueue<Process*>
{

public:
	FCFSQueue()
	{
	}
	bool DeleteInd(int Ind) //Delete from the Queue by Dummy Index start with 0
	{
		if (isEmpty())
			return false;
		else
		{
			Node<Process*>* ptr = frontPtr;
			Node<Process*>* tmp = nullptr;
			if (Ind == 0) //if the first index 
			{
				tmp = frontPtr;
				frontPtr = frontPtr->getNext();
				delete tmp;
				return true;

			}
			for (int i = 0; i < Ind - 1; i++)
			{
				if (ptr->getNext())
					ptr = ptr->getNext();
			}
			if (ptr && ptr->getNext())  //if in the middle of the Queue
			{
				tmp = ptr->getNext();
				ptr->setNext(ptr->getNext()->getNext());
				delete tmp;
				return true;
			}
			else
				if (ptr) //if the last one in the Queue
				{
					Node<Process*>* pre = frontPtr;
					while (pre->getNext())
					{
						pre = pre->getNext();
					}
					tmp = pre->getNext();
					pre->setNext(nullptr);
					backPtr = pre;
					delete tmp;
					return true;
				}
		}
		return false;
	}
	bool canDelete(int ID)
	{
		Node<Process*>* ptr = frontPtr;
		while (ptr)
		{
			if (ptr->getItem() && ptr->getItem()->GetPID() == ID)
				return true;
			if (ptr)
				ptr = ptr->getNext();
		}
		return false;
	}

	void DeleteId(Node<Process*>* &process, int id)
	{
		if (isEmpty())
			return;
		else
		{
			Node<Process*>* ptr = frontPtr;
			Node<Process*>* pre = frontPtr;
			while (ptr)
			{
				if (ptr->getItem()->GetPID() == id)
				{
					if (ptr == frontPtr) //if the first index 
					{
						process = frontPtr;
						frontPtr = frontPtr->getNext();;
						if (!frontPtr)
							backPtr = nullptr;
						return;
					}
					else if (ptr && ptr->getNext())
					{
						process = ptr;
						pre->setNext(ptr->getNext());
						return;
					}

					else if (ptr)
					{
						process = ptr;
						backPtr = pre;
						pre->setNext(nullptr);
						return;
					}
							
				}
				pre = ptr;
				ptr = ptr->getNext();
			}
		}
	}

	~FCFSQueue()
	{
	}
};

