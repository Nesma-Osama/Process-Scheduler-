#include "UI.h"
#include <iostream>
using namespace std;

void UI::printInteractive(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm, int numofRun, int  NumOfRDYinAUX, LinkedQueue<Process*>aux, int n)
{

	printStepByStep(timeStep, numOfProcessors, ProcessorArr, BLK, numOfBLK, TRM, numOfTrm, numofRun, NumOfRDYinAUX, aux, n);
	cout << endl;
	cout << "Press Any Key To Continue: "<<endl;
	cin.get();
	cout << "---------------------------------------------------------------------" << endl;
}

void UI::printStepByStep(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm, int numofRun, int  NumOfRDYinAUX, LinkedQueue<Process*>aux, int n)
{
	cout << endl << "Current Time Step:" << timeStep << endl;
	cout << "--------------------------  RDY Processes  --------------------------" << endl;

	for (int i = 0; i < numOfProcessors; i++)
	{
		cout << "Processor " << i + 1 << " ";
		if (ProcessorArr[i]->getState() == STOP)
		{
			cout << "IS NOT WORKING RIGHT NOW, NEEDS MORE " << ProcessorArr[i]->getRemainingCooling() << " Timesteps" << endl;
			continue;
		}
		ProcessorArr[i]->Print();
		cout << endl;
	
	}

	cout << "--------------------------  BLK Processes  --------------------------" << endl;

	cout << numOfBLK << " BLK: ";
	BLK.printQueue();
	cout << endl;
	cout << "--------------------------  RUN Processes  --------------------------" << endl;

	cout << numofRun << " RUN: ";
	for (int i = 0; i < numOfProcessors; i++)
	{

		cout << "(P" << i + 1 << ")" << ProcessorArr[i]->getRunningProcessID();
		if (i != numOfProcessors - 1)
			cout << ", ";

	}
	cout << endl;
	cout << "--------------------------  TRM Processes  --------------------------" << endl;
	cout << numOfTrm << " TRM: ";
	TRM.printQueue();
	cout << endl;
	cout << "-------------------------- OVERHEAT --------------------------" << endl;
	cout << "Number Of Overheated Processors: " << n << endl;
	if (NumOfRDYinAUX)
	{
		cout << "----------------  ALL PROCESSORS ARE OVERHEATED, PROCESSES ARE MOVED TO AN AUXILIARY LIST  -----------------" << endl;
		cout << NumOfRDYinAUX << " AUX: " << endl;
		aux.printQueue();
		cout << endl;
	}
	else 
		cout << "--------------------------  AUXILIARY LIST IS EMPTY  --------------------------" << endl;
	cout << "---------------------------------------------------------------------" << endl;

}

void UI::printSilent()
{
	if (SilentFT)
	{
		cout << "Silent Mode....... Simulator Starts..." << endl;
		cout << "Simulation ends, Output File has been created"<<endl;
		cout << "----------------------------------------------------" << endl;
		SilentFT = false;
	}

}

UI::UI() :Mode(SILENT)
{
	SilentFT = true;
	cout << "*Select The Interface Mode."<<endl;
	cout << "----------------------------------------------------"<<endl;
	cout << "*For InterActive Mode: Press 1" << endl;
	cout << "----------------------------------------------------"<<endl;
	cout << "*For Step-By-Step Mode: Press 2" << endl;
	cout << "----------------------------------------------------"<<endl;
	cout << "*For Silent Mode: Press 3" << endl;
	cout << "----------------------------------------------------"<<endl;
	cout << "waiting for your choice... ";
	int mode = 0;

	do {

		cin >> mode;
		cout << endl;
		cout << "----------------------------------------------------" << endl;
		switch (mode)
		{
		case 1: Mode = INTERACTIVE;
			break;
		case 2 : Mode = STEP_BY_STEP;
			break;
		case 3: Mode = SILENT;
		}
		if ((mode != 1) && (mode != 2) && (mode != 3))
		{
			cout << "Error, invalid option"<< endl;
			cout << "----------------------------------------------------" << endl;
			cout << "*Select The Interface Mode." << endl;
			cout << "----------------------------------------------------" << endl;
			cout << "*For InterActive Mode: Press 1" << endl;
			cout << "----------------------------------------------------" << endl;
			cout << "*For Step-By-Step Mode: Press 2" << endl;
			cout << "----------------------------------------------------" << endl;
			cout << "*For Silent Mode: Press 3" << endl;
			cout << "----------------------------------------------------" << endl;
			cout << "waiting for your choice... ";
		}
	} while ((mode != 1) && (mode != 2) && (mode != 3));
}

UI::~UI()
{
}



void UI::updateInterface(int timeStep, int numOfProcessors, Processor* ProcessorArr[], LinkedQueue<Process*>& BLK, int numOfBLK, LinkedQueue<Process*>& TRM, int numOfTrm, int numofRun, int  NumOfRDYinAUX, LinkedQueue<Process*>aux, int n)
{
	switch (Mode)
	{
	case INTERACTIVE: printInteractive(timeStep, numOfProcessors, ProcessorArr, BLK, numOfBLK, TRM, numOfTrm, numofRun, NumOfRDYinAUX,aux, n);
	 break;

	case STEP_BY_STEP: printStepByStep(timeStep, numOfProcessors, ProcessorArr, BLK, numOfBLK, TRM, numOfTrm, numofRun, NumOfRDYinAUX,aux, n);
	 break;

	case SILENT: printSilent();
	break;
	}
}

void UI::ReadInput(int& PID, int& CT)
{

	cin >> PID >> CT;
	return;
}
