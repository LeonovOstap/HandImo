#pragma once
#include <GL/glew.h>

inline void DrawCube(Vector3f HandsColor)
{
    GLfloat vertices[] =
   {
        -1, -1, -1,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,
         1, -1, -1,    1, -1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1, -1,  1,    1, -1,  1,    1, -1, -1,
        -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
        -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
        -1, -1,  1,   -1,  1,  1,    1,  1,  1,    1, -1,  1
    };

    GLfloat colors[] =
    {
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2],
        HandsColor[0], HandsColor[1], HandsColor[2],  HandsColor[0], HandsColor[1], HandsColor[2]
    };

    static float alpha = 0;
    glScalef(1.6,1.6,1.6);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 1;
}