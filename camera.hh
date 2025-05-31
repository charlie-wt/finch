#pragma once

#include "geom.hh"


/* TODO #finish */
struct CamOld {
    double fov_degs;
    double dist;
};

// perspective projection
inline vec3 perspective (vec3 v, CamOld const &cam) {
    return v * cam.fov_degs / (cam.dist + v.z());
}

// perspective projection, & convert to
// screen-space.
template<typename Frame>
vec3 projected (vec3 v, Frame const &frame,
                CamOld const &cam) {
    return screen(perspective(v, cam), frame);
}

struct Cam {
    double fov_degs;
    vec3 pos;
    rotor3f orientation;

    // perspective projection
    inline vec3 perspective (vec3 v) const {
        return v * fov_degs / (pos.dist(v) + v.z());
    }

    // perspective projection, & convert to
    // screen-space.
    template<typename Frame>
    vec3 projected (vec3 v, Frame const &frame) const {
        return screen(perspective(v), frame);
    }

};
