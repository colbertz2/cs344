#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {

    int newestDirTime = -1;
    char targetDirName[] = "colbertz.rooms.";
    char newestDirName[50];
    memset(newestDirName, '\0', sizeof(newestDirName));

    DIR* dirToCheck;
    struct dirent *fileInDir;
    struct stat dirAtts;

    dirToCheck = opendir(".");

    if (dirToCheck > 0) {
        while ((fileInDir = readdir(dirToCheck)) != NULL) {
            if (strstr(fileInDir->d_name, targetDirName) != NULL) {
                stat(fileInDir->d_name, &dirAtts);
                if ((int)dirAtts.st_mtime > newestDirTime) {
                    newestDirTime = (int)dirAtts.st_mtime;
                    memset(newestDirName, '\0', sizeof(newestDirName));
                    strcpy(newestDirName, fileInDir->d_name);

                }
            }
        }
    }

    closedir(dirToCheck);

    printf("Newest Dir: %s\n", newestDirName);
    printf("Time: %d\n", newestDirTime);

    return 0;
}