#include <iostream>
#include "GA.hpp"
#include "ship.hpp"
#include <chrono>
#include "Risk.hpp"
int main() {
    auto start = std::chrono::high_resolution_clock ::now();
    /**
     * 数据输入部分
     */
    std::vector<std::vector<double>> boundaryList, shipsList;       // 用于初始化种群的参数列表，分别是边界和船舶信息
    std::vector<std::vector<double>> cpaResult;                 // 存放CPA的结果
    std::vector<double> riskTH = {200, 60};     // 设置DCPA和TCPA的阈值
    shipsList = {                               // 船舶数据 [0]MMSI [1]经度（度） [2]纬度（度） [3]航向（度） [4]航速（节） [5]船长（米） [6]船宽（米） [7]艏向（度）
            {413764576, 116.280456, 29.795691, 37.5, 6.5, 94, 15, 37.0},
            {413775488, 116.299854, 29.815010, 246.5, 3.2, 68, 11, 246.0},
            {413777472, 116.299785, 29.808566, 242.0, 4.4, 72.0, 13.0, 242.0},
            {413825280, 116.285837, 29.800927, 35.9, 6.7, 120, 21, 35.0}
    };
    /**
     * 危险判断部分
     */
    cpaResult = Risk::CPA::getCPAPlus(shipsList.at(0), std::vector<std::vector<double>> (shipsList.begin()+1, shipsList.end()));    // 调用静态函数，计算危险度
    // 判断是否有危险,并获得最短碰撞时间
    double minT = riskTH.at(1) ;
    for (size_t i = 0; i < cpaResult.at(0).size(); ++i) {
        if (cpaResult.at(0).at(i) < riskTH.at(0) && cpaResult.at(1).at(i) < minT)
        {
            minT = cpaResult.at(1).at(i);
        }
    }
    if (minT < riskTH.at(1)) {
        /**
         * 遗传算法生成决策部分
         */
        boundaryList = {{0, minT},
                        {20, 40},
                        {0, 10*minT},
                        {20, 90}};
        auto ships = new Ship::Ships(shipsList, riskTH);           // 用于特定问题适应度函数的类，这里是船舶避碰规划问题，携带特定场景数据
        auto help = new GA::Help(ships);                                // 用于传递类指针的的辅助类，替换不同适应度函数计算的接口
        help->setFitFun(Ship::Ships::fitness);                              // 设置适应度函数计算

        GA::GA1 ga(100, 100, 0.5, 0.6);      // 实例化ga
        ga.init(boundaryList, help);                              // 初始化种群
        ga.setOutput(GA::GA1::outPutBestFitness);                           // 设置最终解的输出方式
        GA::Individual rel = ga.generate(GA::GA1::roulette);            // 遗传算法操作 以及种群更新规则

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> diff = end - start;
        std::cout << diff.count() << std::endl;
        std::cout << rel.rank << std::endl;
        std::cout << rel.chromosome.at(0) << std::endl;
        std::cout << rel.chromosome.at(1) << std::endl;
        std::cout << "have risk" << std::endl;
    } else {
        /**
         * 无危险时的输出
         */
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << diff.count() << std::endl;
        std::cout << "DCPA:" << std::endl;
        for (auto dcpa: cpaResult.at(0)) {
            std::cout << dcpa << std::endl;
        }
        std::cout << "TCPA" << std::endl;
        for (auto tcpa: cpaResult.at(1)) {
            std::cout << tcpa << std::endl;
        }
        std::cout << "no risk" << std::endl;
    }
    return 0; }