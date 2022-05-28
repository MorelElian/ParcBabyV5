
#include "scene.hpp"
#include <math.h>   
#include <cmath>
float extern pi;

float extern disc_radius;
float extern turn_radius;
float extern racetrack_length;
float extern racetrack_thickness;

float extern barrier_height;
float extern barrier_radius1;
int extern post_resolution;
int extern turn_resolution;

float extern departure_height;
float extern departure_thickness;
float extern departure_radius;

float extern tube_height;
float extern tube_thickness;
float extern arc_thickness;

float extern pilar_height;
float extern pilar_thickness;
float extern pivot_height; 


//pb avec la skybox
// pb avec l'orientation du kart
//pb avec la camera, comment la changer ?
using namespace cgp;

void scene_structure::initialize()
{
	// Basic set-up
	// global_frame.initialize(mesh_primitive_frame(), "Frame");
	
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-4.0f,2.0f }, { 0,0,0 });
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	//environment.camera.position_camera = { disc_radius/2.0, 0.6 * racetrack_length/2.0, 5.0f };
	//environment.camera.manipulator_rotate_roll_pitch_yaw(-Pi/2.0, Pi/2.0, Pi/2.0);
	
	// mesh const terrain_mesh = create_terrain_mesh();
	// terrain.initialize(terrain_mesh, "terrain");
	// terrain.shading.color = { 0.6f,0.85f,0.5f };
	skybox.initialize("assets/Gu_RainbowRoad/","skybox");
	racetrack = create_racetrack_mesh_drawable();
	//rollercoaster = create_rollercoaster_mesh_drawable();
	
	buffer<vec3> extern key_positions_rc;
	buffer<float> extern key_steps;
	traj_wagon = Trajectoire("wagon", key_positions_rc, key_steps, interpolation);
	wagon = new Wagon(traj_wagon);
	
	buffer<vec3> key_positions_cam = { {disc_radius / (2.0),racetrack_length / 2.0f ,10},{ 5.0 * disc_radius, -racetrack_length / 2.0f ,7}, { 5.0 * disc_radius ,racetrack_length / 2.0f,5}, {3 * disc_radius ,racetrack_length ,5}, {0,racetrack_length + disc_radius / 2.0f ,6},{-disc_radius,racetrack_length + disc_radius / 2.0f ,4}
	,{-disc_radius / (1.4),- racetrack_length / 2.0f ,6} ,{disc_radius ,racetrack_length / 2.8f ,5},{disc_radius / (2.0),racetrack_length / 2.8f ,10} ,{disc_radius / (2.0),racetrack_length / 2.0f ,10} };
	buffer<float> key_times_cam = { 0.0f, 1.0f, 5.0f, 7.0f, 12.0f, 14.0f,16.0f, 17.0f, 19.0f ,21.0f};
	tCamera = Trajectoire("er", key_positions_cam, key_times_cam, interpolation);
	
	//Initialisation trajectoires
	tabTrajectoire = new Trajectoire[nTraj];
	tabKart = new Kart[nTraj];

	buffer<vec3> key_positions = { 
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
	buffer<float> key_times = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.5f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };
	tabTrajectoire[0] = Trajectoire("TrajLuigi", key_positions, key_times, interpolation);
	tabKart[0] = Kart("kartLuigi", "assets/personnages/sigleLuigi.jpg", 1.2, 0.4, 0.15, 3.0, vec3(0.0, 1.0, 0), vec3(1, 1, 1.0));

	key_positions = { { 3.0 * disc_radius / 4.0 ,-disc_radius / 2.0,0 },
		{ disc_radius * 2.0 / 3.0f,racetrack_length / 3.0f,0},
		{disc_radius * 2.0 / 3.0f, 2.0 * racetrack_length / 3.0 ,0},
		{2.0 * disc_radius / 3.0, racetrack_length + disc_radius / 2.0f ,0},
		{ 0 ,racetrack_length + 3.0 * disc_radius / 4.0f ,0 },
		{ -2.0 * disc_radius / 3.0f ,racetrack_length + disc_radius/ 3.0f ,0 },
		{ -3.0 * disc_radius / 4.0f ,racetrack_length / 3.0,0 },
		{ - 2.0 * disc_radius /3.0,-disc_radius / 3.0f,0 },
		{ 0 ,- 3.0 /4.0 * disc_radius,0 },
		{ 3.0 * disc_radius / 4.0 ,-disc_radius / 2.0,0 },
		{ 2.0 * disc_radius / 3.0f ,racetrack_length / 3.0f,0 },
		{disc_radius * 2.0 / 3.0f, 2.0 * racetrack_length / 3.0 ,0} };
	key_times = { 0.0f, 1.0f, 1.6f, 2.4f, 3.2f, 4.0f, 4.6f, 6.0f, 6.8f, 7.6f,9.0f,10.0f};
	tabTrajectoire[1] = Trajectoire("TrajWaluigi", key_positions, key_times, interpolation);
	tabKart[1] = Kart("kartWaluigi", "assets/personnages/sigleWaluigi.jpg", 1.2, 0.4, 0.15, 3.0, vec3(0.4, 0.0, 0.8), vec3(1, 1, 0.0));
	key_positions_mario = { 
		{ disc_radius / 4.0 ,0,0 },
		{ disc_radius * 1.0 / 3.0f,racetrack_length / 4.0f,0},
		{turn_radius * 2.5, 4.0 * racetrack_length / 5.0 ,0},
		{ - turn_radius * 0.6, racetrack_length + disc_radius/2.0 ,0} ,
		{-4.0 * disc_radius/5.0, racetrack_length + disc_radius /4.0  ,0},
		{ - 1.0/3.0 * disc_radius ,0.5* racetrack_length ,0},
		{ -4.0 * disc_radius / 5.0f , -disc_radius/8.0  ,0 },
		{ -2.0 * disc_radius / 3.0f ,- disc_radius * 0.6,0 },
		{ 0,-disc_radius * 0.4,0 },
		{ disc_radius / 4.0 ,0,0 },
		{ 1.0 * disc_radius / 3.0 ,racetrack_length / 4.0,0 },
		{turn_radius * 2.5, 4.0 * racetrack_length / 5.0 ,0}, };
	std::cout << key_positions_mario.size() << std::endl;
	key_times_mario = { 0.0f, 1.0f, 1.8f, 2.7f, 4.0f, 5.0f, 6.0f, 7.0f, 8.4f, 8.8f,9.0f,10.0f};


	key_positions = { {3.0 * disc_radius / 4.0,0,0},
		{ 3.0 / 4.0 * disc_radius ,racetrack_length / 5.0f, 0},
		{disc_radius * 3.0 / 4.0f, 3.0 *racetrack_length / 5.0 ,0},
		{2.0 * disc_radius / 3.0, racetrack_length +turn_radius ,0},
		{ 0 ,racetrack_length + turn_radius * 1.5,0 },
		{ -2.0 * turn_radius,racetrack_length - turn_radius,0 },
		{ - disc_radius / 2.0f ,racetrack_length / 2.0,0 },
		{ -3.0 * disc_radius / 4.0,0,0 },
		{ 0 ,-3.0 * disc_radius / 4.0,0} ,
		{3.0 * disc_radius / 4.0,0,0},
		{3.0 * disc_radius / 4.0f ,racetrack_length / 5.0f,0},
		{3.0 * disc_radius / 4.0f ,3.0 * racetrack_length / 5.0f,0}};
	key_times = { 0.0f, 1.0f, 1.6f, 2.4f, 3.2f, 4.0f, 4.6f, 6.0f, 6.8f, 7.6f,9.0f,10.0f};
	tabTrajectoire[2] = Trajectoire("TrajWaluigi", key_positions, key_times, interpolation);
	tabKart[2] = Kart("kartPeach", "assets/personnages/siglePeach.png", 1.2, 0.4, 0.15, 3.0, vec3(1.0, 0.4, 1.0), vec3(1, 1, 0.0));
	key_times2 = { 0.0f, 1.0f, 3.0f, 5.0f, 7.0f, 9.0f, 11.0f, 13.0f, 15.0f, 17.0f, 18.0f };
	kartMario = new Kart("kartMario", "assets/sigleMario.png", 1.2, 0.4, 0.15, 3.0, vec3(1.0, 0.0, 0), vec3(0, 0, 1.0));
	kartLuigi = create_kart(1.2,0.4,0.15,10.0,vec3(0.0,0.0,1.0),vec3(1.0,1.0,0.0),"assets/sigleMario.png");

	// Key 3D positions

	// Initialize the helping structure to display/interact with these positions
	keyframe.initialize(key_positions, key_times);

	int N = key_times.size();
	timer.t_min = key_times[1];
	timer.t_max = key_times[N - 2];
	timer.t = timer.t_min;
	//int idx = find_index_of_interval(2.0, key_times);
}

