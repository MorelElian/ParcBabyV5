#include "trajectoire.hpp"

Trajectoire::Trajectoire(const char* _nomTrajectoire, buffer<vec3> _key_positions, buffer<float> _key_times, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolation)
{
	nPoints =_key_times.size();
	nomTrajectoire = _nomTrajectoire;
	key_positions = _key_positions;
	key_times = _key_times;
	fonctionInterpolation = _fonctionInterpolation;
	int N = key_times.size();
	timer.t_min = key_times[1];
	timer.t_max = key_times[N - 2];
	timer.t = timer.t_min;
}
vec3 Trajectoire::positionKart()
{
	timer.update();
	//std::cout << timer.t << std::endl;
	return fonctionInterpolation(timer.t, key_positions,key_times);
}vec3 Trajectoire::positionKartPrec()
{
	return fonctionInterpolation(timer.t - 0.001, key_positions,key_times);
}