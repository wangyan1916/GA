//
// Created by rocky on 2023/5/12.
//

#include "Risk.hpp"
#include "Projection.hpp"
#include <algorithm>
#include <cmath>

std::vector<std::vector<double>>
Risk::CPA::getCPA(std::vector<double> ourShip_, std::vector<std::vector<double>> targetShip_) {

    std::vector<std::vector<double>> result;
    std::vector<double>DCPA;
    std::vector<double>TCPA;
    for (auto ts: targetShip_) {

        // 先计算相对速度方向
        vec2 relaVelocity(ourShip_.at(4) * cos(ourShip_.at(3)) - ts.at(4) * cos(ts.at(3)),
                          ourShip_.at(4) * sin(ourShip_.at(3)) - ts.at(4) * sin(ts.at(3)));
        if (fabs(relaVelocity.x )< 0.0001)
        {
            // 相对航向正南正北
            DCPA.push_back(fabs(ourShip_.at(1) - ts.at(1)));
            TCPA.push_back(fabs(ourShip_.at(2) - ts.at(2))/relaVelocity.norm);

        }else if(fabs(relaVelocity.y )< 0.0001)
        {
            // 相对航向正东正西
            DCPA.push_back(fabs((ourShip_.at(2)) - ts.at(2)));
            TCPA.push_back(fabs(ourShip_.at(1) - ts.at(1))/relaVelocity.norm);
        }else
        {
            // 一般情况下，计算我船航行至最近点的坐标
            double k = tan(relaVelocity.y/relaVelocity.x);
            double kn = -1.0/k;
            double cpx = ts.at(2) - ourShip_.at(2) + k*ourShip_.at(1) - kn*ts.at(1);
            double cpy = k*(cpx - ourShip_.at(1)) + ourShip_.at(2);

            DCPA.push_back(sqrt(pow((cpx - ourShip_.at(1)), 2) + pow((cpy - ourShip_.at(2)), 2)));
            TCPA.push_back(sqrt(pow((cpx - ts.at(1)), 2) + pow((cpy - ts.at(2)), 2))/relaVelocity.norm);
        }
    }
    result.push_back(DCPA);
    result.push_back(TCPA);
    return result;
}

std::vector<std::vector<double>>
Risk::CPA::getCPAPlus(std::vector<double> ourShip_, std::vector<std::vector<double>> targetShip_) {
    std::vector<std::vector<double>> shipsInfo (targetShip_);
    shipsInfo.insert(shipsInfo.begin(), ourShip_);

    // 先计算基准
    std::vector<double> lat, lon;
    for (auto shipInfo_: shipsInfo)
    {
        lat.push_back(shipInfo_.at(1));
        lon.push_back(shipInfo_.at(2));
    }
    auto oMax = std::max_element(lon.begin(), lon.end());
    auto aMax = std::max_element(lat.begin(), lat.end());
    auto oMin = std::min_element(lon.begin(), lon.end());
    auto aMin = std::min_element(lat.begin(), lat.end());
    std::vector<double> refer;
    refer.clear();
    refer.push_back((*oMax + *oMin)/2);
    refer.push_back((*aMax + *aMin)/2);
    // 创建投影类
    Projection proj =  Projection(refer);

    for (auto shipInfo:shipsInfo)
    {
        shipInfo[1] = proj.lon2x(shipInfo.at(1));
        shipInfo[2] = proj.lat2y(shipInfo.at(2));
        shipInfo[3] = shipInfo.at(3)/180*M_PI;
        shipInfo[4] = shipInfo.at(4)*1852/3600;
        shipInfo[7] = shipInfo.at(7)/180*M_PI;
    }
    ourShip_ = shipsInfo.at(0);
    shipsInfo.erase(shipsInfo.begin());
    targetShip_ = shipsInfo;
    return getCPA(ourShip_, targetShip_);
}

std::vector<std::vector<double>> Risk::CPA::getCPAShip(Ship::Ship ourShip_, const Ship::shipSet& targetShips_) {
    std::vector<std::vector<double>> result;
    std::vector<double>DCPA;
    std::vector<double>TCPA;
    for (auto ts: targetShips_) {

        // 先计算相对速度方向
        vec2 relaVelocity(ourShip_.getms() * cos(ourShip_.getRadC()) - ts.getms() * cos(ts.getRadC()),
                          ourShip_.getms() * sin(ourShip_.getRadC()) - ts.getms() * sin(ts.getRadC()));
        if (fabs(relaVelocity.x )< 0.0001)
        {
            // 相对航向正南正北
            DCPA.push_back(fabs(ourShip_.getX() - ts.getX()));
            TCPA.push_back(fabs(ourShip_.getY() - ts.getY())/relaVelocity.norm);

        }else if(fabs(relaVelocity.y )< 0.0001)
        {
            // 相对航向正东正西
            DCPA.push_back(fabs((ourShip_.getY()) - ts.getY()));
            TCPA.push_back(fabs(ourShip_.getX() - ts.getX())/relaVelocity.norm);
        }else
        {
            // 一般情况下，计算我船航行至最近点的坐标
            double k = tan(relaVelocity.y/relaVelocity.x);
            double kn = -1.0/k;
            double cpx = ts.getY() - ourShip_.getY() + k*ourShip_.getX() - kn*ts.getX();
            double cpy = k*(cpx - ourShip_.getX()) + ourShip_.getY();

            DCPA.push_back(sqrt(pow((cpx - ourShip_.getX()), 2) + pow((cpy - ourShip_.getY()), 2)));
            TCPA.push_back(sqrt(pow((cpx - ts.getX()), 2) + pow((cpy - ts.getY()), 2))/relaVelocity.norm);
        }
    }
    result.push_back(DCPA);
    result.push_back(TCPA);
    return result;
}

Risk::vec2::vec2(double x_, double y_) {
    x = x_;
    y = y_;

    norm = sqrt(pow(x,2) + pow(y,2));

}
