#include <vectorpp/vectorpp.h>

#include <iostream>

int main(int, char*[])
{
    auto sum = vectorpp::add(1, 1);
    std::cout << sum << std::endl;
    return 0;
}
