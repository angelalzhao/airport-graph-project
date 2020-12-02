#pragma once

#include <utility>
#include <cmath>

// Latitude, Longitude
typedef std::pair<double, double> Coord;

double Distance(Coord first, Coord second);
double deg2rad(double deg);