#include "cpu-scheduling.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <queue>
#include <math.h>

using namespace std;

void Scheduler::execute()
{
    for (int i = 0; i < schedulongPoliceis.size(); i++)
    {
        clearTables();
        if (schedulongPoliceis[i].first == 1)
        {
            FCFS();
            if (type == "trace")
                trace(1, -1);
            else
                stats(1, -1);
        }
        else if (schedulongPoliceis[i].first == 2)
        {
            RR(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(2, schedulongPoliceis[i].second);
            else
                stats(2, schedulongPoliceis[i].second);
        }
        else if (schedulongPoliceis[i].first == 3)
        {
            SPN();
            if (type == "trace")
                trace(3, -1);
            else
                stats(3, -1);
        }
        else if (schedulongPoliceis[i].first == 4)
        {
            SRT();
            if (type == "trace")
                trace(4, -1);
            else
                stats(4, -1);
        }
        else if (schedulongPoliceis[i].first == 5)
        {
            HRRN();
            if (type == "trace")
                trace(5, -1);
            else
                stats(5, -1);
        }
        else if (schedulongPoliceis[i].first == 6)
        {
            FB1();
            if (type == "trace")
                trace(6, -1);
            else
                stats(6, -1);
        }
        else if (schedulongPoliceis[i].first == 7)
        {
            FB2i();
            if (type == "trace")
                trace(7, -1);
            else
                stats(7, -1);
        }
        else if (schedulongPoliceis[i].first == 8)
        {
            AGE(schedulongPoliceis[i].second);
            if (type == "trace")
                trace(8, -1);
        }
    }
}
void Scheduler::trace(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 2)
    {
        if (argument > 10)
            cout << "RR-" << argument << " ";
        else
            cout << "RR-" << argument << "  ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 3)
    {
        cout << "SPN   ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 4)
    {
        cout << "SRT   ";
        printHeader();
        cout << '\n';
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 5)
    {
        cout << "HRRN  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 6)
    {
        cout << "FB-1  ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 7)
    {
        cout << "FB-2i ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
    else if (policy == 8)
    {
        cout << "Aging ";
        printHeader();
        cout << "\n";
        printTracing();
        printDashes();
        cout << "\n\n";
    }
}
void Scheduler::stats(int policy, int argument)
{
    if (policy == 1)
    {
        cout << "FCFS" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 2)
    {
        cout << "RR-" << argument << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 3)
    {
        cout << "SPN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 4)
    {
        cout << "SRT" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 5)
    {
        cout << "HRRN" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 6)
    {
        cout << "FB-1" << endl;
        printStats();
        cout << '\n';
    }
    else if (policy == 7)
    {
        cout << "FB-2i" << endl;
        printStats();
        cout << '\n';
    }
}
void Scheduler::FCFS()
{
    queue<int> ready;
    int currentProc = -1;
    
    for (int t = 0; t < maxSeconds; t++)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == t)
            {
                ready.push(i);
            }
        }
        
        if (currentProc == -1 && !ready.empty())
        {
            currentProc = ready.front();
            ready.pop();
        }
        
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i == currentProc)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '*';
            }
            else if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '.';
            }
            else
            {
                *(processesPrintingArray + i * maxSeconds + t) = ' ';
            }
        }
        
        if (currentProc != -1)
        {
            processes[currentProc].remainingTime--;
            
            if (processes[currentProc].remainingTime == 0)
            {
                processes[currentProc].finishTime = t + 1;
                processes[currentProc].turnAroundTime = processes[currentProc].finishTime - processes[currentProc].arrivalTime;
                processes[currentProc].NormTurnTime = (float)processes[currentProc].turnAroundTime / processes[currentProc].serveTime;
                currentProc = -1;
            }
        }
    }
}

