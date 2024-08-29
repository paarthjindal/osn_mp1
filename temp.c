#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include <sys/types.h>

int main()
{
    pid_t p_id;
    printf("current process id is %d\n",getpid());
    p_id=fork();
    if(p_id<0)
    {
        printf("fork failsed");
    }
    else if(p_id==0)
    {
        printf("value of child subprocess is %d\n",getpid());
    }
    else
    {
        wait(NULL);

    }
    return 0;
}