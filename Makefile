all: wish.c 
	gcc -g -Wall -o wish wish.c

clean: 
	  $(RM) wish
