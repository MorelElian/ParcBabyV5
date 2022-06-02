
#include "terrain.hpp"


using namespace cgp;

float terrain_length = 700;
int N_terrain_samples = 300;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height(float x, float y)
{
    int Nm = 8;
    vec2 m1 = {terrain_length/2.0f,terrain_length/2.0f};
    vec2 m2 = {terrain_length/2.0f,-terrain_length/2.0f};
    vec2 m3 = {-terrain_length/2.0f,terrain_length/2.0f};
    vec2 m4 = {-terrain_length/2.0f,-terrain_length/2.0f};
    
    vec2 m5 = {0,-terrain_length/2.0f};
    vec2 m6 = {0,terrain_length/2.0f};
    vec2 m7 = {-terrain_length/2.0f, 0};
    vec2 m8 = {terrain_length/2.0f, 0};

    vec2 p_i[8] = { m1, m2, m3, m4, m5, m6, m7, m8 };
    float h_i[8] = { 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f };
    float sigma_i[8] = { terrain_length/5.0f, terrain_length/5.0f, terrain_length/5.0f,terrain_length/5.0f, terrain_length/5.0f, terrain_length/5.0f,terrain_length/5.0f,terrain_length/5.0f };
    float S = 0;
    for (int i = 0; i < Nm; i++)
    {
        float d = norm(vec2(x, y) - p_i[i]) / sigma_i[i];

        float z = h_i[i] * std::exp(-d * d);
        S += z;
    }
    S += noise_perlin(vec2(x/terrain_length + 0.5f, y/terrain_length + 0.5f), 10, 1.0f, 1.3f);
    return S;
}

mesh create_terrain_mesh()
{
    int N = N_terrain_samples;
    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.uv[kv + N * ku] = {u,v};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}
std::vector<cgp::vec3> generate_positions_on_terrain(int N)
{
    std::vector<cgp::vec3> vecFinal;
    cgp::vec3 vecTest[300];
    float x, y,z;
    bool flag = false;
    for (int i = 0; i < N; i++)
    {
        flag = false;
        while (flag == false)
        {
            x = rand_interval(-0.5f, 0.5f) * terrain_length;
            y = rand_interval(-0.5f, 0.5f) * terrain_length;
            z = evaluate_terrain_height(x, y);
            flag = true;
            for (int j = 0; j < i; j++)
            {
                vec2 vect2 = vec2(vecTest[i].x,vecTest[i].y);
                if (!norm(vec2(x, y) - vect2) > 0.4 * 0.4)
                {
                    flag = false;
                }
            }
        }
        vecTest[i] = vec3(x,y,z);
        vecFinal.push_back(vec3(x, y, z));
    }
    return vecFinal;
}

