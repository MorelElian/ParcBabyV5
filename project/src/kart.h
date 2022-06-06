#pragma once
#include "cgp/cgp.hpp"
#include "trajectoire.hpp"
#include "interpolation.h"
using namespace cgp;
class Kart
{
public:
	const char* nomKart;
	const char* nFileFrontKart;
	float longueur_kart;
	float largeur_kart;
	float hauteur_kart;
	float proportion;
	float largeur_roue_arriere;
	float largeur_roue_avant;
	float dimension_volant;
	float direction;
	bool drift;
	bool collision;
	float hitbox;
	hierarchy_mesh_drawable kart;
	vec3 accelerationKart;
	vec3 vitesseKart;
	vec3 positionKart;
	vec3 orientationKart;
	Kart() = default;
	Kart(const char* _nomKart, const char* _nFileFrontKart, float _longueur_kart, float _largeur_kart, float _hauteur_kart, float _proportion, vec3 color1, vec3 color2);
	Kart(const char* _nomKart, const char* _nFileFrontKart, float _longueur_kart, float _largeur_kart, float _hauteur_kart, float _proportion, vec3 color1, vec3 color2, vec3 _positionKart);
	void faireAvancerKart(int deplacement, Trajectoire traj);
	void faireAvancerKartManuel(float& avancement, Trajectoire traj);
	void updateAccelerationKart(float pressForward, Kart* tabKart);
	void updateVitesseKart(float dt);
	void udpatePositionKart(float pressForward,float dt, Kart* tabKart);
	void updateOrientationKart(bool droite);
	vec3 detecterCollisionKart(Kart& kart);
	vec3 detecterCollisionBarriere();
	buffer<vec3> volumeEnglobant();
	buffer<vec3> getRepereAssocie();
protected:
	float tempsDuKart;
	float angleRoueAvant;
	float angleRoueArriere;
	float signeAvancement;
	float turboUtilise;
	
	};
hierarchy_mesh_drawable create_kart(float longueur_kart, float largeur_kart, float hauteur_kart, float proportion, vec3 color1, vec3 color2,const char* nFileFrontKart);
