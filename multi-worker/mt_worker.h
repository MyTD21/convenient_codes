 
/**
 *
 * @file mt_worker.h 
 * @brief 多线程功能库
 *
 **/

#ifndef _MULTI_THREAD_H_
#define _MULTI_THREAD_H_

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <vector>

using namespace std;

//task 
typedef struct {
  int idx;
  string content;
  string result;
}taskT;

class MtJobPool {
public:
  int init();
  int uninit();
  
  taskT* fetch_task();
  vector <taskT> get_task_buf() {
    return task_buf_;
  }
  int load_task(char* inTxt);
 
  vector <taskT> task_buf_;
  int ndealed_;
};

//multi-line worker
class MtWorker {
public:
  int init(MtJobPool* pJobs, char* sz_res);
  int uninit();
  int start();
  int stop();
  int process(taskT* task);
  MtJobPool* pJobs_;
  char result_[1024];
private:
};

void* work(void* args);

#endif
