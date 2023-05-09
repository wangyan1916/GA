//
// Created by rocky on 2023/5/7.
//

#include <random>
#include "GA.hpp"

GA::Individual::Individual(const std::vector<std::vector<double>> &boundaryList_, double (*func_)(Individual)) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis(0.0, 1.0);
    for (auto bound: boundaryList_)
        chromosome.push_back(bound.at(0) + (bound.at(1) - bound.at(0))*dis(gen));
        fitness = 0;

    func = func_;
}




void GA::Individual::getFitness() {
    //fitness = chromosome.at(0) + chromosome.at(1);
    fitness = func(*this);

}

GA::GA1::GA1(size_t popSize_, GA::uint maxGen_, double crossRate_, double mutateRate_)
    :popSize(popSize_), maxGen(maxGen_), crossRate(crossRate_), mutateRate(mutateRate_)
    {
}

void GA::GA1::init(const std::vector<std::vector<double>>& boundaryList_, double (*func_)(Individual)) {
   boundaryList = boundaryList_;
   uint i = 0;
   while (i < popSize)
   {
       pop.emplace_back(boundaryList_, func_);
       i++;
   }

}

GA::population GA::GA1::crossOver() {
    // 思路：从pop的副本中每次随机抽取一对个体，
    // 根据交叉概率决定是否进行交叉
    // 交叉后个体存入子代

    population subC;
    population popCopy = pop;
    uint pt;
    size_t currentSize;
    size_t chromosomeSize = pop.begin()->chromosome.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    while(!popCopy.empty())
    {
        currentSize =  popCopy.size();
        pt = ceil(--currentSize*dis(gen));
        Individual parent1 = popCopy.at(pt);
        popCopy.erase(popCopy.begin() + pt);
        pt = ceil(--currentSize*dis(gen));
        Individual parent2 = popCopy.at(pt);
        popCopy.erase(popCopy.begin() + pt);

        if (dis(gen) < crossRate)
        {
            double tmp;
            for (uint i = 0; i < chromosomeSize; ++i) {
                if (dis(gen) < crossRate)
                {
                    tmp = parent1.chromosome.at(i);
                    parent2.chromosome.at(i) = parent1.chromosome.at(i);
                    parent1.chromosome.at(i) = tmp;
                }
            }
            subC.emplace_back(parent1);
            subC.emplace_back(parent2);
        }
    }
    return subC;
}

GA::population GA::GA1::mutate() {
    // 思路：根据边界列表创建一个离散区间列表
    // 对每个个体通过变异概率判断是否变异
    // 对个体中的每个基因根据变异概率判断是否变异
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t genSize = boundaryList.size();
    std::vector<std::uniform_real_distribution<double>> distList;
    std::uniform_real_distribution<double> dist(0,1);
    for (uint i = 0; i < genSize; ++i) {
        std::uniform_real_distribution<double> tmp(boundaryList.at(i).at(0), boundaryList.at(i).at(1));
        distList.emplace_back(tmp);
    }

    population subM;
    for (uint j = 0; j < popSize; ++j) {
        if (dist(gen) < mutateRate)
        {
            Individual tmpIndividual = pop.at(j);
            for (uint k = 0; k < genSize; ++k) {
                if (dist(gen) < mutateRate)
                {
                    tmpIndividual.chromosome.at(k) = distList.at(k)(gen);
                }
            }
            subM.emplace_back(tmpIndividual);
        }
    }
    return subM;
}


GA::Individual GA::GA1::generate() {
    uint i = 0;
    population subOfC;
    population subOfM;
    while(i++ < maxGen)
    {
        subOfC = crossOver();
        subOfM = mutate();
        population newPop;
        std::copy(pop.begin(), pop.end(), std::back_inserter(newPop));
        std::copy(subOfC.begin(), subOfC.end(), std::back_inserter(newPop));
        std::copy(subOfM.begin(), subOfM.end(), std::back_inserter(newPop));
        pop = roulette(newPop);
    }
    Individual best = *pop.begin();
    for (const auto& ind: pop)
    {
        if (ind.fitness > best.fitness)
            best = ind;
    }
    return best;
}

GA::population GA::GA1::roulette(GA::population& pop_) const {
    // 首先计算所有个体的适应度函数 并求和
    double fitnessSum = calculateFitness(pop_);
    std::vector<double> probabilities;
    // 归一化概率分布
    for(const auto& ind: pop_)
    {
        probabilities.push_back(ind.fitness/fitnessSum);
    }
    // 构建轮盘
    std::vector<double> wheel;
    double sum = 0.0;
    for (auto p : probabilities)
    {
        sum += p;
        wheel.push_back(sum);
    }
    // 随机选择
    population subPop;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution dist(0.0, 1.0);
    for (uint i = 0; i < popSize; ++i) {
        double r = dist(gen);
        auto it = std::lower_bound(wheel.begin(), wheel.end(), r);
        subPop.emplace_back(pop_.at(std::distance(wheel.begin(), it)));
    }
    return subPop;
}

double GA::GA1::calculateFitness(population& pop_) {
    double sum = 0.0;
    for (auto &ind: pop_)
    {

        ind.getFitness();
        sum += ind.fitness;
    }
    return sum;
}


double GA::Fitness::add(double a, double b) {

    return a + b;
}

double GA::Fitness::boxAdd(GA::Individual individual_) {
    return add(individual_.chromosome.at(0), individual_.chromosome.at(1));
}

double GA::Fitness::minus(double a, double b) {
    return a - b;
}

double GA::Fitness::boxMinus(GA::Individual individual_) {
    return(minus(individual_.chromosome.at(0), individual_.chromosome.at(1)));
}
