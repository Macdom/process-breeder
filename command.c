#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

pid_t forkAndExecute(char * command){
	int i;
	pid_t p = fork();
	
	if(p < 0){
		return -1;
	}
	
	if(p == 0){
		system(command); 
		exit(0);
		return 0;
	}
}

void limitProcessNumber(int procNo){
	struct rlimit lim;
	lim.rlim_cur = procNo;
	lim.rlim_max = procNo;
	setrlimit(RLIMIT_NPROC, &lim);
} 

int main(int argc, char ** argv){
	
	// wrong argument number
	if(argc < 2 || argc > 4){
		printf("Usage: command [amount] [limit]\n");
		exit(-1);
	}
	
	// just a command
	if(argc == 2){
		system(argv[1]);
		exit(0);
	}
	
	//command and amount
	if(argc == 3){
		int i;
		int amount = atoi(argv[2]);
		for(i = 0; i < amount; i++){
			system(argv[1]);
		}
		exit(0);
	}
	
	// command, amount, limit
	char* command = argv[1];
	int amount = atoi(argv[2]);
	int limit = atoi(argv[3]);
	if(limit > 10000){
		printf("Maximum limit is 10000\n");
		exit(-1);
	}
	int i;
	int comCount;

	limitProcessNumber(limit+8); // for some reason it's always a few processes short
	while(1){
		pid_t child;
		do{
			child = forkAndExecute(command);
			wait(NULL);
		}
		while(child < 0);
		comCount++;
		if (comCount >= amount) break;
			usleep(2500);								
	}
}
