#pragma once

#include "cgp/cgp.hpp"
#if !defined(M_PI)
#define M_PI 3.1415
#endif



cgp::mesh create_torus_mesh(float radius, float thickness, float start_angle, float end_angle, int radial_resolution, int tubular_resolution);
cgp::mesh mesh_primitive_parapede(float longueur, float largeur, float hauteur);
cgp::mesh mesh_primitive_smoothCylinder(float radius, float height);
cgp::mesh mesh_primitive_sideKart(float longueur, float largeur, float hauteur);
cgp::mesh mesh_primitive_cylinder_bis(float radius, cgp::vec3 const& p0, cgp::vec3 const& p1, int Nu, int Nv, bool is_closed, float fraction_cercle);
cgp::mesh mesh_primitive_disc_bis(float radius, cgp::vec3 const& center, cgp::vec3 const& normal, int N, float fraction_cercle);
