//
// Created by Ngo Thang on 11/30/19.
//

#ifndef TASK1_SERVER_GENERAL_H
#define TASK1_SERVER_GENERAL_H

#include <stdlib.h>
#include <sys/types.h>

const char* TMP_FILE = "/tmp/s245030_ipc";

struct server_info {
    time_t start_time; // time server start
    pid_t pid; //id process
    uid_t uid; //id user
    gid_t gid; //id group process
    time_t work_time; // system uptime in seconds
    double loadavg[3]; //system boot in the last 1, 5, 15 minutes
};

struct msgbuf {
    long mtype;
    struct server_info info;
};



#endif //TASK1_SERVER_GENERAL_H
