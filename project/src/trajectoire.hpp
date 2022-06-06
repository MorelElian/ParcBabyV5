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
	bool demarrage;
	buffer<vec3> key_positions;
	buffer<float> key_times;
	cgp::timer_interval timer;
	cgp::timer_interval timer2;
	std::function<vec3(float, buffer<vec3>,buffer<float>)> fonctionInterpolation;
	vec3 positionKart(int deplacement);
	vec3 positionKart();
	vec3 positionKart(float t);
	vec3 positionKartPrec();
	vec3 positionKartPrec(float t);
	vec3 positionWagon(float delta);
	vec3 positionWagonPrec(float delta);
	Trajectoire() = default;
	Trajectoire(const char* _nomTrajectoire, buffer<vec3> _key_positions, buffer<float> _key_times, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolation);
};