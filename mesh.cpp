#include "mesh.h"

#include "halfedge.h"

#include <iostream>
#include <iomanip> // precision()
#include <fstream>
#include <sstream>
#include <string>

namespace {
// Helper functions
}

namespace cs451 {
void Mesh::computeNormals( MeshNormalStrategy strategy ) {
    
    // Clear any existing normals.
    normals.clear();
    // Make space for a normal for each position. Initialize them to 0.
    normals.resize( positions.size(), vec3(0,0,0) );
    // Since there is a normal for each position, face_normals should be identical
    // to face_positions.
    face_normals = face_positions;
    
    // Your code goes here.
    
    // Iterate over faces.
    for( const auto& f : face_positions ) {
        
        // Compute the face normal.
        const auto n = glm::normalize( glm::cross(
            positions.at( f[1] ) - positions.at( f[0] ),
            positions.at( f[2] ) - positions.at( f[1] )
            ) );
        
        // Add the normal to the three vertices of the face.
        if( strategy == Unweighted ) {
            normals.at( f[0] ) += n;
            normals.at( f[1] ) += n;
            normals.at( f[2] ) += n;
        }
        else if( strategy == AngleWeighted ) {
            // Compute angle weights.
            vec3 weights;
            weights[0] = glm::dot(
                glm::normalize( positions.at( f[1] ) - positions.at( f[0] ) ),
                glm::normalize( positions.at( f[2] ) - positions.at( f[0] ) )
                );
            weights[1] = glm::dot(
                glm::normalize( positions.at( f[0] ) - positions.at( f[1] ) ),
                glm::normalize( positions.at( f[2] ) - positions.at( f[1] ) )
                );
            weights[2] = glm::dot(
                glm::normalize( positions.at( f[1] ) - positions.at( f[2] ) ),
                glm::normalize( positions.at( f[0] ) - positions.at( f[2] ) )
                );
            weights = glm::acos( glm::clamp( weights, -1.0f, 1.0f ) );
            
            normals.at( f[0] ) += weights[0] * n;
            normals.at( f[1] ) += weights[1] * n;
            normals.at( f[2] ) += weights[2] * n;
        }
    }
    
    // Normalize all normals.
    for( auto& n : normals ) { n = glm::normalize(n); }
}

void Mesh::computeNormalsHalfEdge( MeshNormalStrategy strategy ) {
    
    // Clear any existing normals.
    normals.clear();
    // Make space for a normal for each position. Initialize them to 0.
    normals.resize( positions.size(), vec3(0,0,0) );
    // Since there is a normal for each position, face_normals should be identical
    // to face_positions.
    face_normals = face_positions;
    
    // Build the halfedges.
    typedef HalfEdgeTriMesh::Index Index;
    HalfEdgeTriMesh::Edges edges;
    unordered_edges_from_triangles( face_positions, edges );
    HalfEdgeTriMesh halfedges;
    halfedges.build( positions.size(), face_positions, edges );
    
    // Your code goes here.
    
    // Iterate over vertices.
    for( Index vi = 0; vi < positions.size(); ++vi ) {
        // Collect incident faces.
        auto face_neighbors = halfedges.vertex_face_neighbors( vi );
        // Iterate over incident faces.
        for( const Index fi : face_neighbors ) {
            const auto& f = face_positions.at( fi );
            
            // Compute the face normal.
            const auto n = glm::normalize( glm::cross(
                positions.at( f[1] ) - positions.at( f[0] ),
                positions.at( f[2] ) - positions.at( f[1] )
                ) );
            
            // Compute the angle at vi.
            real weight = 1.0;
            if( strategy == AngleWeighted ) {
                if( vi == f[0] ) {
                    weight = glm::dot(
                        glm::normalize( positions.at( f[1] ) - positions.at( f[0] ) ),
                        glm::normalize( positions.at( f[2] ) - positions.at( f[0] ) )
                        );
                } else if( vi == f[1] ) {
                    weight = glm::dot(
                        glm::normalize( positions.at( f[0] ) - positions.at( f[1] ) ),
                        glm::normalize( positions.at( f[2] ) - positions.at( f[1] ) )
                        );
                } else if( vi == f[2] ) {
                    weight = glm::dot(
                        glm::normalize( positions.at( f[1] ) - positions.at( f[2] ) ),
                        glm::normalize( positions.at( f[0] ) - positions.at( f[2] ) )
                        );
                }
                
                // The weight is currently the cosine() of the weight. Call acos().
                weight = glm::acos( glm::clamp( weight, -1.0f, 1.0f ) );
            }
            
            // Add the normal to the vertex.
            normals.at( vi ) += weight * n;
        }
    }
    
    // Normalize all normals.
    for( auto& n : normals ) { n = glm::normalize(n); }
}

void Mesh::computeTangentBitangent() {
    using std::cerr;
    
    if( texcoords.empty() ) {
        cerr << "ERROR: Mesh::computeTangentBitangent() cannot run without texture coordinates.\n";
        return;
    }
    
    assert( face_texcoords.size() == face_positions.size() );
    
    // Clear any existing tangents and bitangents.
    tangents.clear();
    bitangents.clear();
    // Make space for a tangent and bitangent for each position. Initialize them to 0.
    tangents.resize( positions.size(), vec3(0,0,0) );
    bitangents.resize( positions.size(), vec3(0,0,0) );
    // Since there is a normal for each position, face_normals should be identical
    // to face_positions.
    face_tangents = face_positions;
    
    // Your code goes here.
    
    // Iterate over faces.

    // Compute the face normal.

    // Compute the known tangent-to-world examples from the triangle edges.
            
    // Solve for the tangent frame matrix.
    
    // Average the first column (tangent) and second column (bitangent).
    for( const auto& f:face_texcoords){
            /*const auto n = glm::normalize( glm::cross(
                positions.at(f[1])- positions.at(f[0]),
                positions.at(f[2])-positions.at(f[1])
                ) );*/
            //for(int i=0;i<=2;i++){
            auto v0=positions.at(face_positions.at(f[0])[0]);
            auto v1=positions.at(face_positions.at(f[1])[0]);
            auto v2=positions.at(face_positions.at(f[2])[0]);
            auto Uv0=texcoords.at(face_texcoords.at(f[0])[0]);
            auto Uv1=texcoords.at(face_texcoords.at(f[1])[0]);
            auto Uv2=texcoords.at(face_texcoords.at(f[2])[0]);
            auto dPos1=v1[f[1]]-v1[f[0]];
            auto dPos2=v1[f[2]]-v1[f[0]];
            auto dUV1=Uv0[f[1]]-Uv0[f[0]];
            auto dUV2=Uv0[f[2]]-Uv0[f[0]];

        }
         /*auto UV = mat3.create();
         auto UV_inverse = mat3.create();
         auto dv_matrix = mat3.create();
         UV[0] = dUV1[0];
         UV[1] = dUV1[1];
         UV[2] = 0.;
         UV[3] = dUV2[0];
         UV[4] = dUV2[1];
         UV[5] = 0.;
         UV[6] = 0.;
         UV[7] = 0.;
         UV[8] = 1.;
         mat3.invert(UV_inverse, UV);*/
    // Normalize all vectors.
    for( auto& n : tangents ) { n = glm::normalize(n); }
    for( auto& n : bitangents ) { n = glm::normalize(n); }
}

mat4 Mesh::normalizingTransformation() const {
    // Your code goes here.
    
    // The identity matrix.
    // return mat4(1.0);
    
    // If there are no positions, return the identity.
    if( positions.empty() ) {
        return mat4(1.0);
    }
    
    // Initialize the min and max to the first vertex.
    vec3 mindims = positions.front();
    vec3 maxdims = positions.front();
    // Find the min and max coordinate.
    for( const vec3& pos : positions ) {
        mindims = glm::min( mindims, pos );
        maxdims = glm::max( maxdims, pos );
    }
    
    // The center is halfway between the min and max element.
    const vec3 center = real(0.5)*( mindims + maxdims );
    
    mat4 translate(1.0);
    translate[3] = vec4( -center, 1.0 );
    
    // The difference between max and min tells us how much to scale by.
    const vec3 diff = maxdims - mindims;
    const real maxsize = std::max( diff.x, std::max( diff.y, diff.z ) );
    
    mat4 scale(1.0);
    if( maxsize > 0.0 ) {
        scale = mat4( 2.0/maxsize );
    }
    
    // Translate then scale.
    return scale * translate;
}

void Mesh::applyTransformation( const mat4& transform ) {
    // Your code goes here.
    
    // Don't forget to multiply the normals by the inverse-transpose of `mat3(transform)`.
    // Don't forget to glm::normalize() your normals, too.
    
    for( vec3& pos : positions ) {
        pos = vec3( transform * vec4( pos, 1.0 ) );
    }
    
    const mat3 normalMatrix = glm::transpose( glm::inverse( mat3( transform ) ) );
    for( vec3& n : normals ) {
        n = glm::normalize( normalMatrix * n );
    }
}

}

