//ce programme affiche sizeof de void * et de int

#include <stdio.h>

int main(int argc, char **argv)
{
	//affiche: void *      int     long
	printf("%i %i %i",sizeof(void *), sizeof(int),sizeof(long));
	return 0;
}

