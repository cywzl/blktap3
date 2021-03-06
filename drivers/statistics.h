#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "tapdisk-log.h"

#define STAT_TIME_MS 1

extern unsigned long total_io_interval;
extern long total_ios ;

extern pthread_mutex_t statistics_mutex;
extern pthread_t statistics_thread_id;

void* log_time_statistics(void* arg);

int init_statistics();

int start_collecting();

void update_io_interval(struct timespec start_time, struct timespec finish_time);

void update_req_inqueue_time(unsigned long inqueue_time, unsigned long offqueue_time);

int stop_collecting();

int destroy_statistics();
