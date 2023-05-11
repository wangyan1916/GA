//
// Created by rocky on 2023/5/10.
//

#include "ship.hpp"
#include <algorithm>
#include <cmath>

Ship::Ships::Ships(const std::vector<std::vector<double>>& shipsInfo_) {
    // 先计算基准
    std::vector<double> lat, lon;
    for (auto shipInfo_: shipsInfo_)
    {
        lat.push_back(shipInfo_.at(1));
        lon.push_back(shipInfo_.at(2));
    }
    auto oMax = std::max_element(lon.begin(), lon.end());
    auto aMax = std::max_element(lat.begin(), lat.end());
    auto oMin = std::min_element(lon.begin(), lon.end());
    auto aMin = std::min_element(lat.begin(), lat.end());

    refer.clear();
    refer.push_back((*oMax + *oMin)/2);
    refer.push_back((*aMax + *aMin)/2);

    // 创建投影类
    proj = new Projection(refer);

    // 初始化船舶
    for(auto shipInfo_: shipsInfo_)
    {
        shipsInfo.emplace_back(shipInfo_, proj);
    }

}

Ship::Ship::Ship(std::vector<double> shipInfo, Projection* proj_) {
    proj = proj_;
    MMSI = shipInfo.at(0);
    lon = shipInfo.at(1);
    lat = shipInfo.at(2);
    cor = shipInfo.at(3);
    spe = shipInfo.at(4);
    len = shipInfo.at(5);
    wid = shipInfo.at(6);
    phi = shipInfo.at(7);

    x = proj_->lon2x(lon);
    y = proj_->lat2y(lat);

    radC = cor/180*M_PI;
    ms = spe*1852/3600;
    radP = phi/180*M_PI;
}

double Ship::Ship::getLat() const {
    return lat;
}

double Ship::Ship::getLon() const {
    return lon;
}

double Ship::Ship::getCor() const {
    return cor;
}

double Ship::Ship::getSpe() const {
    return spe;
}

double Ship::Ship::getPhi() const {
    return phi;
}

double Ship::Ship::getRadC() const {
    return radC;
}

double Ship::Ship::getms() const {
    return ms;
}

double Ship::Ship::getRadP() const {
    return radP;
}

void Ship::Ship::setLat(double lat_) {
    lat = lat_;
    y = proj->lat2y(lat);
}

void Ship::Ship::setLon(double lon_) {
    lon = lon_;
    x = proj->lon2x(lon);
}

void Ship::Ship::setCor(double cor_) {
    cor = cor_;
    radC = cor/180*M_PI;
}

void Ship::Ship::setSpe(double spe_) {
    spe = spe_;
    ms = spe*1852/3600;
}

void Ship::Ship::setPhi(double phi_) {
    phi = phi_;
    radP = phi/180*M_PI;
}

void Ship::Ship::setRadC(double radC_) {
    radC = radC_;
    cor = radC/180*M_PI;
}

void Ship::Ship::setms(double ms_) {
    ms = ms_;
    spe = ms/1852*3600;
}

void Ship::Ship::setRadP(double radP_) {
    radP = radP_;
    phi = radP/M_PI*180;
}

