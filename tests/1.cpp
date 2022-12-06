#include <iostream>
#include <cassert>     /* assert */

#include "../include/channel.hpp"

int main(int argc, char const *argv[])
{
    Channel<float> ch(5);
    ch << 5;
    float t;
    ch >> t;
    assert(t == 5);
    ch << 6;
    std::cout << ch << std::endl;
    std::cout << t << std::endl;
    return 0;
}
