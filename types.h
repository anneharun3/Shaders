#ifndef __types_h__
#define __types_h__

// OpenGL core profile
// #include <GL/gl3w.h>
// Just define the basic typedefs. This should be enough for headers.
// The .cpp files can include <GL/gl3w.h> as needed.
typedef void GLvoid;
typedef unsigned int GLenum;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;

// We do this to get everything glm supports.
#include <glm/glm.hpp>
// We could only include the subset we need to speed up compile times.
/*
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
*/

// M_PI, std::min(), std::max().
#include <cmath>
#include <algorithm>

#include <vector>

#include <cassert>

namespace cs451 {
    typedef float real;
    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;
    typedef glm::ivec2 ivec2;
    typedef glm::ivec3 ivec3;
    typedef glm::ivec4 ivec4;
    typedef glm::mat2 mat2;
    typedef glm::mat3 mat3;
    typedef glm::mat4 mat4;
    
    struct ray3
    {
        // point
        vec3 p;
        // direction
        vec3 d;
        
        ray3( const vec3& a_p, const vec3& a_d ) : p( a_p ), d( a_d ) {}
    };
    
    typedef ivec3 Triangle;
    typedef std::vector< Triangle > TriangleVec;
}

#endif /* __types_h__ */
