//
// Created by rocky on 2023/5/12.
//

#ifndef GA_RISK_HPP
#define GA_RISK_HPP


#include <vector>
#include "ship.hpp"
namespace Risk{
    /**
     * 创建的二维向量结构体，使用两个double初始化
     */
    struct vec2
    {
        double x, y;
        double norm;    // 存放向量的模
        /**
         * @brief 二维向量构造函数，使用两个double初始化
         * @param x_横坐标
         * @param y_ 纵坐标
         */
        vec2(double x_, double y_);
    };
    class CPA
    {
    private:

    public:
        /**
         * @brief 求我船和他船间的CPA值 换算后版本
         * @param ourShip_ 我船参数：MMSI,横，纵坐标（米），航向（弧度），航速（米/秒），船长，宽（米），艏向（弧度）
         * @param targetShip 二维double，一纬等通ourShip
         * @return CPA 二维double，两个一维向量，分别是我船对所有船的DCPA和TCPA
         */
        static std::vector<std::vector<double>> getCPA(std::vector<double> ourShip_, std::vector<std::vector<double>>targetShip_);
        /**
         * @brief 求我船和他船的CPA值，未换算版本
         * @param ourShip_  我船参数：MMSI,经度，纬度，航向（度），航速（节），船长，宽（米），艏向（度）
         * @param targetShip_ 二维double，一纬等通ourShip
         * @return CPA 二维double，两个一维向量，分别是我船对所有船的DCPA和TCPA
         */
        static std::vector<std::vector<double>> getCPAPlus(std::vector<double> ourShip_, std::vector<std::vector<double>>targetShip_);

        static std::vector<std::vector<double>> getCPAShip(Ship::Ship ourShip_, const Ship::shipSet& targetShips_);

    };
    class Test
    {
    public:
        static int testNoInput() {
            return 2;
        }
        static std::vector<std::vector<double>> getShip(std::vector<std::vector<double>> targetShip_)
        {
            return targetShip_;
        }

    };
}

#endif //GA_RISK_HPP
