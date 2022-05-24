#include "elementar.hpp"
#include "math.h"

using namespace cgp;

mesh create_torus_mesh(float radius, float thickness, float start_angle, float end_angle, int radial_resolution, int tubular_resolution){
    mesh m;
    // generate vertices
    //m.position.resize(2*radial_resolution*tubular_resolution);
    //start_angle = fmod(start_angle, M_PI * 2.0);
    //end_angle = fmod(end_angle, M_PI * 2.0);
    if (end_angle == 0)
        end_angle = 2*M_PI;
    end_angle += 0.1;
    for (int i = 0; i < radial_resolution; i++) {
        for (int j = 0; j < tubular_resolution; j++) {
            float u = start_angle + (float)j / tubular_resolution * (end_angle - start_angle);
            float v =  (float)i  / radial_resolution *  M_PI * 2.0;
            float x = (radius + thickness * std::cos(v)) * std::cos(u);
            float y = (radius + thickness * std::cos(v)) * std::sin(u);
            float z = thickness * std::sin(v);
            //std::cout << x << ", " << y << ", " << z << std::endl;
            m.position.push_back(vec3{x, y, z});
        }
    }
    // add quad faces
    for (int i = 0; i < radial_resolution; i++) {
        int i_next = (i + 1) % radial_resolution;
        for (int j = 0; j < tubular_resolution-1; j++) {
            int j_next = (j + 1) % tubular_resolution;
            int i0 = i * tubular_resolution + j;
            int i1 = i * tubular_resolution + j_next;
            int i2 = i_next * tubular_resolution + j_next;
            int i3 = i_next * tubular_resolution + j;
            uint3 triangle_1 = {i1, i3, i0};
            uint3 triangle_2 = {i1, i2, i3};
            m.connectivity.push_back(triangle_1);
            m.connectivity.push_back(triangle_2);
        }
    }
    m.fill_empty_field();
    return m;
}

mesh mesh_primitive_parapede(float longueur, float largeur, float hauteur)
{
	mesh shape;
	shape.position = { vec3(0,0,0),vec3(0,largeur,0),vec3(longueur,largeur,0),vec3(longueur,0,0),
						vec3(0,0,hauteur), vec3(0,largeur,hauteur), vec3(longueur,largeur,hauteur), vec3(longueur,0,hauteur) };
	shape.connectivity = { {0,1,3},{1,2,3},{4,1,0},{1,4,5},{5,2,1},{2,5,6},{7,3,2},{7,2,6},{4,3,0},{4,7,3},
		{4,6,7},{4,5,6} };
	shape.fill_empty_field();
	return shape;
}

