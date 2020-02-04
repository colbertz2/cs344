#include <stdio.h>
#include <unistd.h>

int main() {
	int pid = getpid();

	printf("My PID is %d\n", pid);

	return 0;
}
