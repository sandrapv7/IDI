#version 330 core

in vec3 vertex;
out vec4 color;


uniform mat4 TG;

void main()  {

    gl_Position = TG * vec4 (vertex, 1.0);
}
