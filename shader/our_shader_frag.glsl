#version 330
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float transparency;
}; 

struct DirLight {
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 lightColor;
};


struct PointLight {
    vec4 position;  
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
  
    float constant;
    float linear;
    float quadratic;
}; 

struct FlashLight {
    vec4  position;
    vec4  direction;
    float cutOff;
};

uniform sampler2D   Texture;
uniform Material    material;
uniform DirLight    directL;
uniform PointLight  pointL;
uniform FlashLight  flashL;
uniform vec2        TextureOffset;
uniform vec4        viewPos;

in vec2 texCoord;
in vec4 normal;
in vec4 FragPos;  


out vec4 FragmentColor;


vec4 calcDirLight(DirLight light, vec4 normal, vec4 viewDir)
{
    vec4 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal,lightDir), 0.0f);
    // specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec4 ambient  = vec4(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset)) *  light.ambient ;
    vec4 diffuse  =  light.diffuse * diff * vec4(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset));
    vec4 specular =   vec4(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset)) * light.specular * spec;
    //return diffuse;
    return (ambient + diffuse + specular);
}  



void main() {

  vec4 norm = normalize(normal);
  vec4 viewDir = normalize(viewPos - FragPos);

  vec4 result = calcDirLight(directL, normal, viewDir);

  float diffuse = max(dot(normal,normalize(directL.direction)), 0.0f);


  FragmentColor = result;
  FragmentColor.a = material.transparency;
}