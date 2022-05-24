#pragma once
#include "cgp/cgp.hpp"
using namespace cgp;

int find_index_of_interval(float t, buffer<float> const& intervals);
vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, vec3 const& p0, vec3 const& p1, vec3 const& p2, vec3 const& p3, float K);
vec3 interpolation(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times);
vec3 derivee_interpolation(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times);
vec3 position_hermite(float t, float tAvant, float tApres, vec3 const& pAvant, vec3 const& pApres, vec3 const& dAvant, vec3 const& dApres);
vec3 interpolation_hermite(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times, buffer<vec3> const& key_derivee);
vec3 derivee_hermite(float t, buffer<vec3> const& key_positions, buffer<float> const& key_times, buffer<vec3> const& key_derivee);


