#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t)(int);

sighandler_t trap_signal(int sig, sighandler_t handler);
void OriginalHandler(int sig);

int main(int argc, char *argv[])
{

    trap_signal(SIGINT, OriginalHandler);
    pause();

    return 1;
}

sighandler_t trap_signal(int sig, sighandler_t handler)
{
    struct sigaction act, old;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if(sigaction(sig, &act, &old) < 0)
    {
        return NULL;
    }

    return old.sa_handler;
}

void OriginalHandler(int sig)
{
    printf("catch signal(%d)\n", sig);
    exit(0);
}
