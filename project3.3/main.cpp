//
//  main.cpp
//  project3.1
//
//  Created by Jared Goddard on 5/7/15.
//  Copyright (c) 2015 Jared. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <queue>
#include <random>
#include <vector>
#include <algorithm>
#include <math.h>
#include "main.h"

float generate_working_time() {
    std::exponential_distribution<float> distribution(lambda);
    float working_time = distribution(generator);
    return working_time;
}

float generate_service_time() {
    std::exponential_distribution<float> distribution(mu);
    float service_time = distribution(generator);
    return service_time;
}

bool compareWorkingTime(const Machine &a, const Machine &b) {
    return a.w_time < b.w_time;
}

void initialize() {
    printf("Enter number of workers: ");
    std::cin>> worker_count;
    printf("Enter number of machines: ");
    std::cin>> machine_count;
    for (int i = 0; i < machine_count; i++) {
        Machine m;
        m.machine_num = i+1;
        m.being_serviced = false;
        m.working = true;
        m.w_time = generate_working_time();
        m.cycles = 0;
        //        printf("Machine num: %d working time: %f\nService time: %f\n", m.machine_num, m.w_time, m.s_time);
        machineList.push_back(m);
    }
    w1.servicing = false;
    w2.servicing = false;
    std::sort(machineList.begin(), machineList.end(), compareWorkingTime); //sort the machine list in ascending order
}

void service(Machine m, Worker w) {
    m.arrival_time = cur_time;
    w.servicing = true;
    m.being_serviced = true;
    m.s_time = generate_service_time();
    service_rate = m.s_time + service_rate;
    service_count += 1;
    m.worker_assigned = w;
    m.cycles += 1;
}

int total_cycles(std::vector<Machine> list) {
    int total_cycles = 0;
    for (int i = 0; i < list.size(); i++) {
        total_cycles += list[i].cycles;
    }
    printf("Current total cycles: %d\n", total_cycles);
    return total_cycles;
}

void service_check(){
    while (!serviceQueue.empty()) {
        Machine first_in_queue = serviceQueue.top();
        if (cur_time >= first_in_queue.arrival_time + first_in_queue.s_time) {
            first_in_queue.cycles += 1;
//            printf("<%f> Machine %d Starts working--Cycles Completed: %d\n", cur_time, first_in_queue.machine_num, first_in_queue.cycles);
            first_in_queue.w_time = generate_working_time();
            if (first_in_queue.cycles >= 100) {
                first_in_queue.w_time = 1000000;
                first_in_queue.cycles = 100;
            }
            machineList.push_back(first_in_queue);
            std::sort(machineList.begin(), machineList.end(), compareWorkingTime);
            serviceQueue.pop();
        }
        first_in_queue.worker_assigned.servicing = false;
    }
}

void run() {
    while (total_cycles(machineList) < (machine_count * 100)) {
        Machine first_in_list = machineList.front();
        
        if (first_in_list.cycles >= 100) {
            first_in_list.cycles = 100;
            first_in_list.w_time = 1000000;
        }
        
        if (w1.servicing == false && w2.servicing == false) {
            cur_time += machineList[0].w_time;
            service(machineList[0], w1);
            serviceQueue.push(machineList[0]);
            
            cur_time += machineList[1].w_time;
            service(machineList[1], w2);
            serviceQueue.push(machineList[1]);
            
            machineList.erase(machineList.begin());
            machineList.erase(machineList.begin());
            service_check();
        }
        
        if (worker_count == 1) {
            cur_time += machineList[0].w_time;
            service(machineList[0], w1);
            serviceQueue.push(machineList[0]);
            machineList.erase(machineList.begin());
            service_check();
        }
        if (worker_count == 2) {
            cur_time += machineList[0].w_time;
            service(machineList[0], w1);
            serviceQueue.push(machineList[0]);
            cur_time += machineList[1].w_time;
            service(machineList[1], w2);
            serviceQueue.push(machineList[1]);
            machineList.erase(machineList.begin());
            machineList.erase(machineList.begin());
            service_check();
        }
        if (worker_count == 3) {
            cur_time += machineList[0].w_time;
            service(machineList[0], w1);
            serviceQueue.push(machineList[0]);
            cur_time += machineList[1].w_time;
            service(machineList[1], w2);
            serviceQueue.push(machineList[1]);
            cur_time += machineList[2].w_time;
            service(machineList[2], w2);
            serviceQueue.push(machineList[2]);
            machineList.erase(machineList.begin());
            machineList.erase(machineList.begin());
            machineList.erase(machineList.begin());
            service_check();
        }
    }
}

int main(int argc, const char * argv[]) {
    initialize();
    run();
    printf("Current time: %f\n", fmod(cur_time, 1000000));
    server_utilization = 1/(service_rate/1000);
    server_utilization = (lambda)/(server_utilization);
    if(worker_count == 1){
        printf("Server Utilization: %f%% \n", lambda/(server_utilization) *100);
    }
    if(worker_count == 2){
        printf("Server Utilization: %f%% \n", lambda/(server_utilization*2) *100);
    }
    if(worker_count == 3){
        printf("Server Utilization: %f%% \n", lambda/(server_utilization*3) *100);
    }
    return 0;
}