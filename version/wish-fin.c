#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_BUFFER 1024
#define ERRMSG  "An error has occurred\n"

#define true 1
#define false 0

char *path[2]; //holds paths of the shell
int len; //arg count of current parsed argument
int exitbatch;
int redir;

//parses output redirection
char *parseOut(char *fpath) {
	//printf("%s\n", fpath);
        char *fedit = strdup(fpath);
	if (fedit == NULL) {
		return NULL;
	}	

	char *back  = fedit + strlen(fedit) - 1;	
	while (strchr(fedit, ' ') != NULL) {
		fedit++;
	}
	if (*fedit == '\0');
		return NULL;
	
	while(strchr(back, ' ') != NULL) {
		back--;
	}
	back[1] = '\0';	
	//printf("%s\n", fedit);
        return fedit;
	
}


int execute(char **argv, char *fpath) {

        
        //built ins
             //   printf("arg[0] is %s\n", argv[0]);
              //  printf("arg[1] is %s\n", argv[1]);

       
         if (strcmp(argv[0], "exit") == 0){//exit
                //printf("exiting\n"); 
		 if (argv[1] != NULL) {
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch)
                       		exit(0);
			return 1;
		 } else
		 exit(0);
         } else if(strcmp(argv[0], "path") == 0 ){//path
                if (len <= 3) {
                    path[0] = argv[1];
                    path[1] = argv[2];
		    if (path[0] != NULL) 
		   	 path[0] = strdup(path[0]);
		    if (path[1] != NULL) 
		    	path[1] = strdup(path[1]);
                    return 0;
                } else {
                   
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch)
                       		exit(0);
                    return 1;
                }
                return 1;
        } else if(strcmp(argv[0], "cd") == 0){//change directory
                
		
		 if (argv[2] == NULL) {
			if (chdir(argv[1])){	
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch)
                       		exit(0);
			return 1;
			}
		} else {	
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch)
                       		exit(0);
			return 1;
		}
                return 0;
                 
        } else if (strcmp(argv[0], "if") == 0){
		if (len != 7) {
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch)
                        exit(0);
			return 1;
		} else {
			char **cut;
			if (strcmp(argv[2], "==") == 0) {
				cut[0] = argv[0];
				if (execute(cut[0], fpath) == atoi(argv[3])) {
				}
				return 0;
			} else if (strcmp(argv[2], "!=") == 0) {	
				printf("no\n");
				return 0;
			}
		}	
	}
	
        
        //access external commands
      // printf("Fork?\n"); 
       pid_t pid;
        if (!(pid = fork())) 
        {	
		argv = argv;
		//fpath = fpath;
		//printf("%s\n", fpath);	
		//printf("args are %d\n", len);
            //error if file has more than one delimiter
	    if (redir) {
		if (fpath == NULL) {
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                        if (exitbatch){
                            exit(0);
                        } else {
                            exit(1);
			}
		}

	      else if (fpath != NULL) {
                fpath = parseOut(fpath);
                if (strchr(fpath, '>') != NULL || fpath == NULL
				|| strchr(fpath, ' ') != NULL
				|| strchr(fpath, '\t') != NULL
				|| strlen(fpath) == 0) {
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG));
                        if (exitbatch){
                            exit(0);
                        } else {
                            exit(1);
                        }
                    }
                	FILE *file = fopen(fpath, "w");
              		close(1);
                	dup2(fileno(file), fileno(stdout));
		
	    	}
	    }
            if (access(argv[0], X_OK) != 0) { 
                char **pathptr = path;
                while (*pathptr != NULL) {
                	char *path_conc = (char *)malloc(strlen(*pathptr) + strlen(argv[0]) + 2);
               		strcat(path_conc, *pathptr);
			strcat(path_conc, "/");
			strcat(path_conc, argv[0]);
                	if (access(path_conc, X_OK) == 0) {
	                    	execv(path_conc, argv);
                	}
               		pathptr++;
			free(path_conc);
			path_conc = NULL;
                }
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG)); 
                       		exit(1);
                return 1;
            } else {
                execv(argv[0], argv);
       	    }
            exit(0);
	}
	int status;
		
	waitpid(pid, &status, 0);

	if ( !WIFEXITED(status))
	{
		//	printf("%d\n", status);	
			write(STDERR_FILENO, ERRMSG, strlen(ERRMSG));
		       return 1;	
	}

	return 1;
}


//parses current cmd/args
char **parseArg(char *cmd, char** args) {
    //edit the cmd string
            while (*cmd != '\0') {
                while(*cmd == ' ' || *cmd == '\t' || *cmd == '\n') {
                        *cmd = '\0';
                        cmd++;
                                               // printf("e\n");
                }
                	args[len] = cmd;
			len++;
			
                while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t' && *cmd != '\n') {
               // printf("a\n");
                        cmd++;
               }
           }

		for(int i = len-1; i >= 0; i--) {
			cmd = args[i] + strlen(args[i]);
			while(*cmd == ' ' || *cmd == '\t' || *cmd == '\n' || *cmd == '\0') {
				cmd--;
			}
			cmd[1] = '\0';
			//printf("args %s|\n ", args[i]);
		}

		if(args[len-1] == NULL || strlen(args[len-1]) == 0
				|| strcmp(args[len-1], "") == 0){
			args[len-1] == NULL;
			len--;
		}
			
            return args;
}
//parses current line
void parseLine(char *cmdline) {
        char **argv = NULL;
        char *fpath = NULL;
	char *cmddup = strdup(cmdline);
        char *cmd = strsep(&cmddup, "&");
        //printf ("parsing %zu\n", strlen(cmd));
        do {
            cmd[strlen(cmd)-1] = '\0';
            len = 0;
            argv = malloc(128 * sizeof(char**));
            argv[0] = NULL;
            if (strchr(cmd, '>') != NULL) {
                cmd = strtok(cmd, ">");
                fpath = strtok(NULL, ">");
		redir = true;
		}
            
	    if (strlen(cmd) == 0)
		    return;

            argv = parseArg(cmd, argv);
            
            //printf("fpath is %s\n", fpath);
            
          //printf("len value is %d\n", len);
        execute(argv, fpath);
        free(argv);
	redir = false;
        fpath = NULL;
        } while ((*cmd = strsep(&cmddup,"&")) != NULL);
	
	wait(NULL);
	
        return;
}



int main(int argc, char **argv) {

        char *buffer;
        size_t buf = MAX_BUFFER;
        int cmdsize;

        path[0] = "/bin";
        buffer = (char *)malloc(buf * sizeof(char));

        if (argc == 2) {
               // printf("system reads cmd\n");
                exitbatch = true;
                FILE *file = fopen(argv[1], "r");
		if (file == NULL) {
		    write(STDERR_FILENO, ERRMSG, strlen(ERRMSG));
		    exit(1);
		}

		while ((cmdsize = getline(&buffer, &buf, file)) != -1)
			parseLine(buffer);

		fclose(file);

        } else if (argc == 1) {
                exitbatch = false;
        do {
    
                printf("Wish> ");
                fflush(stdout);
                if((cmdsize = getline(&buffer, &buf, stdin)) == -1) {
                        exit(0);
                }
                parseLine(buffer);
                

        } while(1);
	} else {
		write(STDERR_FILENO, ERRMSG, strlen(ERRMSG));
		exit(1);
	}
	free(buffer);	
        return 0;
}
