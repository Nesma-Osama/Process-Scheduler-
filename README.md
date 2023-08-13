# Process-Scheduler
## Explanation of the project
### Introduction

Process scheduling is allocating resources to different processes so that they can be completed in an efficient manner. The main objectives of scheduling are to keep the CPU busy as possible and minimize response time and waiting time for processes being scheduled.

### Process States
```
1-NEW : A process that has been created but not arrived yet
2-RDY : A process that has arrived and was moved to the ready queue
3-RUN : A process being served by CPU
4-BLK : A process asked a I/O request 
5-TRM : A process that has finished execution   
```


### The Type of Processors and Its Algorithm

```
* First Come First Serve  (FCFS) : The process that requests CPU first is allocated the CPU first
* Shortest Job First      (SJF)  :  Selects the waiting process with the smallest remaining CPU time to execute next
* Round Rhobin            (RR)   :  The Running process is assigned a fixed  time slice to execute part of the process

```
### The Operations on Process
```
* Process migration : Processes can migrate form one processor to another based on :
                    - RR process should migrate to a SJF queue if the process has a remaining time to finish less than RTF
                    - FCFS process should migrate to a RR queue if the process total waiting time is greater than MaxW

* Work stealing     : The shortest queue steals one or more processes  from the longest queue to help  balance load 
    
* killing a process : (applicable for FCFS only) A process should be killed if it receives a kill signal

* Process forking   : (applicable for FCFS only) A process can fork another process the forking process is the parent and the forked one is the child
                    - The processor asks the scheduler to create a new process .The scheduler should create it and add it to the shortest RDY queue of FCFS processors only
* Orphan process    : If the parent process terminates before its child process the child process and all grandchildren are considered as [ORPHAN] and must be killed immediately and moved to TRM 

* Processor Overhead: A processor may go out of service due to overheading in this state :
                      - Processor should be but in STOP state for n time step (n loaded from the input file)
                      - The process being executed by that processor should be moved to the shortest RDY queue
```

### Statistics
This project simulates the operation of the process scheduler in a multiprocessor system and calculates  some related statistics like:
```
1-Arrival Time        : The time when the process is ready to be scheduled
2-Response Time       : The difference between the arrival  time and the time at which the process gets CPU for the first time  
3-CPU Time            : The total time needed to run the process
4-Termination Time    : Time when the process finishes execution
5-Turnaround duration : Total  time a process spends in the system
6-Waiting Time        : Total time a process spends in the system without being executed by CPU
7-pLoad               : Total processor busy time/total turnaround time for all processes
8-pUtil               : Total processor busy time/(Total processor busy time + Total processor IDLE time)
there are some other statistics be calculated in this project
```
Here are two photos show some calculated statistics
<div>
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/5e2ddd85-7e91-4240-a5bb-2dcde876296f" width=400/ height="300">
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/1330bae1-bdd5-4562-89f0-5cdaf8c8509d"width="400" height="300"/>
</div>
### Program Interface

```
* Interactive Mode  : when the user press the "Enter" key it displays the output of the next time
* Step-By-Step Mode : The program waits for one second then displays the output
* Silent Mode       : The program produces an output file
```
### Input File
```
1- First line contains three integers .Each integer represents the number of processors of each scheduling algorithm :
 - NF :number of FCFS processors
 - NS :number of SJF processors
 - NR :number of RR processors
2- Then a line with one number represents the time slice for RR
3- Then a line for four integers :RTF,MaxW,Stl and the Fork probability
4- overheatProbability and overheatConst for overhead;

5- The a number represents the number of processes
6- The the input file contains M lines (one line for each process) sorted by arrival time in ascending order.each line has the following information:
 - AT :Arrival time
 - PID:Process ID
 - CT :CPU time
 -N: Number of the time process requests I/O the I/O pair (IO_R,IO_D) :
    -  IO_R :For when the process requests the I/O
    -  IO_D :For the time process will be in the BLK queue
 7- The remaining lines for the Kill Signal  :
  - PID :For the process we want to kill
  - T   :The time we whant to kill this process  

```
## How use the program
```
1-Make the input file  
2-Run the program
3-press the number of the Mode you what
4-check the output 
```
### Pictures 
<div>
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/e3ff9189-84f3-4cd1-bea0-d3bde49ce436" width="400" height="300"/>
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/2b6f3782-fa9a-4667-8171-4099e241df0c)" width="400" height="300"/>

</div>
