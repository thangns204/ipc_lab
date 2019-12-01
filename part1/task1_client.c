//
// Created by Ngo Thang on 11/30/19.
//

//
// Created by Ngo Thang on 11/15/19.
//

#include <sys/shm.h>
#include "general.h"
#include <stdio.h>

int main(){


    int mem_id = shmget(1998, sizeof(struct server_info), 0666);
    if (mem_id < 0) {
        fprintf(stderr, "Error: shmget. \n");
        exit(1);
    }

    struct server_info *server_info = (struct server_info*) shmat(mem_id, NULL, 0);

    if (server_info == NULL) {
        fprintf(stderr, "Error: shmat. \n");
        exit(1);
    }

    printf("%li\n%li\n%li\n", (long)server_info->pid, (long)server_info->uid, (long)server_info->gid);
    printf("%li\n", (long)server_info->work_time);
    printf("%.6f %.6f %.6f\n", server_info->loadavg[0], server_info->loadavg[1], server_info->loadavg[2]);
    
    return 0;
}
