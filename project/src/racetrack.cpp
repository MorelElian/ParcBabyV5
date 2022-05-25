#include "elementar.hpp"
#include "racetrack.hpp"
#include "interpolation.h"

using namespace cgp;


float pi = 3.141592653589793;

float disc_radius = 20;
float turn_radius = 4;
float racetrack_length = 60;
float racetrack_thickness = 0.5;

float barrier_height = 0.6;
float barrier_radius = 0.1;
int post_resolution = 10;
int turn_resolution = 5;

float departure_height = 2.5;
float departure_thickness = 0.7;
float departure_radius = 2;
float sign_height = departure_height;

float tube_height = 8;
float tube_thickness = 1;
float arc_thickness = 0.5;

float pilar_height = 7;
float pilar_thickness = 2;
float pivot_height = 4; 


cgp::hierarchy_mesh_drawable create_racetrack_mesh_drawable(){
    
    cgp::hierarchy_mesh_drawable racetrack;

    cgp::mesh_drawable disc1;
	cgp::mesh_drawable disc2;
	cgp::mesh_drawable racetrack1;
	cgp::mesh_drawable racetrack2;
	cgp::mesh_drawable thickness1;
	cgp::mesh_drawable thickness2;
	
    cgp::mesh_drawable barrier;
	cgp::mesh_drawable turn_barrier1;
	cgp::mesh_drawable turn_barrier2;
	cgp::mesh_drawable post;
	
    cgp::mesh_drawable departure;
	cgp::mesh_drawable departure2;
	cgp::mesh_drawable departure3;
	cgp::mesh_drawable departure4;
	cgp::mesh_drawable departure_sign;
	cgp::mesh_drawable departure_sign2;
	cgp::mesh_drawable departure_line;

    cgp::mesh_drawable tube1;
    cgp::mesh_drawable turn1_tube1;
    cgp::mesh_drawable turn2_tube1;
    cgp::mesh_drawable tube_ring;

    cgp::mesh_drawable pilar1;
    cgp::mesh_drawable pilar_ring;
    cgp::mesh_drawable arc;
    cgp::mesh_drawable arcB;
    cgp::mesh_drawable arcC;
    cgp::mesh_drawable arcD;

	cgp::mesh_drawable racetrack_pilar;
	cgp::mesh_drawable racetrack_pivot;


    //PISTE
	disc1.initialize(mesh_primitive_disc(disc_radius), "disc");
	disc1.texture = opengl_load_texture_image("assets/pisteBleue.png", GL_REPEAT, GL_REPEAT);

	disc2.initialize(mesh_primitive_disc(disc_radius), "disc2");
	disc2.texture = opengl_load_texture_image("assets/pisteRouge.png", GL_REPEAT, GL_REPEAT);

	mesh racetrack_mesh = mesh_primitive_quadrangle({ -disc_radius,0,0 }, { disc_radius,0,0 }, { disc_radius,racetrack_length/2.0f,0 }, { -disc_radius,racetrack_length/2.0f,0 });
	racetrack1.initialize(racetrack_mesh, "racetrack1");
	racetrack1.texture = opengl_load_texture_image("assets/pisteBleue.png", GL_REPEAT, GL_REPEAT);

	racetrack2.initialize(racetrack_mesh, "racetrack2");
	racetrack2.texture = opengl_load_texture_image("assets/pisteRouge.png", GL_REPEAT, GL_REPEAT);

	mesh thickness1_mesh = mesh_primitive_cylinder(racetrack_thickness,{disc_radius,0,0},{disc_radius,racetrack_length,0});
	thickness1.initialize(thickness1_mesh, "thickness1");

	mesh thickness2_mesh = mesh_primitive_torus(disc_radius,racetrack_thickness,{0,0,0});
	thickness2.initialize(thickness2_mesh, "thickness2");


	//BARRIERE
	mesh barrier_mesh = mesh_primitive_cylinder(barrier_radius,{0,0,barrier_height},{0,racetrack_length,barrier_height});
	barrier.initialize(barrier_mesh, "barrier");

	mesh barrier2_mesh = create_torus_mesh(disc_radius, barrier_radius,0, M_PI, 100, 100);
	turn_barrier1.initialize(barrier2_mesh, "turn_barrier1");
	
	mesh barrier3_mesh = create_torus_mesh(turn_radius, barrier_radius, 0, M_PI, 100, 100);
	turn_barrier2.initialize(barrier3_mesh, "turn_barrier2");

	mesh post_mesh = mesh_primitive_cylinder(barrier_radius,{0,0,0},{0,0,barrier_height});
	post.initialize(post_mesh, "post");

	post.shading.color = {1,0,0};
	barrier.shading.color = {1,1,0};
	turn_barrier1.shading.color = {1,1,0};
	turn_barrier2.shading.color = {1,1,0};
	
	//DEPART
	mesh departure_mesh = create_torus_mesh(departure_radius, departure_thickness, M_PI/2.0, M_PI, 100, 100);
	departure.initialize(departure_mesh, "departure");

	mesh departure2_mesh = create_torus_mesh(departure_radius, departure_thickness, 0, M_PI/2.0, 100, 100);
	departure2.initialize(departure2_mesh, "departure2");

	mesh departure3_mesh = mesh_primitive_cylinder(departure_thickness,{turn_radius,racetrack_length/2.0,0},{turn_radius,racetrack_length/2.0,departure_height});
	departure3.initialize(departure3_mesh, "departure3");

	mesh departure4_mesh = mesh_primitive_cylinder_bis(departure_thickness, {turn_radius + departure_radius,racetrack_length/2.0,departure_height + departure_radius},{disc_radius-departure_radius,racetrack_length/2.0,departure_height + departure_radius}, 100, 100, true, 1);
	departure4.initialize(departure4_mesh, "departure4");

	mesh departure_sign_mesh = mesh_primitive_parapede(disc_radius - turn_radius - 2*departure_radius, 2*departure_thickness, sign_height);
	departure_sign.initialize(departure_sign_mesh, "departure_sign");

	mesh departure_sign2_mesh = mesh_primitive_quadrangle({turn_radius + departure_radius,racetrack_length/2.0f - departure_thickness, departure_height}, {disc_radius - departure_radius,racetrack_length/2.0f - departure_thickness,departure_height}, {disc_radius - departure_radius,racetrack_length/2.0f - departure_thickness,departure_height + sign_height}, {turn_radius + departure_radius,racetrack_length/2.0f - departure_thickness,departure_height + sign_height});
	departure_sign2_mesh.uv = { {-0.4,0}, {1.4,0}, {1.4,1}, {-0.4,1} };
	departure_sign2.initialize(departure_sign2_mesh, "departure_sign2");

	mesh departure_line_mesh = mesh_primitive_quadrangle({turn_radius,racetrack_length/2.0f - departure_thickness,0.1}, {disc_radius,racetrack_length/2.0f - departure_thickness,0.1}, {disc_radius,racetrack_length/2.0f + departure_thickness,0.1}, {turn_radius,racetrack_length/2.0f + departure_thickness,0.1});
	departure_line_mesh.uv = { {0,0}, {0,8}, {1,8}, {1,0} };
	departure_line.initialize(departure_line_mesh, "departure_line");


	departure.shading.color = {1,0,0};
	departure2.shading.color = {1,0,0};
	departure3.shading.color = {1,0,0};
	departure4.shading.color = {1,0,0};
	departure_sign.shading.color = {1,0,0};
	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	departure_sign2.texture = opengl_load_texture_image("assets/logo.png", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
	departure_line.texture = opengl_load_texture_image("assets/arrival.png", GL_REPEAT, GL_REPEAT);

    //TUBE
    mesh tube_mesh = mesh_primitive_cylinder(tube_thickness,{0,0,tube_height},{0,racetrack_length,tube_height});
    tube1.initialize(tube_mesh,"tube1");

    mesh turn1_tube_mesh = create_torus_mesh(turn_radius, tube_thickness, M_PI, 2*M_PI, 100, 100);
	turn1_tube1.initialize(turn1_tube_mesh,"turn1_tube1");

    mesh turn2_tube_mesh = create_torus_mesh(disc_radius, tube_thickness, M_PI, 2*M_PI, 100, 100);
	turn2_tube1.initialize(turn2_tube_mesh,"turn2_tube1");

    mesh tube_ring_mesh = mesh_primitive_smoothCylinder(1.3*tube_thickness,4*arc_thickness);
    tube_ring.initialize(tube_ring_mesh, "tube_ring");

	tube_ring.shading.color = {1,1,0};

    //ARC
    mesh pilar1_mesh = mesh_primitive_cylinder(turn_radius/3.0,{0,0,0},{0,0,1.5*tube_height});
	pilar1.initialize(pilar1_mesh,"pilar1");

    mesh pilar_ring_mesh = mesh_primitive_smoothCylinder(turn_radius/2.0,0.4*tube_height);
	pilar_ring.initialize(pilar_ring_mesh,"pilar_ring");

    mesh arc_mesh = create_torus_mesh(disc_radius/2.0, arc_thickness, 0, M_PI, 100, 100);
    arc.initialize(arc_mesh,"arc");

    mesh arcB_mesh = mesh_primitive_cylinder(arc_thickness,{0,0,tube_height/3.0},{turn_radius,0,tube_height});
    arcB.initialize(arcB_mesh,"arcB");
    
    mesh arcC_mesh = mesh_primitive_cylinder(arc_thickness,{0,0,tube_height/3.0},{-turn_radius,0,tube_height});
    arcC.initialize(arcC_mesh,"arcC");

    mesh arcD_mesh = create_torus_mesh(turn_radius, arc_thickness, 0, M_PI, 100, 100);
    arcD.initialize(arcD_mesh,"arcD");

	pilar1.shading.color = {0,0,1};
	pilar_ring.shading.color = {1,0,0};
	arc.shading.color = {0,1,0};
	arcB.shading.color = {0,1,0};
	arcC.shading.color = {0,1,0};
	arcD.shading.color = {0,1,0};

	//PILIERS DE PISTE

	mesh racetrack_pilar_mesh = create_torus_mesh(pilar_height, pilar_thickness, M_PI/2.0, 3*M_PI/2.0, 100, 100);
	racetrack_pilar.initialize(racetrack_pilar_mesh, "racetrack_pilar");

	mesh racetrack_pivot_mesh = mesh_primitive_cylinder(pilar_thickness,{disc_radius + pivot_height,racetrack_length/2.0,-pilar_thickness},{-disc_radius - pivot_height,racetrack_length/2.0,-pilar_thickness});
	racetrack_pivot.initialize(racetrack_pivot_mesh, "racetrack_pivot");

	//HIERARCHIE BASE DE RACETRACK -> "racetrack1"

	//PISTE 
	racetrack.add(racetrack1);

	racetrack2.transform.translation = {0,racetrack_length/2.0f,0};
	racetrack.add(racetrack2, "racetrack1");

	racetrack1.name = "racetrack3";
	racetrack1.transform.translation = {0,0,-2*racetrack_thickness};
	racetrack.add(racetrack1, "racetrack1");

	racetrack2.name = "racetrack4";
	racetrack2.transform.translation = {0,racetrack_length/2.0f,-2*racetrack_thickness};
	racetrack.add(racetrack2, "racetrack1");

	racetrack.add(disc1, "racetrack1");
	
	disc2.transform.translation = {0,racetrack_length,0};
	racetrack.add(disc2, "racetrack1");

	disc1.transform.translation = {0,racetrack_length,-2*racetrack_thickness};
	disc1.name = "disc3";
	racetrack.add(disc1, "racetrack1");

	disc1.transform.translation = {0,0,-2*racetrack_thickness};
	disc1.name = "disc4";
	racetrack.add(disc1, "racetrack1");

	thickness1.transform.translation = {0,0,-racetrack_thickness};
	racetrack.add(thickness1, "racetrack1");

	thickness1.transform.translation = {-2*disc_radius,0,-racetrack_thickness};
	thickness1.name = "thickness3";
	racetrack.add(thickness1, "racetrack1");

	thickness2.transform.translation = {0,0,-racetrack_thickness};
	racetrack.add(thickness2, "racetrack1");

	thickness2.transform.translation = {0,racetrack_length,-racetrack_thickness};
	thickness2.name = "thickness4";
	racetrack.add(thickness2, "racetrack1");

	//BARRIERE
	barrier.name = "barrier1";
	barrier.transform.translation = {disc_radius,0,0};
	racetrack.add(barrier,"racetrack1");

    barrier.name = "barrierU1";
	barrier.transform.translation = {disc_radius,0,-3*barrier_radius};
	racetrack.add(barrier,"racetrack1");

	barrier.name = "barrier2";
	barrier.transform.translation = {-disc_radius,0,0};
	racetrack.add(barrier,"racetrack1");

    barrier.name = "barrierU2";
	barrier.transform.translation = {-disc_radius,0,-3*barrier_radius};
	racetrack.add(barrier,"racetrack1");

	barrier.name = "barrier3";
	barrier.transform.translation = {-turn_radius,0,0};
	racetrack.add(barrier,"racetrack1");

    barrier.name = "barrierU3";
	barrier.transform.translation = {-turn_radius,0,-3*barrier_radius};
	racetrack.add(barrier,"racetrack1");

	barrier.name = "barrier4";
	barrier.transform.translation = {turn_radius,0,0};
	racetrack.add(barrier,"racetrack1");

    barrier.name = "barrierU4";
	barrier.transform.translation = {turn_radius,0,-3*barrier_radius};
	racetrack.add(barrier,"racetrack1");

	//BARRIERE VIRAGE
	//VIRAGE EXTERIEUR
	turn_barrier1.transform.translation = {0,racetrack_length,barrier_height};
	racetrack.add(turn_barrier1,"racetrack2");

    turn_barrier1.name = "turn_barrierU1";
    turn_barrier1.transform.translation = {0,racetrack_length,barrier_height - 3*barrier_radius};
	racetrack.add(turn_barrier1,"racetrack2");

	turn_barrier1.name = "turn_barrier3";
	turn_barrier1.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn_barrier1.transform.translation = {0,0,barrier_height};
	racetrack.add(turn_barrier1,"racetrack1");

    turn_barrier1.name = "turn_barrierU3";
	turn_barrier1.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn_barrier1.transform.translation = {0,0,barrier_height - 3*barrier_radius};
	racetrack.add(turn_barrier1,"racetrack1");

	//VIRAGE INTERIEUR

    turn_barrier2.transform.translation = {0,racetrack_length,barrier_height};
	racetrack.add(turn_barrier2,"racetrack1");

    turn_barrier2.name = "turn_barrierU2";
	turn_barrier2.transform.translation = {0,racetrack_length,barrier_height - 3*barrier_radius};
	racetrack.add(turn_barrier2,"racetrack1");

	turn_barrier2.name = "turn_barrier4";
	turn_barrier2.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn_barrier2.transform.translation = {0,0,barrier_height};
	racetrack.add(turn_barrier2,"racetrack1");

    turn_barrier2.name = "turn_barrierU4";
	turn_barrier2.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn_barrier2.transform.translation = {0,0,barrier_height - 3*barrier_radius};
	racetrack.add(turn_barrier2,"racetrack1");

	//POTEAUX
	for(int j = 0; j < turn_resolution+1; j++){

		//VIRAGE INTERIEUR
		post.name = "post" + std::to_string(4*j);
		post.transform.translation = {turn_radius*std::cos(j*pi/turn_resolution),-turn_radius*std::sin(j*pi/turn_resolution),0};	
		racetrack.add(post,"racetrack1");

		post.name = "post" + std::to_string(4*j + 1);
		post.transform.translation = {turn_radius*std::cos(j*pi/turn_resolution),racetrack_length + turn_radius*std::sin(j*pi/turn_resolution),0};	
		racetrack.add(post,"racetrack1");

		//VIRAGE EXTERIEUR
		post.name = "post" + std::to_string(4*j + 2);
		post.transform.translation = {disc_radius*std::cos(j*pi/turn_resolution),-disc_radius*std::sin(j*pi/turn_resolution),0};	
		racetrack.add(post,"racetrack1");

		post.name = "post" + std::to_string(4*j + 3);
		post.transform.translation = {disc_radius*std::cos(j*pi/turn_resolution),racetrack_length + disc_radius*std::sin(j*pi/turn_resolution),0};	
		racetrack.add(post,"racetrack1");
    }

    for(int j = 0; j < post_resolution; j++){
		post.name = "postB" + std::to_string(4*j);
		post.transform.translation = {disc_radius,racetrack_length*j/post_resolution,0};	
		racetrack.add(post,"racetrack1");

		post.name = "postB" + std::to_string(4*j + 1);
		post.transform.translation = {-disc_radius,racetrack_length*j/post_resolution,0};	
		racetrack.add(post,"racetrack1");

		post.name = "postB" + std::to_string(4*j + 2);
		post.transform.translation = {turn_radius,racetrack_length*j/post_resolution,0};	
		racetrack.add(post,"racetrack1");

		post.name = "postB" + std::to_string(4*j + 3);
		post.transform.translation = {-turn_radius,racetrack_length*j/post_resolution,0};	
		racetrack.add(post,"racetrack1");
	}

	//DEPART
	departure.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
	departure.transform.translation = {turn_radius + departure_radius,racetrack_length/2,departure_height};	
	racetrack.add(departure,"racetrack1");

	departure2.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
	departure2.transform.translation = {disc_radius - departure_radius,racetrack_length/2,departure_height};	
	racetrack.add(departure2,"racetrack1");

	racetrack.add(departure3,"racetrack1");

	departure3.name = "departure5";
	departure3.transform.translation = {disc_radius - turn_radius,0,0};
	racetrack.add(departure3,"racetrack1");

	racetrack.add(departure4,"racetrack1");
	
	departure4.name = "departure6";
	departure4.transform.translation = {0,0,sign_height};
	racetrack.add(departure4,"racetrack1");

	departure_sign.transform.translation = {turn_radius + departure_radius,racetrack_length/2.0f - departure_thickness, departure_height + departure_radius};
	racetrack.add(departure_sign,"racetrack1");

	departure_sign2.transform.translation = {0,-0.01f,departure_radius};
	racetrack.add(departure_sign2,"racetrack1");

	racetrack.add(departure_line, "racetrack1");

    //TUBE
    tube1.transform.translation = {disc_radius,0,0};
    racetrack.add(tube1,"racetrack1");

    tube1.name = "tube2";
    tube1.transform.translation = {-disc_radius,0,0};
    racetrack.add(tube1,"racetrack1");

    tube1.name = "tube3";
    tube1.transform.translation = {turn_radius,0,0};
    racetrack.add(tube1,"racetrack1");

    tube1.name = "tube4";
    tube1.transform.translation = {-turn_radius,0,0};
    racetrack.add(tube1,"racetrack1");

    turn1_tube1.transform.translation = {0,0,tube_height};
    racetrack.add(turn1_tube1,"racetrack1");

    turn1_tube1.name = "turn1_tube2";
	turn1_tube1.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn1_tube1.transform.translation = {0,racetrack_length,tube_height};
	racetrack.add(turn1_tube1,"racetrack1");

    turn2_tube1.transform.translation = {0,0,tube_height};
    racetrack.add(turn2_tube1,"racetrack1");

    turn2_tube1.name = "turn2_tube2";
	turn2_tube1.transform.rotation = cgp::rotation_transform::from_axis_angle({ 0,0,1 }, M_PI);
	turn2_tube1.transform.translation = {0,racetrack_length,tube_height};
	racetrack.add(turn2_tube1,"racetrack1");

    for(int i = 0; i < 5; i++){
        tube_ring.name = "tube_ringA" + std::to_string(i);
        tube_ring.transform.rotation = cgp::rotation_transform::from_axis_angle({ 1,0,0 }, M_PI/2.0);
        tube_ring.transform.translation = {turn_radius,2*arc_thickness + i*racetrack_length/4.0,tube_height};
        racetrack.add(tube_ring, "racetrack1");

        tube_ring.name = "tube_ringB" + std::to_string(i);
        tube_ring.transform.rotation = cgp::rotation_transform::from_axis_angle({ 1,0,0 }, M_PI/2.0);
        tube_ring.transform.translation = {-turn_radius,2*arc_thickness + i*racetrack_length/4.0,tube_height};
        racetrack.add(tube_ring, "racetrack1");

        tube_ring.name = "tube_ringC" + std::to_string(i);
        tube_ring.transform.rotation = cgp::rotation_transform::from_axis_angle({ 1,0,0 }, M_PI/2.0);
        tube_ring.transform.translation = {-disc_radius,2*arc_thickness + i*racetrack_length/4.0,tube_height};
        racetrack.add(tube_ring, "racetrack1");

        tube_ring.name = "tube_ringD" + std::to_string(i);
        tube_ring.transform.rotation = cgp::rotation_transform::from_axis_angle({ 1,0,0 }, M_PI/2.0);
        tube_ring.transform.translation = {disc_radius,2*arc_thickness + i*racetrack_length/4.0,tube_height};
        racetrack.add(tube_ring, "racetrack1");
    }




    //ARC
    racetrack.add(pilar1,"racetrack1");

    pilar1.name = "pilar2";
    pilar1.transform.translation = {0,racetrack_length,0};
    racetrack.add(pilar1,"racetrack1");

    pilar1.name = "pilar3";
    pilar1.transform.translation = {0,racetrack_length/2.0,0};
    racetrack.add(pilar1,"racetrack1");

    pilar_ring.name = "pilar_ring1";
    pilar_ring.transform.translation = {0,0,1.2*tube_height};
    racetrack.add(pilar_ring,"racetrack1");

    pilar_ring.name = "pilar_ring2";
    pilar_ring.transform.translation = {0,racetrack_length,1.2*tube_height};
    racetrack.add(pilar_ring,"racetrack1");
    
    pilar_ring.name = "pilar_ring3";
    pilar_ring.transform.translation = {0,racetrack_length/2.0,1.2*tube_height};
    racetrack.add(pilar_ring,"racetrack1");

    pilar_ring.name = "pilar_ring4";
    pilar_ring.transform.translation = {0,0,0};
    racetrack.add(pilar_ring,"racetrack1");

    pilar_ring.name = "pilar_ring5";
    pilar_ring.transform.translation = {0,racetrack_length,0};
    racetrack.add(pilar_ring,"racetrack1");

    pilar_ring.name = "pilar_ring6";
    pilar_ring.transform.translation = {0,racetrack_length/2.0,0};
    racetrack.add(pilar_ring,"racetrack1");

    arc.name = "arc1";
    arc.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
    arc.transform.translation = {disc_radius/2.0,0,tube_height};
    racetrack.add(arc,"racetrack1");

    arc.name = "arc2";
    arc.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
    arc.transform.translation = {-disc_radius/2.0,0,tube_height};
    racetrack.add(arc,"racetrack1");

    arc.name = "arc3";
    arc.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
    arc.transform.translation = {-disc_radius/2.0,racetrack_length,tube_height};
    racetrack.add(arc,"racetrack1");

    arc.name = "arc4";
    arc.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
    arc.transform.translation = {disc_radius/2.0,racetrack_length,tube_height};
    racetrack.add(arc,"racetrack1");

    arcB.name = "arcB1";
    racetrack.add(arcB,"racetrack1");

    arcB.name = "arcB2";
    arcB.transform.translation = {0,racetrack_length,0};
    racetrack.add(arcB,"racetrack1");

    arcC.name = "arcC1";
    racetrack.add(arcC,"racetrack1");

    arcC.name = "arcC2";
    arcC.transform.translation = {0,racetrack_length,0};
    racetrack.add(arcC,"racetrack1");

    arcD.name = "arcD1";
    arcD.transform.rotation = cgp::rotation_transform::from_axis_angle({1,0,0}, M_PI/2.0);
    arcD.transform.translation = {0,racetrack_length/2.0,tube_height};
    racetrack.add(arcD,"racetrack1");

	//PILIERS DE PISTE
	racetrack_pivot.name = "racetrack_pivot1";
	racetrack.add(racetrack_pivot,"departure");

	racetrack_pilar.name = "racetrack_pilar1";
	racetrack_pilar.transform.rotation = cgp::rotation_transform::from_axis_angle({0,1,0}, M_PI/2.0);
	racetrack_pilar.transform.translation = {disc_radius + pivot_height,racetrack_length/2.0,-pilar_height - pilar_thickness};
	racetrack.add(racetrack_pilar,"racetrack_pivot1");

	racetrack_pilar.name = "racetrack_pilar2";
	racetrack_pilar.transform.rotation = cgp::rotation_transform::from_axis_angle({0,1,0}, M_PI/2.0);
	racetrack_pilar.transform.translation = {-disc_radius - pivot_height,racetrack_length/2.0,-pilar_height - pilar_thickness};
	racetrack.add(racetrack_pilar,"racetrack_pivot1");

    return racetrack;
}


buffer<vec3> key_positions = { {disc_radius/2.0f,racetrack_length/2.0f,0}, {disc_radius/2.0f,racetrack_length/2.0f,0}, {disc_radius/2.0f,racetrack_length,0}, {0,racetrack_length + disc_radius/2.0f,0}, {-disc_radius/2.0f,racetrack_length,0}, {-disc_radius/2.0f,racetrack_length/2.0f,0}, {-disc_radius/2.0f,0,0}, {0,-disc_radius/2.0f,0}, {disc_radius/2,0,0}, {disc_radius/2.0f,racetrack_length/2.0f,0}, {disc_radius/2.0f,racetrack_length/2.0f,0} };
buffer<float> key_steps = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };

cgp::mesh_drawable create_rollercoaster_mesh_drawable(){
    cgp::mesh_drawable rollercoaster;
	float rc_thickness = 3;
    float dt = 0.01;
	float t = key_steps[1] + dt;
	float tf = key_steps[key_steps.size() - 3];

	//std::cout << t << std::endl;
	std::cout << interpolation(t, key_positions, key_steps) << std::endl;
	std::cout << interpolation(t+dt, key_positions, key_steps) << std::endl;
	mesh rc = mesh_primitive_cylinder(rc_thickness, interpolation(t, key_positions, key_steps), interpolation(t+dt, key_positions, key_steps));

    while (t < tf){
		std::cout << t << std::endl;
        vec3 center = interpolation(t, key_positions, key_steps);
		t += dt;
		vec3 center2 = interpolation(t, key_positions, key_steps);
		rc.push_back(mesh_primitive_cylinder(rc_thickness, center, center2));
    }
	rollercoaster.initialize(rc, "rollercoaster");
	return rollercoaster;
}