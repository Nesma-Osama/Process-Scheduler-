#include "Processor.h"

//CONSTRUCTORS && DESTRUCTOR//
Processor::Processor()
{
	PLoad = 0;
	PUtil = 0;
	TotalBusy = 0;
	TotalIdle = 0;
	Type = NONE;
	state = IDLE;
	RunningProcess = nullptr;
	remainingCooling = 0;
	processesInRDY = 0;
	ExpectedFinish = 0;
}

//SETTERS && GETTERS//
int Processor::getRunningProcessID() const
{
	if(RunningProcess)
		return RunningProcess->GetPID();
	return 0;

}
void Processor::setRunningProcess(Process* p)
{
	RunningProcess = p;
}
Process* Processor::getRunningProcess()
{
	return RunningProcess;
}
int Processor::getTotalBusy()
{
	return TotalBusy;
}
int Processor::getTotalIdle()
{
	return TotalIdle;
}
void Processor::editTime()
{
	if (state == BUSY)
		TotalBusy++;
	else if (state == IDLE)
		TotalIdle++;
}
ProcessorType Processor::GetType()
{
	return ProcessorType(Type);
}
void Processor::setState(ProcessorState st)
{
	state = st;
}
ProcessorState Processor::getState()
{
	return ProcessorState(state);
}
int Processor::GetExpectedFinish()
{
	return ExpectedFinish;
}
void Processor::SetExpectedFinish(int time)
{
	ExpectedFinish = time;
}
int Processor::getNumOfProcessesInRDY()
{
	return processesInRDY;
}

int Processor::getRemainingCooling()
{
	return remainingCooling;
}

//UTILITY//
void Processor::FromRDYToRun(int TimeStep)
{
	if (state == IDLE && processesInRDY>0)//if run is empty
	{
		Process* ptr = nullptr;
		ptr = GetFromRDY();
		ptr->calculteRT(TimeStep);
		if (ptr == nullptr)
		{
			setRunningProcess(nullptr);
			state = IDLE;
			processesInRDY = 0; //processesInRDY=0;
			return;
		}
		else
		{
			setRunningProcess(ptr);//set it ti running
			state = BUSY;
			processesInRDY--;//decrease count of process in RDY
			ptr->SetTime_To_running(TimeStep);//for time startrunning
		}
	}
}
bool Processor::checkForSteal(Processor* stolenProcessor)
{
	if (!stolenProcessor)
		return false;

	int Long = stolenProcessor->ExpectedFinish;
	int num = stolenProcessor->getNumOfProcessesInRDY();
	int Short = ExpectedFinish;
	float stealLimit = 0;

	if (Long)
		stealLimit = (float(Long - Short) / Long) * 100;

	if (stealLimit > 40 && num > 0)
		return true;

	return false;
}

bool Processor::OverheatSignal(int n)
{
	if (state != STOP)
	{
		remainingCooling = n;
		state = STOP;
		ExpectedFinish = 0;
		return true; //accepted the signal
	}
	return false; //rejected the signal (already overheated)
}

bool Processor::overheatRespond()
{
	if (state == STOP && remainingCooling > 0)
	{
		remainingCooling--;
		if (remainingCooling == 0)
		{
			state = IDLE;
			return true; //if finished cooling
		}
		return false; //if haven't finished yet
	}
	return false; //if it's not overheated}
}

bool Processor::updateProcessInRDY()
{
	if (processesInRDY)
	{
		processesInRDY--;
		return true;
	}
	return false;
}

bool Processor::steal(Processor* stolenProcessor, int& numOfProcessStolen, Process* &InvalidKillProcess, int& numOfKilledProcess)
{
	Process* processToBeStolen = nullptr;

	bool canSteal = checkForSteal(stolenProcessor);

	if (canSteal)
	{
		//int counterToAvoidLoop = stolenProcessor->getNumOfProcessesInRDY();
		processToBeStolen = stolenProcessor->GetFromRDY();
		stolenProcessor->processesInRDY--;
		stolenProcessor->SetExpectedFinish(stolenProcessor->GetExpectedFinish() - (processToBeStolen->getRemainingTime()));
		bool Rkilled = false;
		bool Ekilled = false;
		if (!processToBeStolen)
			return false;

		if (processToBeStolen && processToBeStolen->isChild() && Type != FCFS) //if any processor other than FCFS tries to steal a forked process, Add it to the end of the list and check for the next process 
		{
			Ekilled= stolenProcessor->killProcess(InvalidKillProcess, processToBeStolen->GetPID(), Rkilled);
			if (Ekilled || Rkilled)
				numOfKilledProcess++;
				
		}

		if (processToBeStolen)
		{
			moveToReady(processToBeStolen);
			numOfProcessStolen++;
		}
		return true;
	}
	return false;
}

/*while (processToBeStolen && processToBeStolen->isChild() && Type != FCFS) //if any processor other than FCFS tries to steal a forked process, Add it to the end of the list and check for the next process 
		{
			Ekilled= stolenProcessor->killProcess(processToBeStolen, processToBeStolen->GetPID(), Rkilled);
			/*stolenProcessor->moveToReady(processToBeStolen);
			processToBeStolen = stolenProcessor->GetFromRDY();
			stolenProcessor->processesInRDY--;
			counterToAvoidLoop--;
			if (counterToAvoidLoop == 0)
				break;*/