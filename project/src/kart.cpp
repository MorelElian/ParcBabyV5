#include "kart.h"
#include "elementar.hpp"
using namespace cgp;
Kart::Kart(const char* _nomKart, const char* _nFileFrontKart, float _longueur_kart, float _largeur_kart, float _hauteur_kart, float _proportion, vec3 color1, vec3 color2)
{
	nomKart = _nomKart;
	nFileFrontKart = _nFileFrontKart;
	longueur_kart = _longueur_kart;
	largeur_kart = _largeur_kart;
	hauteur_kart = _hauteur_kart;
	proportion = _proportion;
	longueur_kart *= proportion;
	largeur_kart *= proportion;
	hauteur_kart *= proportion;
	largeur_roue_arriere = 0.2 * proportion;
	dimension_volant = 0.275 * largeur_kart;
	largeur_roue_avant = 0.15 * proportion;
	kart = create_kart(longueur_kart, largeur_kart, hauteur_kart, proportion, color1, color2, nFileFrontKart);
	//kart["baseKartTheorique"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, Pi / 2);
}
Kart::Kart(const char* _nomKart, const char* _nFileFrontKart, float _longueur_kart, float _largeur_kart, float _hauteur_kart, float _proportion, vec3 color1, vec3 color2,vec3 _positionKart)
{
	nomKart = _nomKart;
	nFileFrontKart = _nFileFrontKart;
	longueur_kart = _longueur_kart;
	largeur_kart = _largeur_kart;
	hauteur_kart = _hauteur_kart;
	proportion = _proportion;
	longueur_kart *= proportion;
	largeur_kart *= proportion;
	hauteur_kart *= proportion;
	largeur_roue_arriere = 0.2 * proportion;
	dimension_volant = 0.275 * largeur_kart;
	largeur_roue_avant = 0.15 * proportion;

	kart = create_kart(longueur_kart, largeur_kart, hauteur_kart, proportion, color1, color2, nFileFrontKart);
	positionKart = _positionKart;
	vitesseKart = vec3(0, 0, 0);
	accelerationKart = vec3(0, 0, 0);
	orientationKart = vec3(0, 1, 0);
	tempsDuKart = 0.0;
	direction = 0;
	angleRoueArriere = 0.0;
	angleRoueAvant = 0.0;
	signeAvancement = 1.0;
	turboUtilise = false;
	drift = false;
	//Element Proche de la base du kart
	
	//kart["baseKartTheorique"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, Pi / 2);
}
void Kart::faireAvancerKart(int deplacement, Trajectoire traj)
{
	vec3 p = traj.positionKart(deplacement);
	vec3 p_prec = traj.positionKartPrec();
	positionKart = p;
	orientationKart = (p - p_prec) / norm(p - p_prec);
	kart["baseKart"].transform.rotation = rotation_transform::between_vector(vec3(1, 0, 0), (p - p_prec) / norm(p- p_prec));
	kart["baseKart"].transform.translation = p;
	kart["ReAvtD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * traj.timer.t);
	kart["ReAvtG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * traj.timer.t);
	kart["ReArD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * traj.timer.t);
	kart["ReArG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * traj.timer.t);
}
void Kart::faireAvancerKartManuel(float& avancement, Trajectoire traj)
{
	float t_max = traj.key_times[traj.nPoints-2];
	float t_min = traj.key_times[1];

	if (avancement > t_max - 0.01)
	{
		avancement = t_min + 0.011;

	}
	if (avancement < t_min + 0.01)
	{
		std::cout << "dsgfsdg" << std::endl;
		avancement = t_max - 0.011;
	}
	
	vec3 p = interpolation(avancement, traj.key_positions, traj.key_times);
	vec3 p_prec = interpolation(avancement - 0.01, traj.key_positions, traj.key_times);
	positionKart = p;
	orientationKart = (p - p_prec) / norm(p - p_prec);
	//vec3 p_p = derivee_interpolation(avancement, traj.key_positions, traj.key_times);
	//vec3 p_p_prec = derivee_interpolation(avancementPrec, traj.key_positions, traj.key_times);
	//kart["baseKart"].transform.rotation =  kart["baseKart"].transform.rotation * rotation_transform::between_vector(p_p_prec / norm(p_p_prec),p_p / norm(p_p));

	kart["baseKartTheorique"].transform.rotation = rotation_transform::between_vector({ 1.0,0.0,0.0 }, (p - p_prec) / norm(p - p_prec));
	kart["baseKartTheorique"].transform.translation = p;
	kart["ReAvtD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * avancement);
	kart["ReAvtG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * avancement);
	kart["ReArD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * avancement);
	kart["ReArG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * avancement);
}
void Kart::updateAccelerationKart(float pressForward, Kart* tabKart)
{
	//std::cout << pressForward << std::endl;
	//std::cout << turboUtilise << std::endl;
	float accelerationMoteur;
	if (pressForward ==1)
	{
		accelerationMoteur = 6;
	}
	else if (pressForward == -1)
	{
		accelerationMoteur = -2;
	}
	else if (pressForward == 2 && turboUtilise < 10)
	{
		turboUtilise++;
		accelerationMoteur = 15;
	}
	else if (pressForward == 2)
	{
		accelerationMoteur = 6;
	}
	else
	{
		accelerationMoteur = 0;
	}
	accelerationKart = accelerationMoteur*orientationKart - 0.5 * vitesseKart;
	for (int i = 0; i < 4; i++)
	{
		
		accelerationKart += detecterCollisionKart(tabKart[i]);
	}
}
void Kart::updateVitesseKart(float dt)
{
	if (drift)
	{
		vitesseKart = vitesseKart + dt * accelerationKart;
	}
	else
	{
		float scal = (dt * accelerationKart + vitesseKart).x * orientationKart.x + (dt * accelerationKart + vitesseKart).y * orientationKart.y
			+ (dt * accelerationKart + vitesseKart).z * orientationKart.z;
		vitesseKart.x = scal * orientationKart.x;
		vitesseKart.y = scal * orientationKart.y;
		vitesseKart.z = scal * orientationKart.z;
	}
	
}
void Kart::udpatePositionKart(float pressForward,float dt,Kart* tabKart)
{
	if (pressForward == 1)
	{
		signeAvancement = 1.0;
	}
	else if (pressForward == -1)
	{
		signeAvancement = -1.0;
	}
	updateAccelerationKart(pressForward,tabKart);
	updateVitesseKart(dt);
	positionKart = dt * vitesseKart + positionKart;
	angleRoueAvant += signeAvancement * dt * norm(vitesseKart) / hauteur_kart;
	angleRoueArriere += signeAvancement * dt * norm(vitesseKart) / hauteur_kart;
	tempsDuKart += dt;
	//std::cout <<positionKart <<std::endl;
	kart["baseKartTheorique"].transform.translation = positionKart;
	kart["baseKartTheorique"].transform.rotation = rotation_transform::between_vector(vec3(1, 0, 0),orientationKart / norm(orientationKart));
	kart["ReAvtD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, angleRoueAvant);
	kart["ReAvtG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, angleRoueAvant );
	kart["ReArD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, angleRoueArriere );
	kart["ReArG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, angleRoueArriere );
}
void Kart::updateOrientationKart(bool droite)
{
	if (droite)
	{
		direction = 0.05;
	}
	else
	{
		direction = -0.05;
	}
	kart["lienKtReAvtD"].transform.rotation = rotation_transform::from_axis_angle(vec3(0, 0, 1), direction);
	kart["lienKtReAvtG"].transform.rotation = rotation_transform::from_axis_angle(vec3(0, 0, 1), direction);
	if (norm(vitesseKart) > 2.0)
	{
		mat3 rotation = { {std::cos(direction),std::sin(direction),0},{-std::sin(direction),std::cos(direction),0},{0,0,1} };
		orientationKart = rotation * orientationKart;
	}
	else
	{
		mat3 rotation = { {std::cos(direction* norm(vitesseKart) * 0.2),std::sin(direction * norm(vitesseKart) * 0.2),0},
			{-std::sin(direction * norm(vitesseKart) * 0.2),std::cos(direction * norm(vitesseKart) * 0.2),0},{0,0,1} };
		orientationKart = rotation * orientationKart;
	}
}
buffer<vec3> Kart::volumeEnglobant()
{
	mat3 rot90 = {
		{0,1,0},
		{-1,0,0},
		{0,0,1} };
	vec3 orthoOrient = rot90 * orientationKart ;
	buffer<vec3> volumeEnglobant = {
		positionKart + longueur_kart / 2.0 * 1.1 * orientationKart - 1.1*orthoOrient * largeur_kart / 2.0,
		positionKart + longueur_kart / 2.0 * 1.1*orientationKart + 1.1 * orthoOrient * largeur_kart / 2.0,
		positionKart - 1.1 * longueur_kart / 2.0 * orientationKart + 1.1 * orthoOrient * largeur_kart / 2.0,
		positionKart - 1.1 * longueur_kart / 2.0 * orientationKart - 1.1 * orthoOrient * largeur_kart / 2.0,
	};
	return volumeEnglobant;

}
buffer<vec3> Kart::getRepereAssocie()
{
	mat3 rot90 = {
		{0,1,0},
		{-1,0,0},
		{0,0,1} };
	vec3 orthoOrient = rot90 * orientationKart;
	return buffer<vec3>({ orientationKart,orthoOrient });
}
vec3 Kart::detecterCollisionKart(Kart& kartAdverse)
{
	buffer<vec3> volumeEnglobant = this->volumeEnglobant();
	buffer<vec3> repereAdverse = kartAdverse.getRepereAssocie();
	float projOrient = 0.0;
	float projOrientOrtho = 0.0;
	vec3 directionForce = vec3(0,0,0);
	for (int i = 0; i < 4; i++)
	{
		vec3 a = volumeEnglobant[i] - kartAdverse.positionKart;
		projOrient = a.x * repereAdverse[0].x + a.y * repereAdverse[0].y + a.z * repereAdverse[0].z;
		projOrientOrtho = a.x * repereAdverse[1].x + a.y * repereAdverse[1].y + a.z * repereAdverse[1].z;
		if (!(std::abs(projOrient) > 1.1*longueur_kart / 2.0 || std::abs(projOrientOrtho) > 1.1 * largeur_kart / 2.0))
		{
			std::cout << kartAdverse.nomKart << std::endl;
			std::cout << norm(kartAdverse.positionKart - positionKart) << std::endl;

			directionForce = (positionKart - kartAdverse.positionKart) /norm(positionKart - kartAdverse.positionKart);
			return 5.0 *  directionForce * 1/( 0.1 * norm(positionKart - kartAdverse.positionKart));
		}
	}
	buffer<vec3> reperePropre = getRepereAssocie();
	volumeEnglobant = kartAdverse.volumeEnglobant();
	for (int i = 0; i < 4; i++)
	{
		vec3 a = volumeEnglobant[i] - positionKart;
		projOrient = a.x * reperePropre[0].x + a.y * reperePropre[0].y + a.z * reperePropre[0].z;
		projOrientOrtho = a.x * reperePropre[1].x + a.y * reperePropre[1].y + a.z * reperePropre[1].z;
		if (!(std::abs(projOrient) > 1.1 * longueur_kart / 2.0 || std::abs(projOrientOrtho) > 1.1 * largeur_kart / 2.0))
		{
			std::cout << kartAdverse.nomKart << std::endl;
			
			directionForce = (positionKart - kartAdverse.positionKart) / norm(positionKart - kartAdverse.positionKart);
			return 5.0 * directionForce /( 0.1 *norm(positionKart - kartAdverse.positionKart));
		}
		
	}
	return directionForce;
}

hierarchy_mesh_drawable create_kart(float longueur_kart, float largeur_kart, float hauteur_kart, float proportion, vec3 color1, vec3 color2,const char *nFileFrontKart)
{
	hierarchy_mesh_drawable kart;
	mesh_drawable baseKartTheorique;
	mesh_drawable baseKart;
	mesh_drawable kartGauche;
	mesh_drawable kartDroite;
	mesh_drawable kartArriere;
	mesh_drawable kartPupitre;
	mesh_drawable lienKtReAvtG;
	mesh_drawable lienKtReAvtD;
	mesh_drawable lienKtReArG;
	mesh_drawable lienKtReArD;
	mesh_drawable ReArD;
	mesh_drawable ReAvtD;
	mesh_drawable ReAvtG;
	mesh_drawable ReArG;
	mesh_drawable sideKartD;
	mesh_drawable sideKartG;
	mesh_drawable kartAvant;
	mesh_drawable sideAvtD;
	mesh_drawable sideAvtG;
	mesh_drawable porteVolant;
	mesh_drawable centreVolant;
	mesh_drawable toreVolant;
	mesh_drawable lienCentreToreVolant;
	mesh_drawable lienCentreToreVolant2;
	mesh_drawable lienCentreToreVolant3;
	mesh_drawable plaqueTurbo;
	mesh_drawable porteTurbo1;
	mesh_drawable porteTurbo2;
	mesh_drawable lienTurbo1;
	mesh_drawable lienTurbo2;
	mesh_drawable turbo1;
	mesh_drawable turbo2;
	mesh_drawable discRoueArD;
	mesh_drawable discRoueAvtD;
	mesh_drawable discRoueArG;
	mesh_drawable discRoueAvtG;
	//mesh_drawable derivee;
	
	float largeur_roue_arriere = 0.2 * proportion;
	float largeur_roue_avant = 0.15 * proportion;
	float dimension_volant = 0.075 * largeur_kart ;

	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)

	baseKartTheorique.initialize(mesh_primitive_cube(vec3(0, 0, 0), 0.001), "baseKartTheorique");
	baseKart.initialize(mesh_primitive_parapede(longueur_kart, largeur_kart, hauteur_kart), "baseKart");
	baseKart.shading.color = color2;
	//Element Proche de la base du kart

	kartGauche.initialize(mesh_primitive_parapede(longueur_kart / 2.0, 0.4 * largeur_kart, 1.5 * hauteur_kart), "kartGauche");
	kartGauche.shading.color = color1;
	kartDroite.initialize(mesh_primitive_parapede(longueur_kart / 2.0, 0.4 * largeur_kart, 1.5 * hauteur_kart), "kartDroite");
	kartDroite.shading.color = color1;
	kartArriere.initialize(mesh_primitive_parapede(longueur_kart / 3.0, largeur_kart, 1.4 * hauteur_kart), "kartArriere");
	kartArriere.shading.color = color1;
	kartAvant.initialize(mesh_primitive_parapede(largeur_roue_avant * 1.2, largeur_kart, hauteur_kart), "kartAvant");
	kartAvant.shading.color = color1;
	kartPupitre.initialize(mesh_primitive_sideKart(0.4 * longueur_kart, largeur_kart, 1.8 * hauteur_kart), "kartPupitre");
	kartPupitre.shading.color = { 1.0,1.0,1.0 };

	kartPupitre.texture = opengl_load_texture_image(nFileFrontKart, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	//Lien Avec les roues ( cylindrique)
	lienKtReArD.initialize(mesh_primitive_cylinder(0.1 * largeur_kart, vec3(0, 0, 0), vec3(0, 0.1 * largeur_kart, 0)), "lienKtReArD");
	lienKtReArD.shading.color = { 0,0,0 };
	lienKtReArG.initialize(mesh_primitive_cylinder(0.1 * largeur_kart, vec3(0, 0, 0), vec3(0, 0.1 * largeur_kart, 0)), "lienKtReArG");
	lienKtReArG.shading.color = { 0,0,0 };
	lienKtReAvtD.initialize(mesh_primitive_cylinder(0.05 * largeur_kart, vec3(0, 0, 0), vec3(0, 0.05 * largeur_kart, 0)), "lienKtReAvtD");
	lienKtReAvtD.shading.color = { 0,0,0 };
	lienKtReAvtG.initialize(mesh_primitive_cylinder(0.05 * largeur_kart, vec3(0, 0, 0), vec3(0, 0.05 * largeur_kart, 0)), "lienKtReAvtG");
	lienKtReAvtG.shading.color = { 0,0,0 };
	//Roues 
	ReArD.initialize(mesh_primitive_cylinder(hauteur_kart, vec3(0, 0, 0), vec3(0, largeur_roue_arriere, 0), 10, 20, false), "ReArD");
	ReArD.texture = opengl_load_texture_image("assets/tireBis.png", GL_REPEAT, GL_REPEAT);
	discRoueArD.initialize(mesh_primitive_disc(hauteur_kart, vec3(0, 0, 0), vec3(0, 1, 0)), "discRoueArD");
	discRoueArD.texture = opengl_load_texture_image("assets/jenteBis.png", GL_REPEAT, GL_REPEAT);
	ReArG.initialize(mesh_primitive_cylinder(hauteur_kart, vec3(0, 0, 0), vec3(0, largeur_roue_arriere, 0), 10, 20, false), "ReArG");
	ReArG.texture = opengl_load_texture_image("assets/tireBis.png", GL_REPEAT, GL_REPEAT);
	discRoueArG.initialize(mesh_primitive_disc(hauteur_kart, vec3(0, 0, 0), vec3(0, 1, 0)), "discRoueArG");
	discRoueArG.texture = opengl_load_texture_image("assets/jenteBis.png", GL_REPEAT, GL_REPEAT);
	ReAvtD.initialize(mesh_primitive_cylinder(hauteur_kart * 0.7, vec3(0, 0, 0), vec3(0, largeur_roue_avant, 0), 10, 20, false), "ReAvtD");
	ReAvtD.texture = opengl_load_texture_image("assets/tireBis.png", GL_REPEAT, GL_REPEAT);
	discRoueAvtD.initialize(mesh_primitive_disc(hauteur_kart * 0.7, vec3(0, 0, 0), vec3(0, 1, 0)), "discRoueAvtD");
	discRoueAvtD.texture = opengl_load_texture_image("assets/jenteBis.png", GL_REPEAT, GL_REPEAT);
	ReAvtG.initialize(mesh_primitive_cylinder(hauteur_kart * 0.7, vec3(0, 0, 0), vec3(0, largeur_roue_avant, 0), 10, 20, false), "ReAvtG");
	ReAvtG.texture = opengl_load_texture_image("assets/tireBis.png", GL_REPEAT, GL_REPEAT);
	discRoueAvtG.initialize(mesh_primitive_disc(hauteur_kart * 0.7, vec3(0, 0, 0), vec3(0, 1, 0)), "discRoueAvtG");
	discRoueAvtG.texture = opengl_load_texture_image("assets/jenteBis.png", GL_REPEAT, GL_REPEAT);

	//Ajout des pieces aerodynamiques sur les c�t�s du kart (1/2 pyramides)
	sideKartD.initialize(mesh_primitive_sideKart(longueur_kart / 2.0f, 0.4f * largeur_kart, 0.8f * hauteur_kart), "sideKartD");
	sideKartD.shading.color = color2;
	sideKartG.initialize(mesh_primitive_sideKart(longueur_kart / 2.0f, 0.4f * largeur_kart, 0.8f * hauteur_kart), "sideKartG");
	sideKartG.shading.color = color2;
	//Ajout des protections des roues
	sideAvtG.initialize(mesh_primitive_cylinder_bis(largeur_roue_avant * 1.2, vec3(0, 0, 0), vec3(0, 0, hauteur_kart), 20, 20, true, 4), "sideAvtG");
	sideAvtD.initialize(mesh_primitive_cylinder_bis(largeur_roue_avant * 1.2, vec3(0, 0, 0), vec3(0, 0, hauteur_kart), 20, 20, true, 4), "sideAvtD");
	sideAvtD.shading.color = color2;
	sideAvtG.shading.color = color2;

	//Ajout du volant
	porteVolant.initialize(mesh_primitive_cylinder(dimension_volant, vec3(0, 0, 0), vec3(3 * dimension_volant, 0, 0), 10, 20, true), "porteVolant");
	centreVolant.initialize(mesh_primitive_sphere(dimension_volant), "centreVolant");
	toreVolant.initialize(mesh_primitive_torus(2.5 * dimension_volant, 0.4 * dimension_volant, vec3(0, 0, 0), vec3(1, 0, 0)), "toreVolant");
	lienCentreToreVolant.initialize(mesh_primitive_cylinder(dimension_volant * 0.4, vec3(0, 0, 0), vec3(0, 0, 2.5 * dimension_volant)), "lienCentreToreVolant");
	lienCentreToreVolant2.initialize(mesh_primitive_cylinder(dimension_volant * 0.4, vec3(0, 0, 0), vec3(0, 0, 2.5 * dimension_volant)), "lienCentreToreVolant2");
	lienCentreToreVolant3.initialize(mesh_primitive_cylinder(dimension_volant * 0.4, vec3(0, 0, 0), vec3(0, 0, 2.5 * dimension_volant)), "lienCentreToreVolant3");
	porteVolant.shading.color = { 0.5, 0.5, 0.5 };
	centreVolant.shading.color = { 0.5, 0.5, 0.5 };
	toreVolant.shading.color = { 0.5, 0.5, 0.5 };
	lienCentreToreVolant.shading.color = { 0.5, 0.5, 0.5 };
	lienCentreToreVolant2.shading.color = { 0.5, 0.5, 0.5 };
	lienCentreToreVolant3.shading.color = { 0.5, 0.5, 0.5 };
	//Ajout des turbos arri�res
	plaqueTurbo.initialize(mesh_primitive_parapede(0.15 * longueur_kart, 0.7 * largeur_kart, 0.4 * hauteur_kart), "plaqueTurbo");
	plaqueTurbo.shading.color = { 0.3,0.3,0.3 };
	porteTurbo1.initialize(mesh_primitive_cylinder(0.1 * largeur_kart, vec3(0, 0, 0), vec3(0, 0, 0.3 * largeur_kart)), "porteTurbo1");
	porteTurbo2.initialize(mesh_primitive_cylinder(0.1 * largeur_kart, vec3(0, 0, 0), vec3(0, 0, 0.3 * largeur_kart)), "porteTurbo2");
	lienTurbo1.initialize(mesh_primitive_sphere(0.1 * largeur_kart), "lienTurbo1");
	lienTurbo2.initialize(mesh_primitive_sphere(0.1 * largeur_kart), "lienTurbo2");
	turbo1.initialize(mesh_primitive_cone(0.1 * largeur_kart, 0.3 * longueur_kart, vec3(0, 0, 0), vec3(1, 0, 0)), "turbo1");
	turbo2.initialize(mesh_primitive_cone(0.1 * largeur_kart, 0.3 * longueur_kart, vec3(0, 0, 0), vec3(1, 0, 0)), "turbo2");
	porteTurbo1.shading.color = { 1.0,1.0,1.0 };
	porteTurbo2.shading.color = { 1.0,1.0,1.0 };
	lienTurbo1.shading.color = { 1.0,1.0,1.0 };
	lienTurbo2.shading.color = { 1.0,1.0,1.0 };
	turbo1.shading.color = { 0.3,0.3,0.3 };
	turbo2.shading.color = { 0.3,0.3,0.3 };
	kart.add(baseKartTheorique);
	kart.add(baseKart, "baseKartTheorique", { -longueur_kart / 2.0, -largeur_kart / 2.0,0 });
	kart.add(kartGauche, "baseKart", { longueur_kart / 4.0,largeur_kart,0 });
	kart.add(kartDroite, "baseKart", { longueur_kart / 4.0,-0.4 * largeur_kart,0 });
	kart.add(kartArriere, "baseKart", { 0.0,0,hauteur_kart });
	kart.add(kartAvant, "baseKart", { longueur_kart,0,0 });
	kart.add(kartPupitre, "baseKart", { 0.6 * longueur_kart + 1.2 * largeur_roue_avant,0,hauteur_kart });


	kart.add(lienKtReArD, "baseKart", { longueur_kart * 0.1,largeur_kart,0.8 * hauteur_kart });
	kart.add(lienKtReArG, "baseKart", { longueur_kart * 0.1,-0.1 * largeur_kart,0.8 * hauteur_kart });
	kart.add(lienKtReAvtG, "baseKart", { longueur_kart * 0.9,-0.05 * largeur_kart,hauteur_kart / 2.0 });
	kart.add(lienKtReAvtD, "baseKart", { longueur_kart * 0.9,largeur_kart,hauteur_kart / 2.0 });

	kart.add(ReArD, "lienKtReArD", { 0,0.1 * largeur_kart,0 });
	kart.add(discRoueArD, "ReArD");
	discRoueArD.name = "discRoueExtArD";
	kart.add(discRoueArD, "ReArD", { 0,largeur_roue_arriere,0 });
	kart.add(ReArG, "lienKtReArG", { 0,-largeur_roue_arriere,0 });
	kart.add(discRoueArG, "ReArG");
	discRoueArG.name = "discRoueExtArG";
	kart.add(discRoueArG, "ReArG", { 0,largeur_roue_arriere,0 });

	kart.add(ReAvtD, "lienKtReAvtD", { 0,0.05 * largeur_kart,0 });
	kart.add(discRoueAvtD, "ReAvtD");
	discRoueAvtD.name = "discRoueExtAvtD";
	kart.add(discRoueAvtD, "ReAvtD", { 0,largeur_roue_avant,0 });

	kart.add(ReAvtG, "lienKtReAvtG", { 0,-largeur_roue_avant,0 });
	kart.add(discRoueAvtG, "ReAvtG");
	discRoueAvtG.name = "discRoueExtAvtG";
	kart.add(discRoueAvtG, "ReAvtG", { 0,largeur_roue_avant,0 });

	kart.add(sideKartD, "kartDroite", { 0,0,1.5 * hauteur_kart });
	kart.add(sideKartG, "kartGauche", { 0,0,1.5 * hauteur_kart });
	kart.add(sideAvtD, "kartAvant", { 0,largeur_kart,0 });
	kart.add(sideAvtG, "kartAvant", affine_rts(rotation_transform::from_axis_angle({ 0,0,1 }, 3 * Pi / 2.0), { 0,0,0 }, 1.0));

	kart.add(porteVolant, "kartPupitre", { -3 * dimension_volant ,0.5 * largeur_kart,1.2 * hauteur_kart });
	kart.add(centreVolant, "porteVolant", { 0,0,0 });
	kart.add(toreVolant, "porteVolant");
	kart.add(lienCentreToreVolant, "centreVolant", affine_rts(rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 4.0), { 0,0,0 }, 1.0));
	kart.add(lienCentreToreVolant2, "centreVolant", affine_rts(rotation_transform::from_axis_angle({ 1,0,0 }, -Pi / 4.0), { 0,0,0 }, 1.0));
	kart.add(lienCentreToreVolant3, "centreVolant", affine_rts(rotation_transform::from_axis_angle({ 1,0,0 }, Pi), { 0,0,0 }, 1.0));

	kart.add(plaqueTurbo, "kartArriere", { 0,0.15 * largeur_kart,1.4 * hauteur_kart });
	kart.add(porteTurbo1, "plaqueTurbo", { longueur_kart / 12.0,0.7 * largeur_kart / 4.0 ,0 });
	kart.add(porteTurbo2, "plaqueTurbo", { longueur_kart / 12.0,3.0 * 0.7 * largeur_kart / 4.0,0 });
	kart.add(lienTurbo1, "porteTurbo1", { 0,0,0.3 * largeur_kart });
	kart.add(lienTurbo2, "porteTurbo2", { 0,0,0.3 * largeur_kart });
	kart.add(turbo1, "porteTurbo1", affine_rts(rotation_transform::from_axis_angle({ 0,1,0 }, Pi / 12.0), { -0.25 * longueur_kart,0,0.5 * largeur_kart }, 1.0));
	kart.add(turbo2, "porteTurbo2", affine_rts(rotation_transform::from_axis_angle({ 0,1,0 }, Pi / 12.0), { -0.25 * longueur_kart,0,0.5 * largeur_kart }, 1.0));
	//kart["baseKart"].transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, Pi / 2);

	return kart;
}