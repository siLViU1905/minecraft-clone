#include "Ray.h"

Ray::Ray(): origin(0.0f), direction(0.0f, 0.0f, -1.0f), maxDistance(100.0f)
{
    
}

Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction, float maxDistance) : origin(origin), direction(glm::normalize(direction)), maxDistance(maxDistance) {}

Ray Ray::fromCamera(const Camera &camera, float maxDistance)
{
    return Ray(camera.getPosition(), camera.getFront(), maxDistance);
}

glm::vec3 Ray::getPointAtDistance(float distance) const
{
    if (distance > maxDistance) distance = maxDistance;
    return origin + direction * distance;
}

bool Ray::intersectsAABB(const glm::vec3 &boxMin, const glm::vec3 &boxMax, float &outDistance) const
{
    glm::vec3 invDir = 1.0f / direction;
    glm::vec3 tMin = (boxMin - origin) * invDir;
    glm::vec3 tMax = (boxMax - origin) * invDir;
    
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);
    
    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);
    
    // If tNear > tFar, ray doesn't intersect AABB
    if (tNear > tFar || tFar < 0.0f) return false;
    
    outDistance = tNear >= 0.0f ? tNear : tFar;
    return outDistance <= maxDistance;
}

bool Ray::getTargetedBlock(const glm::ivec3 &worldSize, float stepSize, glm::ivec3 &outBlockPos,
                          bool (*isBlockSolid)(const glm::ivec3 &)) const
{
    glm::vec3 currentPos = origin;
        
    // Step along the ray
    for (float distance = 0.0f; distance < maxDistance; distance += stepSize) {
        currentPos = origin + direction * distance;
        
        // Convert to block coordinates
        glm::ivec3 blockPos = glm::ivec3(
            static_cast<int>(std::floor(currentPos.x)),
            static_cast<int>(std::floor(currentPos.y)),
            static_cast<int>(std::floor(currentPos.z))
        );
        
        // Check if within world bounds
        if (blockPos.x < 0 || blockPos.y < 0 || blockPos.z < 0 ||
            blockPos.x >= worldSize.x || blockPos.y >= worldSize.y || blockPos.z >= worldSize.z) {
            continue;
        }
        
        // Check if block exists at this position
        if (isBlockSolid(blockPos)) {
            outBlockPos = blockPos;
            return true;
        }
    }
    
    return false;
}

const glm::vec3& Ray::getOrigin() const
{
    return origin;
}

const glm::vec3& Ray::getDirection() const
{
    return direction;
}

float Ray::getMaxDistance() const
{
    return maxDistance;
}

void Ray::setOrigin(const glm::vec3 &origin)
{
    this->origin = origin;
}

void Ray::setDirection(const glm::vec3 &direction)
{
    this->direction = direction;
}

void Ray::setMaxDistance(float maxDistance)
{
    this->maxDistance = maxDistance;
}


