//
// Created by rocky on 2023/5/10.
//

#include "Projection.hpp"
#include <cmath>
Projection::Projection(std::vector<double> refer_) {
    refer.push_back(refer_.at(0)/180* M_PI);
    refer.push_back(refer_.at(1)/180* M_PI);

    a = 6378137;
    b = 6356752;
    e = (a*a-b*b)/(a*a);
    getLatR();
    getLonR();

}

double Projection::getLonR() {
    lonR = (a*(1-e))/pow((sqrt(1-e*pow(sin(refer.at(1)), 2))), 3);
    return lonR;
}

double Projection::getLatR() {
    latR = a/(sqrt(1-e*pow(sin(refer.at(1)), 2)))*cos(refer.at(1));
    return latR;
}

std::vector<double> Projection::xy2ll(std::vector<double> xy) {
    std::vector<double> ll;
    ll.push_back(x2lon(xy.at(0)));
    ll.push_back(y2lat(xy.at(1)));
    return ll;
}

double Projection::x2lon(double x) {
    return (x/latR + refer.at(0))/M_PI*180;
}

double Projection::y2lat(double y) {
    return (y/lonR + refer.at(1))/M_PI*180;
}

double Projection::lon2x(double lon) {
    return latR*(lon/180*M_PI - refer.at(0));
}

double Projection::lat2y(double lat) {
    return lonR*(lat/180*M_PI - refer.at(1));
}

std::vector<double> Projection::ll2xy(std::vector<double> ll) {
    std::vector<double> xy;
    xy.push_back(lon2x(ll.at(0)));
    xy.push_back(lat2y(ll.at(1)));
    return xy;
}
