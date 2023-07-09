#pragma once
#include "Process.h"
#include "Processor.h"
#include "LinkedQueue.h"

enum interfaceMode
{
	INTERACTIVE,
	STEP_BY_STEP,
	SILENT
};

class UI
{
private:
	interfaceMode Mode;
	bool SilentFT;
	void printInteractive(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm, int numofRun,int  NumOfRDYinAUX,LinkedQueue<Process*>aux, int n);
	void printStepByStep(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm, int numofRun,int  NumOfRDYinAUX, LinkedQueue<Process*>aux, int n) ;
	void printSilent();
public:
	UI();
	~UI();
	void updateInterface(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm,int numofRun,int  NumOfRDYinAUX, LinkedQueue<Process*>aux, int n) ;
	void ReadInput(int& PID, int& CT);

};

