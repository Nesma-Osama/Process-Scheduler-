#pragma once
#include"SL_PriorityQueue.h"
#include"Processor.h"


class SJFProcessor :
    public Processor
{
private:
    SL_PriorityQueue<Process*>RDY;

public:
    //CONSTRUCTORS && DESTRUCTOR//
    SJFProcessor();
    ~SJFProcessor();

    //UTILITY && OPERATIONS//
    virtual void Print() const;
    virtual void moveToReady(Process* process);//to put in RDYlist
    Process* GetFromRDY();//to get run process fromRDY
    virtual void scheduleAlgo(int TimeStep, Process*& ptr);
    bool killProcess(Process*& ptr, int id, bool& killedInRun);
    virtual bool canDelete(int ID);
    virtual bool Migrate(Processor* otherProcessor, int parameter, int currTime);
    virtual bool Fork(Process* Child, int& Ct, int ForkProbability);
};

