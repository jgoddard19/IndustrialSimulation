//
//  main.cpp
//  project3.1
//
//  Created by Jared Goddard on 5/7/15.
//  Copyright (c) 2015 Jared. All rights reserved.
//

#include <stdio.h>

struct Worker {
    bool servicing;
};

struct Machine {
    bool working;
    bool being_serviced;
    float w_time;
    float s_time;
    int machine_num;
    int cycles;
    Worker worker_assigned;
    float arrival_time;
    
    bool operator< (const Machine &y) const {
        return y.arrival_time < arrival_time;
    }
};

int worker_count = 2;
int machine_count = 10;
const float lambda = (.05);
const float mu = (.2);
float cur_time = 0;
float server_utilization;
float service_rate;
int service_count;
Worker w;
Worker none;
Worker w1;
Worker w2;
std::default_random_engine generator;
std::vector<Machine> machineList;
std::priority_queue<Machine, std::vector<Machine>> serviceQueue;

void initialize();
void service();
float generate_service_time();
float generate_working_time();
void sort(std::vector<Machine> list);