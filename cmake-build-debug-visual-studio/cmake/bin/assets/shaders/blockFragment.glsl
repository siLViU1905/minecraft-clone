#version 460 core

struct Light
{
   vec3 position;

   vec3 diffuse;
   vec3 ambient;
   vec3 specular;

   vec3 color;

   float constant;
   float linear;
   float quadratic;
};

struct Material
{
   sampler2D diffuse;
   vec3 specular;
   float shininess;
};

uniform Material material;

uniform vec3 viewPos;

uniform int lightsNumber;

uniform Light lights[2];

in vec3 FragPos;

in vec3 Normal;

in vec2 texCoords;

uniform sampler2D myTexture;

out vec4 FragColor;

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 result;

   for(int i = 0; i < lightsNumber; i++) {

      vec3 ambient = lights[i].ambient * lights[i].color * vec3(texture(material.diffuse, texCoords));
      vec3 lightDir = normalize(lights[i].position - FragPos);

      vec3 reflectDir = reflect(-lightDir, norm);

      float diff = max(dot(norm, lightDir), 0.0);
      vec3 diffuse = lights[i].diffuse * lights[i].color * diff * vec3(texture(material.diffuse, texCoords));

      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

      vec3 specular = lights[i].specular * lights[i].color * spec * material.specular;

      float distance = length(lights[i].position - FragPos);
      float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));

      ambient *= attenuation;
      diffuse *= attenuation;
      specular *= attenuation;

      result += ambient + diffuse + specular;
   }

   FragColor = vec4(result, 1.0);
}