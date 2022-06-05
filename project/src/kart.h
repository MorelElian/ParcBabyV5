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
	void updateAccelerationKart(float pressForward);
	void updateVitesseKart(float dt);
	void udpatePositionKart(float pressForward,float dt);
	void updateOrientationKart(bool droite);
protected:
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
	float tempsDuKart;
	};
hierarchy_mesh_drawable create_kart(float longueur_kart, float largeur_kart, float hauteur_kart, float proportion, vec3 color1, vec3 color2,const char* nFileFrontKart);
