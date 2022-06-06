
#include "scene.hpp"
#include "trajectoireKart.hpp"
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
float extern sign_height;

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
	
	
	environment.camera.axis = camera_spherical_coordinates_axis::z;
	environment.camera.look_at({ 2.0f,-4.0f,2.0f }, { 0,0,0 });
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	
	mesh const terrain_mesh = create_terrain_mesh();
	terrain.initialize(terrain_mesh, "terrain");
	terrain.shading.color = { 0.6f,0.85f,0.5f };
	
	skybox.initialize("assets/Gu_RainbowRoad/","skybox");
	
	racetrack = create_racetrack_mesh_drawable();
	rollercoaster = create_rollercoaster_mesh_drawable();
	
	nb_wagon = 10;
	delta = 0.15f;
	buffer<vec3> extern key_positions_rc;
	buffer<float> extern key_steps;
	traj_wagon = Trajectoire("train", key_positions_rc, key_steps, interpolation);
	train = new Train(nb_wagon, traj_wagon, delta);
	

	//CAMERA
	

	//vec3 pA0 = {disc_radius / (2.0),racetrack_length / 2.0f ,10};
	vec3 pA1 = { 5.0 * disc_radius, -racetrack_length / 2.0f ,7};
	vec3 pA2 = { 5.0 * disc_radius, -racetrack_length / 2.0f ,7};
	//vec3 pA2 = { 5.0 * disc_radius ,racetrack_length / 2.0f,5};
	vec3 pA3 = {3 * disc_radius ,racetrack_length ,5};
	vec3 pA4 = {0,racetrack_length + disc_radius / 2.0f ,6};
	vec3 pA5 = {-disc_radius,racetrack_length + disc_radius / 2.0f ,4};
	vec3 pA6 = {-disc_radius / (1.4),- racetrack_length / 2.0f ,6};
	vec3 pA7 = {disc_radius ,racetrack_length / 2.8f ,5};
	vec3 pA8 = {disc_radius / (2.0),racetrack_length / 2.8f ,10};
	vec3 pA9 = {disc_radius / (2.0),racetrack_length / 2.0f ,10};

	buffer<vec3> key_positions_camA = {pA2, pA2, pA3, pA3};
	buffer<float> key_times_camA = { 0.0f, 1.0f, 7.0f, 19.0f};
	tCameraA = Trajectoire("tCameraA", key_positions_camA, key_times_camA, interpolation);

	vec3 pB1 = {0, racetrack_length/2.0f, 170};
	vec3 pB2 = {0, racetrack_length/2.0f, 25};

	buffer<vec3> key_positions_camB = {pB1, pB1, pB2, pB2};
	buffer<float> key_times_camB = { 0.0f, 5.0f, 18.0f, 19.0f};
	tCameraB = Trajectoire("tCameraB", key_positions_camB, key_times_camB, interpolation);

	tCameraC = Trajectoire("tCameraC", key_positions_rc, key_steps, interpolation);

	vec3 pD1 = {disc_radius/2.0f, racetrack_length*1.5, 3};
	vec3 pD2 = {disc_radius/2.0f, racetrack_length/4.0f, 3};
	buffer<vec3> key_positions_camD = {pD1, pD1, pD1, pD2, pD2};
	buffer<float> key_times_camD = { 0.0f, 1.0f, 20.0f, 25.0f, 31.0f};

	tCameraD = Trajectoire("tCameraD", key_positions_camD, key_times_camD, interpolation);

	vec3 pE1 = {(disc_radius-turn_radius)/2.0f + turn_radius, 0, 0.5f};
	vec3 pE2 = {(disc_radius-turn_radius)/2.0f + turn_radius, racetrack_length/3.0f, departure_height + sign_height/2.0f};
	buffer<vec3> key_positions_camE = {pE1, pE1, pE1, pE2, pE2, pE2};
	buffer<float> key_times_camE = { 0.0f, 1.0f, 24.0f, 29.0f, 37.0f, 40.0f};

	tCameraE = Trajectoire("tCameraE", key_positions_camE, key_times_camE, interpolation);
	
	//INITIALISATION KARTS ET TRAJECTOIRES

	tabTrajectoire = new Trajectoire[nTraj];
	tabKart = new Kart[nTraj];
	buffer<vec3> key_positions_luigi = trajLuigi();
	buffer<float> key_times = { 0.0f, 1.0f, 2.0f, 3.0f, 4.1f, 5.5f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };
	tabTrajectoire[0] = Trajectoire("TrajLuigi", key_positions_luigi, key_times, interpolation);
	tabKart[0] = Kart("kartLuigi", "assets/personnages/sigleLuigi.jpg", 1.2, 0.4, 0.15, 3.0, vec3(0.0, 1.0, 0), vec3(1, 1, 1.0));

	key_positions = trajWaluigi();
	key_times = { 0.0f, 1.0f, 1.5f, 2.2f, 3.2f, 4.0f, 4.6f, 6.0f, 6.8f, 7.6f,9.0f,10.0f};
	tabTrajectoire[1] = Trajectoire("TrajWaluigi", key_positions, key_times, interpolation);
	tabKart[1] = Kart("kartWaluigi", "assets/personnages/sigleWaluigi.jpg", 1.2, 0.4, 0.15, 3.0, vec3(0.4, 0.0, 0.8), vec3(1, 1, 0.0));

	key_positions_mario = trajMario();
	key_times_mario = { 0.0f, 1.0f, 1.8f, 2.7f, 4.0f, 4.8f, 6.0f, 6.7f, 7.8f, 8.5,9.0f,10.0f};


	key_positions = trajPeach();
	key_times = { 0.0f, 1.0f, 1.6f, 2.4f, 3.2f, 4.0f, 4.8f, 6.2f, 7.4f, 8.4f,9.0f,10.0f};
	tabTrajectoire[2] = Trajectoire("TrajPeach", key_positions, key_times, interpolation);
	tabKart[2] = Kart("kartPeach", "assets/personnages/siglePeach.png", 1.2, 0.4, 0.15, 3.0, vec3(1.0, 0.4, 1.0), vec3(1, 1, 0.0));

	kartMario = Kart("kartMario", "assets/sigleMario.png", 1.2, 0.4, 0.15, 3.0, vec3(1.0, 0.0, 0), vec3(0, 0, 1.0));
	traj_mario = Trajectoire("t1", key_positions_mario, key_times_mario, interpolation);
	tabTrajectoire[3] = traj_mario;
	tabKart[3] = kartMario;

	kartDep = new Kart("kartMario", "assets/personnages/sigleBowser.png", 1.2, 0.4, 0.15, 3.0, vec3(0.1, 0.1, 0.1), vec3(1.0, 0, 0.0), vec3(3.0 * disc_radius / 5.0 , racetrack_length/5.0, 0));
	keyframe.initialize(key_positions, key_times);

	int N = key_times.size();
	timer.start();
	t = 1.001;
}
/// <summary>
///  fonctions pour l'usage des caméras
/// </summary>
void scene_structure::update_camera() // utiliser lorsque le mode "demo" est en route.
{

	vec3 p = tabTrajectoire[2].positionKart(t);
	vec3 p_prec =tabTrajectoire[2].positionKartPrec(t);
	vec3 posCamera = p - (p - p_prec) / norm(p - p_prec) * 6 + vec3(0, 0, 2);
	vec3 regardCamera = p  +  vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}

