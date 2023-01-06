
#include "mt_worker.h"
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>

using namespace std;

pthread_mutex_t worker_lock;

inline void split(std::vector<std::string> &result, const std::string &str,
 const std::vector<char> &delimiters) {
  result.clear();
  size_t start = 0;
  while (start < str.size()) {
   size_t itRes = str.find(delimiters[0], start);
   for (size_t i = 1; i < delimiters.size(); ++i) {
       size_t it = str.find(delimiters[i], start);
       if (it < itRes) itRes = it;
     }
   if (itRes == std::string::npos) {
      result.push_back(str.substr(start, str.size() - start));
    break;
    }
   result.push_back(str.substr(start, itRes - start));
   start = itRes;
   ++start;
 }
}

std::vector < std::vector < std::string > > get_file_content(std::string file_name){
  ifstream file_in(file_name);
  std::vector < std::vector < std::string > > vec_content;
  if (!file_in){
    return vec_content;
  }

  while(!file_in.eof()){
     string line = "";//存放读入每行内容
     getline(file_in, line);
     if(0 == line.size() || line[0] == '#'){
       continue;
     }
     std::vector<std::string> data_str;
     split(data_str,line,{',',' '});
     vec_content.push_back(data_str);
  }
  file_in.close();
  
  return vec_content;
} 

int MtJobPool::init() {
  worker_lock = PTHREAD_MUTEX_INITIALIZER; /**< lock */
  ndealed_ = 0;

  //init res
  
  return 0;
}

int MtJobPool::uninit() {
}

int MtJobPool::load_task(char* inTxt){
  vector < vector < string > > vec_in = get_file_content(inTxt);
  task_buf_.clear();
  for (int i = 0; i < vec_in.size(); i++) {
    taskT unit;
    unit.idx = i;
    unit.content = vec_in[i][0];
    unit.result.clear();
    task_buf_.push_back(unit);
  }

  return 0;
}

taskT* MtJobPool::fetch_task(){
  taskT* task;
  pthread_mutex_lock(&worker_lock);
  if (ndealed_ >= task_buf_.size()) {
    pthread_mutex_unlock(&worker_lock);
    return NULL;
  }
  task = &(task_buf_[ndealed_++]);
  pthread_mutex_unlock(&worker_lock);
  return task;
}

int MtWorker::init(MtJobPool* pJobs,char* sz_res) {
  pJobs_ = pJobs;
  
  //init worker 
  
  return 0;
}

int MtWorker::uninit() {
  return 0;
}

int MtWorker::start() {
  return 0;
}

int MtWorker::stop() {
  return 0;
}

int MtWorker::process(taskT* task) {
  task->result = task->content + "_operated";
  printf("operate : %s\n", task->content.c_str());
  return 0;
}

void* work(void* args) {
  taskT* task=NULL;
  int ret = 0;
  MtWorker* pWorker = (MtWorker*)args;
  MtJobPool* pJobs = pWorker->pJobs_;
  while((task = pJobs->fetch_task()) != NULL){
    ret = pWorker->process(task);
    if (0 != ret) {
      continue;
    }
  }
  return (void*)0;
}

