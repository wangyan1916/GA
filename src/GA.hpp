//
// Created by rocky on 2023/5/7.
//

#ifndef GA_GA_HPP
#define GA_GA_HPP


#include <vector>
#include "ship.hpp"
namespace GA
{
    // 预先声明个体类，后面有正式的声明和实现
    struct Individual;
    using uint = unsigned int;
    // 辅助函数，用于帮助GA算法计算适应度函数
    // 这里初始化引入的类可以根据实际情况做调整
    using helpClass = Ship::Ships*;
    class Help
    {
    public:
        helpClass theHelpClass;
        /**
         * @brief Help构建函数
         * @param theHelpClass 通过在GA中修改helpClass指向的类型，使用help调用不同类型的类
         */
        explicit Help(helpClass theHelpClass);
        static void demoAdd(Individual& individual_, helpClass helpClass_);
        void (*fitness)(Individual&, GA::helpClass);
        void setFitFun(void fitness_(Individual&, helpClass ships_));

    };
    // 个体类
    struct Individual
    {
        std::vector<double> chromosome;
        std::vector<double> fitness;
        double rank;
        explicit Individual(const std::vector<std::vector<double>>& boundaryList_, Help* help);
        Help *help;
        void getFitness();
    };
    using population = std::vector<Individual>;


    // 基础GA框架
    class GA1
    {
    private:
    protected:
        population pop;
        size_t popSize;
        uint maxGen;
        double crossRate;
        double mutateRate;
        std::vector<std::vector<double>> boundaryList;
        population crossOver();
        population mutate();
        Individual (*outPut)(population& pop);  // 根据最终结果输出最佳值
        static double calculateFitness(population& pop_);

    public:
        GA1(size_t popSize_ = 100, uint maxGen_ = 50, double crossRate_ = 0.3, double mutateRate_ = 0.3);
        void init(const std::vector<std::vector<double>>& boundaryList_, Help* help_);   // 使用边界列表初始化种群
        void setOutput(Individual (*outPut_)(population& pop));
        Individual generate(population (updatePop_)(population& pop, size_t size));
        // 示例方法
            // 示例选择方法——轮盘赌
        static population roulette(population& pop_, size_t size_) ;    //轮盘赌选择
            // 示例输出最佳值方法——适应度函数最佳
        static Individual outPutBestFitness(population& pop_);
    };

}

#endif //GA_GA_HPP
