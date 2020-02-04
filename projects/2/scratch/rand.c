#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int num_$get();

int main() {
	srand(getpid());
	printf("%d\n", num_$get());

	return 0;
}

int num_$get() {
	return rand();
}
