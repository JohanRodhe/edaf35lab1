#include "listmalloc.h"
#include <stdio.h>

int main()
{
	int* n = malloc(5*sizeof(int));
	
	free(n);
	printf("test successful\n");
	return 0;
}
