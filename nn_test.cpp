#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "nn.hpp"

int main()
{
    nn a(5, 2);
    a.init(std::vector<double>({0.2, 0.3}));
    std::cout << "initialize\n" << std::endl;
    a.forward();

    std::cout << a << std::endl;
}
