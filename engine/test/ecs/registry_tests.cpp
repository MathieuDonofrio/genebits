#include "genebits/engine/ecs/registry.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(Registry_Tests, EntityCount_AfterInitialization_Zero)
{
  Registry registry;

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
}

TEST(Registry_Tests, Create_AfterInitialization_IncreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int>(10);

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ(entity, 0u);
}

TEST(Registry_Tests, Create_Multiple_IncreaseEntityCount)
{
  Registry registry;

  for (size_t i = 0; i < 10; i++)
  {
    EXPECT_EQ(registry.Create<size_t>(size_t(i)), i);
  }

  EXPECT_EQ(registry.EntityCount(), 10);
  EXPECT_EQ(registry.EntityCount<size_t>(), 10);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
}

TEST(Registry_Tests, Create_MultipleEntitiesMultipleComponents_IncreaseEntityCount)
{
  Registry registry;

  auto entity1 = registry.Create<int, double, float>(10, 0.5, 0.2f);
  auto entity2 = registry.Create<int, float>(11, 0.8f);

  EXPECT_EQ(registry.EntityCount(), 2);
  EXPECT_EQ(registry.EntityCount<int>(), 2);
  EXPECT_EQ(registry.EntityCount<double>(), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 2);
  EXPECT_EQ((registry.EntityCount<int, float>()), 2);
  EXPECT_EQ((registry.EntityCount<int, double>()), 1);
  EXPECT_EQ((registry.EntityCount<int, double, float>()), 1);
  EXPECT_EQ((registry.EntityCount<bool>()), 0);
  EXPECT_EQ(entity1, 0u);
  EXPECT_EQ(entity2, 1u);
}

TEST(Registry_Tests, Destroy_Single_DecreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
}

TEST(Registry_Tests, Destroy_WithView_DecreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int>(10);

  registry.Destroy<int>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
}

TEST(Registry_Tests, Destroy_WithEmptyView_DecreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(Registry_Tests, Destroy_WithPartialView_DecreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<double>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(Registry_Tests, Destroy_WithExactView_DecreaseEntityCount)
{
  Registry registry;

  auto entity = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity);

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<double>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
}

TEST(Registry_Tests, Create_AfterDestroy_Restore)
{
  Registry registry;

  auto entity1 = registry.Create<int, double>(10, 0.5);

  registry.Destroy<int, double>(entity1);

  auto entity2 = registry.Create<int, double>(10, 0.5);

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<double>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 0);

  EXPECT_EQ(entity1, entity2);
}

TEST(Registry_Tests, DestroyAll_MultipleEntities_DecreaseEntityCount)
{
  Registry registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  EXPECT_EQ(registry.EntityCount(), 4);
  EXPECT_EQ(registry.EntityCount<int>(), 2);
  EXPECT_EQ((registry.EntityCount<double, int>()), 1);
  EXPECT_EQ((registry.EntityCount<double>()), 2);
  EXPECT_EQ((registry.EntityCount<float>()), 1);

  registry.DestroyAll();

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);
  EXPECT_EQ((registry.EntityCount<float>()), 0);
}

TEST(Registry_Tests, DestroyAll_WithView_DecreaseEntityCountCorrectly)
{
  Registry registry;

  registry.Create<int, double>(10, 0.5);
  registry.Create<int>(10);
  registry.Create<double>(0.5);
  registry.Create<float>(0.5);

  registry.DestroyAll<double>();

  EXPECT_EQ(registry.EntityCount(), 2);
  EXPECT_EQ(registry.EntityCount<int>(), 1);
  EXPECT_EQ(registry.EntityCount<float>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);

  registry.DestroyAll<int>();

  EXPECT_EQ(registry.EntityCount(), 1);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<float>(), 1);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);

  registry.DestroyAll<float>();

  EXPECT_EQ(registry.EntityCount(), 0);
  EXPECT_EQ(registry.EntityCount<int>(), 0);
  EXPECT_EQ(registry.EntityCount<float>(), 0);
  EXPECT_EQ((registry.EntityCount<double, int>()), 0);
  EXPECT_EQ((registry.EntityCount<double>()), 0);
}