void Scheduler::RR(int quantum)
{
    queue<int> ready;
    int currentProc = -1;
    int timeSlice = 0;

    for (int t = 0; t < maxSeconds; t++)
    {
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime == t)
                ready.push(i);
        }

        if (currentProc != -1 && timeSlice >= quantum && processes[currentProc].remainingTime > 0)
        {
            ready.push(currentProc);
            currentProc = -1;
            timeSlice = 0;
        }

        if (currentProc == -1 && !ready.empty())
        {
            currentProc = ready.front();
            ready.pop();
            timeSlice = 0;
        }

        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i == currentProc)
                processesPrintingArray[i * maxSeconds + t] = '*';
            else if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
                processesPrintingArray[i * maxSeconds + t] = '.';
            else
                processesPrintingArray[i * maxSeconds + t] = ' ';
        }

        if (currentProc != -1)
        {
            processes[currentProc].remainingTime--;
            timeSlice++;

            if (processes[currentProc].remainingTime == 0)
            {
                processes[currentProc].finishTime = t + 1;
                processes[currentProc].turnAroundTime =
                    processes[currentProc]. finishTime - processes[currentProc].arrivalTime;
                processes[currentProc].NormTurnTime =
                    (float)processes[currentProc]. turnAroundTime /
                    processes[currentProc].serveTime;

                currentProc = -1;
                timeSlice = 0;
            }
        }
    }
}

void Scheduler::SPN()
{
    int currentProc = -1;
    
    for (int t = 0; t < maxSeconds; t++)
    {
        if (currentProc == -1)
        {
            int shortest = -1;
            int shortestTime = 999999;
            
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
                {
                    if (processes[i].serveTime < shortestTime || 
                       (processes[i].serveTime == shortestTime && (shortest == -1 || processes[i].arrivalTime < processes[shortest].arrivalTime)))
                    {
                        shortestTime = processes[i].serveTime;
                        shortest = i;
                    }
                }
            }
            currentProc = shortest;
        }
        
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i == currentProc)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '*';
            }
            else if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '.';
            }
            else
            {
                *(processesPrintingArray + i * maxSeconds + t) = ' ';
            }
        }
        
        if (currentProc != -1)
        {
            processes[currentProc].remainingTime--;
            
            if (processes[currentProc].remainingTime == 0)
            {
                processes[currentProc].finishTime = t + 1;
                processes[currentProc].turnAroundTime = processes[currentProc].finishTime - processes[currentProc].arrivalTime;
                processes[currentProc].NormTurnTime = (float)processes[currentProc].turnAroundTime / processes[currentProc].serveTime;
                currentProc = -1;
            }
        }
    }
}

void Scheduler::SRT()
{
    for (int t = 0; t < maxSeconds; t++)
    {
        int shortest = -1;
        int shortestTime = 999999;
        
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
            {
                if (processes[i].remainingTime < shortestTime || 
                   (processes[i].remainingTime == shortestTime && (shortest == -1 || processes[i].arrivalTime < processes[shortest].arrivalTime)))
                {
                    shortestTime = processes[i].remainingTime;
                    shortest = i;
                }
            }
        }
        
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i == shortest)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '*';
            }
            else if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '.';
            }
            else
            {
                *(processesPrintingArray + i * maxSeconds + t) = ' ';
            }
        }
        
        if (shortest != -1)
        {
            processes[shortest].remainingTime--;
            
            if (processes[shortest].remainingTime == 0)
            {
                processes[shortest].finishTime = t + 1;
                processes[shortest].turnAroundTime = processes[shortest].finishTime - processes[shortest].arrivalTime;
                processes[shortest].NormTurnTime = (float)processes[shortest].turnAroundTime / processes[shortest].serveTime;
            }
        }
    }
}

void Scheduler::HRRN()
{
    int currentProc = -1;
    
    for (int t = 0; t < maxSeconds; t++)
    {
        if (currentProc == -1)
        {
            int best = -1;
            float bestRatio = -1.0;
            
            for (int i = 0; i < numberOfProcesses; i++)
            {
                if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
                {
                    int waitTime = t - processes[i].arrivalTime;
                    float ratio = (waitTime + processes[i].serveTime) / (float)processes[i].serveTime;
                    
                    if (ratio > bestRatio || 
                       (ratio == bestRatio && (best == -1 || processes[i].arrivalTime < processes[best].arrivalTime)))
                    {
                        bestRatio = ratio;
                        best = i;
                    }
                }
            }
            currentProc = best;
        }
        
        for (int i = 0; i < numberOfProcesses; i++)
        {
            if (i == currentProc)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '*';
            }
            else if (processes[i].arrivalTime <= t && processes[i].remainingTime > 0)
            {
                *(processesPrintingArray + i * maxSeconds + t) = '.';
            }
            else
            {
                *(processesPrintingArray + i * maxSeconds + t) = ' ';
            }
        }
        
        if (currentProc != -1)
        {
            processes[currentProc].remainingTime--;
            
            if (processes[currentProc].remainingTime == 0)
            {
                processes[currentProc].finishTime = t + 1;
                processes[currentProc].turnAroundTime = processes[currentProc].finishTime - processes[currentProc].arrivalTime;
                processes[currentProc].NormTurnTime = (float)processes[currentProc].turnAroundTime / processes[currentProc].serveTime;
                currentProc = -1;
            }
        }
    }
}

