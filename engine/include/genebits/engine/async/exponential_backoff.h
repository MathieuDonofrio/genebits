#ifndef GENEBITS_ENGINE_ASYNC_EXPONENTIAL_BACKOFF_H
#define GENEBITS_ENGINE_ASYNC_EXPONENTIAL_BACKOFF_H

#include <cstdint>

#include "genebits/engine/os/thread.h"

namespace genebits::engine
{
///
/// Uses in spin wait loops to exponentially wait for longer durations.
///
class ExponentialBackoff
{
public:
  ///
  /// Constructor.
  ///
  constexpr ExponentialBackoff() noexcept : count_(1) {}

  ///
  /// Waits for short amount of cycles. Every time this is called the wait duration grows exponentially.
  ///
  /// After a while, will simply yield.
  ///
  void Wait() noexcept
  {
    if (count_ <= cMaxPauseLoops)
    {
      // Busy-waiting

      for (size_t i = 0; i != count_; ++i)
      {
        // Double pause to save more power
        this_thread::Pause();
        this_thread::Pause();
      }

      count_ <<= 1;
    }
    else
    {
      // Fallback to yielding spins

      std::this_thread::yield();
    }
  }

  ///
  /// Resets the internal counter. The next wait, will have the smallest possible duration.
  ///
  void Reset()
  {
    count_ = 1;
  }

private:
  static constexpr uint_fast16_t cMaxPauseLoops = 1 << 8;

  uint_fast16_t count_;
};
} // namespace genebits::engine

#endif