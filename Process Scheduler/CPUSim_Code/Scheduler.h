#pragma once
#include"Processor.h"
#include"LinkedQueue.h"
#include<cstdlib>
#include<ctime>
#include<fstream>
#include"FCFSProcessor.h"
#include"RRProcessor.h"
#include"SJFProcessor.h"
#include"UI.h"
#include <random>

class KillPair
{
	int KillID;
	int KillTime;
public:
	void SetKillID(int k)
	{
		KillID = k;
	}
	void SetKillTime(int t)
	{
		KillTime = t;
	}
	int GetKillID() const
	{
		return KillID;
	}
	int GetKillTime() const
	{
		return KillTime;
	}
};

class Scheduler
{
private:
	//LISTS//
	Processor* ProcessorList[100];
	LinkedQueue<Process*> NewList;
	LinkedQueue<Process*> BLKList;
	LinkedQueue<Process*> termList;
	LinkedQueue<KillPair*> KillPairQueue;
	LinkedQueue<Process*> Aux;
	UI* UIptr;

	//NUMBER IN LISTS//
	int ProcessNum;
	int ProcessorNum;
	int Nf, Ns, Nr;
	int numOfRDYinAUX;
	int TermCount;
	int BlockCount;
	int RUNCount;

	//GENERAL PARAMETERS//
	int TimeStep;
	int RTF;//for migration
	int MaxW;//for migration
	int STL;//for still
	float Steallimit;//initialy 40
	int LastID;//to know the new id for dorking process;
	int ForkProbablity;
	int numOfForked;
	int numOfKilledProcess;

	//STATISTICS DATA//
	int overheatConst;
	int overheatProbability;
	int numOfOverheatedProcessors;
	int numOfProcessStolen;
	float Util, WT, RT, TRT, N_RTF, N_Max;
	float KILL, FORK, STEAL, LOAD;
	float totIO_D;

public:
	//CONSTRUCTORS & DESTRUCTOR//
	Scheduler();
	Scheduler(UI* ptr);
	~Scheduler();

	//MOVE FROM TO//
	void MoveToNew(Process* process);
	void FromAuxToRDY();
	void MoveToBlock(Process* process);
	void MoveToTerm(Process* process);
	void FromRDYtoRUNSim();
	void fromBLKtoRDY();
	void FromNEWtoRDY();

	//SIMULATION//
	void Simulator();
	void Migrate();
	void stealProcessSim();
	void killProcessSim();
	void ForkingSim();
	void killOrphSim(Process* process);
	void scheduleAlgo();
	void ProcessorOverheatSimulation();

	//UTILIZATION//
	void sendToShortestQueue(Process* process, int except = -11, ProcessorType type = NONE);
	int getLongestQueueIND();
	int getShortestQueueIND(ProcessorType type = NONE, int except = -11);

	//SETTERS & GETTERS//
	int GetProcessorNum()const;
	void SetProcessNum(int Num);
	void SetProcessor(Processor* P);

	//INPUT AND OUTPUT FILES//
	void generateOutputFile();
	void Load();
	
};
