#include <stdio.h>
#include <signal.h>

void sigHandler() {
	printf("Ctrl-C was pressed \n");
}


int main(int argc, char const *argv[]) {
	struct sigaction act;
	act.sa_handler = &sigHandler;
	sigaction(SIGINT, &act, NULL);
	while(1){
		
	}
	return 0;
}