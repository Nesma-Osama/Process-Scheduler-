#include "Scheduler.h"


//CONSTRUCTORS & DESTRUCTOR//
Scheduler::Scheduler(UI* ptr) :Scheduler()
{
	UIptr = ptr;
}

Scheduler::~Scheduler()
{
	for (int i = 0; i < ProcessorNum; i++)  //Allocating the SJF processors
	{
		delete ProcessorList[i];
		ProcessorList[i] = nullptr;
	}
}

Scheduler::Scheduler()
{
	totIO_D = 0;
	overheatConst = 0;
	numOfRDYinAUX = 0;
	numOfOverheatedProcessors = 0;
	overheatProbability = 0;
	WT = 0;
	Util = 0;
	TRT = 0;
	RT = 0;
	STEAL = 0;
	N_RTF = 0;
	FORK = 0;
	KILL = 0;
	LOAD = 0;
	N_Max = 0;
	numOfProcessStolen = 0;
	numOfForked = 0;
	ForkProbablity = 0;
	LastID = 0;
	MaxW = 0;
	RTF = 0;
	STL = 0;
	Steallimit = 0;
	UIptr = nullptr;
	RUNCount = 0;
	ProcessNum = 0;
	ProcessorNum = 0;
	TermCount = 0;
	BlockCount = 0;
	TimeStep = 0;//time step
	numOfKilledProcess = 0;
	for (int i = 0; i < 100; i++)
		ProcessorList[i] = nullptr;
	Nf = Ns = Nr = 0;
}

//MOVE FROM TO//
void Scheduler::MoveToNew(Process* process)
{
	if (process)
		NewList.enqueue(process);
} 
void Scheduler::FromNEWtoRDY()
{
	Process* process = nullptr;
	int Time = 0;
	if (NewList.peek(process))
	{
		Time = process->GetAT();//get arrive time
		while (TimeStep == Time)//ckeck if thete are many process arrive at same time
		{
			NewList.dequeue(process);
			sendToShortestQueue(process);
			process->editState(RDY,TimeStep);
			if (NewList.peek(process)) {
				Time = process->GetAT();
			}
			else
				break;
		}
	}
} 
void Scheduler::FromRDYtoRUNSim()
{
	RUNCount = 0;
	for (int i = 0; i < ProcessorNum; i++)
	{
		if (ProcessorList[i]->getState() == BUSY)
			RUNCount++;
		ProcessorList[i]->editTime();
	}
} 
void Scheduler::MoveToBlock(Process* process)
{
	if (process)
	{
		BLKList.enqueue(process);
		BlockCount++;
	}

}
void Scheduler::fromBLKtoRDY()
{
	Process* process = nullptr;

	BLKList.peek(process);
	if (process)
	{
		bool finished = process->increaseTimeSpentInIO();
		if (finished)
		{
			int ind = getShortestQueueIND(NONE);
			if (ind >= 0)
			{
				ProcessorList[ind]->moveToReady(process);
				BLKList.dequeue(process);
				BlockCount--;
			}
		}
	}
}
void Scheduler::scheduleAlgo()
{
	for (int i = 0; i < ProcessorNum; i++)
	{
		Process* ptr = nullptr;
		if (ProcessorList[i]->getState() == STOP)
			continue;
		ProcessorList[i]->scheduleAlgo(TimeStep, ptr);

		if (ptr != nullptr)
		{
			if (ptr->GetState() == TRM)
				MoveToTerm(ptr);
			else if (ptr->GetState() == BLK)
				MoveToBlock(ptr);
		}
	}
}
void Scheduler::MoveToTerm(Process* process)
{
	if (process)
	{
		termList.enqueue(process);
		TermCount++;
		if (process->GetState() == TRM || process->GetState() == KILLED)
		{
			process->calculateTT(TimeStep);//set the timestep to termination time
			process->calculateTRT();//calculate turnaround Duration
			process->calculateWT();//calculte wating time
		}
		killOrphSim(process);
	}
}
void Scheduler::FromAuxToRDY()
{
	if (numOfOverheatedProcessors < ProcessorNum)
	{
		Process* processToBeMoved = nullptr;
		int n = numOfRDYinAUX;
		for (int i = 0; i < n; i++)
		{
			if (Aux.dequeue(processToBeMoved))
			{
				numOfRDYinAUX--;
				if (processToBeMoved && processToBeMoved->isChild())
				{
					int ind = getShortestQueueIND(FCFS);
					if (ind >= 0)
						ProcessorList[ind]->moveToReady(processToBeMoved);
					else if (ind < 0) //all FCFS Processors are overheated, then just kill the process
					{
						processToBeMoved->editState(TRM, TimeStep);
						MoveToTerm(processToBeMoved);
					}
				}
				else
					sendToShortestQueue(processToBeMoved);
			}
		}
	}
}

