#include "engine/events/event_bus.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  struct TestEvent1
  {
    size_t value;
  };

  size_t sum1_1 = 0;

  void listen1_1(const TestEvent1& event)
  {
    sum1_1 = event.value;
  }

  void listen1_2(const TestEvent1&)
  {
  }
} // namespace

TEST(EventBus_Tests, Count_OnConstruction_Zero)
{
  EventBus bus;

  ASSERT_EQ(bus.Count<TestEvent1>(), 0u);
}

TEST(EventBus_Tests, Subscribe_SingleEventType_IncreaseCount)
{
  EventBus bus;

  bus.Subscribe<TestEvent1>(listen1_1);

  ASSERT_EQ(bus.Count<TestEvent1>(), 1u);

  bus.Subscribe<TestEvent1>(listen1_2);

  ASSERT_EQ(bus.Count<TestEvent1>(), 2u);
}

TEST(EventBus_Tests, Unsubscribe_SingleEventType_DecreaseCount)
{
  EventBus bus;

  bus.Subscribe<TestEvent1>(listen1_1);
  bus.Unsubscribe<TestEvent1>(listen1_1);

  ASSERT_EQ(bus.Count<TestEvent1>(), 0u);
}

TEST(EventBus_Tests, Publish_SingleSubscribe_CallsSubscribe)
{
  EventBus bus;

  bus.Subscribe<TestEvent1>(listen1_1);

  sum1_1 = 0;

  bus.Publish(TestEvent1 { 10u });

  ASSERT_EQ(sum1_1, 10u);
}

} // namespace genebits::engine::tests