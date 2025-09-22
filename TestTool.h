#pragma once
#include "common.h"
#include <random>
#include <sstream>

using namespace std;

random_device rd;
mt19937 mt(rd());

uniform_int_distribution<int> dist(-100, 100);

auto rnd = bind(dist, mt);

void simulate_hard_computation()
{
    this_thread::sleep_for(std::chrono::milliseconds(500 + rnd()));
}

void multiply(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    stringstream ss;
    ss << "multiply: " << a << " * " << b << " = " << res;
    poollog(ss.str(), NONE);
}

void multiply_output(int &out, const int a, const int b)
{
    out = a * b;
    stringstream ss;
    ss << "multiply_output: " << a << " * " << b << " = " << out;
    poollog(ss.str(), NONE);
}

int multiply_return(const int a, const int b)
{
    const int res = a * b;
    stringstream ss;
    ss << "multiply_return: " << a << " * " << b << " = " << res;
    poollog(ss.str(), NONE);
    return res;
}
