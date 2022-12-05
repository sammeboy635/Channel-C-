#include <iostream>
#include "../include/channel.hpp"

int main(int argc, char const *argv[])
{
    Channel<float> ch(5);
    ch << 5;
    float t;
    ch >> t;
    ch << 6;
    std::cout << ch << std::endl;
    std::cout << t << std::endl;
    return 0;
}
