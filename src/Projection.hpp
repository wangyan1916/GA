//
// Created by rocky on 2023/5/10.
//

#ifndef GA_PROJECTION_HPP
#define GA_PROJECTION_HPP

#include <vector>
class Projection {
private:
    std::vector<double> refer;  // 参考点
    double a;                   // 长半轴长度，米
    double b;                   // 短半轴长度，米
    double e; 					// 第一偏心率，半轴长改变时下面变量重新计算
    double lonR;				// 子午圈曲率半径——等同于经线圈半径，米
    double latR; 			    // 纬线圈半径，米

    double getLonR();           // 获得经圈半径
    double getLatR();           // 获得纬圈半径

public:
    explicit Projection(std::vector<double> refer_);
    // 坐标转经纬度
    double x2lon(double x);
    double y2lat(double y);

    // 经纬度转坐标
    double lon2x(double lon);
    double lat2y(double lat);

    // 整个坐标计算
    std::vector<double> xy2ll(std::vector<double> xy);  // 坐标转经纬度
    std::vector<double> ll2xy(std::vector<double> ll);  // 经纬度转坐标



};


#endif //GA_PROJECTION_HPP
