#include "math.h"
#include "interpolation.h"
#include "key_positions_structure.hpp"
using namespace cgp;

/** Find the index k such that intervals[k] < t < intervals[k+1]
* - Assume intervals is a sorted array of N time values
* - Assume t \in [ intervals[0], intervals[N-1] [       */
vec3 interpolation(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times)
{
	// Find idx such that key_times[idx] < t < key_times[idx+1]
	int idx = find_index_of_interval(t, key_times);
	if (idx < 0)
	{
		return vec3(0,0,0);
	}
	//std::cout << idx << std::endl;
	// Get parameters (time and position) used to compute the linear interpolation
	//   (You will need to get more parameters for the spline interpolation)
	float t0 = key_times[idx - 1];
	float t1 = key_times[idx]; // = t_i
	float t2 = key_times[idx + 1]; // = t_{i+1}
	float t3 = key_times[idx + 2];

	//std::cout << "c" << std::endl;
	vec3 const& p0 = key_positions[idx - 1];
	vec3 const& p1 = key_positions[idx]; // = p_i
	vec3 const& p2 = key_positions[idx + 1]; // = p_{i+1}
	vec3 const& p3 = key_positions[idx + 2]; // = p_{i+2}
	float K = 1;

	// Call the interpolation
	vec3 p = cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, K);

	return p;
}
vec3 derivee_interpolation(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times)
{
	int idx = find_index_of_interval(t, key_times);

	// Get parameters (time and position) used to compute the linear interpolation
	//   (You will need to get more parameters for the spline interpolation)
	float t0 = key_times[idx - 1];
	float t1 = key_times[idx]; // = t_i
	float t2 = key_times[idx + 1]; // = t_{i+1}
	float t3 = key_times[idx + 2];
	vec3 const& p0 = key_positions[idx - 1];
	vec3 const& p1 = key_positions[idx]; // = p_i
	vec3 const& p2 = key_positions[idx + 1]; // = p_{i+1}
	vec3 const& p3 = key_positions[idx + 2]; // = p_{i+2}
	float K = 1;
	float s = (t - t1) / (t2 - t1);

	float s2 = s * s;
	float s3 = s * s * s;
	float ds = t / (t2 - t1);
	float ds2 = 2.0 * ds * s;
	float ds3 = 3.0 * ds * s2;

	vec3 di = K * (p2 - p0) / (t2 - t0);
	vec3 di1 = K * (p3 - p1) / (t3 - t1);
	vec3 p_p = (2.0 * ds3 - 3.0 * ds2 + 1) * p1 + (ds3 - 2.0 * ds2 + ds) * di + (-2.0 * ds3 + 3.0 * ds2) * p2 + (ds3 - ds2) * di1;
	return p_p;

}
vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, vec3 const& p0, vec3 const& p1, vec3 const& p2, vec3 const& p3, float K)
{
	// To do: fill the function to compute p(t) as a cardinal spline interpolation
	float s = (t - t1) / (t2 - t1);
	float s2 = s * s;
	float s3 = s * s * s;
	vec3 di = K * (p2 - p0) / (t2 - t0);
	vec3 di1 = K * (p3 - p1) / (t3 - t1);
	vec3 const p = (2.0 * s3 - 3.0 * s2 + 1) * p1 + (s3 - 2.0 * s2 + s) * di + (-2.0 * s3 + 3.0 * s2) * p2 + (s3 - s2) * di1;
	return p;
}


vec3 interpolation_hermite(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times, buffer<vec3> const& key_derivee)
{
	int idx = find_index_of_interval(t, key_times);
	float tAvant = key_times[idx]; // = t_i
	float tApres = key_times[idx + 1]; // = t_{i+1}
	vec3 const& pAvant = key_positions[idx];
	vec3 const& pApres = key_positions[idx + 1];
	vec3 const& dAvant = key_derivee[idx];
	vec3 const& dApres = key_derivee[idx + 1];
	vec3 p = position_hermite(t, tAvant, tApres, pAvant, pApres, dAvant, dApres);
	return p;


}
vec3 derivee_hermite(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times, buffer<vec3> const& key_derivee)
{
	int idx = find_index_of_interval(t, key_times);

	// Get parameters (time and position) used to compute the linear interpolation
	//   (You will need to get more parameters for the spline interpolation)
	float t1 = key_times[idx]; // = t_i
	float t2 = key_times[idx + 1]; // = t_{i+1}
	vec3 const& p1 = key_positions[idx]; // = p_i
	vec3 const& p2 = key_positions[idx + 1]; // = p_{i+1}
	float s = (t - t1) / (t2 - t1);
	float s2 = s * s;
	float s3 = s * s * s;
	float ds = t / (t2 - t1);
	float ds2 = 2.0 * ds * s;
	float ds3 = 3.0 * ds * s2;

	vec3 di = key_derivee[idx];
	vec3 di1 = key_derivee[idx + 1];
	vec3 p_p = (2.0 * ds3 - 3.0 * ds2 + 1) * p1 + (ds3 - 2.0 * ds2 + ds) * di + (-2.0 * ds3 + 3.0 * ds2) * p2 + (ds3 - ds2) * di1;
	return p_p;

}
vec3 position_hermite(float t, float tAvant, float tApres, vec3 const& pAvant, vec3 const& pApres, vec3 const& dAvant, vec3 const& dApres)
{
	float s = (t - tAvant) / (tApres - tAvant);
	float s2 = s * s;
	float s3 = s * s * s;
	vec3 const p = (2.0 * s3 - 3.0 * s2 + 1) * pAvant + (s3 - 2.0 * s2 + s) * dAvant + (-2.0 * s3 + 3.0 * s2) * pApres + (s3 - s2) * dApres;
	return p;

}
int find_index_of_interval(float t, buffer<float> const& intervals)
{
	int const N = intervals.size();
	bool error = false;

	if (intervals.size() < 2) {
		std::cout << "Error: Intervals should have at least two values; current size=" << intervals.size() << std::endl;
		error = true;
	}
	if (N > 0 && t < intervals[0]) {
		std::cout << "Error: current time t is smaller than the first time of the interval" << std::endl;
		error = true;
	}
	if (N > 0 && t > intervals[N - 1]) {
		std::cout << "Error: current time t is greater than the last time of the interval" << std::endl;
		error = true;
	}
	if (error == true) {
		std::string const error_str = "Error trying to find interval for t=" + str(t) + " within values: [" + str(intervals) + "]";
		error_cgp(error_str);
	}


	int k = 0;
	while (intervals[k + 1] < t)
		++k;
	return k;
}