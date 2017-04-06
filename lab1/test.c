#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {

        char* ls = "/bin/ls";
        char* wc = "/usr/bin/wc";
        int pipe_fd[2];
	pipe(pipe_fd);
        int child1, child2, status1, status2;
        child1 = fork();


        if (child1 == 0){
                dup2(pipe_fd[1],1);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
        }
        if (child1 != 0){
                child2 = fork();
        }

        if (child2 == 0){
                dup2(pipe_fd[0],0);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
        }

        if (child1 == 0){
                char*  strings[] = {"ls", NULL};
                execv(ls, strings);
        }

        if (child2 == 0){
                char*  strings[] = {"wc", NULL};
                execv(wc, strings);
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        if (child1 > 0 && child2 > 0){
                waitpid(child1, &status1, 0);
                waitpid(child2, &status2, 0);
        }




        return 0;
}
