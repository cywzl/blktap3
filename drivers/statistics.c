#include "../drivers/statistics.h"

#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "tapdisk-log.h"

unsigned long total_time_elapsed = 0;
long total_ios = 0;

pthread_mutex_t statistics_mutex;
pthread_t statistics_thread_id;
long clks_per_sec;

void* log_time_statistics(void* arg){

	DPRINTF("Log Statistics Thread Started");

	while(1){
		sleep(STAT_TIME_MS);

		pthread_mutex_lock(&statistics_mutex);
		if (total_ios != 0)
			DPRINTF("Avg I/O operation latency during the last %d ns: %lu, (total elapsed:%lu)", STAT_TIME_MS, total_time_elapsed / total_ios, total_time_elapsed );
		total_time_elapsed = 0;
		total_ios = 0;
		pthread_mutex_unlock(&statistics_mutex);
	}
	return NULL;
}

int init_statistics(){
	 clks_per_sec = sysconf(_SC_CLK_TCK);
	return pthread_mutex_init(&statistics_mutex, NULL);
}

int start_collecting(){
	return pthread_create(&statistics_thread_id, NULL, log_time_statistics, NULL);
}

void update_time(struct timespec start_time, struct timespec finish_time){

	long tv = 0;

	pthread_mutex_lock(&statistics_mutex);

	if(start_time.tv_sec == finish_time.tv_sec){
		tv = finish_time.tv_nsec - start_time.tv_nsec;
	}else{
		tv = (long)((finish_time.tv_sec - start_time.tv_sec)*(1000000000)) + (long)(finish_time.tv_nsec - start_time.tv_nsec);
	}
	/*DPRINTF("Update Time Stats: io start at: %ld secs, %ld nanosecs; finish at: %ld secs, %ld nanosecs, elapsed: %lu", start_time.tv_sec, start_time.tv_nsec, finish_time.tv_sec, finish_time.tv_nsec, tv);*/

	total_time_elapsed += tv;
	total_ios++;

	pthread_mutex_unlock(&statistics_mutex);
}

int stop_collecting(){
	return pthread_cancel(statistics_thread_id);
}

int destroy_statistics(){
	return pthread_mutex_destroy(&statistics_mutex);
}
