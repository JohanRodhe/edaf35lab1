#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>


void psigset(sigset_t *set) {
	int i, MAXSIG;

	for (i = 1; i <= MAXSIG; i++) {
		if (sigismember(set,i) == 1){	
			printf("Signal %d blocked\n", i);
		}
	}
}

int main(int argc, char const *argv[]) {
	sigset_t set, pendset;
	sigfillset(&set);
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	time_t start = time(NULL);
	time_t end = start + 5;
	
	printf("Entering loop \n");
	while(start < end) {
		start = time(NULL);
	}
	printf("\nExiting loop \n");

	sigpending(&pendset);
	psigset(&pendset);
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	return 0;
}