//SIMULATION//
void Scheduler::Simulator()
{
	// the simulator is to be edited, the random scheduling will be deleted//
	Load(); //call load function
	int Time = 0;
	int count = 0;
	Process* process = nullptr;
	while (TermCount < ProcessNum)//check if all process in term
	{

		/////////////////////////////////////////////////////////// Move From NEW to RDY Part ///////////////////////////////////////////////////////////////////////////////
		FromNEWtoRDY();
		/////////////////////////////////////////////////////////// Fork a child-Simulation Part ///////////////////////////////////////////////////////////////////////////////
		ForkingSim();
		/////////////////////////////////////////////////////////// Overheat-Simulation part ///////////////////////////////////////////////////////////////////////////////
		ProcessorOverheatSimulation();
		/////////////////////////////////////////////////////////// Move From NEW to RDY Part ///////////////////////////////////////////////////////////////////////////////
		Migrate();
		/////////////////////////////////////////////////////////// Kill Process-Simulation Part ////////////////////////////////////////////////////////////////////////////
		killProcessSim();
		/////////////////////////////////////////////////////////// Steal Process-Simulation Part ////////////////////////////////////////////////////////////////////////////
		stealProcessSim();
		///////////////////////////////////////////////////////////  From BLK To RDY  ///////////////////////////////////////////////////////////////////////////////
		fromBLKtoRDY();
		/////////////////////////////////////////////////////////// Move From Aux To RDY ///////////////////////////////////////////////////////////////////////////////
		//FromAuxToRDY(); 
		///////////////////////////////////////////////////////////  ScheduleAlgo-Simulation Part  ///////////////////////////////////////////////////////////////////////////////
		scheduleAlgo();
		///////////////////////////////////////////////////////////// Move From NEW to RDY Part ///////////////////////////////////////////////////////////////////////////////
 		FromRDYtoRUNSim();
		/////////////////////////////////////////////////////////// Update Interface Part //////////////////////////////////////////////////////////////////////////////////
		UIptr->updateInterface(TimeStep, ProcessorNum, ProcessorList, BLKList, BlockCount, termList, TermCount, RUNCount, numOfRDYinAUX, Aux, numOfOverheatedProcessors);
		/////////////////////////////////////////////////////////// TO THE NEXT TIME STEP //////////////////////////////////////////////////////////////////////////////////
		TimeStep++;
	}   /////////////////////////////////////////////////////////// Generating Output File Part ////////////////////////////////////////////////////////////////////////////
	generateOutputFile();

	return;
}
void Scheduler::Migrate()
{
	for (int i = 0; i < ProcessorNum; i++)
	{
		if (ProcessorList[i]->GetType() == FCFS)
		{
			bool canMigrate = true;
			int ind = 0;

			while (canMigrate)
			{
				while (ProcessorList[i]->getRunningProcess() && ProcessorList[i]->getRunningProcess()->isChild())
				{
					MoveToTerm(ProcessorList[i]->getRunningProcess());
					ProcessorList[i]->setRunningProcess(nullptr);
					ProcessorList[i]->setState(IDLE);
					ProcessorList[i]->FromRDYToRun(TimeStep);
				}

				ind = getShortestQueueIND(RR);
				if (ind >= 0)
				{
					canMigrate = ProcessorList[i]->Migrate(ProcessorList[ind], MaxW, TimeStep);
				}
				else
					canMigrate = false;
				if (canMigrate)
					N_Max++;
			}
		}
		else if (ProcessorList[i]->GetType() == RR)
		{
			bool canMigrateRR = true;
			int indSJF;
			while (canMigrateRR)
			{
				indSJF = getShortestQueueIND(SJF);
				if (indSJF >= 0)
					canMigrateRR = ProcessorList[i]->Migrate(ProcessorList[indSJF], RTF, TimeStep);
				else
					canMigrateRR = false;
				if (canMigrateRR)
					N_RTF++;
			}
		}
	}

}
void Scheduler::ForkingSim()
{
	for (int i = 0; i < Nf + Nr + Ns; i++)
	{
		int At = TimeStep;
		int Ct = 0;
		Process* Child = nullptr;
		if (ProcessorList[i]->Fork(Child, Ct, ForkProbablity))
		{
			Child = new Process(++LastID, Ct, At, true);
			if (ProcessorList[i]->getRunningProcess()->SetChild(Child))
			{
				int ind = getShortestQueueIND(FCFS); //moving to the shortest FCFS queue 
				if (ind >= 0)
				{
					ProcessorList[ind]->moveToReady(Child);
					ProcessNum++;
					numOfForked++;
				}
				else
				{
					--LastID;
					delete Child;
					Child = nullptr;
				}
			}
			else
			{
				--LastID;
				delete Child;
				Child = nullptr;
			}

		}
	}
}
void Scheduler::killOrphSim(Process* process)
{
	Process* Lchild = nullptr;
	Process* Rchild = nullptr;
	Process* Lkill = nullptr;
	Process* Rkill = nullptr;
	if (process)
	{
		Lchild = process->getXChild('L');
		Rchild = process->getXChild('R');
	}
	else return;

	for (int i = Ns; i < Ns + Nf; i++)
	{
		bool LkilledInRun = false;
		bool LkilledInReady = false;
		bool RkilledInRun = false;
		bool RkilledInReady = false;

		if (Lchild)
		{
			LkilledInReady = ProcessorList[i]->killProcess(Lkill, Lchild->GetPID(), LkilledInRun);
		}

		if (Rchild)
		{	
			RkilledInReady = ProcessorList[i]->killProcess(Rkill, Rchild->GetPID(), RkilledInRun);
		}

		if (LkilledInReady)
		{
			numOfKilledProcess++;
			if (Lkill)
			{
				Lkill->editState(KILLED, TimeStep);
				MoveToTerm(Lkill);
			}
		}
		if (LkilledInRun)
		{
			numOfKilledProcess++;
			if (Lkill)
			{
				Lkill->editState(KILLED, TimeStep);
				ProcessorList[i]->FromRDYToRun(TimeStep);
				MoveToTerm(Lkill);
			}
		}
		if (RkilledInReady)
		{
			numOfKilledProcess++;
			if (Rkill)
			{
				Rkill->editState(KILLED, TimeStep);
				MoveToTerm(Rkill);
			}

		}
		if (RkilledInRun)
		{
			numOfKilledProcess++;
			if (Rkill)
			{
				Rkill->editState(KILLED, TimeStep);
				ProcessorList[i]->FromRDYToRun(TimeStep);
				MoveToTerm(Rkill);
			}
		}
	}
}
void Scheduler::ProcessorOverheatSimulation()
{
	if (numOfOverheatedProcessors != ProcessorNum)
	{
		for (int i = 0; i < ProcessorNum; i++)
		{
			int r = rand() % 100 + 1;
			if (r < overheatProbability) //just for testing  the true statement->>> if(r<10)
			{
				bool done = ProcessorList[i]->OverheatSignal(overheatConst);
				if (done) //if not overheated
				{
					numOfOverheatedProcessors++;
					if (ProcessorNum == numOfOverheatedProcessors)
					{
						Process* ProcessToBeMoved = ProcessorList[i]->getRunningProcess();
						ProcessorList[i]->setRunningProcess(nullptr);
						ProcessorList[i]->setState(STOP);
						do
						{
							if (Aux.enqueue(ProcessToBeMoved))
								numOfRDYinAUX++;
							ProcessToBeMoved = ProcessorList[i]->GetFromRDY();
							if (ProcessToBeMoved)
								ProcessorList[i]->updateProcessInRDY();

						} while (ProcessToBeMoved);
					}

					else
					{
						Process* ProcessToBeMoved = ProcessorList[i]->getRunningProcess();
						ProcessorList[i]->setRunningProcess(nullptr);
						ProcessorList[i]->setState(STOP);						
						do
						{
							ProcessorType t = NONE;
							if (ProcessToBeMoved &&ProcessToBeMoved->isChild())
								t = FCFS;
							sendToShortestQueue(ProcessToBeMoved, i, t);
							ProcessToBeMoved = ProcessorList[i]->GetFromRDY();

							if (ProcessToBeMoved)
								ProcessorList[i]->updateProcessInRDY();

						} while (ProcessToBeMoved);
					}
					ProcessorList[i]->SetExpectedFinish(0);
				}
			}
		}
	}

	for (int j = 0; j < ProcessorNum; j++)
		if (ProcessorList[j]->overheatRespond())
			numOfOverheatedProcessors--;
	
	FromAuxToRDY();
}
void Scheduler::stealProcessSim()
{
	if (STL != 0 && TimeStep % STL == 0)
	{
		int longestIND = getLongestQueueIND();
		int shortestIND = getShortestQueueIND(NONE);
		bool stolen = true;
		Process* invalidSteal = nullptr;
		if (longestIND == -1 || shortestIND == -1)
			return;
		if (longestIND >= 0 && shortestIND >= 0 && longestIND != shortestIND)
			while (stolen)
			{
				stolen = ProcessorList[shortestIND]->steal(ProcessorList[longestIND], numOfProcessStolen, invalidSteal, numOfKilledProcess);
				if (invalidSteal)
					MoveToTerm(invalidSteal);
				invalidSteal = nullptr;
			}
	}
}
void Scheduler::killProcessSim()
{
	int Time = 0;
	KillPair* p = nullptr;
	KillPairQueue.peek(p);
	Process* ptr = nullptr;

	if (p)
	{
		Time = p->GetKillTime();
	}

	while (p && Time == TimeStep)
	{

		if (!KillPairQueue.dequeue(p))
		{
			p = nullptr;
			break;
		}

		for (int i = 0; i < ProcessorNum; i++)
		{
			bool killedInRun = false;
			bool killedInReady = false;

			if (p)
			{
				killedInReady = ProcessorList[i]->killProcess(ptr, p->GetKillID(), killedInRun);
			}

			if (killedInReady)
			{
				numOfKilledProcess++;
				if (ptr)
				{
					ptr->editState(KILLED, TimeStep);
					MoveToTerm(ptr);
				}
				break;
			}

			else if (killedInRun)
			{
				numOfKilledProcess++;
				if (ptr)
				{
					ptr->editState(KILLED, TimeStep);
					ProcessorList[i]->FromRDYToRun(TimeStep);
					MoveToTerm(ptr);
				}
				break;
			}
		}

		if (KillPairQueue.peek(p))
			Time = p->GetKillTime();
		else Time = -1;

		if (Time != TimeStep)
			break;
	}

}

