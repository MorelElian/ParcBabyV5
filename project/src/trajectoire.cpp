#include "trajectoire.hpp"

Trajectoire::Trajectoire(const char* _nomTrajectoire, buffer<vec3> _key_positions, buffer<float> _key_times, std::function<vec3(float, buffer<vec3>, buffer<float>)> _fonctionInterpolation)
{
	nPoints =_key_times.size();
	nomTrajectoire = _nomTrajectoire;
	key_positions = _key_positions;
	key_times = _key_times;
	fonctionInterpolation = _fonctionInterpolation;
	int N = key_times.size();
	
	timer2.t_min = key_times[0];
	timer2.t_max = key_times[N - 2];
	timer2.t = timer2.t_min;
	demarrage = false;
	timer.t_min = key_times[1];
	timer.t_max = key_times[key_times.size() - 2];
	timer.t = timer.t_min + 0.02;
	timer.start();
}
vec3 Trajectoire::positionKart(int deplacement)
{
	if(deplacement ==1)
	{
		timer.update();
	}
	
		return fonctionInterpolation(timer.t, key_positions, key_times);

	

}
vec3 Trajectoire::positionKart(float t)
{
	return fonctionInterpolation(t, key_positions, key_times);
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
vec3 Trajectoire::positionKartPrec(float t)
{
	return fonctionInterpolation(t - 0.001, key_positions, key_times);
}

vec3 Trajectoire::positionWagon(float delta)
{
	timer.update();
	float T = timer.t + delta;
	if (T > timer.t_max){
		T = T - (timer.t_max-timer.t_min);
	}
	return fonctionInterpolation(T, key_positions,key_times);
}

vec3 Trajectoire::positionWagonPrec(float delta)
{	
	float T = timer.t + delta - 0.0001;
	if (T > timer.t_max){
		T = T - (timer.t_max-timer.t_min);
	}
	return fonctionInterpolation(T, key_positions,key_times);
}