void scene_structure::update_camera()
{

	// The camera moves forward all the time
	//   We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
	float const dt = timer.update();
	update_camera_actif = true;
	//vec3 const forward_displacement = 0;
	//camera.position_camera += forward_displacement;
	// camera.position_camera += (dt, dt, dt);
	//camera.manipulator_rotate_trackball({ 0,0 }, { dt,dt });
	float compteur = 0;
	float t = timer.t;
	vec3 p = interpolation(avancementKart, key_positions_mario,key_times_mario);
	vec3 p_prec = interpolation(avancementKart - 0.001, key_positions_mario, key_times_mario);
	vec3 posCamera =p -  (p-p_prec) / norm(p - p_prec) * 8 + vec3(-0.5,0,3);
	vec3 regardCamera = p + vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}
void scene_structure::update_cameraArriere()
{

	vec3 p = interpolation(avancementKart, key_positions_mario, key_times_mario);
	vec3 p_prec = interpolation(avancementKart - 0.001,key_positions_mario, key_times_mario);
	vec3 posCamera = p + (p - p_prec) / norm(p - p_prec) * 8 + vec3(0, 0, 3);
	vec3 regardCamera = p + vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}

void scene_structure::update_cameraPresentation()
{
	vec3 posCameraPres = tCamera.positionKart();
	environment.camera.look_at(posCameraPres, { disc_radius / (2.0),racetrack_length / 2.0f ,8 });
}
void scene_structure::display()
{
	// Basic elements of the scene
	
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	environment.light = environment.camera.position();
	draw(skybox, environment);
	
	if (gui.display_frame)
		draw(global_frame, environment);
	
	//draw(terrain,environment);
	
	//float racetrack_incline = M_PI/12.0;
	//racetrack["racetrack1"].transform.translation = {0,0,evaluate_terrain_height(0, 0) + pilar_height};
	//racetrack["racetrack_pivot1"].transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, -racetrack_incline);
	//racetrack["racetrack1"].transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, racetrack_incline);

	racetrack.update_local_to_global_coordinates();
	draw(racetrack, environment);
	draw(rollercoaster, environment);
	
	// Update the current time
	timer.update();

	wagon->faireAvancerWagon(t);
	wagon->wagon.update_local_to_global_coordinates();
	draw(wagon->wagon, environment);

	// clear trajectory when the timer restart
	//if (t < timer.t_min + 0.1f)
	//	keyframe.trajectory.clear();

	// Compute the interpolated position
	//  This is this function that you need to complete
	//vec3 p = interpolation_hermite(t, keyframe.key_positions, keyframe.key_times, key_derivee);
	//vec3 p_p = derivee_hermite(t, keyframe.key_positions, keyframe.key_times,key_derivee);
	if (keyboard.up)
	{
		avancementKart += 0.03;
		std::cout << avancementKart << std::endl;
	}
	if (keyboard.down)
	{
		avancementKart -= 0.03;
	}
	
	kartLuigi.update_local_to_global_coordinates();
	Trajectoire t1("t1",key_positions_mario,key_times_mario, interpolation);
	Trajectoire t2("t2", keyframe.key_positions, key_times2, interpolation);
	kartMario->faireAvancerKartManuel(avancementKart, t1);
	kartMario->kart.update_local_to_global_coordinates();
	if (update_camera_actif || true)
	{
		for (int i = 0; i < nTraj; i++)
		{
			
			tabKart[i].faireAvancerKartManuel(avancementKart, tabTrajectoire[i]);
			tabKart[i].kart.update_local_to_global_coordinates();

			draw(tabKart[i].kart, environment);
		}
	}
	draw(kartMario->kart, environment);
	//draw(kartLuigi, environment);
	
	//keyframe.display_current_position(interpolation(avancementKart, keyframe.key_positions, keyframe.key_times), environment);
	
}


void scene_structure::display_gui()
{
	//ImGui::Checkbox("Frame", &gui.display_frame);
	//ImGui::SliderFloat("Time scale", &timer.scale, 0.0f, 2.0f);
	//ImGui::SliderFloat("Particle emission", &timer.event_period, 0.1f, 2.0f);
}


