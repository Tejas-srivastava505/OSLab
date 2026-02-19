#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int deadline;
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

void EDF(vector<Process> processes) {
    int n = processes.size();
    vector<int> ganttOrder, ganttTime;
    int time = 0;
    ganttTime.push_back(time);

    int completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        
        int idx = -1;
        int minDeadline = 1e9;
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= time && processes[i].deadline < minDeadline) {
                idx = i;
                minDeadline = processes[i].deadline;
            }
        }

        if (idx == -1) {
            
            time++;
            continue;
        }

        
        ganttOrder.push_back(processes[idx].pid);
        time += processes[idx].burst;
        processes[idx].completion = time;
        processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
        processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
        ganttTime.push_back(time);
        done[idx] = true;
        completed++;
    }

    printGantt(ganttOrder, ganttTime);

    double avgWT = 0, avgTAT = 0;
    cout << "\nProcess\tAT\tBT\tDL\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.deadline << "\t" << p.completion << "\t" << p.turnaround
             << "\t" << p.waiting << "\n";
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
        processes[i].pid = i + 1;
        cout << "Process " << i + 1 << " Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Process " << i + 1 << " Burst Time: ";
        cin >> processes[i].burst;
        cout << "Process " << i + 1 << " Deadline: ";
        cin >> processes[i].deadline;
    }

    EDF(processes);

    return 0;
}
