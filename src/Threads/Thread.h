//
// Created by kgaft on 5/16/24.
//

#ifndef THREAD_H
#define THREAD_H
#include <apr_thread_proc.h>
#include <iostream>
#include <stdexcept>

#include "Memory/MemoryPoolManager.h"


class Thread {
private:
    static inline apr_pool_t* threadPool = nullptr;
    static void * APR_THREAD_FUNC threadMain(apr_thread_t *thd, void *data) {
        ((Thread*)data)->runnable(((Thread*)data)->runnableData);
        ((Thread*)data)->threadExecuting = false;
        apr_thread_exit(thd, 0);
    }
public:
    Thread() {
        if(!threadPool) {
            threadPool = MemoryPoolManager::acquireMemoryPool();

        }
    }
private:
    apr_thread_t* baseThread;
    void (*runnable)(void*);
    void* runnableData;
    bool threadExecuting = false;
public:
    void run(void (*runnable)(void*), void* dataForRunnable) {
        if(!threadExecuting) {
            Thread::runnable = runnable;
            Thread::runnableData = dataForRunnable;
            if(apr_thread_create(&baseThread, nullptr, threadMain, this, threadPool)!=APR_SUCCESS) {
                throw std::runtime_error("Failed to start thread");
            }
            threadExecuting = true;
        }else {
            std::cerr<<"Cannot execute second runnable on executable thread"<<std::endl;

        }
    }
    apr_status_t join() {
        apr_status_t res = 0;
        apr_thread_join(&res, baseThread);
        return res;
    }
    ~Thread() {
    }
};



#endif //THREAD_H