void Scheduler::FB1()
{
    vector<queue<int>> FBQueues(100);
    int currentProcIdx = -1;
    processorBusy = false;

    for (int i = 0; i < numberOfProcesses; i++) {
        processes[i].FBLevel = 0;
        processes[i].remainingTime = processes[i].serveTime;
    }

    for (int currentTime = 0; currentTime < maxSeconds; currentTime++) {
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime) {
                processes[i].FBLevel = 0;
                FBQueues[0].push(i);
            }
        }

        if (processorBusy) {
            bool anyoneWaiting = false;
            for (int q = 0; q < (int)FBQueues.size(); q++) {
                if (!FBQueues[q].empty()) {
                    anyoneWaiting = true;
                    break;
                }
            }

            if (anyoneWaiting) {
                int currentLevel = processes[currentProcIdx].FBLevel;
                int nextLevel = (currentLevel + 1 < 100) ? currentLevel + 1 : currentLevel;
                processes[currentProcIdx].FBLevel = nextLevel;
                FBQueues[nextLevel].push(currentProcIdx);
                processorBusy = false;
            } else {
                FBQueues[processes[currentProcIdx].FBLevel].push(currentProcIdx);
                processorBusy = false;
            }
        }

        if (!processorBusy) {
            for (int q = 0; q < (int)FBQueues.size(); q++) {
                if (!FBQueues[q].empty()) {
                    currentProcIdx = FBQueues[q].front();
                    FBQueues[q].pop();
                    processes[currentProcIdx].FBLevel = q;
                    processorBusy = true;
                    break;
                }
            }
        }

        for (int q = 0; q < (int)FBQueues.size(); q++) {
            int qSize = FBQueues[q].size();
            for (int k = 0; k < qSize; k++) {
                int id = FBQueues[q].front();
                FBQueues[q].pop();
                processesPrintingArray[id * maxSeconds + currentTime] = '.';
                FBQueues[q].push(id);
            }
        }

        if (processorBusy) {
            processesPrintingArray[currentProcIdx * maxSeconds + currentTime] = '*';
            processes[currentProcIdx].remainingTime--;

            if (processes[currentProcIdx].remainingTime == 0) {
                processes[currentProcIdx].finishTime = currentTime + 1;
                processes[currentProcIdx].turnAroundTime = processes[currentProcIdx].finishTime - processes[currentProcIdx].arrivalTime;
                processes[currentProcIdx].NormTurnTime = (float)processes[currentProcIdx].turnAroundTime / (float)processes[currentProcIdx].serveTime;
                processorBusy = false;
                currentProcIdx = -1;
            }
        }
    }
}

