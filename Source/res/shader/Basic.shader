#shader vertex //vertexshader source
#version 460 core   //GLSL version 4.60.5 460

layout(location = 0) in vec4 position;    //location = index = 0
layout(location = 1) in vec2 texCoord;    //location = index = 0

out vec2 v_TexCoord;

uniform mat4 u_MVP;   //model view projection matrix

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment //fragmentshader source
#version 460 core   //GLSL version 4.60.5 460

layout(location = 0) out vec4 color;    //location = index = 0

in vec2 v_TexCoord;

//uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor;
   //color = vec4(1);
};