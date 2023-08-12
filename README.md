# Process-Scheduler
## This project is a Data Structures project 
### Explanation of the project
Process scheduling is allocating resources to different processes so that they can be completed in an efficient manner. The main objectives of scheduling are to keep the CPU busy as possible and minimize response time and waiting time for processes being scheduled.

### Process states
```
1-NEW : A process that has been created but not arrived yet
2-RDY : A process that has arrived and was moved to ready queue
3-RUN : A process being served by CPU
4-BLK : A process asked a I/O    
5-TRM : A process that has finished execution   
```


### The type of processors  

```
* First Come First Serve  (FCFS) : The process that requests CPU first is allocated the CPU first
* Shortest Job First      (SJF)  :  Selects the waiting process with the smallest remaining CPU time to exceute next
* Round Rhobin            (RR)   :  The Running process is assigned a fixed  time slice to execute part of the process

```
### The operations on process
```
* Process migration : Processes can migrate for one processor to another based on
                    - RR process should migrate to a SJF queue if the process has a remaining time to finish less than RTF
                    - FCFS process should migrate to a RR queue if the process total waiting time is greater than MaxW

* Work stealing     : The shortest queue steals one or more process from the longest queue to help  balance load 
    
* killing a process : (applicable for FCFS only) A process should be killed if it receives a kill signal

* Process forking   : (applicable for FCFS only) A process can fork another process the forking process is the parent and forked one is the child
                    - The processor asks the scheduler to create a new process .The scheduler should create it and add it to the shortest RDY queue of FCFS processors only
* Orphan process    : If the parent process terminates before its child process the child process and all grandchildren are considered as [ORPHAN] must be killed immediately and moved to TRM list
```

### Statistics
This project simulates the operation of the process scheduler in a multiprocessor system and calculates  some related statistics like:
```
1-Arrival Time        : The time when the process is ready to be scheduled
2-Response Time       : The difference between the arival time and the time at which the process gets CPU for the first time  
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
### Program interface

```
* Interactive Mode  : when the user press "Enter" key it displays the output of the next time
* Step-By-Step Mode : The program waits for one second than displays the output
* Silent Mode       : The program produces an output file
```
### Pictures 
<div>
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/e3ff9189-84f3-4cd1-bea0-d3bde49ce436" width="400" height="300"/>
  <img src="https://github.com/Nesma-Osama/Process-Scheduler-/assets/128908402/2b6f3782-fa9a-4667-8171-4099e241df0c)" width="400" height="300"/>

</div>