void Scheduler::FB2i()
{
    vector<queue<int>> FBQueues(100);
    int currentProcIdx = -1;
    int timeSlice = 0;
    processorBusy = false;

    for (int i = 0; i < numberOfProcesses; i++) {
        processes[i].FBLevel = 0;
        processes[i].remainingTime = processes[i].serveTime;
    }

    for (int currentTime = 0; currentTime < maxSeconds; currentTime++) {
        
        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime) {
                processes[i].FBLevel = 0;
                FBQueues[0].push(i);
            }
        }

        if (processorBusy) {
            int currentLevel = processes[currentProcIdx].FBLevel;
            int currentQuantum = pow(2, currentLevel);

            if (timeSlice >= currentQuantum) {
                bool anyoneWaiting = false;
                for (int q = 0; q < (int)FBQueues.size(); q++) {
                    if (!FBQueues[q].empty()) {
                        anyoneWaiting = true;
                        break;
                    }
                }

                if (anyoneWaiting) {
                    int nextLevel = (currentLevel + 1 < 100) ? currentLevel + 1 : currentLevel;
                    processes[currentProcIdx].FBLevel = nextLevel;
                    FBQueues[nextLevel].push(currentProcIdx);
                    processorBusy = false;
                } else {
                    FBQueues[currentLevel].push(currentProcIdx);
                    processorBusy = false;
                }
            }
        }

        if (!processorBusy) {
            for (int q = 0; q < (int)FBQueues.size(); q++) {
                if (!FBQueues[q].empty()) {
                    currentProcIdx = FBQueues[q].front();
                    FBQueues[q].pop();
                    processes[currentProcIdx].FBLevel = q;
                    processorBusy = true;
                    timeSlice = 0;
                    break;
                }
            }
        }

        for (int q = 0; q < (int)FBQueues.size(); q++) {
            int qSize = FBQueues[q].size();
            for (int k = 0; k < qSize; k++) {
                int id = FBQueues[q].front();
                FBQueues[q].pop();
                processesPrintingArray[id * maxSeconds + currentTime] = '.';
                FBQueues[q].push(id);
            }
        }

        if (processorBusy) {
            processesPrintingArray[currentProcIdx * maxSeconds + currentTime] = '*';
            processes[currentProcIdx].remainingTime--;
            timeSlice++;

            if (processes[currentProcIdx].remainingTime == 0) {
                processes[currentProcIdx].finishTime = currentTime + 1;
                processes[currentProcIdx].turnAroundTime = processes[currentProcIdx].finishTime - processes[currentProcIdx].arrivalTime;
                processes[currentProcIdx].NormTurnTime = (float)processes[currentProcIdx].turnAroundTime / (float)processes[currentProcIdx].serveTime;
                processorBusy = false;
                currentProcIdx = -1;
            }
        }
    }
}

void Scheduler::AGE(int quantum)
{
    processorBusy = false;
    currentProcess.id = -1; 
    int currentTime = 0;
    int timeSlice = 0;
    vector<int> readyList;

    for (int i = 0; i < numberOfProcesses; i++) {
        processes[i].currentPriority = processes[i].priority;
        processes[i].processState = ' ';
    }

    while (currentTime < maxSeconds)
    {

        for (int i = 0; i < numberOfProcesses; i++) {
            if (processes[i].arrivalTime == currentTime && processes[i].processState == ' ') {
                processes[i].processState = 'R';
                readyList.push_back(i);
            }
        }

        if (!processorBusy || timeSlice >= quantum)
        {
            for (int i = 0; i < (int)readyList.size(); i++) {
                processes[readyList[i]].currentPriority++;
            }

            if (processorBusy) {
                processes[currentProcess.id].currentPriority = processes[currentProcess.id].priority;
                readyList.push_back(currentProcess.id);
                processorBusy = false;
            }
            
            if (!readyList.empty()) {
                int bestIdxInReadyList = 0;
                for (int i = 1; i < (int)readyList.size(); i++) {
                    if (processes[readyList[i]].currentPriority > processes[readyList[bestIdxInReadyList]].currentPriority) {
                        bestIdxInReadyList = i;
                    }
                }
                
                int selectedID = readyList[bestIdxInReadyList];
                currentProcess.id = selectedID;
                readyList.erase(readyList.begin() + bestIdxInReadyList);
                
                processorBusy = true;
                timeSlice = 0;
            }
        }

        if (processorBusy) {
            processesPrintingArray[currentProcess.id * maxSeconds + currentTime] = '*';
            timeSlice++;
            
            for (int id : readyList) {
                processesPrintingArray[id * maxSeconds + currentTime] = '.';
            }
        }
        currentTime++;
    }
}

