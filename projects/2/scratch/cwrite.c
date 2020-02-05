#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	char str[80];
	
	memset(str, '\0', 80 * sizeof(char));
	strcat(str, "ROOM NAME: ");
	strcat(str, "Test!");
	strcat(str, "\n");
	write(fd, str, strlen(str) * sizeof(char));

	close(fd);

	return 0;
}

