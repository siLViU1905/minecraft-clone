#ifndef __RAY_H__
#define __RAY_H__
#include "../mine/Camera.h"

class Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    float maxDistance;

public:
    Ray();

    Ray(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance = 100.0f);

    static Ray fromCamera(const Camera &camera, float maxDistance = 100.0f);

    glm::vec3 getPointAtDistance(float distance) const;

    bool intersectsAABB(const glm::vec3 &boxMin, const glm::vec3 &boxMax, float &outDistance) const;

    bool getTargetedBlock(const glm::ivec3 &worldSize, float stepSize, glm::ivec3 &outBlockPos,
                          bool (*isBlockSolid)(const glm::ivec3 &)) const;

    const glm::vec3 &getOrigin() const;
    const glm::vec3 &getDirection() const;
    float getMaxDistance() const;

    void setOrigin(const glm::vec3 &origin);
    void setDirection(const glm::vec3 &direction);
    void setMaxDistance(float maxDistance);
};

#endif // __RAY_H__