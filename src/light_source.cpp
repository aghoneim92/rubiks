#include "light_source.h"

LightSource::LightSource(vec4 position, color4 ambient,
         color4 diffuse, color4 specular) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular= specular;

    this->position = position;
}

vec4  LightSource::getDirection() {
    vec4 a = normalize(this->position);
    a[3] = 0.0;
    return a;
}

color4 LightSource::getDirectionFromPoint(vec4 point) {
    return normalize(this->position - point);
}
