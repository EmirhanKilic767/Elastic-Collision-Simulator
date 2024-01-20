#version 330

in vec2 texCoord;
uniform sampler2D tex0;
out vec4 frag_colour;

void main ()
{
	frag_colour = texture(tex0, texCoord);
}

