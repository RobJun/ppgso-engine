#version 400
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 lightSpaceMatrix_spot[10];
uniform int num_spot_shadows;

out vec2 texCoord;
out vec4 normal;
out vec4 FragPos;  
out vec4 FragPosLightSpace;
out vec4 FragPosLightSpace_spot[10];

void main(void){
  texCoord = TexCoord;
  normal = normalize(ModelMatrix * vec4(Normal, 0.0f));
  FragPos = vec4(ModelMatrix * vec4(Position,1.0));
  FragPosLightSpace = lightSpaceMatrix * FragPos;
  for(int i = 0;i < num_spot_shadows;i++){
	FragPosLightSpace_spot[i] = lightSpaceMatrix_spot[i] * FragPos;
  }
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}