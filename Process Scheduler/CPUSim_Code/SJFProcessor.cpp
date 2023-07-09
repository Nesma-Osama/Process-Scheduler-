#include "SJFProcessor.h"
#include <iostream>
using namespace std;

//CONSTRUCTORS && DESTRUCTOR//
SJFProcessor::SJFProcessor()
{
    Type = SJF;
    state = IDLE;

}
SJFProcessor::~SJFProcessor()
{

}

//UTILITY && OPERATIONS//
bool SJFProcessor::Migrate(Processor* otherProcessor, int parameter, int currTime)
{
    return false;
}
bool SJFProcessor::Fork(Process* Child, int& Ct, int ForkProbability)
{
    return false;
}
void SJFProcessor::scheduleAlgo(int TimeStep, Process*& ptr)
{
    FromRDYToRun(TimeStep);//to check if there element in run or not and put one
    Process* ptr2 = getRunningProcess();

    if (ptr2 != nullptr)
    {
        ptr2->calculteRT(TimeStep);
        int EX = 0; int Req = 0;
        int t;
        //int time;
        if (ptr2->getRemainingTime() == 0)
        {
            ptr2->editState(TRM,TimeStep);//put this process in term
            ptr = ptr2;
            setRunningProcess(nullptr);//set this running process null
            state = IDLE;
            FromRDYToRun(TimeStep);//to tacke new element to run
            ptr2 = getRunningProcess();
            if (ptr2 == nullptr)
            {
                state = IDLE;
                return;
            }
        }

        if (ptr2->CheckPair(Req, EX))
        {
            if (TimeStep == ptr2->GetTime_To_running() + Req)//go to block
            {
                ptr2->SetCountTime(0);//modified Req
                ptr2->SetIoEx(EX);//for time of excuetion in block
                t = GetExpectedFinish() - ptr2->getRemainingTime();
                SetExpectedFinish(t);//update expected finsih
                ptr2->editState(BLK,TimeStep);
                ptr = ptr2;
                setRunningProcess(nullptr);//set this running process null
                ptr2->GetPairIO();//remove this pair from pair_ioqueue
                FromRDYToRun(TimeStep);//to tacke new element to run
                ptr2 = getRunningProcess();
                if (ptr2 == nullptr)
                {
                    state = IDLE;
                    return;
                }
            }
        }

        t = GetExpectedFinish() - 1;//if process in run and in each time step
        //it will decrease it cpu time so will decrease expectedfinish
        SetExpectedFinish(t);
        ptr2->editRemainingTime(1);//for decreaces cpu ramminig time each time step
        ptr2->editState(RUN,TimeStep);
        ptr2->SetCountTime(ptr2->GetCountTime() + 1);//count time be in run
        ptr2->editState(RUN, TimeStep);
        state = BUSY;
    }
}
bool SJFProcessor::killProcess(Process*& ptr, int id,bool& killedInRun)
{
    return false;
}
bool SJFProcessor::canDelete(int ID)
{
    return false;
}
void  SJFProcessor::moveToReady(Process* process)
{
    if (process && state != STOP)
    {
         SetExpectedFinish(  ( GetExpectedFinish() + process->getRemainingTime()));
        RDY.add(process);
        processesInRDY++;
    }
}
Process*SJFProcessor::GetFromRDY()//to get process from RDY
{
    if (RDY.isEmpty())
        return nullptr;//return null if empty
    else
    {
        Process* ptr = nullptr;
        RDY.remove(ptr);//get this process
        return ptr;
    }
}
void SJFProcessor::Print() const
{
    cout << "[SJF ]: " << processesInRDY<<" RDY: ";
    RDY.print();
}
