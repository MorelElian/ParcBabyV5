#include "trajectoire.hpp"

Trajectoire::Trajectoire(const char* _nomTrajectoire, buffer<vec3> _key_positions, buffer<float> _key_times, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolation, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolationDerivee)
{
	nomTrajectoire = _nomTrajectoire;
	key_positions = _key_positions;
	key_times = _key_times;
	fonctionInterpolation = _fonctionInterpolation;
	fonctionInterpolationDerivee = _fonctionInterpolationDerivee;
}
vec3 Trajectoire::positionKart(float t)
{
	return fonctionInterpolation(t, key_positions,key_times);
}
vec3 Trajectoire::deriveePositionKart(float t)
{
	return fonctionInterpolationDerivee(t, key_positions,key_times);
}