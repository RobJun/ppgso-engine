#version 400
struct Material {
    float shininess;
    float transparency;
}; 

struct DirLight {
    sampler2D   shadowMap;
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
};


struct PointLight {
    sampler2D   shadowMap;
    vec3 position;  
    vec3 color;

    float ambient;
    float diffuse;
    float specular;
  
    float constant;
    float linear;
    float quadratic;
}; 

struct SpotLight  {
    sampler2D   shadowMap;
    vec3  position;
    vec3  direction;
    vec3 color;

    float cutOff;
    float outerCutOff;
    

    float constant;
    float linear;
    float quadratic;
  
    float ambient;
    float diffuse;
    float specular;     
};

uniform sampler2D   Texture;
//uniform sampler2D   shadowMap;
uniform Material    material;
uniform DirLight    directL;
uniform PointLight  pointL[10];
uniform SpotLight   spotLight[10];
uniform vec2        TextureOffset;
uniform vec4        viewPos;

uniform int         number_of_point_lights;
uniform int         number_of_spot_lights;

in vec2 texCoord;
in vec4 normal;
in vec4 FragPos; 
in vec4 FragPosLightSpace;
in vec4 FragPosLightSpace_spot[10];


out vec4 FragmentColor;


float ShadowCalculation(sampler2D shadowMap, vec3 lighDir,vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.025 * 1.0 - dot(lighDir,normal),0.025);
    float shadow = currentDepth > closestDepth+bias  ? 1.0 : 0.0;

    //return 0.0;
    return shadow;
}  

float ShadowCalculationSpot(sampler2D shadowMap, vec3 lighDir,vec3 normal,int i)
{
    // perform perspective divide
    vec3 projCoords = FragPosLightSpace_spot[i].xyz / FragPosLightSpace_spot[i].w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.025 * 1.0 - dot(lighDir,normal),0.0025);
    float shadow = currentDepth > closestDepth+bias  ? 1.0 : 0.0;

    return shadow;
}  


vec4 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal,lightDir), 0.0f);
    float spec =  pow(max(dot(viewDir, reflectDir),0.0f), material.shininess);
    float shadow = ShadowCalculation(light.shadowMap,lightDir, normal);
    
    float ambient  =  light.ambient;
    float diffuse  =  light.diffuse * diff*(1.0f - shadow);
    float specular = 0;
    if(dot(lightDir, normal) > 0){ 
       specular =  light.specular * spec  *(1.0f - shadow) ;
     }
     return vec4(light.color * (ambient + diffuse + specular),0.f);
}  


vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    
    // combine results
    float ambient = light.ambient;
    float diffuse = light.diffuse * diff; 
    float specular = 0;
    if(dot(normal, lightDir) > 0){ 
        specular =  light.specular * spec;//  *(1.0f - shadow) ;
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return vec4(light.color* (ambient + diffuse + specular),1.f);
}


vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int i)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float shadow = ShadowCalculationSpot(light.shadowMap,lightDir, normal,i);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position.xyz - fragPos.xyz);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
 
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // combine results

    //float ambient = light.ambient;
    float ambient = 0;
    float diffuse = light.diffuse *(1.0f - shadow)* diff;
    //float specular = light.specular *(1.0f - shadow)* spec;
    float specular = 0;
    if(dot(normal, lightDir) > 0){ 
      specular =  light.specular * spec  *(1.0f - shadow) ;
    }
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return vec4(light.color* (ambient + diffuse + specular),1.f);
}


void main() {

  vec3 norm = vec3(normalize(normal));
  vec3 viewDir = normalize(viewPos.xyz - FragPos.xyz);

  vec4 result = calcDirLight(directL, norm, viewDir);
  for(int i = 0; i < number_of_point_lights; i++){
      result += CalcPointLight(pointL[i], norm, FragPos.xyz, viewDir);  

   }
   //result = CalcSpotLight(spotLight, norm, FragPos, viewDir);
  for(int i = 0; i < number_of_spot_lights; i++){
       result += CalcSpotLight(spotLight[i], norm, FragPos.xyz, viewDir,i);
   }

  FragmentColor = result * vec4(texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset)); 
  FragmentColor.a = material.transparency;
}