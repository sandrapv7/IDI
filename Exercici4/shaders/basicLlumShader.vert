#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform mat4 posFocus;

vec3 posFocus0 = vec3(0.5,0.5,-1.0);
vec3 posFocus1 = vec3(-0.5,0.5,-1.0);
vec3 posFocus2 = vec3(0.8,0.8,0.8);

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;

out vec3 L0;
out vec3 L1;
out vec3 L2;



void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;

    vec4 posf0 = View * posFocus * vec4(posFocus0,1.0);
    vec4 posf1 = View * posFocus * vec4(posFocus1,1.0);
    vec4 posf2 = vec4(posFocus2,1.0);

    mat3 NormalMatrix = inverse(transpose(mat3(View*TG)));
    fvertex = (View * TG * vec4(vertex, 1.0)).xyz;
    fnormal = (NormalMatrix * normal);

    L0 = (posf0.xyz - fvertex);
    L1 = (posf1.xyz - fvertex);
    L2 = (posf2.xyz - fvertex);


    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
