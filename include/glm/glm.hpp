#ifndef GLM_HPP
#define GLM_HPP

namespace glm
{
    struct vec3
    {
        float x, y, z;
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        float r() const { return x; }
        float g() const { return y; }
        float b() const { return z; }
    };
}
#endif