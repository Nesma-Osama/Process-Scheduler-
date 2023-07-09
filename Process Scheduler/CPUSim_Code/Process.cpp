#include "Process.h"


//FRIENDS && OVERLOADING//
bool Process:: operator >(const Process& process)const//for copartor in pariority queue
{

	if (this->GetCT() > process.GetCT())
		return true;
	else
		return false;
}
ostream& operator<<(ostream& output, const Process* process)
{
	if (process)
		output << process->GetPID();
	return output;
}

//CONSTRUCTORS && DESTRUCTOR//
Process::Process()
{
	state = NEW;
	PID = 0;
	AT = 0;
	RT = 0;
	CT = 0;
	TT = 0;
	TRT = 0;
	WT = 0;
	IsFork = false;
	IsChild = false;
	IsOrphan = false;
	Lchild = nullptr;
	Rchild = nullptr;
	totalIO_D = 0;
	TimeSpentSoFar = 0;
	CountTime = 0;
	TimeIOEX = 0;
	Time_To_running = 0;
	numOfRequests = 0;
	FirstRun = 0;
	FirstTimeRunMoment = 0;
	RemainingCT = CT;
	timeSpentInRun = 0;
}
Process::Process(int pid, int cpu, int at, bool isC) :PID(pid), CT(cpu),AT(at),IsChild(isC)
{
	state = NEW;
	RT = 0;
	TT = 0;
	TRT = 0;
	WT = 0;
	IsOrphan = false;
	IsFork = false;
	Lchild = nullptr;
	Rchild = nullptr;
	totalIO_D = 0;
	TimeSpentSoFar = 0;
	CountTime = 0;
	TimeIOEX = 0;
	Time_To_running = 0;
	numOfRequests = 0;
	FirstRun = 0;
	FirstTimeRunMoment = 0;
	RemainingCT = CT;
	timeSpentInRun = 0;
}
Process::~Process()
{
}

//SETTERS && GETTERS//
void Process::SetPID(int id)
{
	PID = id;
}
int Process::GetPID() const
{
	return PID;
}
void Process::SetAT(int t)
{
	AT = t;
}
int Process::GetAT() const
{
	return AT;
}
void Process::SetRT(int t)
{
	RT = t;
}
int Process::GetRT() const
{
	return RT;
}
void Process::SetCT(int t)
{
	CT = t;
}
int Process::GetCT() const
{
	return CT;
}
void Process::SetTT(int t)
{
	TT = t;
}
int Process::GetTT() const
{
	return TT;
}
void Process::SetTRT(int t)
{
	TRT = t;
}
int Process::GetTRT() const
{
	return TRT;
}
void Process::SetWT(int t)
{
	WT = t;
}
int Process::GetWT() const
{
	return WT;
}
processState Process::GetState()
{
	return processState(state);
}
void Process::editState(processState ps, int TimeStep)
{
	state = ps;
	if (state == TRM)
	{
		calculateTT(TimeStep);
		calculateTRT();
		calculateWT();
	}
	else if (state == RUN)
	{
		calculteRT(TimeStep);
	}
}

//RUN FUNCTIONS//
int Process::getRemainingTime()
{
	return RemainingCT;
}
bool Process::editRemainingTime(int timeSpentInCPU)
{
	if (RemainingCT - timeSpentInCPU >= 0)//initial remaning=cp 
	{
		RemainingCT -= timeSpentInCPU;
		return true;
	}
	return false;
}
int Process::getFirstTimeRunMoment()
{
	return FirstRun;
}
void Process::setFirstTimeRunMoment(int currTime)
{
	FirstTimeRunMoment = currTime;
}

//IO FUNCTIONS//
void Process::SetPairIO(int R, int D)//to put pair in queue
{
	Pair_IO io;
	io.SetIO_Req(R);
	io.SetIO_Ext(D);
	PIO.enqueue(io);
}
bool Process::GetPairIO()//to get pair from queue
{
	Pair_IO io;
	if (PIO.dequeue(io))
	{
		return true;
	}
	return false;
}
bool Process::CheckPair(int& R, int& D)//to get pair from queue
{
	Pair_IO io;
	if (PIO.peek(io))
	{
		R = io.GetIO_Req();
		D = io.GetIO_Ext();
		return true;
	}
	return false;
}
bool Process::EditPair(int R, int D)//to get pair from queue
{
	Pair_IO io;
	if (PIO.peek(io))
	{
		io.SetIO_Req(R);
		io.SetIO_Ext(D);
		return true;
	}
	return false;
}
bool Process::increaseTimeSpentInIO()
{
	TimeSpentSoFar++;
	Pair_IO pair;
	PIO.peek(pair);
	if (TimeSpentSoFar == GetTimeIOEX())
	{
		totalIO_D += GetTimeIOEX();
		PIO.dequeue(pair);
		TimeSpentSoFar = 0;
		return true;
	}
	return false;
}
bool Process::hasFinishedIO()
{
	return false;
}
void Process::SetTime_To_running(int time)
{
	Time_To_running = time;
}
int Process::GetTime_To_running()
{
	return Time_To_running;
}
void Process::SetCountTime(int t)
{
	CountTime = t;
}
int Process::GetCountTime()
{
	return CountTime;
}
void Process::SetIoEx(int time)
{
	TimeIOEX = time;
}
int Process::GetTimeIOEX()
{
	return TimeIOEX;
}

//CALCULATIONS//
int Process::WTsoFar(int timeStep)
{
	return timeStep - AT - (CT - RemainingCT) + 1;
}
void Process::calculteRT(int currTime)
{
	FirstRun++;
	if (FirstRun == 1)
	{
		RT = currTime - AT;
	}
}
void Process::calculateTT(int currTime)
{
	if (state == TRM || state == KILLED)
	{
		TT = currTime;
	}
}
void Process::calculateTRT()
{
	if (state == TRM || state == KILLED)
		TRT = TT - AT;
}
void Process::calculateWT()
{
	if (state == TRM)
		WT = TRT - CT;
	else if (state == KILLED)
		WT = TRT - CT + RemainingCT;
}
int Process::getTotalIO_D()
{
	if (state == TRM || state == KILLED)
		return totalIO_D;
	return 0;
}

//FORK FUNCTIONS//
bool Process::SetChild(Process* ptr)
{
	if (!Lchild)
	{
		Lchild = ptr;
		IsFork = true;
		return true;
	}
	else if (Lchild && !Rchild)
	{
		Rchild = ptr;
		return true;
	}
	else return false;
}
Process* Process::getXChild(char Xchild)
{
	if (Xchild == 'L')
		return Lchild;
	if (Xchild == 'R')
		return Rchild;
	return nullptr;
}
void Process::setIsForked(bool x)
{
	IsFork = x;
}
bool Process::getIsForked()
{
	return IsFork;
}
bool Process::isChild()
{
	return IsChild;
}
