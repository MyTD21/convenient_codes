
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "mt_worker.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("input err,usage: %s lst result thread_num\n", argv[0]);
    return -1;
  }

  int threadnum = atoi(argv[3]);
  
  MtJobPool tJobPool;
  tJobPool.init();
  tJobPool.load_task(argv[1]);

  pthread_t* tids;
  tids = (pthread_t*)malloc(sizeof(pthread_t) * threadnum);
  if (NULL == tids) {
    printf("failed to allocate for pthreads");
    return -1;
  }

  MtWorker *pWorkers = (MtWorker*)malloc(sizeof(MtWorker) * threadnum);
  struct timeval start_time;
  struct timeval end_time;
  gettimeofday(&start_time, NULL);
  
  for (int i = 0; i < threadnum; i++) {
    pWorkers[i].init(&tJobPool, argv[2]);
  }

  for (int i=0; i<threadnum; i++) {
    if (pthread_create(tids+i, NULL, work, (void*)(&(pWorkers[i]))) < 0) {
      return -1;
    }
  }

  for(int i=0; i<threadnum; i++){
    pthread_join(tids[i], NULL);
  }

  for (int i = 0; i < threadnum; i++) {
    pWorkers[i].uninit();
  }
  free(pWorkers);

  printf("show result:");

  vector <taskT> task_buf = tJobPool.get_task_buf();
  for (int i = 0; i < task_buf.size(); i++) {
    printf("input : %s, output : %s\n", task_buf[i].content.c_str(), task_buf[i].result.c_str());
  }

  tJobPool.uninit();

  gettimeofday(&end_time, NULL);
  int cpu_time = (end_time.tv_sec - start_time.tv_sec)*1000000 + (end_time.tv_usec - start_time.tv_usec);
  printf("TimeUsed : %d us\n", cpu_time);

  return 0;
}
