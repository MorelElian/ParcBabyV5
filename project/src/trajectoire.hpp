#pragma once
//#include "key_positions_structure.hpp"
#include "cgp/cgp.hpp"
#include <functional>
#include <iostream>
using namespace cgp;
class Trajectoire
{
public:
	int nPoints;
	const char* nomTrajectoire;
	buffer<vec3> key_positions;
	buffer<float> key_times;
	std::function<vec3(float, buffer<vec3>,buffer<float>)> fonctionInterpolation;
	vec3 positionKart(float t);
	Trajectoire() = default;
	Trajectoire(const char* _nomTrajectoire, buffer<vec3> _key_positions, buffer<float> _key_times, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolation, int _nPoints);
};