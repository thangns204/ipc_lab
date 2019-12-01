#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "general.h"
#include <sys/shm.h>

void set_param(struct server_info *server_info){
    time_t now;
    time(&now);
    server_info->work_time = now - server_info->start_time;
    getloadavg(server_info->loadavg, 3);
    printf("%ld -- %f %f %f\n", server_info->work_time, server_info->loadavg[0], server_info->loadavg[1], server_info->loadavg[2]);
}

int main(){
    //shmget returns an identifier in shmid
    //IPC_PRIVATE: means that new IPC resource should be created. The resource is not private , only newly created
    int mem_id = shmget(1998, sizeof(struct server_info), 0666|IPC_CREAT);
    if (mem_id < 0) {
        fprintf(stderr, "Error: shmget. \n");
        exit(1);
    }

    //shmat to attach shared memory
    struct server_info *server_info = (struct server_info*)shmat(mem_id, NULL ,0);

    if (server_info == NULL) {
        fprintf(stderr, "Error: shmat. \n");
        exit(1);
    }


    time(&server_info->start_time);
    server_info->pid = getpid();
    server_info->uid = getuid();
    server_info->gid = getgid();
    while (true){
        sleep(1);
        set_param(server_info);
    }

    return 0;
}
