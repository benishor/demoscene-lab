#pragma once

#include <vector>
#include <memory>
#include <GLMheaders.h>

namespace Acidrain {

enum class AnimatedValueType {
    Integer,
    Float,
    Vec3,
    Vec4,
    Quaternion
};

// sadly we cannot use union here since we have non-POD types
struct AnimatedValue {
    int         intVal;
    float       floatVal;
    glm::vec3   vec3Val;
    glm::vec4   vec4Val;
    glm::quat   quatVal;
};

struct Key {
    float           time;
    AnimatedValue   value;

    static Key* vec3Key(float t, glm::vec3 val) {
        Key* result = new Key();
        result->time = t;
        result->value.vec3Val = val;
        return result;
    }

    static Key* floatKey(float t, float val) {
        Key* result = new Key();
        result->time = t;
        result->value.floatVal = val;
        return result;
    }
};

class AnimationTrack {
public:
    void 					addKey(Key* key);
    void 					addControlledObject(void* controlledObject);

    void 					setAt(float time);

    virtual AnimatedValue 	getValue(const AnimatedValue& first, const AnimatedValue& second, float alpha) = 0;
    virtual void 			controlObject(void* controlledObject, const AnimatedValue& value) = 0;

    std::vector<std::shared_ptr<Key>> 	keys;
    std::vector<void*> 					controlledObjects;
};


} // namespace Acidrain

