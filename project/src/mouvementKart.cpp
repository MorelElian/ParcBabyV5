#include "mouvementKart.hpp"
using namespace cgp;
void faireAvancerKart(float avancementPrec, float& avancement, hierarchy_mesh_drawable& kart, float t_min, float t_max, keyframe_structure keyframe)
{
	//std::cout << avancement << std::endl;
	if (avancement > t_max - 0.01)
	{
		avancement = t_min + 0.1;

	}
	if (avancement < t_min + 0.005)
	{
		std::cout << "dsgfsdg" << std::endl;
		avancement = t_max - 0.011;
	}
	vec3 p = interpolation(avancement, keyframe.key_positions, keyframe.key_times);
	vec3 p_prec = interpolation(avancement -0.01, keyframe.key_positions, keyframe.key_times);
	vec3 p_p = derivee_interpolation(avancement, keyframe.key_positions, keyframe.key_times);
	vec3 p_p_prec = derivee_interpolation(avancementPrec, keyframe.key_positions, keyframe.key_times);
	//kart["baseKart"].transform.rotation =  kart["baseKart"].transform.rotation * rotation_transform::between_vector(p_p_prec / norm(p_p_prec),p_p / norm(p_p));
	
	kart["baseKart"].transform.rotation = rotation_transform::between_vector({1.0,0.0,0.0}, (p - p_prec) / norm(p - p_prec));
	kart["baseKart"].transform.translation = p;
	kart["ReAvtD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * avancement);
	kart["ReAvtG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 / 0.7 * avancement);
	kart["ReArD"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * avancement);
	kart["ReArG"].transform.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, 40 * avancement);
	
}