#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include <stddef.h>     /* offsetof */

#include "vmath.h"
#include "semantic.h"
#include "shader.h"
#include "texture.h"
#include "rasterizer.h"
#include "pipeline.h"

#include "model.h"

using namespace Device;

static const U32 WIDTH = 1024;
static const U32 HEIGHT = 768;

void test_rasterizer()
{
    typedef Vec3f Color;
    Rasterizer rasterizer{};
    rasterizer.resize(WIDTH, HEIGHT);

    Surface<Color> colorTarget{WIDTH, HEIGHT};
    Surface<float> depthTarget{WIDTH, HEIGHT};

    struct PSInFormat
    {
        Vec3f position;  // NDC space position.
        Vec3f color;     // pixel color.
    };

    PSInFormat psInArray[] = {
        //PSInFormat{Vec3f{-1.0f, -1.0f, -1.0f}, Vec3f{1.0f, -1.0f, -1.0f}},
        //PSInFormat{Vec3f{1.0f, -1.0f, 0.0f}, Vec3f{-1.0f, 1.0f, -1.0f}},
        //PSInFormat{Vec3f{0.0f, 1.0f, 1.0f}, Vec3f{-1.0f, -1.0f, 1.0f}},

        PSInFormat{Vec3f{-1.0f,  -1.0f,  -1.0f}, Vec3f{1.0f, 0.0f, 0.0f}},
        PSInFormat{Vec3f{0.0f,  -1.0f,  0.0f},   Vec3f{0.0f, 1.0f, 0.0f}},
        PSInFormat{Vec3f{-0.5f, 0.0f,  1.0f},    Vec3f{0.0f, 0.0f, 1.0f}},

        PSInFormat{Vec3f{0.0f,  -1.0f,  -1.0f},  Vec3f{1.0f, 0.0f, 0.0f}},
        PSInFormat{Vec3f{1.0f,  -1.0f,  0.0f},   Vec3f{0.0f, 1.0f, 0.0f}},
        PSInFormat{Vec3f{0.5f, 0.0f,  1.0f},     Vec3f{0.0f, 0.0f, 1.0f}},

        PSInFormat{Vec3f{-1.0f,  0.0f,  -1.0f},  Vec3f{1.0f, 0.0f, 0.0f}},
        PSInFormat{Vec3f{0.0f,  0.0f,  0.0f},    Vec3f{0.0f, 1.0f, 0.0f}},
        PSInFormat{Vec3f{-0.5f, 1.0f,  1.0f},    Vec3f{0.0f, 0.0f, 1.0f}},

        PSInFormat{Vec3f{0.0f,  0.0f,  -1.0f},   Vec3f{1.0f, 0.0f, 0.0f}},
        PSInFormat{Vec3f{1.0f,  0.0f,  0.0f},    Vec3f{0.0f, 1.0f, 0.0f}},
        PSInFormat{Vec3f{0.5f, 1.0f,  1.0f},     Vec3f{0.0f, 0.0f, 1.0f}},
    };

    for (U32 i = 0; i < COUNT_OF(psInArray); i +=3)
    {
        Vec3f a_pos = psInArray[i+0].position;
        Vec3f b_pos = psInArray[i+1].position;
        Vec3f c_pos = psInArray[i+2].position;

        Vec3f a_color = psInArray[i+0].color;
        Vec3f b_color = psInArray[i+1].color;
        Vec3f c_color = psInArray[i+2].color;

        std::vector<BaryCentricCoff> rasterOut = rasterizer.rasterizeTriangle(
            Vec4f{a_pos.x, a_pos.y, a_pos.z, 1.0f},
            Vec4f{b_pos.x, b_pos.y, b_pos.z, 1.0f},
            Vec4f{c_pos.x, c_pos.y, c_pos.z, 1.0f});

        for (U32 j = 0; j < rasterOut.size(); j ++)
        {
            BaryCentricCoff const& coff = rasterOut[j];

            Vec3f pixelPos = interpolate(a_pos, coff.u, b_pos, coff.v, c_pos, coff.w);
            Vec3f pixelColor = interpolate(a_color, coff.u, b_color, coff.v, c_color, coff.w);
            float depth = interpolate(a_pos.z, coff.u, b_pos.z, coff.v, c_pos.z, coff.w);

            int screen_x = std::nearbyint(pixelPos.x * WIDTH);
            int screen_y = std::nearbyint(pixelPos.y * HEIGHT);

            // screen_x, screen_y is around a odd number, save to divide by 2.
            screen_x = (screen_x + WIDTH) / 2;
            screen_y = (screen_y + HEIGHT) / 2;

            colorTarget.setPixel(screen_x, screen_y, pixelColor);
            depthTarget.setPixel(screen_x, screen_y, depth);
        }
    }

    saveAsBmp("fb_color.bmp", colorTarget);
    saveAsBmp("fb_depth.bmp", depthTarget);
}

