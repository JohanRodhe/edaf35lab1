#include <stdio.h>
#include <signal.h>
#include <time.h>

int proceed = 1;

void sigHandler() {
	proceed = 0;
}


int main(int argc, char const *argv[]) {
	int count = 0;
	alarm(5);
	int start = time(NULL);
	int end = start + 1;
	signal(SIGALRM, sigHandler);
	while(proceed){
		start = time(NULL);
		if (start > end){
			printf("%d so far\n", count);
			end = start + 1;
		}
		++count;
	}
	printf("Number of iterations: %d\n",count);
	return 0;

}

