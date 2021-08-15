

#include <iostream>

#include "engine/events/listener.h"
#include "engine/graphics/window.h"

namespace genebits::sandbox
{
using namespace genebits::engine;

struct TestWindowListener : public Listener<TestWindowListener,
                              WindowCloseEvent,
                              WindowFocusEvent,
                              WindowMaximizeEvent,
                              WindowIconifyEvent,
                              WindowResizeEvent>
{
  void listen(const WindowCloseEvent&)
  {
    std::cout << "window close event" << std::endl;
  }

  void listen(const WindowFocusEvent& event)
  {
    std::cout << "window focus event: " << ((event.state == WindowFocusEvent::FocusState::Gained) ? "Gained" : "Lost")
              << std::endl;
  }

  void listen(const WindowMaximizeEvent&)
  {
    std::cout << "window maximise event" << std::endl;
  }

  void listen(const WindowIconifyEvent&)
  {
    std::cout << "window iconify event" << std::endl;
  }

  void listen(const WindowResizeEvent& event)
  {
    std::cout << "window resize event: " << event.width << ", " << event.height << std::endl;
  }
};

int run()
{
  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  Window* window = CreateWindow("Hello world", 256, 256, hints);

  TestWindowListener listener {};

  while (!window->IsClosing())
  {
    window->WaitEvents(0.5);
    window->PollEvents();
  }

  delete window;

  return 0;
}
} // namespace genebits::sandbox

int main(int, char**)
{
  return genebits::sandbox::run();
}