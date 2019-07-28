#include "cube.h"

Cube::Cube(vec4 center, float width, float height, float depth)
{
    wireframe = true;
    pointsNum = 24;

    points = new vec4[pointsNum];
    normals = new vec4[pointsNum];
    /*
     *      7 +--------+ 6
     *       /        /|
     *      /        / |
     *      +--------+ |
     *      |3      2| |
     *      |        | + 5
     *      |        | /
     *      |        |/
     *      +--------+
     *      0        1
     */
    int dx[] = {-1, +1, +1, -1, -1, +1, +1, -1};
    int dy[] = {-1, -1, +1, +1, -1, -1, +1, +1};
    int dz[] = {-1, -1, -1, -1, +1, +1, +1, +1};
    vec4 v[8];
    for (int i = 0; i < 8; i++)
    {
        v[i] = center;
        v[i].x += dx[i] * width / 2;
        v[i].y += dy[i] * height / 2;
        v[i].z += dz[i] * depth / 2;
    }

    //indices array
    //NOTE: that indexed buffer objects are not used
    unsigned int indices[] = {
        //near plane
        0, 1, 1, 2, 2, 3, 3, 0,
        //far plane
        4, 5, 5, 6, 6, 7, 7, 4,
        //sides
        0, 4, 1, 5, 2, 6, 3, 7};

    for (int i = 0; i < pointsNum; i++)
        points[i] = v[indices[i]];

    this->init();
}

void Cube::calculateNormals()
{
    //wireframe cubes aren't shaded
    //no need for normals calculations
}

void Cube::render()
{
    glBindVertexArrayAPPLE(vao);
    glDrawArrays(GL_LINES, 0, pointsNum);
}

Cube::~Cube()
{
    delete[] points;
    delete[] normals;
}

//--------------------------------------------------
// FILLED Cube CLASS

FilledCube::FilledCube(vec4 center, float width, float height, float depth)
{
    pointsNum = 36;
    points = new vec4[pointsNum];
    normals = new vec4[pointsNum];
    /*
     *      7 +--------+ 6
     *       /        /|
     *      /        / |
     *      +--------+ |
     *      |3      2| |
     *      |        | + 5
     *      |        | /
     *      |        |/
     *      +--------+
     *      0        1
     */

    int dx[] = {-1, +1, +1, -1, -1, +1, +1, -1};
    int dy[] = {-1, -1, +1, +1, -1, -1, +1, +1};
    int dz[] = {+1, +1, +1, +1, -1, -1, -1, -1};
    vec4 v[8];
    for (int i = 0; i < 8; i++)
    {
        v[i] = center;
        v[i].x += dx[i] * width / 2;
        v[i].y += dy[i] * height / 2;
        v[i].z += dz[i] * depth / 2;
    }

    //indices array
    //triangles indices (triangles that form the cube faces)
    // NOTE: IBO are not used here for simplicity (vertices are repeated so we
    // can have normal per vertex) indices array is just used to simplify
    // vertices creation
    // NOTE2: indices are arranged such that the cross product of any two lines
    // in a triangle gives a normal to the triangle outwards
    unsigned int indices[] = {
        //near plane
        0, 1, 2, 0, 2, 3,
        //far plane
        4, 6, 5, 4, 7, 6,
        //up plane
        3, 2, 6, 3, 6, 7,
        //bottom plane
        0, 5, 1, 0, 4, 5,
        //right side
        1, 5, 2, 2, 5, 6,
        //left side
        0, 3, 4, 3, 7, 4};

    for (int i = 0; i < pointsNum; i++)
        points[i] = v[indices[i]];

    this->init();
}

void FilledCube::calculateNormals()
{
    for (int i = 0; i < pointsNum; i += 3)
    {
        vec4 v0, v1, v2;
        v0 = points[i];
        v1 = points[i + 1];
        v2 = points[i + 2];

        vec4 n = cross(v1 - v0, v2 - v0);
        normals[i + 0] = normalize(n);
        normals[i + 1] = normalize(n);
        normals[i + 2] = normalize(n);

        normals[i + 0].w = 0;
        normals[i + 1].w = 0;
        normals[i + 2].w = 0;
    }
}

void FilledCube::render()
{
    glBindVertexArrayAPPLE(vao);
    glDrawArrays(GL_TRIANGLES, 0, pointsNum);
}

FilledCube::~FilledCube()
{
    delete[] points;
    delete[] normals;
}
