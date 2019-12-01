#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include "general.h"
#include <sys/shm.h>
#include <sys/msg.h>


int main(void) {
    struct msgbuf msgbuf;

    int msg_id;


    /* setup message queue */
    msg_id = msgget(1998, IPC_CREAT | 0666);
    if(msg_id < 0) {
        perror("msgget");
        return 1;
    }

    /* Get msg */
    if(msgrcv(msg_id, &msgbuf, sizeof(msgbuf), 0, 0 < 0) < 0) {
        perror("msgrcv");
        return 1;
    }

    printf("%li\n%li\n%li\n", (long)msgbuf.info.pid, (long)msgbuf.info.uid, (long)msgbuf.info.gid);
    printf("%li\n", (long)msgbuf.info.work_time);
    printf("%.6f %.6f %.6f\n", msgbuf.info.loadavg[0], msgbuf.info.loadavg[1], msgbuf.info.loadavg[2]);

    return 0;
}
