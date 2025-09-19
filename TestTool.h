#pragma once
#include "common.h"
#include <format>
#include <random>

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
    string s = format("multiply: {} * {} = {}", a, b, res);
    poollog(s, NONE);
}

void multiply_output(int &out, const int a, const int b)
{
    simulate_hard_computation();
    out = a * b;
    string s = format("multiply_output: {} * {} = {}", a, b, out);
    poollog(s, NONE);
}

int multiply_return(const int a, const int b)
{
    simulate_hard_computation();
    const int res = a * b;
    string s = format("multiply_return: {} * {} = {}", a, b, res);
    poollog(s, NONE);
    return res;
}
