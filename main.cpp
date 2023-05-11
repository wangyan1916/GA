#include <iostream>
#include "GA.hpp"
#include "ship.hpp"
#include <chrono>
int main() {
    auto start = std::chrono::high_resolution_clock ::now();
    std::vector<std::vector<double>> boundaryList, shipsList;       // 用于初始化种群的参数列表，分别是边界和船舶信息
    boundaryList = {{0,10}, {0,10}};
    shipsList = {
            {413764576, 116.280456, 29.795691, 37.5, 6.5, 94, 15, 37.0},
            {413775488, 116.299854, 29.815010, 246.5, 3.2, 68, 11, 246.0},
            {413777472, 116.299785, 29.808566, 242.0, 4.4, 72.0, 13.0, 242.0},
            {413825280, 116.285837, 29.800927, 35.9, 6.7, 120, 21, 35.0}
    };
    auto ships = new Ship::Ships(shipsList);                // 用于传递的船舶信息类
    auto  help = new GA::Help(ships);                          // 用于特定场景计算适应度函数的辅助类
    help->setFitFun(GA::Help::demoAdd);                       // 设置适应度函数计算

    GA::GA1 ga(1000, 1000, 0.5,0.6);      // 实例化ga
    ga.init(boundaryList, help);                            // 初始化种群
    ga.setOutput(GA::GA1::outPutBestFitness);                          // 设置最终解的输出方式
    GA::Individual rel = ga.generate(GA::GA1::roulette);            // 遗传算法操作 以及种群更新规则

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cout << diff.count() << std::endl;
//    for (auto fit: rel.fitness)
//    {
//        std::cout << fit << std::endl;
//    }
    std::cout << rel.rank<< std::endl;
    std::cout << rel.chromosome.at(0)<< std::endl;
    std::cout << rel.chromosome.at(1)<< std::endl;


    return 0;
}
