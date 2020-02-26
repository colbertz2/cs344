#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int retval = -5;
    char* myStr = "cd";
    char myScan[10];
    memset(myScan, '\0', sizeof(myScan));

    sscanf(myStr, "%*s %s", myScan);

    if (strcmp(myScan, "") == 0) {
        retval = chdir(getenv("HOME"));
    }

    printf("%d\n", retval);

    return 0;
}