void scene_structure::update_cameraManuelle() // Mode "Race"
{
	float compteur = 0;
	vec3 p = kartDep->positionKart;
	vec3 orient = kartDep->orientationKart;
	vec3 posCamera = p  - 7 * orient + vec3(0,0,2);
	vec3 regardCamera = p + orient  * 2 + vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}
void scene_structure::update_cameraArriere() // mode "Demo" Camera Arriere
{

	vec3 p = tabTrajectoire[2].positionKart(t);
	vec3 p_prec = tabTrajectoire[2].positionKartPrec(t);
	vec3 posCamera = p + (p - p_prec) / norm(p - p_prec) * 5 + vec3(0, 0, 3);
	vec3 regardCamera = p + vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}
void scene_structure::update_cameraArriereManuelle() // mode "Race" Camera Arriere
{
	vec3 p = kartDep->positionKart;
	vec3 orient = kartDep->orientationKart;
	vec3 posCamera = p + 7 * orient + vec3(0, 0, 2);
	vec3 regardCamera = p + vec3(0, 0, 2.0);
	environment.camera.look_at(posCamera, regardCamera);
}
// camera pour la présentaion
void scene_structure::update_cameraPresentationA()
{
	vec3 posCameraPres = tCameraA.positionKart(1);
	environment.camera.look_at(posCameraPres, { disc_radius / (2.0),racetrack_length / 2.0f ,8 });
}
void scene_structure::update_cameraPresentationB()
{
	vec3 p_cam = tCameraB.positionKart(1);
	vec3 p_dir = {0, racetrack_length/2.0f, 0};
	environment.camera.look_at(p_cam, p_dir);
}
void scene_structure::update_cameraPresentationC()
{
	vec3 p_cam = tCameraC.positionWagon(delta*(nb_wagon + 1));
	vec3 p_dir = tCameraC.positionWagonPrec(delta*(nb_wagon + 1) + 0.5);
	p_cam.z += 3;
	environment.camera.look_at(p_cam, p_dir);
}
void scene_structure::update_cameraPresentationD()
{
	vec3 p_cam = tCameraD.positionKart(1);
	vec3 p_dir = {disc_radius/2.0f, 0, 0};
	environment.camera.look_at(p_cam, p_dir);
}
void scene_structure::update_cameraPresentationE()
{
	vec3 p_cam = tCameraE.positionKart(1);
	vec3 p_dir = {(disc_radius-turn_radius)/2.0f + turn_radius, racetrack_length/2.0f, departure_height + sign_height/2.0f};
	environment.camera.look_at(p_cam, p_dir);
}

