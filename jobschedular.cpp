#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <fstream>
#include <climits>
using namespace std;

struct Job {
    int id, arrivalTime, coresReq, memReq, exeTime;
};

struct WorkerNode {
    int id, availableCores, availableMem;
};

queue<Job> fcfsQueue, sjfQueue, sdjfQueue;
vector<WorkerNode> workerNodes(128, {0, 24, 64});

void scheduleFCFS() {
    while (!fcfsQueue.empty()) {
        Job currentJob = fcfsQueue.front();
        fcfsQueue.pop();

        for (auto& node : workerNodes) {
            if (node.availableCores >= currentJob.coresReq && node.availableMem >= currentJob.memReq) {
                node.availableCores -= currentJob.coresReq;
                node.availableMem -= currentJob.memReq;
                break;
            }
        }
    }
}

void scheduleSJF() {
    vector<Job> sortedJobs;
    while (!sjfQueue.empty()) {
        sortedJobs.push_back(sjfQueue.front());
        sjfQueue.pop();
    }
    sort(sortedJobs.begin(), sortedJobs.end(), [](const Job& a, const Job& b) {
        return (a.coresReq + a.memReq) < (b.coresReq + b.memReq);
    });

    for (const Job& currentJob : sortedJobs) {
        for (auto& node : workerNodes) {
            if (node.availableCores >= currentJob.coresReq && node.availableMem >= currentJob.memReq) {
                node.availableCores -= currentJob.coresReq;
                node.availableMem -= currentJob.memReq;
                break;
            }
        }
    }
}

void scheduleSDJF() {
    vector<Job> sortedJobs;
    while (!sdjfQueue.empty()) {
        sortedJobs.push_back(sdjfQueue.front());
        sdjfQueue.pop();
    }
    sort(sortedJobs.begin(), sortedJobs.end(), [](const Job& a, const Job& b) {
        return a.exeTime < b.exeTime;
    });

    for (const Job& currentJob : sortedJobs) {
        for (auto& node : workerNodes) {
            if (node.availableCores >= currentJob.coresReq && node.availableMem >= currentJob.memReq) {
                node.availableCores -= currentJob.coresReq;
                node.availableMem -= currentJob.memReq;
                break;
            }
        }
    }
}

void allocateFirstFit(Job currentJob) {
    for (auto& node : workerNodes) {
        if (node.availableCores >= currentJob.coresReq && node.availableMem >= currentJob.memReq) {
            node.availableCores -= currentJob.coresReq;
            node.availableMem -= currentJob.memReq;
            return;
        }
    }
}

void allocateBestFit(Job currentJob) {
    int bestNodeId = -1;
    int INT_MAX;
    int minWaste = INT_MAX;

    for (int i = 0; i < workerNodes.size(); i++) {
        if (workerNodes[i].availableCores >= currentJob.coresReq && workerNodes[i].availableMem >= currentJob.memReq) {
            int waste = workerNodes[i].availableCores - currentJob.coresReq + workerNodes[i].availableMem - currentJob.memReq;
            if (waste < minWaste) {
                minWaste = waste;
                bestNodeId = i;
            }
        }
    }

    if (bestNodeId != -1) {
        workerNodes[bestNodeId].availableCores -= currentJob.coresReq;
        workerNodes[bestNodeId].availableMem -= currentJob.memReq;
    }
}

void allocateWorstFit(Job currentJob) {
    int bestNodeId = -1;
    int INT_MIN;
    int maxWaste = INT_MIN;

    for (int i = 0; i < workerNodes.size(); i++) {
        if (workerNodes[i].availableCores >= currentJob.coresReq && workerNodes[i].availableMem >= currentJob.memReq) {
            int waste = workerNodes[i].availableCores - currentJob.coresReq + workerNodes[i].availableMem - currentJob.memReq;
            if (waste > maxWaste) {
                maxWaste = waste;
                bestNodeId = i;
            }
        }
    }

    if (bestNodeId != -1) {
        workerNodes[bestNodeId].availableCores -= currentJob.coresReq;
        workerNodes[bestNodeId].availableMem -= currentJob.memReq;
    }
}

int main() {
    
    fcfsQueue.push({1, 1, 10, 16, 5});
    fcfsQueue.push({2, 2, 12, 32, 3});
    fcfsQueue.push({3, 3, 8, 16, 4});
    sjfQueue.push({1, 1, 10, 16, 5});
    sjfQueue.push({2, 2, 12, 32, 3});
    sjfQueue.push({3, 3, 8, 16, 4});
    sdjfQueue.push({1, 1, 10, 16, 5});
    sdjfQueue.push({2, 2, 12, 32, 3});
    sdjfQueue.push({3, 3, 8, 16, 4});

   
    cout << "FCFS Scheduling:" << endl;
    scheduleFCFS();
    cout << "SJF Scheduling:" << endl;
    scheduleSJF();
    cout << "SDJF Scheduling:" << endl;
    scheduleSDJF();

    
    Job job1 = {1, 1, 10, 16, 5};
    Job job2 = {2, 2, 12, 32, 3};
    Job job3 = {3, 3, 8, 16, 4};

    cout << "First Fit Allocation:" << endl;
    allocateFirstFit(job1);
    allocateFirstFit(job2);
    allocateFirstFit(job3);

    cout << "Best Fit Allocation:" << endl;
    allocateBestFit(job1);
    allocateBestFit(job2);
    allocateBestFit(job3);

    cout << "Worst Fit Allocation:" << endl;
    allocateWorstFit(job1);
    allocateWorstFit(job2);
    allocateWorstFit(job3);

    
    double avgCpuUtil = 0.0;
    double avgMemUtil = 0.0;

    for (const auto& node : workerNodes) {
        avgCpuUtil += (24 - node.availableCores) / 24.0;
        avgMemUtil += (64 - node.availableMem) / 64.0;
    }

    avgCpuUtil /= workerNodes.size();
    avgMemUtil /= workerNodes.size();

    cout << "Average CPU Utilization: " << avgCpuUtil * 100 << "%" << endl;
    cout << "Average Memory Utilization: " << avgMemUtil * 100 << "%" << endl;

    return 0;
}