#include<iostream>
#include "Scheduler.h"
#include "UI.h"

int main()
{
	interfaceMode mode = INTERACTIVE;
	srand(time(0));
	UI ui;
	Scheduler scheduler(&ui);
	scheduler.Simulator();
	return 0;

}
