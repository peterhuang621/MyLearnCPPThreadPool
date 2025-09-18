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
#include <unordered_map>
#include <vector>
using namespace std;

// for likely compatibility
#ifdef __GNUC__ || __clang__

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
    INFO,
    WARN,
    ERROR
};

inline void poollog(const string &msg, const LOGLEVEL level)
{
    switch (level)
    {
    case INFO:
        cerr << "\x1b[32;1;4m[INFO] " << msg << "\x1b[0m\n";
        break;
    case WARN:
        cerr << "\x1b[33;1;4m[WARN] " << msg << "\x1b[0m\n";
        break;
    default:
        cerr << "\x1b[31;1;4m[ERROR] " << msg << "\x1b[0m\n";
        break;
    }
}
