#include "utils.h"

double Distance(Coord first, Coord second) {
    double dLat = deg2rad(second.first - first.first);
    double dLon = deg2rad(second.second - first.second);
    double a = pow(sin(dLat / 2), 2) + cos(deg2rad(first.first)) * cos(deg2rad(second.first)) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return 6371 * c;
}

double deg2rad(double deg) {
    return deg * 3.1415926535897932 / 180.0;
}