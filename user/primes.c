#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
check_primes(int p_rec)
{
    char buf[1];
    int curt_n = 1;
    int proc_n = 1;
    int child_flag = 0;
    int p_snd;
    int p[2];
    
    read(p_rec, buf, 1);
    curt_n = buf[0];
    fprintf(1, "prime %d\n", curt_n);
    
    while (0 != read(p_rec, buf, 1)) {
        proc_n = buf[0];
        
        if (0 == proc_n % curt_n) {
            continue;
        }

        if (child_flag) {
            write(p_snd, buf, 1);
            continue;
        }
        
        pipe(p);
        if(0 == fork()){
            curt_n = proc_n;
            close(p_rec);
            close(p[1]);
            p_rec = p[0];
            fprintf(1, "prime %d\n", curt_n);
        }
        else {
            child_flag = 1;
            close(p[0]);
            p_snd = p[1];
        }
    }
    
    close(p_rec);
    if(child_flag) {
        close(p_snd);
        wait(0);
    }

    exit(0);
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    
    if (0 == fork()) {
        close(p[1]);
        check_primes(p[0]);
        close(p[0]);
    }
    else {
        close(p[0]);
        for (int i = 2; i <=35; i++) {
            write(p[1], &i, 1);
            sleep(1);
        }
        close(p[1]);
        wait(0);
    }
    
    exit(0);
}
