#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_BUFFER 1024
#define ERRMSG  "An error has occurred\n"


void parseArg(char *cmdline, char **argv) {
            while (*cmdline != '\0') {
                while(*cmdline == ' ' || *cmdline == '\t' || *cmdline == '\n') {
                //printf("parsing whitespace %c\n", *cmdline);
			*cmdline++ = '\0';
                }
                *argv++ = cmdline;
                while(*cmdline != '\0' && *cmdline != ' ' && *cmdline != '\t' && *cmdline != '\n') {
                //printf("parsing  arg%c\n", *cmdline);
                        cmdline++;
                }
            }
        *argv = '\0';
}

void execute(char *path, char **argv) {
        printf("executing...");

        pid_t pid;

        if ((pid = fork()) < 0) {
                printf(ERRMSG);
                exit(1);
        } else if (pid == 0) {
                if (access("/bin/ls", X_OK) == 0) {
                        execlp("/bin", "ls", NULL);
                } else {
                        printf(ERRMSG);
                        exit(1);
                }
        } else {
                wait(NULL);
        }
        return;
}



int main(int argc, char **argv) {

        char *buffer;
        char *cmdline;
        char *args[65];
        char *path[3];
	path[0] = "/bin";
        size_t buf = MAX_BUFFER;
        int cmdsize;

        buffer = (char *)malloc(buf * sizeof(char));
        if (argc >= 2) {
                printf("system reads cmd\n");
                //execlp(path, "/bin/ls", NULL);
                exit(0);
        } else if (argc == 1) {
            printf("Wish> ");
        }
        while((cmdsize = getline(&buffer, &buf, stdin)) != -1) {
                
                buffer[cmdsize-1] = '\0';
                cmdline = buffer;
                parseArg(cmdline, args);
                //printf("%c\n", buffer[cmdsize-1]);
                //printf("%s\n", args[0]);
                //printf("%s\n", args[1]);

        if (strcmp(args[0], "exit") == 0){
                exit(0);
        }

	else if(strcmp(args[0], "path") == 0 ){
		printf("path is now %s", path[0]);
		}	
       	else if (strcmp(args[0], "cd") == 0){
		char s[100];
		printf("current dir is %s\n", getcwd(s, 100));
	}		


        printf("Wish> ");
        }
        printf("\n");
        exit(0);
        return 0;
}

