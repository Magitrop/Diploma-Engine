#include "vector2.h"

#include <engine/dependencies/glm/glm/detail/func_geometric.inl>

namespace engine
{
    bool Vector2::isAlmostZero() const
    {
        static const float epsilon = 1e-5;
        return 
            fabs(x) < epsilon &&
            fabs(y) < epsilon;
    }

    float Vector2::magnitude() const
    {
        return glm::length(static_cast<Base>(*this));
    }

    Vector2 Vector2::normalized() const
    {
        return glm::normalize(*this);
    }

    void Vector2::normalize()
    {
        *this = glm::normalize(*this);
    }

    float Vector2::dot(Vector2 a, Vector2 b)
    { 
        return glm::dot(static_cast<Base>(a), static_cast<Base>(b));
    }
} // namespace engine
