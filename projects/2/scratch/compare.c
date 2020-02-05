#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	
	int ti, tc;
	char* a = "TEST", *b = "NOT TEST";

	printf("4 == 4 = %d\n", 4==4);
	printf("4 == 6 = %d\n", 4==6);
	printf("strcmp('TEST', 'TEST') = %d\n", strcmp(a, a));
	printf("strcmp('TEST', 'NOT TEST') = %d\n", strcmp(a, b));

	return 0;
}
