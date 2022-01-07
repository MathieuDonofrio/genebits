#include "genebits/engine/parallel/job.h"

#include <benchmark/benchmark.h>

#include "fake_work.h"

namespace genebits::engine::bench
{
void BasicJob_ScheduleAndComplete_NoWork(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  for (auto _ : state)
  {
    BasicLambdaJob job(
      [&t]()
      {
        t++;
        benchmark::ClobberMemory();
      });

    JobHandle handle = scheduler.Schedule(&job);

    handle.Complete();

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);
}

BENCHMARK(BasicJob_ScheduleAndComplete_NoWork);

void BasicJob_ScheduleAndComplete_Work(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  const size_t amount = state.range(0);

  const size_t iterations = 1;
  const size_t work_per_iteration = amount / iterations;

  for (auto _ : state)
  {
    BasicLambdaJob job(
      [&t, work_per_iteration]()
      {
        Work(work_per_iteration);
        t++;
        benchmark::ClobberMemory();
      });

    JobHandle handle = scheduler.Schedule(&job);

    handle.Complete();

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);

  state.SetComplexityN(amount);
}

BENCHMARK(BasicJob_ScheduleAndComplete_Work)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

void ParallelForJob_ScheduleAndComplete_NoWork(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  for (auto _ : state)
  {
    ParallelForLambdaJob job(
      [&t](size_t index)
      {
        t += index;
        benchmark::ClobberMemory();
      },
      16);

    JobHandle handle = scheduler.Schedule(&job);

    handle.Complete();

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);
}

BENCHMARK(ParallelForJob_ScheduleAndComplete_NoWork);

void ParallelForJob_ScheduleAndComplete_Work(benchmark::State& state)
{
  ThreadPool pool;

  JobScheduler scheduler(pool);

  time_t t = std::time(nullptr);

  const size_t amount = state.range(0);

  const size_t iterations = 50;
  const size_t work_per_iteration = amount / iterations;

  for (auto _ : state)
  {
    ParallelForLambdaJob job(
      [&t, work_per_iteration](size_t index)
      {
        Work(work_per_iteration);
        t += index;
        benchmark::ClobberMemory();
      },
      iterations);

    JobHandle handle = scheduler.Schedule(&job);

    handle.Complete();

    benchmark::DoNotOptimize(handle);
    benchmark::DoNotOptimize(job);
  }

  benchmark::DoNotOptimize(scheduler);
  benchmark::DoNotOptimize(t);

  state.SetComplexityN(amount);
}

BENCHMARK(ParallelForJob_ScheduleAndComplete_Work)->Arg(1000)->Arg(10000)->Arg(100000)->Complexity();

} // namespace genebits::engine::bench