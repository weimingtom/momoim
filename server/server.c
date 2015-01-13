#include "server.h"
#include "datafile/base.h"
#include <stdlib.h>
#include <logger.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <run/gc.h>
#include "run/jobs.h"

int IsServerRunning = 1;
WorkerType worker[CONFIG_WORKER_COUNT];

pthread_t ThreadListener;

static void initWorker(int id, WorkerType *worker)
{
    worker->workerId = id;
    pthread_create(&worker->WorkerThread, NULL, WorkerMain, worker);
}

static void sigInterupt(int sig)
{
    log_info("MAIN", "Server is exiting...\n");
    IsServerRunning = 0;
    pthread_kill(ThreadListener, SIGINT);
    for (int i = 0; i < CONFIG_WORKER_COUNT; ++i)
    {
        pthread_kill(worker[i].WorkerThread, SIGINT);
    }
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
    sigemptyset(&act.sa_mask);
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

    for (int i = 0; i < CONFIG_WORKER_COUNT; i++)
    {
        initWorker(i, worker + i);
    }
    InitUserManager();

    pthread_create(&ThreadListener, NULL, ListenMain, NULL);
    GarbageCollectorInitialize();
    while (IsServerRunning)
    {
        sleep(1);
    }
    pthread_join(ThreadListener, NULL);
    GarbageCollectorFinalize();
    for (int i = 0; i < CONFIG_WORKER_COUNT; i++)
    {
        pthread_join(worker[i].WorkerThread, NULL);
    }
    FinalizeUserManager();
    DataModuleFinalize();
    return EXIT_SUCCESS;
}