TEST(Registry_Tests, Unpack_Single_Correct)
{
  Registry registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  EXPECT_EQ(10, registry.Unpack<int>(created_entity));
  EXPECT_EQ(0.5, registry.Unpack<double>(created_entity));
}

TEST(Registry_Tests, Unpack_Modify_ModifiedValue)
{
  Registry registry;

  auto created_entity = registry.Create<int, double>(10, 0.5);

  registry.Unpack<int>(created_entity) = 11;

  EXPECT_EQ(11, registry.Unpack<int>(created_entity));
  EXPECT_EQ(0.5, registry.Unpack<double>(created_entity));

  registry.Unpack<double>(created_entity) = 1.5;

  EXPECT_EQ(11, registry.Unpack<int>(created_entity));
  EXPECT_EQ(1.5, registry.Unpack<double>(created_entity));
}

TEST(Registry_Tests, HasComponents_Zero_False)
{
  Registry registry;

  auto created_entity = registry.Create<int>(10);

  EXPECT_FALSE(registry.HasComponents<double>(created_entity));
}

TEST(Registry_Tests, HasComponents_One_False)
{
  Registry registry;

  auto created_entity = registry.Create<int>(10);

  EXPECT_TRUE(registry.HasComponents<int>(created_entity));
}

TEST(Registry_Tests, HasComponents_Multiple_False)
{
  Registry registry;

  auto created_entity = registry.Create<int, double, float>(10, 0.5, 0.2f);

  EXPECT_TRUE(registry.HasComponents<int>(created_entity));
  EXPECT_TRUE(registry.HasComponents<double>(created_entity));
  EXPECT_TRUE(registry.HasComponents<float>(created_entity));
  EXPECT_FALSE(registry.HasComponents<bool>(created_entity));

  EXPECT_TRUE((registry.HasComponents<int, double>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<double, int>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<float, int>(created_entity)));
  EXPECT_TRUE((registry.HasComponents<double, float>(created_entity)));
  EXPECT_FALSE((registry.HasComponents<double, bool>(created_entity)));
  EXPECT_FALSE((registry.HasComponents<int, bool>(created_entity)));

  EXPECT_TRUE((registry.HasComponents<int, double, float>(created_entity)));
}

TEST(ViewIterator_Tests, PreIncrement_Empty_NoIterations)
{
  Registry registry;

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 0);
}

TEST(ViewIterator_Tests, PreIncrement_Single_OneIteration)
{
  Registry registry;

  registry.Create<int>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PreIncrement_Double_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<double>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(ViewIterator_Tests, PreIncrement_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, PreIncrement_WithMultipleEntities_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<double>(0);
  registry.Create<double>(0);

  auto view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(ViewIterator_Tests, PostIncrement_Single_OneIteration)
{
  Registry registry;

  registry.Create<int>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); it++)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PostIncrement_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = view.begin(); it != view.end(); it++)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, PreDecrement_Single_OneIteration)
{
  Registry registry;

  registry.Create<int>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = --view.end(); it != view.begin() - 1; --it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, PostDecrement_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  size_t iterations = 0;

  for (auto it = --view.end(); it != view.begin() - 1; it--)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 4);
}

TEST(ViewIterator_Tests, AddAssign_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin();
  it += 3;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, SubtractAssign_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin();
  it += 3;
  it -= 2;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 3);
}

TEST(ViewIterator_Tests, Add_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = view.begin() + 3;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 1);
}

TEST(ViewIterator_Tests, Subtract_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<float>(0);
  registry.Create<double>(0);
  registry.Create<long>(0);

  View view = registry.ViewFor();

  auto it = (view.begin() + 3) - 2;

  size_t iterations = 0;

  for (; it != view.end(); ++it)
  {
    iterations++;
  }

  EXPECT_EQ(iterations, 3);
}

