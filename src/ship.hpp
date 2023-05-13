//
// Created by rocky on 2023/5/10.
//

#ifndef GA_SHIP_HPP
#define GA_SHIP_HPP

#include <vector>
#include "Projection.hpp"
namespace Ship{
    class Ships;
}
namespace GA {
    struct Individual;
    using helpClass = Ship::Ships*;
}
namespace Ship {
    struct Ship {
    private:
        Projection* proj;
        double MMSI;    // 航向
        double lat;     // 经度 度
        double lon;     // 纬度 度
        double cor;     // 航向 度     （航迹向）
        double spe;     // 航速 节
        double len;     // 船长 米
        double wid;     // 船宽 米
        double phi;     // 艏向 度     （船头指向）

        double x;       // 相对于基准点横坐标
        double y;       // 相对于基准点纵坐标
        double radC;    // 航向 弧度
        double ms;      // 航速 米/秒
        double radP;    // 艏向 弧度

    public:
        explicit Ship(std::vector<double> shipInfo, Projection* proj_);
        double getLat() const;
        double getLon() const;
        double getCor() const;
        double getSpe() const;
        double getPhi() const;
        double getRadC() const;
        double getms() const;
        double getRadP() const;

        double getX();
        double getY();

        void setLat(double lat_);
        void setLon(double lon_);
        void setCor(double cor_);
        void setSpe(double spe_);
        void setPhi(double phi_);
        void setRadC(double radC_);
        void setms(double ms_);
        void setRadP(double radP_);
    };

    using shipSet = std::vector<Ship>;
    class Ships
    {
    private:
        std::vector<double> refer;

        Projection *proj;   // 用于坐标间转换
        std::vector<double> riskTH;

    public:
        /**
         * @brief 初始化Ships类
         * @param shipsInfo_ 船舶信息，包括我船和他船
         * @param riskTH_  阈值{DVOI TVOI} 单位分别为米和秒
         */
        explicit Ships(const std::vector<std::vector<double>>& shipsInfo_, std::vector<double> riskTH_);
        static void fitness(GA::Individual& individual_, GA::helpClass ships_);
        shipSet shipsInfo ;
        int zero = 0;

    };


}

using helpClass = Ship::Ships*;
#endif //GA_SHIP_HPP
