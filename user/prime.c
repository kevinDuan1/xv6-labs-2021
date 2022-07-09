#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void f(int* leftpipe) {
    if(fork() == 0){
        int p;
        if (read(leftpipe[0], &p, sizeof(p)) == 0) {
            close(leftpipe[0]);
            return;
        }
        printf("prime %d\n", p);
        int rightpipe[2];
        pipe(rightpipe);
        
        while(1){
            int i;
            if (read(leftpipe[0], &i, sizeof(i)) == 0) {
                break;
            }

            if (i%p != 0) {
                write(rightpipe[1], &i, sizeof(i));
            }
        }
        close(leftpipe[0]);
        close(rightpipe[1]);
        f(rightpipe); //recursive step
    }else{
        wait((int*) 0);
    }
}

int main() {
    int p[2];
    pipe(p);
    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    f(p);
    exit(0);
}