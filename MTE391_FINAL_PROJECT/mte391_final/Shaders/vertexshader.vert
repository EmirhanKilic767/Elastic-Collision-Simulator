#version 330
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout (location = 2) in vec2 aTex;

uniform mat4 MV;
uniform mat4 P;

out vec2 texCoord;

void main ()
{
	gl_Position = P * MV * vec4(Position, 1.0);
	texCoord = aTex;
}
