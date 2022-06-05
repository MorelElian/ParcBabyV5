#include "rollercoaster.hpp"
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

vec3 start = {0, racetrack_length*0.75f, tube_height/2.0f};
vec3 p1 = {0.5*disc_radius, racetrack_length, tube_height/2.0f};
vec3 p2 = {0, racetrack_length + 0.5*disc_radius, tube_height};
vec3 p3 = {-0.5*disc_radius, racetrack_length, tube_height};
vec3 p4 = {0, racetrack_length/2.0f, tube_height*3};
vec3 p5 = {0, 0, tube_height*3};
vec3 p6 = {0, -disc_radius*0.5, tube_height};
vec3 p7 = {disc_radius, -disc_radius, 0};
vec3 p8 = {1.5*disc_radius, 0, tube_height*0.5f};
vec3 p9 = {0, racetrack_length/4.0f, tube_height*2};
vec3 p10 = {-disc_radius*0.5, racetrack_length/2.0f, tube_height*0.5f};

buffer<vec3> key_positions_rc = { p10, start, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, start, p1};
buffer<float> key_steps = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f };

cgp::mesh_drawable create_rollercoaster_mesh_drawable(){
    cgp::mesh_drawable rollercoaster;
	float rc_thickness = 0.2;
    float dt = 0.01;
	float t = key_steps[1] + dt;
	float tf = key_positions_rc.size() - 2;

	// std::cout << t << std::endl;
	// std::cout << interpolation(t, key_positions, key_steps) << std::endl;
	// std::cout << interpolation(t+dt, key_positions, key_steps) << std::endl;
    float rc_width = 1.5;

    vec3 center = interpolation(t, key_positions_rc, key_steps);
    vec3 center2 = interpolation(t-dt, key_positions_rc, key_steps);
    center.x += rc_width/2.0f;
    center2.x += rc_width/2.0f;
	mesh rc = mesh_primitive_cylinder(rc_thickness, center, center2);
    
    center.x -= rc_width;
    center2.x -= rc_width;
    mesh rc2 = mesh_primitive_cylinder(rc_thickness, center, center2);

    int i = 0; 
    vec3 center3;

    while (t < tf){
        center = interpolation(t, key_positions_rc, key_steps);
		t += dt;
        if (t > tf)
            t = tf;
		center2 = interpolation(t, key_positions_rc, key_steps);
        center.x += rc_width/2.0f;
        center2.x += rc_width/2.0f;
        if(i%5 == 0){
            center3 = {center.x, center.y, center.z};
        }
		rc.push_back(mesh_primitive_cylinder(rc_thickness, center, center2));
        
        center.x -= rc_width;
        center2.x -= rc_width;
        rc2.push_back(mesh_primitive_cylinder(rc_thickness, center, center2));

        if(i%5 == 0){
            rc2.push_back(mesh_primitive_cylinder(rc_thickness, center, center3));
        }

        i++;
    }

    rc.push_back(rc2);
    rollercoaster.initialize(rc, "rollercoaster");

	rollercoaster.shading.color = {1,0,0};
	return rollercoaster;
}

float wagon_length = 4;
float wagon_width = 2;
float wagon_height = 2;


cgp::hierarchy_mesh_drawable Wagon::create_wagon_mesh_drawable(vec3 color){
    cgp::hierarchy_mesh_drawable wagon;

    cgp::mesh_drawable w1;
    mesh w = mesh_primitive_parapede(wagon_length, wagon_width, wagon_height);
    w1.initialize(w, "wagon");
    w1.shading.color = color;
    wagon.add(w1);
    return wagon;
}

void Wagon::faireAvancerWagon(float delta)
{
	vec3 p = traj.positionWagon(delta);
	vec3 p_prec = traj.positionWagonPrec(delta);
	//std::cout << "a" << std::endl;
    wagon["wagon"].transform.translation = p;
	wagon["wagon"].transform.rotation = rotation_transform::between_vector(vec3(1, 0, 0), (p - p_prec) / norm(p- p_prec));
	
}

Wagon::Wagon(Trajectoire _traj, vec3 color){
    wagon = create_wagon_mesh_drawable(color);
    traj = _traj;
}

vec3 red = {1,0,0};
vec3 green = {0,1,0};
vec3 blue = {0,0,1};
vec3 yellow = {1,1,0};
vec3 aaa = {0,1,1};

buffer<vec3> train_colors = {red, green, blue, yellow, aaa};

Train::Train(int _nb_wagon, Trajectoire traj){
    nb_wagon = _nb_wagon;
    train = new Wagon[nb_wagon];
    int nb_colors = train_colors.size();
    for(int i = 0; i < nb_wagon; i++){
        train[i] = Wagon(traj, train_colors[i % nb_colors]);
    }
}

float delta = 0.2f;

void Train::faireAvancerTrain(){
    for(int i = 0; i < nb_wagon; i++){
        train[i].faireAvancerWagon(delta*i);
        train[i].wagon.update_local_to_global_coordinates();
    }
}