void scene_structure::display()
{
	// Basic elements of the scene
	
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	environment.light = environment.camera.position();
	draw(skybox, environment);
	// Gestion de la box de paramètres
	if (gui.display_frame)
		draw(global_frame, environment);
	if (gui.drift)
	{
		kartDep->drift = true;
	}
	else
	{
		kartDep->drift = false;
	}
	if (gui.vuePeach)
	{
		vuePeach = true;
	}
	else
	{
		vuePeach = false;
	}

	// terrain et piste
	terrain.transform.translation = {0,0, -2*pilar_height};
	racetrack.update_local_to_global_coordinates();

	draw(terrain, environment);
	draw(racetrack, environment);
	draw(rollercoaster, environment);
	
	// Gestion du timer
	timer.update();
	std::cout << timer.t << std::endl;
	if (timer.t < 33.0 && camera)
	{
		t = 1.1;
	}
	else
	{
		if (t > 9.0)
		{
			t = 1.0;
		}
		t += 0.025;
	}
	// Avancement du train
	train->faireAvancerTrain();
	for(int i = 0; i < train->nb_wagon; i++){
		draw(train->train[i].wagon, environment);
	}
	
	
	// avancement des karts
    for (int i = 0; i < nTraj; i++)
	{
		tabKart[i].faireAvancerKart(t, tabTrajectoire[i]);
		tabKart[i].kart.update_local_to_global_coordinates();
		draw(tabKart[i].kart, environment);
	}
	//gestion des Inputs en mode racing
	float pressForward = 0;
	if (keyboard.up)
	{
		avancementKart += 0.03;
		//std::cout << avancementKart << std::endl;
		pressForward = 1;
	}
	if (keyboard.down)
	{
		avancementKart -= 0.03;
		pressForward = -1;
	}
	if (keyboard.right)
	{
		kartDep->updateOrientationKart(true);
	}
	else if (keyboard.left)
	{
		kartDep->updateOrientationKart(false);
	}
	if (keyboard.ctrl)
	{
		pressForward = 2;
	}
	else
	{
		//kartDep->drift = false;
	}
	//std::cout << pressForward << std::endl;
	kartDep->udpatePositionKart(pressForward,0.1,tabKart,accelerationMoteur);
	kartDep->kart.update_local_to_global_coordinates();
	draw(kartDep->kart, environment);
	
}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Drift", &gui.drift);
	ImGui::Checkbox("vuePeach", &gui.vuePeach);
	ImGui::SliderFloat("Time scale", &accelerationMoteur, 0.0f, 10.0f);
	//ImGui::SliderFloat("Particle emission", &timer.event_period, 0.1f, 2.0f);
}


