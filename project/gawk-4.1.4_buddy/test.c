#include "buddymalloc.h" 

int main()
{
	int* hej =  malloc(300*sizeof(int));
	free(hej);
	return 0;
}