namespace {
// Helper functions

struct VertexBundle {
    VertexBundle() {}
    VertexBundle( const std::string& str ) { fromString( str ); }
    void fromString( const std::string& str ) {
        std::string v_str, vt_str, vn_str;
        
        // Grab everything up to the first '/'
        auto first_slash = str.find( '/' );
        v_str = str.substr( 0, first_slash );
        
        if( first_slash != std::string::npos ) {
            auto second_slash = str.find( '/', first_slash+1 );
            if( second_slash == std::string::npos ) {
                vt_str = str.substr( first_slash+1 );
            } else {
                vt_str = str.substr( first_slash+1, second_slash-(first_slash+1) );
                vn_str = str.substr( second_slash+1 );
            }
        }
        
        if( !v_str.empty()  ) v  = std::stoi(v_str);
        if( !vt_str.empty() ) vt = std::stoi(vt_str);
        if( !vn_str.empty() ) vn = std::stoi(vn_str);
    }
    
    int v = 0;
    int vt = 0;
    int vn = 0;
};

}

namespace cs451 {

void Mesh::clear() {
    positions.clear();
    face_positions.clear();
    
    normals.clear();
    face_normals.clear();
    
    texcoords.clear();
    face_texcoords.clear();
}

// Wikipedia has a nice definition of the Wavefront OBJ file format:
//    https://en.wikipedia.org/wiki/Wavefront_.obj_file
bool Mesh::loadFromOBJ( const std::string& path ) {
    using namespace std;
    
    clear();
    
    // TODO: Error checking with a printout to std::cerr and return false.
    
    // Open the file.
    ifstream mesh( path );
    if( !mesh ) {
        cerr << "ERROR: Unable to access path: " << path << '\n';
        return false;
    }
    while( !( mesh >> ws ).eof() ) {
        // Get each line.
        string line;
        getline( mesh, line );
        
        // Skip blank lines.
        if( line.empty() ) continue;
        
        istringstream linestream( line );
        
        // Get the first word.
        string first_word;
        linestream >> first_word;
        
        if( first_word == "v" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            positions.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vn" ) {
            real x,y,z;
            linestream >> x >> y >> z;
            normals.push_back( vec3( x,y,z ) );
        }
        else if( first_word == "vt" ) {
            real x,y;
            linestream >> x >> y;
            texcoords.push_back( vec2( x,y ) );
        }
        else if( first_word == "f" ) {
            std::vector< string > fb;
            fb.reserve( 4 );
            while( !( linestream >> ws ).eof() ) {
                string b;
                linestream >> b;
                fb.push_back( b );
            }
            if( fb.size() < 3 ) {
                cerr << "ERROR: Skipping a face with less than 3 vertices.\n";
                continue;
            }
            
            // OBJ files are 1-indexed
            // Negative indices subtract.
            
            std::vector< VertexBundle > vb;
            vb.resize( fb.size() );
            for( int i = 0; i < vb.size(); ++i ) {
                // Convert the string separated by slashes to integers.
                vb[i] = VertexBundle( fb[i] );
                
                // We must have positions, so they can't be zero.
                assert( vb[i].v != 0 );
                
                // If any index is negative, add it to that attribute's length.
                // Otherwise, subtract 1.
                if( vb[i].v  < 0 ) vb[i].v  += positions.size(); else vb[i].v  -= 1;
                if( vb[i].vt < 0 ) vb[i].vt += texcoords.size(); else vb[i].vt -= 1;
                if( vb[i].vn < 0 ) vb[i].vn += normals.size();   else vb[i].vn -= 1;
            }
            
            // We either have no normals/texcoords or we have normal/texcoord faces
            // in correspondence with position faces.
            assert( face_normals.size() == 0 || face_normals.size() == face_positions.size() );
            assert( face_texcoords.size() == 0 || face_texcoords.size() == face_positions.size() );
            
            // Add all the faces.
            if( vb.size() != 3 ) {
                cerr << "Triangulating a face with " << vb.size() << " vertices.\n";
            }
            for( int i = 2; i < vb.size(); ++i ) {
                // Add the position face.
                face_positions.emplace_back( vb[0].v, vb[i-1].v, vb[i].v );
                
                // If one vertex bundle has normals or texcoords,
                // they all must have normals/texcoords.
                // We have already converted the OBJ coordinates back to 0-indexing,
                // so check for -1.
                assert( ( vb[0].vn == -1 ) == ( vb[i-1].vn == -1 ) );
                assert( ( vb[0].vn == -1 ) == ( vb[i].vn == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i-1].vt == -1 ) );
                assert( ( vb[0].vt == -1 ) == ( vb[i].vt == -1 ) );
                
                if( vb[0].vn != -1 ) {
                    face_normals.emplace_back( vb[0].vn, vb[i-1].vn, vb[i].vn );
                }
                
                if( vb[0].vt != -1 ) {
                    face_texcoords.emplace_back( vb[0].vt, vb[i-1].vt, vb[i].vt );
                }
            }
        }
    }
    
