#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
};

// Function prototypes
void fcfs(struct Process p[], int n);
void sjf(struct Process p[], int n);
void roundRobin(struct Process p[], int n, int quantum);
void displayResults(struct Process p[], int n);
void resetProcesses(struct Process p[], int n);

int main() {
    int n, choice, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i+1;
        printf("Process P%d Arrival Time: ", i+1);
        scanf("%d", &p[i].arrivalTime);
        printf("Process P%d Burst Time: ", i+1);
        scanf("%d", &p[i].burstTime);
        p[i].remainingTime = p[i].burstTime;
    }

    do {
        printf("\n--- CPU Scheduling Simulator ---\n");
        printf("1. First-Come-First-Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF - Non-preemptive)\n");
        printf("3. Round Robin\n");
        printf("4. Exit\n");
        printf("Select Scheduling Algorithm: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(p, n);
                resetProcesses(p, n);
                break;
            case 2:
                sjf(p, n);
                resetProcesses(p, n);
                break;
            case 3:
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                roundRobin(p, n, quantum);
                resetProcesses(p, n);
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);

    return 0;
}

void fcfs(struct Process p[], int n) {
    int currentTime = 0;
    float totalWT = 0, totalTAT = 0;

    printf("\n--- FCFS Scheduling ---\n");
    printf("Gantt Chart: ");

    for (int i = 0; i < n; i++) {
        if (currentTime < p[i].arrivalTime)
            currentTime = p[i].arrivalTime;

        printf("| P%d ", p[i].pid);
        currentTime += p[i].burstTime;
        p[i].completionTime = currentTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;

        totalWT += p[i].waitingTime;
        totalTAT += p[i].turnaroundTime;
    }
    printf("|\n");

    displayResults(p, n);

    printf("Average Waiting Time: %.2f\n", totalWT/n);
    printf("Average Turnaround Time: %.2f\n", totalTAT/n);
}

void sjf(struct Process p[], int n) {
    int completed = 0, currentTime = 0;
    float totalWT = 0, totalTAT = 0;

    printf("\n--- SJF (Non-preemptive) Scheduling ---\n");
    printf("Gantt Chart: ");

    while (completed < n) {
        int idx = -1;
        int minBT = 1e9;

        for (int i = 0; i < n; i++) {
            if (p[i].arrivalTime <= currentTime && p[i].remainingTime > 0 && p[i].burstTime < minBT) {
                minBT = p[i].burstTime;
                idx = i;
            }
        }

        if (idx != -1) {
            printf("| P%d ", p[idx].pid);
            currentTime += p[idx].burstTime;
            p[idx].completionTime = currentTime;
            p[idx].turnaroundTime = p[idx].completionTime - p[idx].arrivalTime;
            p[idx].waitingTime = p[idx].turnaroundTime - p[idx].burstTime;
            p[idx].remainingTime = 0;

            totalWT += p[idx].waitingTime;
            totalTAT += p[idx].turnaroundTime;
            completed++;
        } else {
            currentTime++;
        }
    }
    printf("|\n");

    displayResults(p, n);
    printf("Average Waiting Time: %.2f\n", totalWT/n);
    printf("Average Turnaround Time: %.2f\n", totalTAT/n);
}

void roundRobin(struct Process p[], int n, int quantum) {
    int completed = 0, currentTime = 0;
    float totalWT = 0, totalTAT = 0;

    printf("\n--- Round Robin Scheduling (Time Quantum = %d) ---\n", quantum);
    printf("Gantt Chart: ");

    while (completed < n) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (p[i].remainingTime > 0 && p[i].arrivalTime <= currentTime) {
                done = false;
                if (p[i].remainingTime > quantum) {
                    printf("| P%d ", p[i].pid);
                    currentTime += quantum;
                    p[i].remainingTime -= quantum;
                } else {
                    printf("| P%d ", p[i].pid);
                    currentTime += p[i].remainingTime;
                    p[i].remainingTime = 0;
                    p[i].completionTime = currentTime;
                    p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
                    p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;

                    totalWT += p[i].waitingTime;
                    totalTAT += p[i].turnaroundTime;
                    completed++;
                }
            }
        }
        if (done) currentTime++;
    }
    printf("|\n");

    displayResults(p, n);
    printf("Average Waiting Time: %.2f\n", totalWT/n);
    printf("Average Turnaround Time: %.2f\n", totalTAT/n);
}

void displayResults(struct Process p[], int n) {
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrivalTime, p[i].burstTime, p[i].completionTime, p[i].turnaroundTime, p[i].waitingTime);
    }
}

void resetProcesses(struct Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remainingTime = p[i].burstTime;
        p[i].completionTime = 0;
        p[i].waitingTime = 0;
        p[i].turnaroundTime = 0;
    }
}

