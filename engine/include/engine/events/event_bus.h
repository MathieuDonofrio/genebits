#ifndef GENEBITS_ENGINE_UTIL_EVENTBUS_H
#define GENEBITS_ENGINE_UTIL_EVENTBUS_H

#include <memory>
#include <mutex>

#include "engine/events/event_handler.h"
#include "engine/util/fast_vector.h"
#include "engine/util/meta.h"
#include "engine/util/type_map.h"

namespace genebits::engine
{
namespace
{
  ///
  /// Pool of event handlers.
  ///
  /// @tparam Event Event type to handle
  /// @tparam AllocatorImpl Allocator to use to allocated memory.
  ///
  template<typename Event, Allocator AllocatorImpl>
  class EventHandlerPool
  {
  public:
    ///
    /// Propagates the event by invoking every handler sequentially.
    ///
    /// @param event Event to propagate.
    ///
    void Propagate(const Event& event)
    {
      for (EventHandler<Event>& handler : handlers_)
      {
        handler.Invoke(event);
      }
    }

    ///
    /// Adds the event handler to the pool.
    ///
    /// @param handler Handler to add.
    ///
    void Add(EventHandler<Event> handler)
    {
      handlers_.PushBack(handler);
    }

    ///
    /// Removes the event handler from the pool.
    ///
    /// @param handler Handle to remove
    ///
    void Remove(EventHandler<Event> handler)
    {
      handlers_.Erase(std::ranges::find(handlers_, handler));
    }

    ///
    /// Returns the amount of handlers currently in the pool.
    ///
    /// @return Amount of stored handlers.
    ///
    [[nodiscard]] size_t Count() const
    {
      return handlers_.Size();
    }

  private:
    FastVector<EventHandler<Event>, AllocatorImpl> handlers_;
  };
} // namespace

///
/// Event bus is used as a container for event handlers of any event type.
///
/// You can subscribe event handlers to the bus and publish events.
///
/// @tparam TypeMapAllocator Allocator used to allocate memory for the type map.
/// @tparam HandlersAllocator Allocator used to allocator memory for the handlers.
///
template<Allocator TypeMapAllocator = Mallocator, Allocator HandlersAllocator = Mallocator>
class EventBus
{
public:
  ///
  /// Publishes the specified event to all event handlers for the event type.
  ///
  /// @tparam Event Type of event to publish.
  /// @param event Event to publish.
  ///
  template<typename Event>
  void Publish(const Event& event)
  {
    Assure<Event>()->Propagate(event);
  }

  ///
  /// Subscribes the event handler to the bus. Subscribed event handlers will
  /// receive all published event of the same type.
  ///
  /// @tparam Event Type of event.
  /// @param handler Handler of the event.
  ///
  template<typename Event>
  void Subscribe(EventHandler<Event> handler)
  {
    Assure<Event>()->Add(handler);
  }

  ///
  /// Unsubscribes the event handle to from the bus.
  ///
  /// The event handler will no longer receive published events in from this
  /// event bus.
  ///
  /// @tparam Event Type of event.
  /// @param handler Handler of the event.
  ///
  template<typename Event>
  void Unsubscribe(EventHandler<Event> handler)
  {
    Assure<Event>()->Remove(handler);
  }

  ///
  /// Returns the amount of event handlers currently subscribed to the templated
  /// event type.
  ///
  /// @tparam Event Type of event to get handlers count for.
  ///
  /// @return Amount of event handlers subscribed for the event type.
  ///
  template<typename Event>
  [[nodiscard]] size_t Count()
  {
    return Assure<Event>()->Count();
  }

private:
  ///
  /// Safely returns the event handler pool for the event type. Properly intializes the pool if
  /// it has never been accessed before.
  ///
  /// @tparam Event Event type to get handlers pool for.
  ///
  /// @return Event handler pool for the event type.
  ///
  template<typename Event>
  std::shared_ptr<EventHandlerPool<Event, HandlersAllocator>> Assure() noexcept
  {
    auto& pool = pools_.Assure<Event>();

    if (!pool)
    {
      pool = std::static_pointer_cast<void>(std::make_shared<EventHandlerPool<Event, HandlersAllocator>>());
    }

    return std::static_pointer_cast<EventHandlerPool<Event, HandlersAllocator>>(pool);
  }

private:
  TypeMap<std::shared_ptr<void>, TypeMapAllocator> pools_;
};

} // namespace genebits::engine

#endif
