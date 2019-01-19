#include <fstream>
#include <sstream>

#include "model.h"

namespace Device { namespace Model {
    struct VertexFormat
    {
        Vec3f position; // world space position.
        Vec3f color;    // color.
        Vec3f normal;   // normal.
    };

    static void fillVectors(
        std::vector<Vec3f>& vertices,
        std::vector<Vec3f>& normals,
        std::vector<Vec2f>& texCoords,
        std::vector<U32>& indices,
        VertexFormat* vb, U32 vbLength,
        U32* ib, U32 ibLength)
    {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        for (U32 i = 0; i < vbLength; i ++)
        {
            vertices.push_back(vb[i].position);
            normals.push_back(vb[i].normal);
            texCoords.push_back({0.0f, 0.0f});
        }

        indices.clear();
        for (U32 i = 0; i < ibLength; i ++)
        {
            indices.push_back(ib[i]);
        }
    }

    void genTriangle(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices)
    {
        static VertexFormat vb[] =
        {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{+1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{+0.0f, +1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        };

        static U32 ib[] =
        {
            0, 1, 2,
        };

        fillVectors(vertices, normals, texCoords, indices, vb, COUNT_OF(vb), ib, COUNT_OF(ib));
    }

    void genTriangles(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices)
    {
        static VertexFormat vb[] =
        {
            // position,           color,              normal
            {{-1.0f, -1.0f, +0.0f}, {1.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+0.0f, -1.0f, +0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{-0.5f, +0.0f, +0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f} },

            {{+0.0f, -1.0f, +0.0f}, {1.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+1.0f, -1.0f, +0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+0.5f, +0.0f, +0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f} },

            {{-1.0f, +0.0f, +0.0f}, {1.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+0.0f, +0.0f, +0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{-0.5f, +1.0f, +0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f} },

            {{+0.0f, +0.0f, +0.0f}, {1.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+1.0f, +0.0f, +0.0f}, {0.0f, 1.0f, 0.0f }, {0.0f, 0.0f, 1.0f} },
            {{+0.5f, +1.0f, +0.0f}, {0.0f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f} },
        };

        static U32 ib[] =
        {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
        };

        fillVectors(vertices, normals, texCoords, indices, vb, COUNT_OF(vb), ib, COUNT_OF(ib));
    }

    void genCube(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices)
    {
        static VertexFormat vb[] =
        {
            // front
            {{-0.5f, -0.5f, +0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},
            {{+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},
            {{+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, +1.0f}},
            {{-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, +1.0f}},

            // back
            {{+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
            {{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
            {{+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

            // left
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.5f, +0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},

            // right
            {{+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},
            {{+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},
            {{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f}},
            {{+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f}},

            // bottom
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
            {{-0.5f, -0.5f, +0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},

            // top
            {{-0.5f, +0.5f, +0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},
            {{+0.5f, +0.5f, +0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},
            {{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, +1.0f, 0.0f}},
            {{-0.5f, +0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, +1.0f, 0.0f}},
        };

        static U32 ib[] =
        {
            // front
            0, 1, 3,
            3, 1, 2,

            // back
            4, 5, 7,
            7, 5, 6,

            // left
            8, 9, 11,
            11, 9, 10,

            // right
            12, 13, 15,
            15, 13, 14,

            // bottom
            16, 17, 19,
            19, 17, 18,

            //// top
            20, 21, 23,
            23, 21, 22,
        };

        fillVectors(vertices, normals, texCoords, indices, vb, COUNT_OF(vb), ib, COUNT_OF(ib));
    }

    // [ref](http://www.songho.ca/opengl/gl_sphere.html)
    void genSphere(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices)
    {
        // clear memory of prev arrays
        vertices.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();

        float const radius = 1.0f;
        U32 sectorCount = 20;
        U32 stackCount = 20;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for(U32 i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(U32 j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertices.push_back({x, y, z});

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back({nx, ny, nz});

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back({s, t});
            }
        }

        // generate CCW index list of sphere triangles
        U32 k1, k2;
        for(U32 i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(U32 j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount-1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }

    // [ref](https://graphics.stanford.edu/courses/cs148-10-summer/as3/code/as3/teapot.obj)
    // TODO: fix the problem
    void genTeapot(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices)
    {
        vertices.clear();
        normals.clear();
        texCoords.clear();
        indices.clear();

        std::ifstream infile("model/teapot.obj");

        std::string line;
        while (std::getline(infile, line))
        {
            if (line.length() == 0) continue;

            std::istringstream iss(line);
            char recType;

            iss >> recType;

            if (recType == 'v')
            {
                float a, b, c;
                iss >> a >> b >> c;
                vertices.push_back({a, b, c});
                normals.push_back({a, b, c});
                texCoords.push_back({0.0f, 0.0f});
            }
            else if (recType == 'f')
            {
                U32 a, b, c;
                iss >> a >> b >> c;
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);
            }
            else
            {
                assert(0);
            }
        }

        //std::vector<std::vector<Vec3f>> vertexNormals;
        //vertexNormals.resize(vertexBuffer.size());
        //// generate normal for faces.
        //for (U32 i = 0; i < indexBuffer.size(); i += 3)
        //{
        //    U32 ia = indexBuffer[i];
        //    U32 ib = indexBuffer[i+1];
        //    U32 ic = indexBuffer[i+2];

        //    Vec3f va = vertexBuffer[ia].position;
        //    Vec3f vb = vertexBuffer[ib].position;
        //    Vec3f vc = vertexBuffer[ic].position;

        //    Vec3f ab = vb - va;
        //    Vec3f bc = vc - vb;
        //    Vec3f ca = va - vc;

        //    Vec3f nb = cross(ab, bc);
        //    Vec3f nc = cross(bc, ca);
        //    Vec3f na = cross(ca, ab);

        //    vertexNormals[ia].push_back(na);
        //    vertexNormals[ib].push_back(nb);
        //    vertexNormals[ic].push_back(nc);
        //}

        //for (U32 i = 0; i < vertexBuffer.size(); i ++)
        //{
        //    Vec3f n{0.0f, 0.0f, 0.0f};
        //    for (U32 j = 0; j < vertexNormals[i].size(); j ++)
        //    {
        //        n = n + vertexNormals[i][j];
        //    }

        //    vertexBuffer[i].normal.x = n.x / vertexNormals[i].size();
        //    vertexBuffer[i].normal.y = n.y / vertexNormals[i].size();
        //    vertexBuffer[i].normal.z = n.z / vertexNormals[i].size();

        //    // std::cout << vertexBuffer[i].normal << std::endl;
        //    std::cout << vertexNormals[i].size() << std::endl;

        //    vertexBuffer[i].normal.normalize();
        //}
    }
} // namespace Model
} // namepsace Device
