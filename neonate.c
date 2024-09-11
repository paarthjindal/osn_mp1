#include "neonate.h"
#include "dirent.h"

int get_most_recent_pid() {
    // Implement logic to get the most recent process PID
    // One possible solution is to check the contents of the /proc directory
    // and return the PID with the most recent creation timestamp.
    
    struct dirent *entry;
    DIR *dp = opendir("/proc");
    int recent_pid = 0;
    time_t latest_time = 0;

    if (dp == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dp))) {
        int pid = atoi(entry->d_name);
        if (pid > 0) {
            char path[256];
            snprintf(path, sizeof(path), "/proc/%d", pid);

            struct stat statbuf;
            if (stat(path, &statbuf) == 0) {
                if (statbuf.st_ctime > latest_time) {
                    latest_time = statbuf.st_ctime;
                    recent_pid = pid;
                }
            }
        }
    }

    closedir(dp);
    return recent_pid;
}

// Function to enable non-blocking input
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void neonate(int interval) {
    while (1) {
        if (kbhit()) {
            char ch = getchar();
            if (ch == 'x') {
                printf("Exiting neonate...\n");
                break;
            }
        }

        int recent_pid = get_most_recent_pid();
        printf("%d\n", recent_pid);

        sleep(interval);
    }
}