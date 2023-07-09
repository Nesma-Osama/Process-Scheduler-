#include <iostream>
#include <cstdlib>
#include "FCFSProcessor.h"

//CONSTRUCTORS && DESTRUCTOR//
FCFSProcessor::FCFSProcessor() :Processor()
{
    Type = FCFS;
    state = IDLE;
}
FCFSProcessor::~FCFSProcessor()
{
}

//UTILITY && OPERATIONS//
Process* FCFSProcessor::GetFromRDY()
{
    if (state != STOP)
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
    return nullptr;
}
void FCFSProcessor::moveToReady(Process* process)
{
    if (process && state != STOP)
    {
        bool added = RDY.enqueue(process);
        if (added)
        {
            processesInRDY++;
            int t = GetExpectedFinish();//to calculete the expected finsh to know shortest processor
            t += process->getRemainingTime();//get this time add add to it cpu time for process
            SetExpectedFinish(t);
        }
    }
}
void FCFSProcessor::scheduleAlgo(int TimeStep, Process*& ptr)
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
            setRunningProcess(nullptr);//set this running process null
            state = IDLE;
            ptr = ptr2;
            FromRDYToRun(TimeStep);//to take new element to run
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
                ptr2->editState(BLK, TimeStep);
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

        t = GetExpectedFinish() - 1;//if process in run and in each time step
        //it will decrease it cpu time so will decrease expectedfinish
        SetExpectedFinish(t);
        ptr2->editRemainingTime(1);//for decreaces cpu ramminig time each time step
        ptr2->editState(RUN, TimeStep);
        ptr2->SetCountTime(ptr2->GetCountTime() + 1);//count time be in run
        state = BUSY;
    }

}
void FCFSProcessor::Print() const
{
    cout << "[FCFS]: " << processesInRDY << " RDY: ";
    RDY.printQueue();
}
bool FCFSProcessor::killProcess(Process*& ptr, int id, bool& killedInRun) //bool to check if the process can be killed
{
    if (state == STOP)
        return false;

    bool canDeleteRDY = 0;
    bool canDeleteRUN = 0;
    Node<Process*>* process;
    if (RDY.canDelete(id))
        canDeleteRDY = true;
    else if (getRunningProcess() && getRunningProcess()->GetPID() == id)
        canDeleteRUN = true;

    if (canDeleteRDY) //if the ID exists
    {
        RDY.DeleteId(process, id);

        if (process)
        {
            ptr = process->getItem();
            SetExpectedFinish(GetExpectedFinish() - ptr->getRemainingTime());
            process = nullptr;
            processesInRDY--;
        }

        return true;
    }
    else if (canDeleteRUN)
    {
        ptr = getRunningProcess();
        setRunningProcess(nullptr);
        state = IDLE;
        SetExpectedFinish(GetExpectedFinish() - ptr->getRemainingTime());
        killedInRun = true;
        return false;
    }
    return false;
}
bool FCFSProcessor::Migrate(Processor* otherProcessor, int parameter, int currTime)
{

    int wt = -1;
    Process* runningProcess = getRunningProcess();
    if (!runningProcess)
        return false;

    wt = getRunningProcess()->WTsoFar(currTime);
    int Req = 0, EX = 0;
    if (wt > parameter && !runningProcess->isChild() && runningProcess->getRemainingTime())
    {
        if (runningProcess->CheckPair(Req, EX))
        {
            runningProcess->EditPair(Req - runningProcess->GetCountTime(), EX);
            runningProcess->SetCountTime(0);
        }

        SetExpectedFinish(GetExpectedFinish() - getRunningProcess()->getRemainingTime());
        otherProcessor->moveToReady(getRunningProcess());
        setRunningProcess(nullptr);
        state = IDLE;
        FromRDYToRun(currTime);
        return true;
    }
    else
        return false;
}
bool FCFSProcessor::Fork(Process* Child, int& Ct, int ForkProbability)
{
    if (state == BUSY)
    {
        int r = rand() % 100 + 1;
        if (r < ForkProbability)//ForkProbability)
            if (getRunningProcess() && getRunningProcess()->getXChild('L') && getRunningProcess()->getXChild('R'))
                return false;
            else if(getRunningProcess())
            {
                Ct = getRunningProcess()->getRemainingTime();
                return true;
            }
    }
    return false;
}
