#pragma once
#include"Processor.h"
#include"FCFSQueue.h"
class FCFSProcessor : public Processor
{
private:
    FCFSQueue RDY;
public:
    //CONSTRUCTORS && DESTRUCTOR//
    FCFSProcessor();
    ~FCFSProcessor();

    //UTILITY && OPERATIONS//
    Process* GetFromRDY();//to get run process fromRDY
    virtual void moveToReady(Process* process);
    virtual void scheduleAlgo(int TimeStep, Process*& ptr);
    virtual void Print() const;
    bool killProcess(Process*& ptr, int id, bool& killedInRun);
    virtual bool Migrate(Processor* otherProcessor, int parameter, int currTime);
    bool Fork(Process* Child, int& Ct, int ForkProbability);
};


