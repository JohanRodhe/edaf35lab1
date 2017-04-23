#include <stdio.h>
#include <signal.h>

void sigHandler2() {
	printf("Signal USR2 was sent \n");
}

void sigHandler() {
	printf("Ctrl-C was pressed \n");
	while(1) {
		signal(SIGUSR1, SIG_IGN);
		signal(SIGUSR2, sigHandler2);
	}
	
}


int main(int argc, char const *argv[]) {
	while(1){
		signal(SIGINT, sigHandler);
	}
	return 0;

}

