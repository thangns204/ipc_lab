#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include "general.h"
#include <sys/mman.h>
#include <fcntl.h>


struct server_info save_info(void) {
    struct server_info server_info;
    server_info.pid = getpid();
    server_info.uid = getuid();
    server_info.gid = getgid();
    server_info.work_time = 0;
    getloadavg(server_info.loadavg, 3);

    return server_info;
}


int main(void) {
    struct server_info *srv_info;
    time_t start_time = time(NULL);

    time_t cur_time;
    int fd;

    /* remove socket file if exists */
    unlink(TMP_FILE);

    /* create ipc file */
    fd = open(TMP_FILE, O_RDWR | O_CREAT, 0644);
    if(fd < 0) {
        perror(TMP_FILE);
        exit(EXIT_FAILURE);
    }
    /* memory-mapping */
    srv_info = (struct server_info*)mmap(NULL, sizeof(struct server_info), PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
    if(srv_info == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* Prevent SIGBUS */
    // used to shrink or extend the size of each FILE to the specified size
    // if a FILE is larger than specified size , the extra data is lost, but if A FILE is shorter,
    // it is extended and the extended part (hole) reads as zero bytes.
    truncate(TMP_FILE, sizeof(struct server_info));

    *srv_info = save_info();

    while(1) {
        cur_time = time(NULL);
        srv_info->work_time = cur_time - start_time;
        getloadavg(srv_info->loadavg, 3);
        sleep(1);
    }

    return 0;
}
