#include "TestTool.h"
#include "ThreadPool.h"
#include <benchmark/benchmark.h>

void example()
{
    ThreadPool pool(3);
    pool.init();

    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= 10; ++j)
        {
            pool.submit(multiply, i, j);
        }

    int output_ref;
    auto future1 = pool.submit(multiply_output, std::ref(output_ref), 5, 6);

    future1.get();
    cout << "\x1b[32;1;4m[INFO]\x1b[0m Last operation result is equals to " << output_ref << endl;

    for (int i = 4; i <= 6; ++i)
        for (int j = 11; j <= 15; ++j)
        {
            pool.submit(multiply_return, i, j);
        }

    auto future2 = pool.submit(multiply_return, 0, 0);

    int res = future2.get();
    cout << "\x1b[32;1;4m[INFO]\x1b[0m Last operation result is equals to " << res << endl;

    pool.shutdown();
}

static void BM_example(benchmark::State &state)
{
    for (auto _ : state)
    {
        example();
    }
}

BENCHMARK(BM_example);

int main(int argc, char **argv)
{
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();

    return 0;
}
