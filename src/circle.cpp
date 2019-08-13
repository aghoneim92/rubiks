#include <circle.h>
//-----------
// FILLED CIRCLE

//constructors
FilledCircle::FilledCircle(vec4 center, GLfloat radius, int np)
{
    pointsNum = np + 1; //np + center point

    points = new vec4[pointsNum];
    normals = new vec4[pointsNum];
    points[0] = center;
    for (int i = 0; i < np; i++)
    {
        double angle = 360.0 / (np - 1) * i;
        double rad = angle / 180.0 * 3.1415;
        points[i + 1] = center + vec4(radius * cos(rad), radius * sin(rad), 0.0, 0.0);
    }
    this->init();
}

void FilledCircle::render()
{
    glBindVertexArrayAPPLE(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, pointsNum);
}

FilledCircle::~FilledCircle()
{
    delete[] points;
}

void FilledCircle::calculateNormals()
{
    vec4 l0 = points[pointsNum / 2] - points[0], l1 = points[1] - points[0];
    int i;
    vec4 normal = vec4(cross(l1, l0), 0);
    for (i = 0; i < pointsNum; i++)
        normals[i] = normal;
}