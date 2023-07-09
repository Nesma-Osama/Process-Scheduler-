#include "RRProcessor.h"
#include <iostream>
#include "Node.h"
using namespace std;

//CONSTRUCTORS && DESTRUCTOR//
RRProcessor::RRProcessor():timeSlice(0)
{
    Type = RR;
    state = IDLE;
}
RRProcessor::RRProcessor(int t): timeSlice(t)
{
    Type = RR;
}
RRProcessor::~RRProcessor()
{

}

//UTILITY && OPERATIONS//
void RRProcessor::scheduleAlgo(int TimeStep, Process*& ptr)
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
            ptr2->editState(TRM, TimeStep);//put this process in term
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
                ptr2->SetCountTime(0);//moved to block
                ptr2->SetIoEx(EX);//for time of excuetion in block
                t = GetExpectedFinish() - ptr2->getRemainingTime();
                SetExpectedFinish(t);//update expected finsih
                ptr2->editState(BLK,TimeStep);
                ptr = ptr2;
                setRunningProcess(nullptr);//set this running process null
                state = IDLE;
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
        if (TimeStep == ptr2->GetTime_To_running() + timeSlice)
        {
            if (Req != 0)//if it has reqest
            {
                ptr2->EditPair(Req - timeSlice, EX);//if it process has a io reqest it will cut this request
                //and complete it after that when become running agin with io reqest less
                ptr2->SetCountTime(0);//modified Req
            }
            t = GetExpectedFinish() - ptr2->getRemainingTime();//remove the time from rxpected finih to put it agin when enque
            SetExpectedFinish(t);
            moveToReady(ptr2);
            setRunningProcess(nullptr);
            state = IDLE;
            FromRDYToRun(TimeStep);//to take new element to run
            ptr2 = getRunningProcess();
            if (ptr2 == nullptr)
            {
                state = IDLE;
                return;
            }
        }

        t = GetExpectedFinish() - 1;//if process in run and in each time step
        //it will decrease it cpu time so will decrease expectedfinish
        SetExpectedFinish(t);
        ptr2->editRemainingTime(1);//for decreaces cpu ramminig time each time step
        ptr2->editState(RUN, TimeStep);
       ptr2->SetCountTime( ptr2->GetCountTime() + 1);//count time be in run
       ptr2->editState(RUN, TimeStep);
       state = BUSY;
    }
}
Process* RRProcessor::GetFromRDY()//to get process from RDY
{
    if (RDY.isEmpty())
        return nullptr;//return null if empty
    else
    {
        Process* ptr = nullptr;
        RDY.dequeue(ptr);//get this process
        return ptr;
    }
}
void  RRProcessor::moveToReady(Process* process)
{
    if (process && state != STOP)
    {
        RDY.enqueue(process);
        processesInRDY++;
        int t = GetExpectedFinish();//to calculete the expected finsh to know shortest processor
        t += process->getRemainingTime();//get this time add add to it cpu time for process        SetExpectedFinish(t);
        SetExpectedFinish(t);
    }
}
void RRProcessor::Print() const
{
    cout << "[RR  ]: " << processesInRDY<<" RDY: ";
    RDY.printQueue();
}
bool RRProcessor::killProcess(Process*& ptr, int id, bool& killedInRun)
{
    return false;
}
bool RRProcessor::canDelete(int ID)
{
    return false;
}
bool RRProcessor::Migrate(Processor* otherProcessor, int parameter, int currTime)
{
    Process* ptr = getRunningProcess();
    if (ptr == nullptr)
        return false;
    if (ptr->getIsForked())
        return false;
    else
    {
        int Req = 0,EX=0;
        if (ptr->getRemainingTime() < parameter&& ptr->getRemainingTime() != 0 && !ptr->getIsForked())
        {
            if (ptr->CheckPair(Req, EX))
            {
                ptr->EditPair(Req - ptr->GetCountTime(), EX);
                ptr->SetCountTime(0);//moved from run
            }
            int t = GetExpectedFinish() - ptr->getRemainingTime();//decrease expected finish
            SetExpectedFinish(t);
            setRunningProcess(nullptr);
            state = IDLE;
            otherProcessor->moveToReady(ptr);//move this process to shorts SJF processor
            FromRDYToRun(currTime);//to take new element to run  
            ptr= getRunningProcess();//to Know if more than one process <RTF
            if (ptr != nullptr)
            {
                if (ptr->getRemainingTime() < parameter&& ptr->getRemainingTime()!=0)
                    return true;
                else
                    return false;
            }
            return false;
        }
        return false;
    }
}
bool RRProcessor::Fork(Process* Child, int& Ct, int ForkProbability)
{
    return false;
}
