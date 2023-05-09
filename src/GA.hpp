//
// Created by rocky on 2023/5/7.
//

#ifndef GA_GA_HPP
#define GA_GA_HPP


#include <vector>

namespace GA
{
    using uint = unsigned int;
    struct Individual
    {
        double (*func)(Individual);
        std::vector<double> chromosome;
        double fitness;
        explicit Individual(const std::vector<std::vector<double>>& boundaryList_, double (*func_)(Individual));
        void getFitness();
    };
    using population = std::vector<Individual>;
    class Fitness
    {
        static double add(double a, double b);
        static double minus(double a, double b);
    public:
        static double boxAdd(Individual individual_);
        static double boxMinus(Individual individual_);
    };
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
        double (*func)(Individual); // 用于存储适应度函数的指针
        population (*updatePop)(population& pop, size_t size_); // 用于存储更新种群的函数的指针
        static double calculateFitness(population& pop_);

    public:
        GA1(size_t popSize_ = 100, uint maxGen_ = 50, double crossRate_ = 0.3, double mutateRate_ = 0.3);
        void init(const std::vector<std::vector<double>>& boundaryList_, double(*func_)(Individual));   // 使用边界列表初始化种群
        Individual generate(population (updatePop_)(population& pop, size_t size));
        static population roulette(population& pop_, size_t size_) ;    //轮盘赌选择
    };
    class NSGA2 : GA1
    {
    public:
        Individual generate();
    };

}

#endif //GA_GA_HPP
