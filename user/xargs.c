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

    int read_finish = 0;
    while (read_finish == 0) {
        if (p >= MAXARG) {
            fprintf(2, "xargs: arg number out of range\n");
            exit(0);
        }
        
        buf_p = 0;
        for (;;) {
            if (buf_p >= MAXARG) {
                fprintf(2, "xargs: arg number out of range\n");
                exit(0);   
            }
            
            read_n = read(0, buf_chr, sizeof(buf_chr));
            
            if(read_n < 0) {
                fprintf(2, "xargs: read error\n");
                exit(0);
            }

            if(read_n == 0) {
                read_finish = 1;
                buf_str[buf_p] = '\0';
                break;
            }

            if(strcmp(buf_chr, "\n") == 0 ||
                strcmp(buf_chr, " ") == 0) {
                buf_str[buf_p] = '\0';
                break;
            }

            buf_str[buf_p++] = buf_chr[0];
        }
        if(buf_str[0] == '\0')
            continue;
        *(new_argv + p) = (char *) malloc(MAXARG * sizeof(char));
        strcpy(*(new_argv + p++), buf_str);
    }

    // exec new command
    if(fork() == 0) {
        exec(argv[1], new_argv);
        exit(0);
    }

    wait(0);
    exit(0);
}
