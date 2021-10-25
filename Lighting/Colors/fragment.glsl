#version 330 core

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos; 
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    FragColor = vec4(objectColor, 1.0f);
}