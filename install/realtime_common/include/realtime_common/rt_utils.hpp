#pragma once

#include <pthread.h>
#include <sched.h>
#include <cstring>
#include <iostream>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <new>
#include <utility>
#include <type_traits>

namespace realtime_common
{

inline bool set_realtime_priority(int priority = 80)
{
  if (priority < 1 || priority > 90) {
    std::cerr << "Priority must be 1-90 (90 is max safe)" << std::endl;
    return false;
  }

  struct sched_param params {};
  params.sched_priority = priority;
  
  if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &params) != 0) {
    std::cerr << "Failed to set SCHED_FIFO priority " << priority 
              << ": " << strerror(errno) << std::endl;
    return false;
  }
  
  return true;
}

inline bool set_cpu_affinity(int cpu_core)
{
  if (cpu_core < 1 || cpu_core > 15) {
    std::cerr << "CPU core must be 1-15" << std::endl;
    return false;
  }

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(cpu_core, &cpuset);
  
  if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
    std::cerr << "Failed to set CPU affinity to core " << cpu_core << std::endl;
    return false;
  }
  
  return true;
}

}  // namespace realtime_common