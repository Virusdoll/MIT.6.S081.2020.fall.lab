#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "xargs: argument number error\n");
        exit(0);
    }

    char *new_argv[MAXARG];
    char buf_chr[1];
    char buf_str[MAXARG];
    int p, read_n, buf_p;

    p = 0;
    for(int i = 1; i < argc; i++, p++) {
        if(p >= MAXARG) {
            fprintf(2, "xargs: arg number out of range\n");
            exit(0);
        }
        *(new_argv + p) = argv[i];
    }

    *(new_argv + p) = buf_str;
    buf_p = 0;
    for(;;) {
        read_n = read(0, buf_chr, sizeof(buf_chr));

        if(read_n < 0) {
            fprintf(2, "xargs: read error\n");
            exit(0);
        }

        if(read_n == 0) {
            break;
        }
        
        if(p >= MAXARG) {
            fprintf(2, "xargs: arg number out of range\n");
            exit(0);
        }

        if(buf_p >= MAXARG - 1) {
            fprintf(2, "xargs: arg length out of range\n");
            exit(0);
        }
        
        if(strcmp(buf_chr, "\n") == 0){
            *(*(new_argv + p++) + buf_p) = 0;
            buf_p=0;
            char buf_str[MAXARG];
            *(new_argv + p) = buf_str;
            continue;
        }

        *(*(new_argv + p) + buf_p++) = buf_chr[0];
    }
    
    if(fork() == 0) {
        exec(argv[1], new_argv);
        exit(0);
    }

    wait(0);
    exit(0);
}
