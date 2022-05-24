#include "key_positions_structure.hpp"

using namespace cgp;

void keyframe_structure::initialize(buffer<vec3> const& key_positions_arg, buffer<float> const& key_times_arg)
{
	key_positions = key_positions_arg;
	key_times = key_times_arg;

	// Initialise the mesh_drawable
	sphere_key_positions.initialize(mesh_primitive_sphere(0.05f), "Sphere keyframe");
	sphere_current.initialize(mesh_primitive_sphere(0.06f), "Sphere current");
	sphere_key_positions.shading.color = { 1,1,1 };
	sphere_current.shading.color = { 0,0,1 };

	polygon_key_positions.initialize(key_positions);
	polygon_key_positions.color = { 1,0,0 };
}

void keyframe_structure::display_key_positions(environmentType const& environment)
{
	assert_cgp(key_times.size() == key_positions.size(), "key_time and key_positions should have the same size");

	// Display the key positions
	if (display_keyposition)
	{
		int N = key_positions.size();
		for (int k = 0; k < N; ++k) {
			if (picking.active && picking.index == k)
				sphere_key_positions.shading.color = { 1,0,0 };
			else
				sphere_key_positions.shading.color = { 1,1,1 };
			sphere_key_positions.transform.translation = key_positions[k];
			draw(sphere_key_positions, environment);
		}
	}
		

	// Display the polygon linking the key positions
	if (picking.active)
		polygon_key_positions.update(key_positions); // update the polygon if needed
	if (display_polygon)
		draw(polygon_key_positions, environment);

}

void keyframe_structure::display_current_position(vec3 const& p, environmentType& environment)
{
	// Display the interpolated position
	sphere_current.transform.translation = p;
	draw(sphere_current, environment);

	// Display the trajectory
	trajectory.visual.color = { 0,0,1 };
	trajectory.add(p, 0 /*arbitrary time*/);
	if (display_trajectory)
		draw(trajectory, environment);
}

void keyframe_structure::display_gui()
{
	ImGui::Checkbox("Display key positions", &display_keyposition);
	ImGui::Checkbox("Display polygon", &display_polygon);
	ImGui::Checkbox("Display trajectory", &display_trajectory);
	bool new_size = ImGui::SliderInt("Trajectory size", &trajectory_storage, 2, 500);

	if (new_size) {
		trajectory.clear();
		trajectory = trajectory_drawable(trajectory_storage);
	}
}