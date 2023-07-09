#pragma once
#include "Process.h"

//finished checking//

enum ProcessorType
{
	FCFS, SJF, RR, NONE
};

enum ProcessorState
{
	BUSY, IDLE, STOP
};

class Processor
{
private:
	float PLoad;  
	float PUtil; 
	int TotalBusy;
	int TotalIdle;
protected:
	ProcessorType Type;
	ProcessorState state;
	Process* RunningProcess;
	int remainingCooling; 
	int processesInRDY;
	int ExpectedFinish;
public:
	//CONSTRUCTORS && DESTRUCTOR//
	Processor();

	//SETTERS && GETTERS//
	int getRunningProcessID() const;
	void setRunningProcess(Process* p);
	Process* getRunningProcess();
	int getTotalBusy();
	int getTotalIdle();
	void editTime();
	ProcessorType GetType();//fo get processor type
	void setState(ProcessorState st);
	ProcessorState getState();
	int GetExpectedFinish();
	void SetExpectedFinish(int time);
	int getNumOfProcessesInRDY();
	int getRemainingCooling();

	//UTILITY//
	void FromRDYToRun(int TimeStep);//move From RDYto Run
	bool steal(Processor* stolenProcessor, int& numOfProcessStolen, Process*& InvalidKillProcess, int& numOfKilledProcess);
	bool checkForSteal(Processor* stolenProcessor);
	bool OverheatSignal(int n);
	bool overheatRespond();
	bool updateProcessInRDY();

	//PURE VIRTUAL FUNCTIONS//
	virtual bool killProcess(Process*& ptr, int id, bool& killedInRun) = 0;
	virtual bool Migrate(Processor* otherProcessor, int parameter, int currTime) = 0;
	virtual bool Fork(Process* Child, int& Ct, int ForkProbability) = 0;
	virtual void scheduleAlgo(int TimeStep, Process*& ptr) = 0;
	virtual Process* GetFromRDY() = 0;//get process from redy list
	virtual void moveToReady(Process* process) = 0;
	virtual void Print() const = 0;

};