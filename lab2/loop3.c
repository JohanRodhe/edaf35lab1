#include <stdio.h>
#include <signal.h>

void sigusr2() {
	printf("Signal USR2 was sent \n");
}

void sigHandler() {
	printf("Ctrl-C was pressed \n");
	struct sigaction usr1act, usr2act;
	usr1act.sa_handler = SIG_IGN;
	usr2act.sa_handler = &sigusr2;
	sigaction(SIGUSR1, &usr1act, NULL);
	sigaction(SIGUSR2, &usr2act, NULL);
	while(1) {
	}
	
}


int main(int argc, char const *argv[]) {
	struct sigaction act;
	act.sa_handler = &sigHandler;
	sigaction(SIGINT, &act, NULL);
	while(1){	
	}
	return 0;

}

