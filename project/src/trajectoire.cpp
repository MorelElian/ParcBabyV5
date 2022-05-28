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
	timer.t = timer.t_min + 0.02;
	timer2.t_min = key_times[0];
	timer2.t_max = key_times[N - 2];
	timer2.t = timer2.t_min;
	demarrage = false;
}
vec3 Trajectoire::positionKart(int deplacement)
{
	if ( deplacement == 2)
	{
		
		std::cout << timer.t << std::endl;
		std::cout << "hello" << std::endl;
		timer.update();
		std::cout << timer.t << std::endl;
	}
	else if (deplacement ==1)
	{
		//std::cout << " test" << std::endl;
		timer.start();
		timer.update();
		std::cout << timer.t << std::endl;
	}
	//std::cout << timer.t << std::endl;
		return fonctionInterpolation(timer.t, key_positions, key_times);

	
	//std::cout << timer.t << std::endl;
}
vec3 Trajectoire::positionKart()
{
	timer.update();
	return fonctionInterpolation(timer.t, key_positions,key_times);
}

vec3 Trajectoire::positionKartPrec()
{
	return fonctionInterpolation(timer.t - 0.001, key_positions,key_times);
}

vec3 Trajectoire::positionWagon(float delta_t)
{
	timer.update();
	return fonctionInterpolation(timer.t + delta_t, key_positions,key_times);
}

vec3 Trajectoire::positionWagonPrec(float delta_t)
{
	return fonctionInterpolation(timer.t + delta_t - 0.001, key_positions,key_times);
}