void Scheduler::printTracing()
{
    for (int process = 0; process < numberOfProcesses; process++)
    {
        cout << processes[process].processName << "     |";
        for (int time = 0; time < maxSeconds; time++)
            cout << *(processesPrintingArray + process * maxSeconds + time) << '|';
        cout << " \n";
    }
}
void Scheduler::printStats()
{
    float sum, mean, sum2;
    cout << "Process    |";
    for (int i = 0; i < numberOfProcesses; i++)
        cout << "  " << processes[i].processName << "  |";
    cout << endl;
    cout << "Arrival    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].arrivalTime << "  |";
        else
            cout << " " << processes[i].arrivalTime << "  |";
    }
    cout << endl;
    cout << "Service    |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].arrivalTime < 10)
            cout << "  " << processes[i].serveTime << "  |";
        else
            cout << " " << processes[i].serveTime << "  |";
    }
    cout << " Mean|" << endl;
    cout << "Finish     |";
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].finishTime >= 10)
            cout << " " << processes[i].finishTime << "  |";
        else
            cout << "  " << processes[i].finishTime << "  |";
    }
    cout << "-----|" << endl;
    cout << "Turnaround |";
    for (int i = 0; i < numberOfProcesses; i++)
    {

        if (processes[i].turnAroundTime >= 10)
            cout << " " << (int)processes[i].turnAroundTime << "  |";
        else
            cout << "  " << (int)processes[i].turnAroundTime << "  |";
        sum += processes[i].turnAroundTime;
    }
    cout << fixed;
    cout << setprecision(2);
    mean = (sum * 1.0) / numberOfProcesses;
    if (mean >= 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
    cout << "NormTurn   |";
    sum2 = 0;
    for (int i = 0; i < numberOfProcesses; i++)
    {
        if (processes[i].NormTurnTime > 10)
            cout << processes[i].NormTurnTime << "|";
        else
            cout << " " << processes[i].NormTurnTime << "|";
        sum2 += (processes[i].NormTurnTime * 1.0);
    }

    mean = (sum2 * 1.0) / numberOfProcesses;
    if (mean > 10)
        cout << mean << "|";
    else
        cout << " " << mean << "|";
    cout << endl;
}
void Scheduler::clearTables()
{

    for (int i = 0; i < numberOfProcesses; i++)
    {
        for (int j = 0; j < maxSeconds; j++)
            *(processesPrintingArray + i * maxSeconds + j) = ' ';
    }
    for (int i = 0; i < numberOfProcesses; i++)
    {
        processes[i].finishTime = 0;
        processes[i].turnAroundTime = 0;
        processes[i].NormTurnTime = 0;
        processes[i].processState = ' ';
        processes[i].remainingTime = processes[i].serveTime;
    }
    processorBusy = false;
    while (!readyQueue.empty())
        readyQueue.pop();
    while (!readyPriorityQueue.empty())
        readyPriorityQueue.pop();
}
void Scheduler::splitPolicyAndParameter(string str)
{
    string w = "";
    pair<int, int> policies;
    bool parameterExists = false;
    policies.second = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '-')
        {
            parameterExists = true;
            policies.first = stoi(w);
            w = "";
        }
        else if (str[i] == ',')
        {
            if (parameterExists)
                policies.second = stoi(w);
            else
            {
                policies.first = stoi(w);
                policies.second = -1;
            }
            w = "";
            schedulongPoliceis.push_back(policies);
            parameterExists = false;
        }
        else
            w = w + str[i];
    }
    if (parameterExists)
        policies.second = stoi(w);
    else
        policies.first = stoi(w);
    schedulongPoliceis.push_back(policies);
}
void Scheduler::splitPrcoessAndTimes(string str, int id)
{
    Process process;
    string w = "";
    process.processName = str[0];
    for (int i = 2; i < str.length(); i++)
    {
        if (str[i] == ',')
        {
            process.arrivalTime = stoi(w);
            w = "";
        }
        else
            w = w + str[i];
    }
    processorBusy = false;
    process.processState = ' ';
    if (schedulongPoliceis[0].first == 8)
    {
        process.priority = stoi(w);
        process.currentPriority = stoi(w);
    }
    else
        process.serveTime = stoi(w);
    process.remainingTime = process.serveTime;
    process.waitingTime = 0;
    process.id = id;
    processes.push_back(process);
}
void Scheduler::readFile()
{
    processorBusy = false;
    string temp1, temp2;
    cin >> type;
    cin >> temp1;
    splitPolicyAndParameter(temp1);
    cin >> maxSeconds;
    cin >> numberOfProcesses;

    for (int i = 0; i < numberOfProcesses; i++)
    {
        cin >> temp2;
        splitPrcoessAndTimes(temp2, i);
    }
    processesPrintingArray = new char[numberOfProcesses * maxSeconds];
    currentProcess.processName = 0;
    currentProcess.q = 0;
    clearTables();
}
void Scheduler::printHeader()
{
    for (int i = 0; i < maxSeconds + 1; i++)
        cout << i % 10 << ' ';
    cout << "\n";
    printDashes();
}
void Scheduler::printDashes()
{
    cout << "------------------------------------------------";
}
int main(void)
{
    Scheduler scheduler;
    scheduler.readFile();
    scheduler.execute();
    return 0;
}
