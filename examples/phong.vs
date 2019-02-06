#version 330

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat3 uNormalMatrix;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
    // Your code goes here.
    gl_Position=vec4(1.0,1.0,1.0,1.0 );
    fTexCoord=vTexCoord;
    gl_Position=uProjectionMatrix*uViewMatrix*vec4(vPos,1.0 );
	fPos=vec3(uViewMatrix*vec4(vPos,1.0));
    fNormal=vec3(uNormalMatrix*vNormal);
	
}
