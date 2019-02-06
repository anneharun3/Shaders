#version 330

// Eye-space.
in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;
in vec3 fTangent;
in vec3 fBitangent;

uniform mat3 uNormalMatrix;

uniform sampler2D uDiffuseTex;
uniform sampler2D uNormalTex;
uniform samplerCube uEnvironmentTex;

uniform bool use_diffuse_texture;
uniform bool use_normal_map;

uniform struct Material {
    vec3 color_ambient;
    vec3 color_diffuse;
    vec3 color_specular;
    float shininess;
    bool reflective;
    vec3 color_reflect;
    bool refractive;
    vec3 color_refract;
    float index_of_refraction;
} material;

struct Light {
    // Eye-space.
    vec3 position;
    vec3 color;
    vec3 color_ambient;
};
const int MAX_LIGHTS = 5;
uniform Light lights[ MAX_LIGHTS ];

uniform int num_lights;

// gl_FragColor is old-fashioned, but it's what WebGL 1 uses.
// From: https://stackoverflow.com/questions/9222217/how-does-the-fragment-shader-know-what-variable-to-use-for-the-color-of-a-pixel
layout(location = 0) out vec4 FragColor;

void main()
{
    // Your code goes here.       
    FragColor = vec4(1.0,0.0,1.0,1.0);
    mat3 spacemat=transpose(uNormalMatrix);
    vec3 normal=spacemat*(fNormal);
    vec3 viewdir=normalize(fPos);
    vec3 C=vec3(0.0);
    int i=0;
    while(i<num_lights){
        C+=vec3(L(lights[i]));
        i++;
    }
    vec3 reflectedviewdirection=normalize(reflect(spacemat*normalize(fPos),normal));
    vec3 reflectcolor=vec3(material.color_reflect)*vec3(texture(uEnvironmentTex,reflectedviewdirection));
    vec3 refractviewdirection=refract((spacemat)*normalize(-fPos),normal,0.5*material.index_of_refraction);
    vec3 refractcolor=vec3(material.color_refract)*vec3(texture(uEnvironmentTex,refractviewdirection));
    C+=(material.reflective?reflectcolor:vec3(0.0))+(material.refractive?refractcolor:vec3(0.0));
    FragColor=vec4(C,1.0);
}

vec3 L(Light light){
    vec3 norm=normalize(fNormal);
    vec3 tnorm=normalize(fTangent);
    vec3 btnorm=normalize(fBitangent);
    vec4 color=texture2D(uDiffuseTex,fTexCoord);
    vec3 N=normalize(texture2D(uNormalTex,fTexCoord).rgb*2.0-1.0);
    mat3 tangent_temp = mat3(tnorm,btnorm,norm);
	mat3 tangent = mat3(1.0);
	tangent[0][0]=tangent_temp[0][0];tangent[1][0]=tangent_temp[0][1];tangent[2][0]=tangent_temp[0][2];
	tangent[0][1]=tangent_temp[1][0];tangent[1][1]=tangent_temp[1][1];tangent[2][1]=tangent_temp[1][2];
	tangent[0][2]=tangent_temp[2][0];tangent[1][2]=tangent_temp[2][1];tangent[2][2]=tangent_temp[2][2];
    vec3 A=tangent*N;
    vec3 lightdirec=normalize(vec3(light.position-fPos));
    vec3 viewdirec=normalize(-fPos);    
    vec3 reflectdirec=normalize(-reflect(lightdirec,A));    
    float nL=max(dot(A,lightdirec),0.0);
    float vR=max(dot(viewdirec,reflectdirec),0.0);
    vec3 diffusecol=material.color_diffuse*light.color*nL;
    if(use_diffuse_texture){
        vec4 temp=texture(uDiffuseTex,fTexCoord);
        diffusecol=diffusecol*vec3(temp);
    }
    vec3 speccol=material.color_specular*light.color*pow(vR,material.shininess);
    vec3 ambcol=material.color_ambient*light.color_ambient;
    return ambcol+diffcol+speccol;
}
