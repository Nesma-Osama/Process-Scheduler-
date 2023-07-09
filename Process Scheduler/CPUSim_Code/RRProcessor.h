#pragma once
#include"Processor.h"
#include "LinkedQueue.h"

class RRProcessor :public Processor
{
private:
    LinkedQueue<Process*> RDY;
    int timeSlice;

public:
    //CONSTRUCTORS && DESTRUCTOR//
    RRProcessor();
    RRProcessor(int slice);
    ~RRProcessor();

    //UTILITY && OPERATIONS//
    void scheduleAlgo(int TimeStep, Process*& ptr);
    void moveToReady(Process* process);//to put in RDYlist
    Process* GetFromRDY();//to get run process fromRDY
    virtual void Print() const;
    bool killProcess(Process*& ptr, int id, bool& killedInRun);
    virtual bool canDelete(int ID);
    virtual bool Migrate(Processor* otherProcessor, int parameter, int currTime);
    virtual bool Fork(Process* Child, int& Ct, int ForkProbability);
};
