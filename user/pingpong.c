#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p[2];
    char buf[4];
    int pid;
    int msg_number;
    pipe(p);

    if(fork() == 0) {
        pid = getpid();
        
        msg_number = read(p[0], buf, 4);
        fprintf(1, "%d(child): received ping -- %s(%dchars)\n", pid, buf, msg_number);
        
        write(p[1], "CtoP", 4);
        
        close(p[0]);
        close(p[1]);
        exit(0);
    } else {
        pid = getpid();
        
        write(p[1], "PtoC", 4);
        
        wait(0);
        msg_number = read(p[0], buf, 4);
        fprintf(1, "%d(parent): received pong -- %s(%dchars)\n", pid, buf, msg_number);

        close(p[0]);
        close(p[1]);
    }
    exit(0);
}
