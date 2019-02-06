#version 330

// Eye-space.
in vec3 fPos;
in vec3 fNormal;
in vec2 fTexCoord;
uniform mat3 uNormalMatrix;
uniform sampler2D uDiffuseTex;
uniform struct Material {
    vec3 color;
    vec3 color_diffuse;
    float shininess;
    int bands;
} material;

struct Light {
    // Eye-space.
    vec3 position;
    float color;
    float color_ambient;
};
const int MAX_LIGHTS = 5;
uniform Light lights[ MAX_LIGHTS ];

uniform int num_lights;
uniform bool use_diffuse_texture;
// gl_FragColor is old-fashioned, but it's what WebGL 1 uses.
// From: https://stackoverflow.com/questions/9222217/how-does-the-fragment-shader-know-what-variable-to-use-for-the-color-of-a-pixel
layout(location = 0) out vec4 FragColor;

void main()
{
    // Your code goes here.
    FragColor = vec4(0.0);
    int k=0;
    while(k<num_lights){
    vec3 n=normalize(fNormal);
    vec3 lightdirec=normalize(vec3(lights[k].position-fPos));
    vec3 viewdirec=normalize(-fPos);
    float nL=max(dot(n,lightdirec),0.0);
    vec3 reflectdirec=normalize(-reflect(lightdirec,n));
    float vR=max(dot(viewdirec,reflectdirec),0.0);
    vec3 diffcol=vec3(lights[k].color*nL);
    vec3 speccol=vec3(lights[k].color*pow(vR,material.shininess));
    FragColor+=vec4((lights[k].color_ambient)+diffcol+speccol,1.0);
    k++;
    }
    FragColor=floor(FragColor*FragColor*material.bands)/(material.bands-1);
    /*vec3 K=material.color_diffuse;
    if(use_diffuse_texture){
        vec4 temp=texture(uDiffuseTex,fTexCoord);
        K=K*vec3(temp);
    }
    vec3 M=K*fdiscrete;*/
    FragColor=min(FragColor,vec4(1.0));
}
