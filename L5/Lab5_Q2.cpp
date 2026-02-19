#include <iostream>
#include <vector>
#include <deque>   
#include <algorithm>
#include <iomanip>

// 2. Write a C program to simulate multi-level feedback queue scheduling algorithm.


using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
};


void printGantt(vector<int> &order, vector<int> &time) {
    cout << "\nGantt Chart:\n";
    for (int pid : order) cout << "| P" << pid << " ";
    cout << "|\n";
    for (int t : time) cout << t << "     ";
    cout << time.back() << "\n";
}

void MLFQ(vector<Process> processes) {
    int n = processes.size();
    
    int q1 = 4, q2 = 8, q3 = 12;
    

    for (auto &p : processes) p.remaining = p.burst;

    int time = 0, completed = 0;
    vector<int> ganttOrder, ganttTime;
    ganttTime.push_back(time);

    // Queues: 0 = high, 1 = medium, 2 = low
    deque<int> q[3];

    for (int i = 0; i < n; i++)
        if (processes[i].arrival <= time) q[0].push_back(i);

    while (completed < n) {
        int idx = -1;
        int level = -1;

        for (int i = 0; i < 3; i++) {
            if (!q[i].empty()) {
                idx = q[i].front(); q[i].pop_front();
                level = i;
                break;
            }
        }

        if (idx == -1) { 
            time++;
            for (int i = 0; i < n; i++)
                if (processes[i].arrival <= time && processes[i].remaining > 0 &&
                    find(q[0].begin(), q[0].end(), i) == q[0].end() &&
                    find(q[1].begin(), q[1].end(), i) == q[1].end() &&
                    find(q[2].begin(), q[2].end(), i) == q[2].end()) 
                    q[0].push_back(i);
            continue;
        }

        ganttOrder.push_back(processes[idx].pid);
        int exec;
        if (level == 0) exec = min(q1, processes[idx].remaining);
        else if (level == 1) exec = min(q2, processes[idx].remaining);
        else exec = min(q3, processes[idx].remaining);

        processes[idx].remaining -= exec;
        time += exec;
        ganttTime.push_back(time);

        for (int i = 0; i < n; i++)
            if (processes[i].arrival <= time && processes[i].remaining > 0 &&
                i != idx &&
                find(q[0].begin(), q[0].end(), i) == q[0].end() &&
                find(q[1].begin(), q[1].end(), i) == q[1].end() &&
                find(q[2].begin(), q[2].end(), i) == q[2].end()) 
                q[0].push_back(i);

        if (processes[idx].remaining > 0) {
            if (level < 2) q[level+1].push_back(idx);
            else q[2].push_back(idx);
        } else {
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
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst 
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
    }

    MLFQ(processes);
    return 0;
}
