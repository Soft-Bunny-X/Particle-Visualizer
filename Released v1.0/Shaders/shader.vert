#version 330

layout (location = 0) in vec3 pos;

out vec4 vCol;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 color;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = color;
	gl_PointSize = 30 / length(gl_Position);
}