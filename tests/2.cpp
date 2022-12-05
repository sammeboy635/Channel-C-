#include "../include/channel.hpp"
#define XDEBUG
#include "../../xdebug/include/xdebug.hpp"
void TestChannel() {
  Channel<int> channel(10);
  std::thread t1([&channel] {
    for (int i = 0; i < 100; i++) {
      channel << i;
    }
  });
  std::thread t2([&channel] {
    for (int i = 0; i < 100; i++) {
      int element;
      channel >> element;
      DEBUGT("Element: ",element);
    }
  });
  t1.join();
  t2.join();
}
int main(int argc, char const *argv[])
{
    TestChannel();
    return 0;
}
