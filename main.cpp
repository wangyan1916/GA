#include <iostream>
#include "GA.hpp"
int main() {
    std::vector<std::vector<double>> boundaryList;
    boundaryList = {{0,100}, {0,10}};
    GA::GA1 ga(100, 50, 0.6,0.02);
    ga.init(boundaryList, GA::Fitness::boxMinus);
    GA::Individual rel = ga.generate();

    std::cout << rel.fitness<< std::endl;
    std::cout << rel.chromosome.at(0)<< std::endl;
    std::cout << rel.chromosome.at(1)<< std::endl;

    return 0;
}
