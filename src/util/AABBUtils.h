#ifndef AABBUTILS_H
#define AABBUTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Types.h"

namespace AABBUtils {

    struct AABBMesh {
        ui32 vao;
        ui32 vbo;
        ui32 ibo;
        ui32 numIndices;
    };

    struct AABB {
        Vector3f min;
        Vector3f max;
        AABBMesh mesh;
    };

    struct AABBMeshData {
        Vector3f* vertices;
        i32 verticesCount;
        ui32* indices;
        i32 indicesCount;
    };

    void loadAABBMesh(const Vector3f& min, const Vector3f& max, AABBMesh& result);
    void bindAABBMesh(const AABBMesh& aabbMesh);
    void unbindAABBMesh();
    void generateAABBMeshData(const Vector3f& min, const Vector3f& max, f32 lineThickness, AABBMeshData& result);
}

#endif //AABBUTILS_H