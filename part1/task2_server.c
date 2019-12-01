#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include "general.h"
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

int msg_id;
struct msgbuf msgbuf;

void sighandler(int signo) {
    (void)signo;
    msgctl(msg_id, IPC_RMID, NULL);
    _exit(0);
}

struct server_info save_info(void) {
    struct server_info server_info;
    server_info.pid = getpid();
    server_info.uid = getuid();
    server_info.gid = getgid();
    server_info.work_time = 0;
    getloadavg(server_info.loadavg, 3);

    return server_info;
}


int main(){

    time_t start_time = time(NULL);

    time_t cur_time;

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = sighandler;
    sigaction(SIGINT, &act, NULL);

    /* setup message queue */
    msg_id = msgget(1998, IPC_CREAT | 0666);
    if(msg_id < 0) {
        perror("msgget");
        exit(1);
    }


    msgbuf.info = save_info();
    msgbuf.mtype = 1;


    while(1) {
        cur_time = time(NULL);

        msgbuf.mtype++;
        msgbuf.info.work_time = cur_time - start_time;
        getloadavg(msgbuf.info.loadavg, 3);
        if(msgsnd(msg_id, &msgbuf, sizeof(msgbuf), 0) < 0) {
            perror("msgsnd");
        }


        sleep(1);
    }

    return 0;

}
