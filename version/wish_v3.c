#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_BUFFER 1024
#define ERRMSG  "An error has occurred\n"


char *path[2];


void execute(int argc, char **argv) {
        
	printf("arg[0] is %s\n", argv[0]);
	//built ins
	if (argv[1] != NULL) {
		printf("arg[1] is %s\n", argv[1]);
	}	
       	 if (strcmp(argv[0], "exit") == 0){
               	 exit(0);
         } else if(strcmp(argv[0], "path") == 0 ){
		if (argc <= 3) {
			path[0] = argv[1];
			path[1] = argv[2]; 
		 
		 printf("path is now %s", path[0]);
		} else {
			printf("%s", ERRMSG);
		}
		return;
	} else if(strcmp(argv[0], "cd") == 0){
		if (argc <= 2) {
		char s[100];
		chdir(argv[1]);
		printf("current dir is %s\n", getcwd(s, 100));
		} else {
			printf("%s", ERRMSG);
		}
		return;
	}
        return;
}

void parseArg(char *cmdline) {
	char **argv = NULL;
	char *cmd = strsep(&cmdline, "&");
	cmd[strlen(cmd)-1] = '\0';
	//printf ("parsing %zu", strlen(cmd));
	do {
	    int len = 0;
	    argv = malloc(sizeof(char**));
	    argv[0] = NULL;
	    if (strchr(cmd, '>') != NULL) {
		cmd = strtok(command, ">");
		char *fpath = strtok(NULL, ">");

	    }
            while (*cmd != '\0') {
                while(*cmd == ' ' || *cmd == '\t' || *cmd == '\n') {
			*cmd = '\0';
			cmd++;
			printf("e\n");
                }
                argv[len] = cmd;
		printf("arg %s\n", argv[len]);
		len++;
                while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') {
                printf("a\n");
                        cmd++;
               }
            }
	  printf("len value is %d\n", len);
	execute(len, argv);
	free(argv);
	} while ((*cmd = strsep(&cmdline,"&")) != NULL);
	return;
}



int main(int argc, char **argv) {

        char *buffer;
        char *cmdline;
        size_t buf = MAX_BUFFER;
        int cmdsize;

	path[0] = "/bin";
        buffer = (char *)malloc(buf * sizeof(char));

        if (argc >= 2) {
                printf("system reads cmd\n");
                //execlp(path, "/bin/ls", NULL);
                exit(0);
        } else if (argc == 1) {
           
	do {
                
        	printf("Wish> ");
		fflush(stdout);
	 	if((cmdsize = getline(&buffer, &buf, stdin)) == -1) {
			exit(0);
		}
	//	printf("here");	
                parseArg(buffer);
		
	} while(1);
	}	
        printf("\n");
        exit(0);
        return 0;
}