mesh mesh_primitive_smoothCylinder(float radius, float height)
{
    float radius2 = radius/5.0;
    vec3 p1 = {0,0,0};
    vec3 p2 = {0,0,height};
    mesh disc1 = mesh_primitive_disc(radius - radius2,p1);
    mesh disc2 = mesh_primitive_disc(radius- radius2,p2);
    
    vec3 p3 = {0,0,radius2};
    vec3 p4 = {0,0,height - radius2};
    mesh cylinder = mesh_primitive_cylinder(radius,p3,p4);

    mesh torus1 = mesh_primitive_torus(radius-radius2,radius2,p3);
    mesh torus2 = mesh_primitive_torus(radius-radius2,radius2,p4);

	cylinder.push_back(disc1);
    cylinder.push_back(disc2);
    cylinder.push_back(torus1);
    cylinder.push_back(torus2);
	return cylinder;
}
mesh mesh_primitive_sideKart(float longueur, float largeur, float hauteur)
{
    mesh shape;
    shape.position = { vec3(0,0,0),vec3(0,0,hauteur),vec3(longueur,0,0),vec3(0,largeur,0),vec3(0,largeur,hauteur),vec3(longueur,largeur,0) };
    shape.connectivity = { { 1,2,0 }, { 4,5,3 }, { 5,2,4 }, { 2,1,4 }, { 3,1,4 }, { 0,1,3 } };
    shape.uv.push_back({ 2,2 });
    shape.uv.push_back({ 0,1 });
    shape.uv.push_back({ 0,0 });
    shape.uv.push_back({ 2,2 });
    shape.uv.push_back({ 1,1 });
    shape.uv.push_back({ 1,0 });

    shape.fill_empty_field();
    return shape;

}
static buffer<uint3> connectivity_grid(size_t Nu, size_t Nv)
{
	buffer<uint3> connectivity;
	for (size_t ku = 0; ku < Nu - 1; ++ku) {
		for (size_t kv = 0; kv < Nv - 1; ++kv) {
			unsigned int k00 = static_cast<unsigned int>(kv + Nv * ku);
			unsigned int k10 = static_cast<unsigned int>(kv + 1 + Nv * ku);
			unsigned int k01 = static_cast<unsigned int>(kv + Nv * (ku + 1));
			unsigned int k11 = static_cast<unsigned int>(kv + 1 + Nv * (ku + 1));

			connectivity.push_back(uint3{ k00, k10, k11 });
			connectivity.push_back(uint3{ k00, k11, k01 });
		}
	}
	return connectivity;
}
mesh mesh_primitive_disc_bis(float radius, vec3 const& center, vec3 const& normal, int N, float fraction_cercle)
{
	assert_cgp(radius > 0, "Disc radius (" + str(radius) + ") must be >0");
	assert_cgp(N > 2, "Disc samples (" + str(N) + ") must be >2");

	mesh shape;

	rotation_transform const r = rotation_transform::between_vector({ 0,0,1 }, normal);

	for (size_t k = 0; k < size_t(N); ++k)
	{
		float const u = k / (N - 1.0f);
		vec3 const p = radius * r * vec3(std::cos(2 * Pi * u / fraction_cercle), std::sin(2 * Pi * u / fraction_cercle), 0.0f) + center;
		shape.position.push_back(p);
		shape.uv.push_back({ float(std::cos(2 * Pi * u / fraction_cercle)) * 0.5f + 0.5f, float(std::sin(2 * Pi * u / fraction_cercle)) * 0.5f + 0.5f });
		shape.normal.push_back(normal);
	}
	// middle point
	shape.position.push_back(center);
	shape.uv.push_back({ 0.5f, 0.5f });
	shape.normal.push_back(normal);

	for (size_t k = 0; k < size_t(N - 1); ++k)
		shape.connectivity.push_back(uint3{ unsigned(N), unsigned(k), unsigned(k + 1) });

	shape.fill_empty_field();
	return shape;
}
mesh mesh_primitive_cylinder_bis(float radius, vec3 const& p0, vec3 const& p1, int Nu, int Nv, bool is_closed, float fraction_cercle)
{
	vec3 const p01 = p1 - p0;
	float const L = norm(p01);
	assert_cgp(L > 1e-6f, "Cylinder has 0 length");

	vec3 const dir = p01 / L;
	rotation_transform const R = rotation_transform::between_vector({ 0,0,1 }, dir);

	mesh shape;
	for (size_t ku = 0; ku < size_t(Nu); ++ku) {
		for (size_t kv = 0; kv < size_t(Nv); ++kv) {
			float const u = ku / (Nu - 1.0f);
			float const v = kv / (Nv - 1.0f);

			float const theta = 2 * Pi * v / fraction_cercle;

			// cylinder oriented along local z-axis
			vec3 const q = { radius * std::cos(theta), radius * std::sin(theta), L * u };

			// rotate and translate to p1
			vec3 const p = R * q + p0;

			// normal
			vec3 const n = R * vec3{ std::cos(theta), std::sin(theta), 0 };
			// uv
			vec2 const uv = { u,v };

			shape.position.push_back(p);
			shape.normal.push_back(n);
			shape.uv.push_back(uv);
		}
	}

	shape.connectivity = connectivity_grid(Nu, Nv);

	if (is_closed) {
		shape.push_back(mesh_primitive_disc_bis(radius, p0, dir, Nv, fraction_cercle).flip_connectivity());
		shape.push_back(mesh_primitive_disc_bis(radius, p1, dir, Nv, fraction_cercle));
	}

	shape.fill_empty_field();

	return shape;
}