#include "trajectoireKart.hpp"
#include "racetrack.hpp"
float extern disc_radius;
float extern turn_radius;
float extern racetrack_length;
float extern racetrack_thickness;
using namespace cgp;
buffer<vec3> trajLuigi()
{
	buffer<vec3> key_positions_luigi_2 = {
			{disc_radius / 2.0 + 0,0,0},
			{disc_radius / 2.0f,0.8 * racetrack_length / 2.0f,0},
			{disc_radius / 2.0f,racetrack_length ,0},
			{0,racetrack_length + disc_radius / 2.0f ,0},
			{-disc_radius / 2.0f ,racetrack_length ,0},
			{-disc_radius / 2.0f,racetrack_length / 2.0f,0},
			{-disc_radius / 2.0f ,0,0}, {0,-disc_radius / 2.0f,0},
			{disc_radius / 2.0 + 0,0,0},
			{disc_radius / 2.0f ,0.8 * racetrack_length / 2.0f,0},
			{disc_radius / 2.0f,racetrack_length ,0} };
	return key_positions_luigi_2;
}
buffer<vec3> trajWaluigi()
{
	buffer<vec3> key_positions = { { 3.0 * disc_radius / 4.0 ,-disc_radius / 2.0,0 },
		{ disc_radius * 2.0 / 3.0f,racetrack_length / 3.0f,0},
		{disc_radius * 2.0 / 3.0f, 2.0 * racetrack_length / 3.0 ,0},
		{2.0 * disc_radius / 3.0, racetrack_length + disc_radius / 2.0f ,0},
		{ 0 ,racetrack_length + 3.0 * disc_radius / 4.0f ,0 },
		{ -2.0 * disc_radius / 3.0f ,racetrack_length + disc_radius / 3.0f ,0 },
		{ -3.0 * disc_radius / 4.0f ,racetrack_length / 3.0,0 },
		{ -2.0 * disc_radius / 3.0,-disc_radius / 3.0f,0 },
		{ 0 ,-3.0 / 4.0 * disc_radius,0 },
		{ 3.0 * disc_radius / 4.0 ,-disc_radius / 2.0,0 },
		{ 2.0 * disc_radius / 3.0f ,racetrack_length / 3.0f,0 },
		{disc_radius * 2.0 / 3.0f, 2.0 * racetrack_length / 3.0 ,0} };
	return key_positions;
}
buffer<vec3> trajMario()
{
	buffer<vec3> key_positions_mario2 = {
		{ 1.2 * disc_radius / 4.0 ,0,0 },
		{1.4 *  disc_radius / 3.0f,racetrack_length / 4.0f,0},
		{turn_radius * 2.5, 4.0 * racetrack_length / 5.0 ,0},
		{ -turn_radius * 0.6, racetrack_length + disc_radius / 2.0 ,0} ,
		{-4.0 * disc_radius / 5.0, racetrack_length + disc_radius / 4.0  ,0},
		{ -1.0 / 3.0 * disc_radius ,0.5 * racetrack_length ,0},
		{ -4.0 * disc_radius / 5.0f , -disc_radius / 8.0  ,0 },
		{ -2.0 * disc_radius / 3.0f ,-disc_radius * 0.6,0 },
		{ 0,-disc_radius * 0.5,0 },
		{ 1.2 * disc_radius / 4.0 ,0,0 },
		{1.7*  disc_radius / 3.0 ,racetrack_length / 4.0,0 },
		{turn_radius * 2.5, 4.0 * racetrack_length / 5.0 ,0} };
return key_positions_mario2;
}
buffer<vec3> trajPeach()
{
	buffer<vec3> key_peach = { {3.0 * disc_radius / 4.0, 0, 0},
		{ 3.0 / 4.0 * disc_radius ,racetrack_length / 5.0f, 0 },
		{ disc_radius * 3.0 / 4.0f, 3.0 * racetrack_length / 5.0 ,0 },
		{ 2.0 * disc_radius / 3.0, racetrack_length + turn_radius ,0 },
		{ 0 ,racetrack_length + turn_radius * 1.5,0 },
		{ -2.0 * turn_radius,racetrack_length - turn_radius,0 },
		{ -disc_radius / 2.0f ,racetrack_length / 2.0,0 },
		{ -3.0 * disc_radius / 4.0,0,0 },
		{ 0 ,-3.0 * disc_radius / 4.0,0 },
		{ 3.0 * disc_radius / 4.0,0,0 },
		{ 3.0 * disc_radius / 4.0f ,racetrack_length / 5.0f,0 },
		{ 3.0 * disc_radius / 4.0f ,3.0 * racetrack_length / 5.0f,0 }};
	return key_peach;
}