#include <stdio.h>
#include <sys/types.h>
int main()
{
	printf("sizeof(short)     = %d\n", sizeof(short));
	printf("sizeof(int*)       = %d\n", sizeof(int*));
	printf("sizeof(long)      = %d\n", sizeof(long));
	printf("sizeof(long long) = %d\n\n", sizeof(long long));

	printf("sizeof(size_t)    = %d\n", sizeof(size_t));
	printf("sizeof(off_t)     = %d\n", sizeof(off_t));
	printf("sizeof(void *)    = %d\n", sizeof(void *));
}
3B3B