    return true;
}

bool Mesh::writeToOBJ( const std::string& path ) {
    using namespace std;
    
    if( face_normals.size() > 0 && face_normals.size() != face_positions.size() ) {
        cerr << "ERROR: Faces for normals don't match faces for positions. Can't write mesh to OBJ.\n";
        return false;
    }
    if( face_texcoords.size() > 0 && face_texcoords.size() != face_positions.size() ) {
        cerr << "ERROR: Faces for texture coordinates don't match faces for positions. Can't write mesh to OBJ.\n";
        return false;
    }
    
    // Open the file.
    ofstream out( path );
    if( !out ) {
        cerr << "ERROR: Could not open file for writing: " << path << '\n';
        return false;
    }
    
    out << "# OBJ written by Mesh::writeToOBJ()\n";
    out.precision( 32 );
    
    out << '\n';
    for( const auto& v : positions ) {
        out << "v " << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';
    }
    
    out << '\n';
    for( const auto& vt : texcoords ) {
        out << "vt " << vt[0] << ' ' << vt[1] << '\n';
    }
    
    out << '\n';
    for( const auto& vn : normals ) {
        out << "vn " << vn[0] << ' ' << vn[1] << ' ' << vn[2] << '\n';
    }
    
    // OBJ's are 1-indexed, so add one to all indices.
    
    out << '\n';
    if( face_texcoords.empty() && face_normals.empty() ) {
        for( const auto& face : face_positions ) {
            out << "f " << face[0]+1 << ' ' << face[1]+1 << ' ' << face[2]+1 << '\n';
        }
    }
    else if( face_normals.empty() ) {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& t = face_texcoords.at(face_index);
            out << "f "
                << p[0]+1 << '/' << t[0]+1 << ' '
                << p[1]+1 << '/' << t[1]+1 << ' '
                << p[2]+1 << '/' << t[2]+1 << '\n';
        }
    }
    else if( face_texcoords.empty() ) {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& n = face_normals.at(face_index);
            out << "f "
                << p[0]+1 << "//" << n[0]+1 << ' '
                << p[1]+1 << "//" << n[1]+1 << ' '
                << p[2]+1 << "//" << n[2]+1 << '\n';
        }
    }
    else {
        for( int face_index = 0; face_index < face_positions.size(); ++face_index ) {
            const auto& p = face_positions.at(face_index);
            const auto& t = face_texcoords.at(face_index);
            const auto& n = face_normals.at(face_index);
            out << "f "
                << p[0]+1 << '/' << t[0]+1 << '/' << n[0]+1 << ' '
                << p[1]+1 << '/' << t[1]+1 << '/' << n[1]+1 << ' '
                << p[2]+1 << '/' << t[2]+1 << '/' << n[2]+1 << '\n';
        }
    }

    return true;
}

}
