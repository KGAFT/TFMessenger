//
// Created by kgaft on 5/16/24.
//

#ifndef MEMORYPOOLMANAGER_H
#define MEMORYPOOLMANAGER_H
#include <apr_pools.h>
#include <map>
#include <vector>


class MemoryPoolManager {
private:
    static inline std::map<apr_pool_t*, bool> allocatedPools = std::map<apr_pool_t*, bool>();
    static inline bool poolsInitialized = false;
    static inline uint32_t releasedPoolCount = 0;
public:
    static apr_pool_t* acquireMemoryPool() {
        if(!poolsInitialized) {
            apr_pool_initialize();
            poolsInitialized = true;
        }
        for (auto pool : allocatedPools) {
            if(!pool.second) {
                apr_pool_clear(pool.first);
                pool.second = true;
                releasedPoolCount--;
                return pool.first;
            }
        }
        apr_pool_t* newPool;
        apr_pool_create(&newPool, nullptr);
        allocatedPools[newPool] = true;
        return newPool;
    }
    static void poolRelease(apr_pool_t* pool) {
        allocatedPools[pool] = false;
        releasedPoolCount++;
        if(releasedPoolCount>6) {
            cleanUpRoutine();
        }
    }
private:
    static void cleanUpRoutine() {
        std::vector<apr_pool_t*> deallocatedPools;
        for (auto pool : allocatedPools) {
            if(!pool.second) {
                deallocatedPools.push_back(pool.first);
                apr_pool_destroy(pool.first);
                releasedPoolCount--;
            }
            if(releasedPoolCount<=2) {
                break;
            }
        }
        for (auto pool : deallocatedPools) {
            allocatedPools.erase(pool);
        }
    }
};



#endif //MEMORYPOOLMANAGER_H
