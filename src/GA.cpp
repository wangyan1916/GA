//
// Created by rocky on 2023/5/7.
//

#include <random>
#include "GA.hpp"

GA::Individual::Individual(const std::vector<std::vector<double>> &boundaryList_, Help* help_) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis(0.0, 1.0);
    for (auto bound: boundaryList_)
        chromosome.push_back(bound.at(0) + (bound.at(1) - bound.at(0))*dis(gen));

    rank = 0;
    fitness.clear();
    help = help_;

}
void GA::Individual::getFitness() {
    help->fitness(*this, help->ships);

}

GA::GA1::GA1(size_t popSize_, GA::uint maxGen_, double crossRate_, double mutateRate_)
    :popSize(popSize_), maxGen(maxGen_), crossRate(crossRate_), mutateRate(mutateRate_)
    {
    outPut = outPutBestFitness;
}

void GA::GA1::init(const std::vector<std::vector<double>>& boundaryList_, Help* help_) {
   boundaryList = boundaryList_;
   uint i = 0;
   while (i < popSize)
   {
       pop.emplace_back(boundaryList_, help_);
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


GA::Individual GA::GA1::generate(population (updatePop_)(population& pop, size_t size)) {
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
        pop = updatePop_(newPop, popSize);  // 更新子代
    }

    return outPut(pop);
}

GA::population GA::GA1::roulette(GA::population& pop_, size_t size)  {
    // 首先计算所有个体的适应度函数 并求和
    double fitnessSum = calculateFitness(pop_);
    std::vector<double> probabilities;
    // 归一化概率分布
    for(const auto& ind: pop_)
    {
        probabilities.push_back(ind.rank/fitnessSum);
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
    for (uint i = 0; i < size; ++i) {
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
        ind.rank = std::accumulate(ind.fitness.begin(), ind.fitness.end(), 0.0);
        sum += ind.rank;
    }
    return sum;
}

GA::Individual GA::GA1::outPutBestFitness(GA::population &pop_) {
    Individual best = *pop_.begin();
    for (const auto& ind: pop_)
    {
        if (ind.rank > best.rank)
            best = ind;
    }
    return best;

}

void GA::GA1::setOutput(GA::Individual (*outPut_)(population &)) {
    outPut = outPut_;

}

GA::Individual GA::NSGA2::generate() {
    return pop.back();
}

GA::Help::Help(GA::helpClass ships_) {
    ships = ships_;
}

void GA::Help::demoAdd(GA::Individual &individual_, helpClass ships_) {

    ships_->shipsInfo.at(0).getLat();
    individual_.fitness = {individual_.chromosome.at(0), individual_.chromosome.at(1)};


}

void GA::Help::setFitFun(void (*fitness_)(Individual &, helpClass)) {
    fitness = fitness_;

}
