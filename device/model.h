#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>

#include "vmath.h"

namespace Device { namespace Model {
    void genTriangle(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices);

    void genTriangles(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices);

    void genCuoid(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices, float wx=1.5f, float wy=1.5f, float wz=1.5f);

    void genSphere(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices, float radius=1.0f, U32 sectorCount=20, U32 stackCount=20);

    void genTeapot(std::vector<Vec3f>& vertices, std::vector<Vec3f>& normals, std::vector<Vec2f>& texCoords, std::vector<U32>& indices);
} // namespace Model
} // namepsace Device

#endif // _MODEL_H_
