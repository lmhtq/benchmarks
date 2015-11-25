#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

int threads_num = 1000;
int sum = 0;
int finish = 0;

void show_usage(char *cmd)
{
    printf("%s threads_num\n", cmd);
}

void on_timer(int signo)
{
    finish = 1;
}

void mutex_test()
{

}

void rcu_test()
{
    
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        show_usage(argv[0]);
    else {

    }

    itimerval value, ovalue;
    value.it_value.tv_sec = 2;
    value.it_value.tv_usec = 0;
    value.it_interval.tv_sec = 2;
    value.it_interval.tv_usec = 0;
    signal(SIGALRM, on_timer);
    setitimer(ITIMER_REAL, &value, &ovalue);

    while(!finish) {

    }

    return 0;
}