TEST(SubView_Tests, Contains_SingleExact_True)
{
  Registry registry;

  auto entity = registry.Create<int>(0);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_TRUE(mono_view.Contains(entity));
}

TEST(SubView_Tests, Contains_SingleDestroyed_False)
{
  Registry registry;

  auto entity = registry.Create<int>(0);
  registry.Destroy(entity);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_FALSE(mono_view.Contains(entity));
}

TEST(SubView_Tests, EntityCount_Multiple_CorrectEntityCount)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_EQ(mono_view.Size(), 3);
}

TEST(SubView_Tests, Unpack_Single_CorrectValue)
{
  Registry registry;

  auto entity = registry.Create<int>(10);

  SubView mono_view = *registry.ViewFor<int>().begin();

  EXPECT_EQ(mono_view.Unpack<int>(entity), 10);
}

TEST(SubView_Tests, ForEach_None_NoIteration)
{
  Registry registry;

  registry.Destroy(registry.Create<int>(0));

  View<int> poly_view = registry.ViewFor<int>();

  SubView<int> mono_view = *poly_view.begin();

  size_t iterations = 0;

  mono_view.ForEach([&iterations](int) { iterations++; });

  EXPECT_EQ(iterations, 0);
}

TEST(SubView_Tests, ForEach_Single_OneIteration)
{
  Registry registry;

  auto entity = registry.Create<int>(0);

  View poly_view = registry.ViewFor<int>();

  SubView mono_view = *poly_view.begin();

  size_t iterations = 0;

  mono_view.ForEach(
    [&](int e)
    {
      EXPECT_EQ(entity, static_cast<Entity>(e));
      iterations++;
    });

  EXPECT_EQ(iterations, 1);
}

TEST(SubView_Tests, ForEach_Double_TwoIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);

  View poly_view = registry.ViewFor<int>();

  SubView mono_view = *poly_view.begin();

  size_t iterations = 0;

  mono_view.ForEach([&](int) { iterations++; });

  EXPECT_EQ(iterations, 2);
}

TEST(SubView_Tests, ForEach_Multiple_CorrectIterations)
{
  Registry registry;

  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);
  registry.Create<int>(0);

  registry.Create<double>(0);
  registry.Create<double>(0);

  SubView mono_view1 = *registry.ViewFor<int>().begin();
  SubView mono_view2 = *(registry.ViewFor<double>().begin());

  size_t iterations = 0;

  mono_view1.ForEach([&](int) { iterations++; });

  EXPECT_EQ(iterations, 6);

  iterations = 0;

  mono_view2.ForEach([&](double) { iterations++; });

  EXPECT_EQ(iterations, 2);
}

TEST(SubViewIterator_Tests, Dereference_Single_CorrectEntity)
{
  Registry registry;

  registry.Create<int>(99);

  SubView<int> mono_view = *registry.ViewFor<int>().begin();

  auto it = mono_view.begin();

  int value = *std::get<int*>(*mono_view.begin());

  EXPECT_EQ(value, 99);
}

TEST(SubViewIterator_Tests, PreIncrement_Double_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  int value1 = *std::get<int*>(*mono_view.begin());
  int value2 = *std::get<int*>(*(++mono_view.begin()));

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PreDecrement_Double_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto incremented = ++mono_view.begin();

  int value1 = *std::get<int*>(*incremented);
  int value2 = *std::get<int*>(*(--incremented));

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PostIncrement_Double_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto value = mono_view.begin();

  int value1 = *std::get<int*>(*value);

  value++;

  int value2 = *std::get<int*>(*(value));

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, PostDecrement_Double_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto value = ++mono_view.begin();

  int value1 = *std::get<int*>(*value);

  value--;

  int value2 = *std::get<int*>(*(value));

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, AddAssign_Multiple_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto value = mono_view.begin();

  int value1 = *std::get<int*>(*value);

  value += 2;

  int value2 = *std::get<int*>(*value);

  EXPECT_EQ(value1 + value2, 4);
}

