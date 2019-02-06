#version 330

// Eye-space.
in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;

uniform mat3 uNormalMatrix;

uniform sampler2D uDiffuseTex;
uniform samplerCube uEnvironmentTex;

uniform bool use_diffuse_texture;

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
    FragColor = vec4(1.0,0.0,1.0,1.0 );
    mat3 spacemat=transpose(uNormalMatrix);
    vec3 normal=spacemat*(fNormal);
    vec3 viewdir=normalize(fPos);
    vec3 C=vec3(0.0);
    int k=0;
    while(k<num_lights){
    vec3 n=normalize(fNormal);
    vec3 lightdirec=normalize(vec3(lights[k].position-fPos));
    vec3 viewdirec=normalize(-fPos);
    float nL=max(dot(n,lightdirec),0.0);
    vec3 reflectdirec=normalize(-reflect(lightdirec,n));
    float vR=max(dot(viewdirec,reflectdirec),0.0);
    vec3 diffcol=material.color_diffuse*lights[k].color*nL;
    if(use_diffuse_texture){
        vec4 t=texture(uDiffuseTex,fTexCoord);
        diffcol*=vec3(t);
    }
    vec3 speccol=material.color_specular*lights[k].color*pow(vR,material.shininess);
    vec3 ambcol=material.color_ambient*lights[k].color_ambient;
    C+=vec3(ambcol+diffcol+speccol);
    k++;
    }
    vec3 reflectedviewdirection=normalize(reflect(spacemat*normalize(fPos),normal));
    vec3 reflectcolor=vec3(material.color_reflect)*vec3(texture(uEnvironmentTex,reflectedviewdirection));
    vec3 refractviewdirection=refract((spacemat)*normalize(-fPos),normal,0.5*material.index_of_refraction);
    vec3 refractcolor=vec3(material.color_refract)*vec3(texture(uEnvironmentTex,refractviewdirection));
    C=C+(material.reflective?reflectcolor:vec3(0.0))+(material.refractive?refractcolor:vec3(0.0));
    FragColor=vec4(C,1.0);   
    
}
