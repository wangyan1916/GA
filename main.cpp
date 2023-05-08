#include <iostream>
#include <random>
#include "GA.hpp"
int main() {
    std::vector<std::vector<double>> boundaryList;
    boundaryList = {{0,10}, {0,100}};
    GA::GA1 ga;
    ga.init(boundaryList);
    GA::Individual rel = ga.generate();

    std::cout << rel.fitness<< std::endl;
    std::cout << rel.chromosome.at(0)<< std::endl;
    std::cout << rel.chromosome.at(1)<< std::endl;

    return 0;
}
