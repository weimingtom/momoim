#include "server.h"
#include "datafile/base.h"
#include <stdlib.h>
#include <logger.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "run/jobs.h"

int IsServerRunning = 1;
WorkerType worker[WORKER_COUNT];

pthread_t ThreadListener;

static void initWorker(int id, WorkerType *worker)
{
    worker->workerId = id;
    sem_init(&worker->ready, 0, 0);
    pthread_mutex_init(&worker->lock, NULL);
    pthread_create(&worker->WorkerThread, NULL, WorkerMain, worker);
}

static void sigInterupt(int sig)
{
    log_info("MAIN", "Server is exiting...\n");
    IsServerRunning = 0;

    struct sigaction act = {
            .sa_handler=SIG_DFL,
    };
    sigaction(sig, &act, NULL);
}

int main(int argc, char **argv)
{
    //setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    struct sigaction act = {
            .sa_flags=0
    };
    act.sa_handler = sigInterupt;
    sigaction(SIGINT, &act, NULL);
    act.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &act, NULL);
    if (!DataModuleInit())
    {
        log_error("MAIN", "Fail to initliaze data module.\n");
        return EXIT_FAILURE;
    }

    InitJobManger();

    int i;
    for (i = 0; i < WORKER_COUNT; i++)
    {
        initWorker(i, worker + i);
    }
    InitUserManager();

    pthread_create(&ThreadListener, NULL, ListenMain, NULL);
    while (1)
    {
        sleep(1);
        log_info("DEBUG", "Key\n");
    }
    pthread_join(ThreadListener, NULL);
    for (i = 0; i < WORKER_COUNT; i++)
    {
        pthread_join(worker[i].WorkerThread, NULL);
    }
    FinalizeUserManager();
    DataModuleFinalize();
    return EXIT_SUCCESS;

}