// Ref: http://www.songho.ca/opengl/gl_camera.html
// Calculate WorldView matrix according to camera postion and direction in world space.
Mat44f calcViewMatrix(Vec3f const& camPos, Vec3f const& camUp, Vec3f const& camRight)
{
    Mat44f matTrans;
    matTrans.make_identity();

    // transform cam pos to origin.
    transform(matTrans, 0.0f - camPos);

    Vec3f r = normalize(camRight);
    Vec3f u = normalize(camUp);
    Vec3f o = cross(r, u);

    //Mat44f matRot = {
    //    r.x, u.x, o.x, 0.0,
    //    r.y, u.y, o.y, 0.0,
    //    r.z, u.z, o.z, 0.0,
    //    0.0, 0.0, 0.0, 1.0,
    //};
    // the inverse of rotate matrix
    Mat44f matInverseRot = {
        r.x, r.y, r.z, 0.0,
        u.x, u.y, u.z, 0.0,
        o.x, o.y, o.z, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    Mat44f worldToView = matInverseRot * matTrans;

    return worldToView;
}

Mat44f lookAtViewMatrix(Vec3f const& target, Vec3f const& camPos, Vec3f const& camUp)
{
    Vec3f camOut = camPos - target;
    camOut.normalize();

    Vec3f camRight = cross(camUp, camOut);
    camRight.normalize();

    Vec3f refinedUp = cross(camOut, camRight);

    return calcViewMatrix(camPos, refinedUp, camRight);
}

// https://en.wikipedia.org/wiki/Orthographic_projection
// maps ((left, bottom, near), (right, top, far)) to ((-1, -1, -1), (1, 1, 1)), the clip space
Mat44f projOrthographic(float left, float right, float bottom, float top, float near, float far)
{
    Mat44f mat;

    mat[0][0] = 2.0f/(right-left);
    mat[0][1] = 0.0f;
    mat[0][2] = 0.0f;
    mat[0][3] = -(right+left)/(right-left);

    mat[1][0] = 0.0f;
    mat[1][1] = 2.0f/(top-bottom);
    mat[1][2] = 0.0f;
    mat[1][3] = -(top+bottom)/(top-bottom);

    mat[2][0] = 0.0f;
    mat[2][1] = 0.0f;
    mat[2][2] = -2.0f/(far-near);
    mat[2][3] = -(far+near)/(far-near);

    mat[3][0] = 0.0f;
    mat[3][1] = 0.0f;
    mat[3][2] = 0.0f;
    mat[3][3] = 1;

    return mat;
}

// [Ref](https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix)
// set the OpenGL perspective projection matrix
Mat44f projPerspective(
    const float &b, const float &t, const float &l, const float &r, const float &n, const float &f)
{
    Mat44f mat;

    mat[0][0] = 2 * n / (r - l);
    mat[1][0] = 0;
    mat[2][0] = 0;
    mat[3][0] = 0;

    mat[0][1] = 0;
    mat[1][1] = 2 * n / (t - b);
    mat[2][1] = 0;
    mat[3][1] = 0;

    mat[0][2] = (r + l) / (r - l);
    mat[1][2] = (t + b) / (t - b);
    mat[2][2] = -(f + n) / (f - n);
    mat[3][2] = -1;

    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = -2 * f * n / (f - n);
    mat[3][3] = 0;

    return mat;
}

Mat44f projPerspective(
    const float &angleOfView,
    const float &imageAspectRatio,
    const float &n, const float &f)
{
    float scale = std::tan(angleOfView * 0.5 * M_PI / 180) * n;
    float r = imageAspectRatio * scale;
    float l = -r;
    float t = scale;
    float b = -t;

    return projPerspective(b, t, l, r, n, f);
}


void test_fixed_pipeline()
{
    Pipeline device{};
    device.setTargetSize(WIDTH, HEIGHT);

    Shader vsShader = loadVS_Simple();
    Shader psShader = loadPS_Simple();

    // setup vs constants.
    {
        // transformation matrices
        //Vec3f camPos = {0.0f, 0.0f, 1.0f};
        //Vec3f camUp = {0.0f, 1.0f, 0.0f};
        //Vec3f camRight = {1.0f, 0.0f, 0.0f};
        // Mat44f matWorldView = calcViewMatrix(camPos, camUp, camRight);

        Vec3f targetPos{0.0, 0.0, 0.0};
        Vec3f camPos{3.0, 3.0, 5.0};
        Vec3f camUp{0.0, 1.0, 0.0};
        Mat44f matWorldView = lookAtViewMatrix(targetPos, camPos, camUp);

        //float orthWidth = 3.0f;
        //float orthHeight = orthWidth * HEIGHT / WIDTH;
        //Mat44f matProj = projOrthographic(-orthWidth/2, orthWidth/2, -orthHeight/2, orthHeight/2, -0.1, -100);
        Mat44f matProj = projPerspective(40, (float)WIDTH/HEIGHT, 1.1, 20);

        Mat44f matWorldViewProj = matProj * matWorldView;

        Mat44f* pMatWorldView = (Mat44f*)vsShader.getConstantAddr("mWorldView");
        *pMatWorldView = matWorldView;

        Mat44f* pMatWorldViewProj = (Mat44f*)vsShader.getConstantAddr("mWorldViewProj");
        *pMatWorldViewProj = matWorldViewProj;

        //Mat44f matWorld;
        //Mat44f matWorldViewIT;
        //Mat44f matViewIT;
    }

    device.setVSProgram(vsShader);
    device.setPSProgram(psShader);

    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> texCoords;
    std::vector<U32> indices;
    // setup geometries
    {
        Model::genSphere(vertices, normals, texCoords, indices);

        device.setVertexBufferChannel(Semantic::Position0, (U8*)vertices.data(), 0, sizeof(Vec3f));
        // device.setVertexBufferChannel(Semantic::Color0, (U8*)normals.data(), 0, sizeof(Vec3f));
        device.setVertexBufferChannel(Semantic::Normal0, (U8*)normals.data(), 0, sizeof(Vec3f));

        device.setVertexBufferLength(vertices.size());

        device.setIndexBuffer((U8*)indices.data(), 0, sizeof(U32), indices.size());
    }

    device.drawIndexed();

    device.present();
}

int main()
{
    // test_rasterizer();

    test_fixed_pipeline();
}

// TODO: move to driver layer or something?
struct BufferDesc
{
    enum ElementType
    {
    };

    struct Field
    {
        ElementType eleType;
        size_t offset;
    };

    std::vector<Field> fields;
    size_t stride;
    size_t size;
};

// TODO
class PrimitiveAssembler
{
    enum class PrimitiveType : unsigned char {
        Point,
        Line,
        Triangle
    };
};

