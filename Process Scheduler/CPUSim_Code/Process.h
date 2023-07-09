#pragma once
#include <iostream>
using namespace std;
#include"LinkedQueue.h"
#ifndef PROCESS_
#define PROCESS_

//finished checking//

//IO request pair
class Pair_IO {//to make it a pair of io
private:
	int IO_Req=0;//io request time 
	int IO_Ext=0;//io extuation time
	
public:
	void SetIO_Req(int req)
	{
		IO_Req = req;
	}
	int GetIO_Req()
	{
		return IO_Req;
	}
	void SetIO_Ext(int ext)
	{
		IO_Ext = ext;
	}
	int GetIO_Ext()
	{
		return IO_Ext;
	}
};

//Process state
enum processState
{
	NEW,
	RDY,
	RUN,
	BLK,
	TRM,
	ORPH,
	KILLED,
	FORKED,
	AUX,
	T1,
	T2
};

class Process
{
private:
	//GENERAL PROPERTIES//
	enum processState state;
	int PID;   //process ID
	int AT;    //Arrival Time 
	int RT;    //Response Time
	int CT;    //CPU Time
	int TT;    //Termination time
	int TRT;   //Turnarround Time
	int WT;    //Waiting Time

	//FORK PROPERTIES//
	bool IsFork;
	bool IsChild;
	bool IsOrphan;
	Process* Lchild;
	Process* Rchild;

	//IO PROPERTIES//
	LinkedQueue<Pair_IO> PIO;
	int totalIO_D;
	int TimeSpentSoFar; //time spent in IO so far
	int CountTime;//to modify requred thime for block
	int TimeIOEX;//for process in block
	int Time_To_running;
	int numOfRequests;

	//RUNNING PROPERTIES//
	int FirstRun;
	int FirstTimeRunMoment;
	int RemainingCT;
	int timeSpentInRun; //the time that the process has spent in the current process

public:

	//FRIENDS && OVERLOADING//
	bool operator>(const Process& process)const;
	friend ostream& operator<<(ostream& output, const Process* process);

	//CONSTRUCTORS && DESTRUCTOR//
	Process();
	Process(int pid, int cpu, int at, bool isC);
	~Process();

	//SETTERS && GETTERS//
	void SetPID(int id);//set ID
	int GetPID() const;//getID
	void SetAT(int t);//set arrival time
	int GetAT()const;//get arrival time
	void SetRT(int t);//set response time
	int GetRT()const;//get response time
	void SetCT(int t);//set cpu time
	int GetCT()const;//get cpu time
	void SetTT(int t);//set termination time
	int GetTT()const;//get termination  time
	void SetTRT(int t);//set turn around  time
	int GetTRT()const;//get turn arround  time
	void SetWT(int t);//set wating  time
	int GetWT()const;//get wating time
	processState GetState();
	void editState(processState ps, int TimeStep);

	//RUN FUNCTIONS//
	int getRemainingTime();
	bool editRemainingTime(int timeSpentInCPU); //if the remainingCT=0, it returns false 
	int getFirstTimeRunMoment();
	void setFirstTimeRunMoment(int currTime);

	//IO FUNCTIONS//
	void SetPairIO(int R, int D);
	bool GetPairIO();
	bool CheckPair(int& R, int& D);
	bool EditPair(int R, int D);
	bool increaseTimeSpentInIO();
	bool hasFinishedIO();
	void SetTime_To_running(int time);
	int GetTime_To_running();
	void SetCountTime(int t);
	int GetCountTime();
	void SetIoEx(int time);
	int GetTimeIOEX();

	//CALCULATIONS//
	int WTsoFar(int timeStep);
	void calculteRT(int currTime);
	void calculateTT(int currTime);
	void calculateTRT();
	void calculateWT();
	int getTotalIO_D();

	//FORK FUNCTIONS//
	bool SetChild(Process* ptr); //for forking process
	Process* getXChild(char Xchild);
	void setIsForked(bool x);
	bool getIsForked();
	bool isChild();

};


#endif

