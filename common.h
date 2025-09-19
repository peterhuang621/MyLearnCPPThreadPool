#pragma once

#include <concepts>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>
using namespace std;

#define USELOG 1

// for likely compatibility
#ifdef __GNUC__

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#else

#define likely(x) (x)
#define unlikely(x) (x)

#endif

// NonCopyable class
class NonCopyable
{
  protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

  private:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

enum LOGLEVEL
{
    NONE,
    INFO,
    WARN,
    ERROR
};

inline void poollog(const std::string &msg, const LOGLEVEL level)
{
    if (!USELOG)
        return;

    std::string line;
    switch (level)
    {
    case NONE:
        line = msg + "\n";
        break;
    case INFO:
        line = "\x1b[32;1;4m[INFO]\x1b[0m " + msg + "\n";
        break;
    case WARN:
        line = "\x1b[33;1;4m[WARN]\x1b[0m " + msg + "\n";
        break;
    default:
        line = "\x1b[31;1;4m[ERROR]\x1b[0m " + msg + "\n";
        break;
    }

    write(STDERR_FILENO, line.data(), line.size());
}
