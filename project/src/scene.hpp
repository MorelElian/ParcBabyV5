#pragma once


#include "cgp/cgp.hpp"
#include "racetrack.hpp"
#include "environment_camera_head/environment_camera_head.h"
//#include "key_positions_structure.hpp"
#include "mouvementKart.hpp"
#include "elementar.hpp"
#include "terrain.hpp"
#include "interpolation.h"
#include "kart.h"
#include "trajectoire.hpp"
#include "rollercoaster.hpp"

#include <list>

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = true;
	bool drift = false;
};


// Structure of a particle
struct particle_structure
{
	cgp::vec3 p; // Position
	cgp::vec3 v; // Speed
};

// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	cgp::mesh_drawable global_frame;          // The standard global frame
	cgp::scene_environment_basic_camera_spherical_coords environment; // Standard environment controler
	//scene_environment_camera_head environment;
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	gui_parameters gui;                        // Standard GUI element storage

	// A helper structure used to store and display the key positions/time
	keyframe_structure keyframe;

	// Timer used for the interpolation of the position
	cgp::timer_interval timer;

	cgp::mesh_drawable terrain;
	cgp::skybox_drawable skybox;
	cgp::hierarchy_mesh_drawable racetrack;
	Kart* kartMario;
	Kart* kartDep;
	cgp::hierarchy_mesh_drawable kartLuigi;
	cgp::mesh_drawable rollercoaster;
	
	Train* train;
	Trajectoire traj_wagon;



	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
	void update_camera();
	void update_cameraArriere();
	void update_cameraPresentation();
	void update_cameraManuelle();
	void update_cameraArriereManuelle();
	bool update_camera_actif = false;
	cgp::buffer<float> key_times;
	cgp::buffer<float> key_times_mario;
	cgp::buffer<cgp::vec3> key_positions;
	cgp::buffer<cgp::vec3> key_positions_mario;
	cgp::buffer<cgp::vec3> key_derivee;
	cgp::buffer<float> key_times2;
	float avancementKart = 1.011;
	float avancementPrecedent = 1.1;
	Kart* tabKart;
	int nTraj = 3;
	Trajectoire *tabTrajectoire;
	Trajectoire tCamera;
	const char** tabNomTraj[3];

	//double avancement_update = 1.0;

};





