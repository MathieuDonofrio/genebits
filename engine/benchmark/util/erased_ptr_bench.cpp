#include "genebits/engine/util/erased_ptr.h"

#include <memory>

#include <benchmark/benchmark.h>

namespace genebits::engine::bench
{
static void ErasedPtr_STD_SharedPtr_Cast(benchmark::State& state)
{
  for (auto _ : state)
  {
    std::shared_ptr<void> erased = std::static_pointer_cast<void>(std::make_shared<int>(10));

    int* casted = std::static_pointer_cast<int>(erased).get();

    benchmark::DoNotOptimize(casted);
  }
}

BENCHMARK(ErasedPtr_STD_SharedPtr_Cast);

static void ErasedPtr_Cast(benchmark::State& state)
{
  for (auto _ : state)
  {
    ErasedPtr<void> erased(new int(10));

    int* casted = erased.Cast<int>();

    benchmark::DoNotOptimize(casted);
  }
}

BENCHMARK(ErasedPtr_Cast);

} // namespace genebits::engine::bench