//UTILIZATION//
int Scheduler::getShortestQueueIND(ProcessorType type, int except)
{
	int shortest = INT_MAX;
	int shortestInd = -1;
	int endLoop = 0;
	int startInd = -1;

	if (type == NONE)
	{
		startInd = 0;
		endLoop = ProcessorNum;
	}
	else if (type == FCFS)
	{
		startInd = Ns;
		endLoop = Ns + Nf;
	}
	else if (type == RR)
	{
		startInd = Nf + Ns;
		endLoop = startInd + Nr;
	}
	else if (type == SJF)
	{
		startInd = Nf;
		endLoop = startInd + Ns;
	}

	if (startInd >= 0)
		for (int i = startInd; i < endLoop; i++)
		{
			if (except == i)
				continue;
			if (ProcessorList[i] && ProcessorList[i]->getState() == STOP)
				continue;

			if (i >= 0 && ProcessorList[i] && ProcessorList[i]->GetExpectedFinish() == 0)
			{
				shortestInd = i;
				break;
			}
			if (i >= 0 && ProcessorList[i] && ProcessorList[i]->GetExpectedFinish() < shortest)
			{
				shortest = ProcessorList[i]->GetExpectedFinish();
				shortestInd = i;
			}
		}
	return shortestInd;
}
int Scheduler::getLongestQueueIND()
{
	int longest = -1;
	int longestInd = -1;
	for (int i = 0; i < ProcessorNum; i++)
	{
		if (ProcessorList[i] && ProcessorList[i]->getState() == STOP)
			continue;

		if (ProcessorList[i] && ProcessorList[i]->GetExpectedFinish() > longest)
		{
			longest = ProcessorList[i]->GetExpectedFinish();
			longestInd = i;
		}
	}
	return longestInd;

}
void Scheduler::sendToShortestQueue(Process* process, int except, ProcessorType type)
{
	int shortestInd = getShortestQueueIND(type, except);

	if (shortestInd >= 0 && shortestInd < ProcessorNum)
		ProcessorList[shortestInd]->moveToReady(process);
	else
	{
		Aux.enqueue(process);
		numOfRDYinAUX++;
	}
}

