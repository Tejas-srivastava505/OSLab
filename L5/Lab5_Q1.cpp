#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;


// Write a menu driven C/C++ program to simulate the following CPU scheduling algorithms.
// Display Gantt chart showing the order of execution of each process. Compute waiting time and
// turnaround time for each process. Hence, compute average waiting time and average turnaround
// time.
// (i) FCFS (ii) SRTF (iii) Round-Robin (quantum = 10 ) iv) non-preemptive priority (higher
// the number higher the priority)
// 2. Write a C program to simulate multi-level feedback queue scheduling algorithm.
// 3. Write a C program to simulate Earliest-Deadline-First scheduling for real time systems.
struct Process {
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;    // For SRTF / RR
    int completion;
    int waiting;
    int turnaround;
};

void printGantt(vector<int> &order, vector<int> &time) {
    cout << "\nGantt Chart:\n";
    for (int pid : order) cout << "| P" << pid << " ";
    cout << "|\n";
    for (int t : time) cout << t << "     ";
    cout << time.back() << "\n";
}

void FCFS(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b){ return a.arrival < b.arrival; });
    int time = 0;
    vector<int> ganttOrder, ganttTime;
    ganttTime.push_back(time);

    for (auto &p : processes) {
        if (time < p.arrival) time = p.arrival;
        ganttOrder.push_back(p.pid);
        time += p.burst;
        p.completion = time;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        ganttTime.push_back(time);
    }

    printGantt(ganttOrder, ganttTime);

    double avgWT = 0, avgTAT = 0;
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" 
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT: " << avgWT / processes.size() << "\n";
    cout << "Average TAT: " << avgTAT / processes.size() << "\n";
}

// SRTF Scheduling
void SRTF(vector<Process> processes) {
    int n = processes.size(), completed = 0, time = 0;
    vector<int> ganttOrder, ganttTime;
    ganttTime.push_back(time);

    for (auto &p : processes) p.remaining = p.burst;

    while (completed < n) {
        int idx = -1, minRem = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0) {
                if (processes[i].remaining < minRem) {
                    minRem = processes[i].remaining;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            time++;
            ganttTime.push_back(time);
        } else {
            ganttOrder.push_back(processes[idx].pid);
            processes[idx].remaining--;
            time++;
            ganttTime.push_back(time);
            if (processes[idx].remaining == 0) {
                processes[idx].completion = time;
                processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
                processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
                completed++;
            }
        }
    }

    printGantt(ganttOrder, ganttTime);

    double avgWT = 0, avgTAT = 0;
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" 
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT: " << avgWT / n << "\n";
    cout << "Average TAT: " << avgTAT / n << "\n";
}

void RR(vector<Process> processes, int quantum = 10) {
    int n = processes.size();
    for (auto &p : processes) p.remaining = p.burst;

    int time = 0, completed = 0;
    vector<int> ganttOrder, ganttTime;
    ganttTime.push_back(time);

    vector<int> queue;

    for (int i = 0; i < n; i++) if (processes[i].arrival <= time) queue.push_back(i);

    while (completed < n) {
        if (queue.empty()) {
            time++;
            for (int i = 0; i < n; i++)
                if (processes[i].arrival <= time && processes[i].remaining > 0 && 
                    find(queue.begin(), queue.end(), i) == queue.end()) queue.push_back(i);
            continue;
        }

        int idx = queue.front(); queue.erase(queue.begin());
        ganttOrder.push_back(processes[idx].pid);
        int exec = min(quantum, processes[idx].remaining);
        time += exec;
        processes[idx].remaining -= exec;
        ganttTime.push_back(time);

   
        for (int i = 0; i < n; i++)
            if (processes[i].arrival <= time && processes[i].remaining > 0 && 
                find(queue.begin(), queue.end(), i) == queue.end() && i != idx) queue.push_back(i);

        if (processes[idx].remaining > 0)
            queue.push_back(idx);
        else {
            processes[idx].completion = time;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            completed++;
        }
    }

    printGantt(ganttOrder, ganttTime);

    double avgWT = 0, avgTAT = 0;
    cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" 
             << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT: " << avgWT / n << "\n";
    cout << "Average TAT: " << avgTAT / n << "\n";
}


void PriorityNP(vector<Process> processes) {
    int n = processes.size();
    int time = 0;
    vector<int> ganttOrder, ganttTime;
    ganttTime.push_back(time);
    int completed = 0;

    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1, maxPrio = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= time) {
                if (processes[i].priority > maxPrio) {
                    maxPrio = processes[i].priority;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        ganttOrder.push_back(processes[idx].pid);
        time += processes[idx].burst;
        ganttTime.push_back(time);
        processes[idx].completion = time;
        processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
        processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
        done[idx] = true;
        completed++;
    }

    printGantt(ganttOrder, ganttTime);

    double avgWT = 0, avgTAT = 0;
    cout << "\nProcess\tAT\tBT\tPrio\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t" << p.priority 
             << "\t" << p.completion << "\t" << p.turnaround << "\t" << p.waiting << "\n";
        avgWT += p.waiting;
        avgTAT += p.turnaround;
    }
    cout << fixed << setprecision(2);
    cout << "Average WT: " << avgWT / n << "\n";
    cout << "Average TAT: " << avgTAT / n << "\n";
}


int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].pid = i+1;
        cout << "Process " << i+1 << " Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Process " << i+1 << " Burst Time: ";
        cin >> processes[i].burst;
        cout << "Process " << i+1 << " Priority: ";
        cin >> processes[i].priority;
    }

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. FCFS\n2. SRTF\n3. Round Robin (quantum 10)\n4. Non-preemptive Priority\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch(choice) {
            case 1: FCFS(processes); break;
            case 2: SRTF(processes); break;
            case 3: RR(processes, 10); break;
            case 4: PriorityNP(processes); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }
    } while(choice != 5);

    return 0;
}
