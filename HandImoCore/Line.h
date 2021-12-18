#pragma once
#include <vector>
#include <GL/glew.h>

#include "Math/MathTypes.h"

inline void DrawLine(const Vector3f& Start, const Vector3f& End,float Color[3])
{
    GLfloat vertices[] =
    {
        Start.X, Start.Y, Start.Z,
        End.X, End.Y, End.Z
    };

    GLfloat colors[] =
    {
        Color[0], Color[1], Color[2],
        Color[0], Color[1], Color[2],
    };

    static float alpha = 0;
    //glScalef(1.6,1.6,1.6);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);
    
    glDrawArrays(GL_LINES, 0, 6);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
