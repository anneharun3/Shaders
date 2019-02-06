#ifndef __glsimplescene_h__
#define __glsimplescene_h__

#include "types.h"

// Forward declarations.
// This header can't directly include any OpenGL or Qt headers,
// because it gets included by Qt and anti-Qt compilation units.
class QMouseEvent;
#include "glfwd.h"
namespace cs451 { class FileWatcher; }

#include <string>

namespace cs451 {

class GLSimpleScene
{
public:
    GLSimpleScene( const std::string& scene_path );
    ~GLSimpleScene();

    void initializeGL();
    void resizeGL( int w, int h );
    void paintGL();

    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void timerEvent( real seconds_since_creation );
    
private:
    void loadScene( const std::string& scene_path );
    
    // The object to draw.
    DrawablePtr m_drawable;
    
    std::string m_scene_path;
};

}

#endif /* __glsimplescene_h__ */