TEST(SubViewIterator_Tests, SubstractAssign_Multiple_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto value = ++(++mono_view.begin());

  int value1 = *std::get<int*>(*value);

  value -= 2;

  int value2 = *std::get<int*>(*value);

  EXPECT_EQ(value1 + value2, 4);
}

TEST(SubViewIterator_Tests, Add_Multiple_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView mono_view = *registry.ViewFor<int>().begin();

  int value1 = *std::get<int*>(*(mono_view.begin() + 1));
  int value2 = *std::get<int*>(*(mono_view.begin() + 2));

  EXPECT_EQ(value1 + value2, 5);
}

TEST(SubViewIterator_Tests, Subtract_Multiple_CorrectEntities)
{
  Registry registry;

  registry.Create<int>(1);
  registry.Create<int>(2);
  registry.Create<int>(3);

  SubView mono_view = *registry.ViewFor<int>().begin();

  auto value = mono_view.begin() + 2;

  int value1 = *std::get<int*>(*(value - 1));
  int value2 = *std::get<int*>(*(value - 2));

  EXPECT_EQ(value1 + value2, 3);
}

TEST(SubViewIterator_Tests, CopyAssign_Single_CorrectEntity)
{
  Registry registry;

  registry.Create<int>(99);

  SubView mono_view = *registry.ViewFor<int>().begin();

  SubView<int>::Iterator it = mono_view.begin();

  ++it;

  it = (it - 1);

  int value = *std::get<int*>(*it);

  EXPECT_EQ(value, 99);
}

TEST(SubViewEntityIterator_Tests, Increment_Double_CorrectEntities)
{
  Registry registry;

  auto entity1 = registry.Create<int>(1);
  auto entity2 = registry.Create<int>(2);

  SubView mono_view = *registry.ViewFor<int>().begin();

  int iterations = 0;

  size_t last = 9999999;

  for (auto it = mono_view.ebegin(); it != mono_view.eend(); ++it)
  {
    auto entity = *it;

    EXPECT_NE(entity, last);

    EXPECT_TRUE(entity == entity1 || entity == entity2);

    last = entity;

    iterations++;
  }

  EXPECT_EQ(iterations, 2);
}

TEST(EntityApply_Tests, FreeFunction_Reduced_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    static void function(int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<int>(&TestClass::function, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, FreeFunction_Extended_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    static void function(Entity, int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<int>(&TestClass::function, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, Lambda_Reduced_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  auto function = [](int actual)
  {
    call_count++;

    EXPECT_EQ(value, actual);
  };

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<decltype(function), int>(function, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, Lambda_Extended_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  auto function = [](int actual)
  {
    call_count++;

    EXPECT_EQ(value, actual);
  };

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<decltype(function), int>(function, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, MemberFunction_Reduced_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    void function(int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  TestClass instance;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<TestClass, int>(&TestClass::function, &instance, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, MemberFunction_Extended_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    void function(Entity, int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  TestClass instance;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<TestClass, int>(&TestClass::function, &instance, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, ConstMemberFunction_Reduced_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    void function(int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  TestClass instance;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<TestClass, int>(&TestClass::function, &instance, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

TEST(EntityApply_Tests, ConstMemberFunction_Extended_CallsFunction)
{
  static size_t call_count = 0;

  static constexpr int value = 99;

  struct TestClass
  {
    void function(Entity, int actual)
    {
      call_count++;

      EXPECT_EQ(value, actual);
    }
  };

  TestClass instance;

  Registry registry;

  registry.Create(value);
  registry.Create(value);

  for (auto sub_view : registry.ViewFor<int>())
  {
    for (auto& data : sub_view)
    {
      EntityApply<TestClass, int>(&TestClass::function, &instance, data);
    }
  }

  EXPECT_EQ(call_count, 2);
}

} // namespace genebits::engine::tests