//SETTERS & GETTERS//
void Scheduler::SetProcessor(Processor* P)
{
	ProcessorList[ProcessorNum] = P;
	ProcessorNum++;
}
int Scheduler::GetProcessorNum() const
{
	return ProcessorNum;
}
void Scheduler::SetProcessNum(int Num)
{
	ProcessNum = Num;
}

//INPUT AND OUTPUT FILES//
void Scheduler::Load()
{
	int NF, NS, NR, TS;
	int ProcessNum, AT, PID, CT, N;
	ifstream Infile("Input File 6.txt");
	Infile >> NS >> NF >> NR >> TS >> RTF >> MaxW >> STL >> ForkProbablity;
	Infile >> overheatProbability >> overheatConst;
	Nf = NF;
	Ns = NS;
	Nr = NR;
	Infile >> ProcessNum;
	LastID = ProcessNum;
	for (int i = 0; i < NS; i++)  //Allocating the SJF processors
	{
		Processor* P = new SJFProcessor;
		SetProcessor(P);
	}
	for (int i = 0; i < NF; i++) //Allocating the FCFS processors
	{
		Processor* P = new FCFSProcessor;
		SetProcessor(P);
	}
	for (int i = 0; i < NR; i++)  //Allocating the RR processors with timeslice
	{
		Processor* P = new RRProcessor(TS);
		SetProcessor(P);
	}
	SetProcessNum(ProcessNum);
	for (int i = 0; i < ProcessNum; i++) //Ading processes to new list
	{
		Infile >> AT >> PID >> CT >> N;
		Process* process = new Process(PID,CT,AT,false);
		int R, D;
		char c;
		for (int j = 0; j < N; j++)
		{
			Infile >> c >> R >> c >> D >> c;
			if (j < N - 1)
				Infile >> c;
			process->SetPairIO(R, D);
		}
		MoveToNew(process);
	}

	int killTime, killID;
	while (Infile >> killTime >> killID)
	{
		KillPair* setPair = new KillPair;
		setPair->SetKillID(killID);
		setPair->SetKillTime(killTime);
		KillPairQueue.enqueue(setPair);
	}
	Infile.close();
}
void Scheduler::generateOutputFile()
{
	ofstream outFile{ "OutputFile 6.txt",ios::out | ios::binary };

	if (outFile.is_open() && ProcessNum)
	{

		outFile << "TT      PID      AT      CT      IO_D      WT      RT      TRT" << endl;

		for (int i = 0; i < ProcessNum; i++)
		{
			int totD = 0;
			Process* ptr = nullptr;
			termList.dequeue(ptr);
			if (ptr)
			{
				outFile << ptr->GetTT() << "      " << ptr->GetPID() << "      " << ptr->GetAT() << "      " << ptr->GetCT() << "      " << ptr->getTotalIO_D() << "      " << ptr->GetWT() << "      " << ptr->GetRT() << "      " << ptr->GetTRT() << endl;
				WT += ptr->GetWT();
				RT += ptr->GetRT();
				TRT += ptr->GetTRT();
				delete ptr;
				ptr = nullptr;
			}
		}

		outFile << endl;
		outFile << "Processes: " << ProcessNum << endl;
		outFile << "Avg WT= " <<  float(WT) / ProcessNum << "   Avg RT= " <<  float(RT) / ProcessNum << "   Avg TRT=" <<  float(TRT) / ProcessNum << endl;
		outFile << "Migration%:    RTF=" << ( float(N_RTF) / ProcessNum) * 100 << "%    MaxW=" << (float(N_Max) / ProcessNum) * 100 << "%" << endl;
		outFile << "Work Steal%: " << ( float(numOfProcessStolen) / ProcessNum) * 100 << "%" << endl;
		outFile << "Forked Process: " << ( float(numOfForked) / ProcessNum) * 100 << "%" << endl;
		outFile << "Killed Process: " << ( float(numOfKilledProcess) / ProcessNum) * 100 << "%" << endl;
		outFile << endl;
		outFile << endl;
		outFile << "Processors: " << ProcessorNum << " [ " << Nf << " FCFS, " << Ns << " SJF, " << Nr << " RR ]" << endl;
		outFile << "Processors Load: " << endl;

		for (int i = 0; i < ProcessorNum; i++)
		{
			outFile << "P" << i + 1 << "=" <<  (long(ProcessorList[i]->getTotalBusy()) / TRT) * 100 <<"%";
			if (i != ProcessorNum - 1)
				outFile << ", ";
		}

		outFile << endl;
		outFile << "Processor Utilization: " << endl;
		float totUtil = 0;
		for (int i = 0; i < ProcessorNum; i++)
		{
			 float thisUtil = (float(ProcessorList[i]->getTotalBusy()) / (ProcessorList[i]->getTotalBusy() + ProcessorList[i]->getTotalIdle()));
			totUtil += thisUtil;
			outFile << "P" << i + 1 << "=" << thisUtil*100<<"%";
			if (i != ProcessorNum - 1)
				outFile << ", ";
		}
		outFile << endl;
		outFile << "Avg Utilization = " << (totUtil / ProcessorNum) * 100 << "%" << endl;

	}
	else
		outFile << endl << "THERE ARE NO PROCESSES" << endl;

	outFile.close();
}