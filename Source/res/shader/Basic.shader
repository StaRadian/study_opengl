#shader vertex //vertexshader source
#version 460 core   //GLSL version 4.60.5 460

layout(location = 0) in vec4 position;    //location = index = 0

void main()
{
   gl_Position = position;
};

#shader fragment //fragmentshader source
#version 460 core   //GLSL version 4.60.5 460

layout(location = 0) out vec4 color;    //location = index = 0

uniform vec4 u_Color;

void main()
{
   color = u_Color;
};