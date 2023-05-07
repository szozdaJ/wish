#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER 512

int main(int argc, char *argv[]) {

	char *cmdline;
	size_t buf = MAX_BUFFER;
//	char errMSG[30] = "An error has occurred\n";

	cmdline = (char *)malloc(MAX_BUFFER * sizeof(char));

	if (argc == 2) {
		printf("system reads cmd\n");
		exit(0);
	} else if (argc == 1) {
	printf("Wish> ");
	}

	while(getline(&cmdline, &buf, stdin) != -1) {
	
       	if (strcmp(cmdline, "exit\n") == 0){
		exit(0);
//		break;
	}


	printf("Wish> ");
	}
	free(cmdline);
	printf("\n");
	return 0;
}
