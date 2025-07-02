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
    mat4 view;
    mat4 proj;

    mat4 view_proj () const { return proj % view; }

    // transform to view-space & project.
    vec3 projected (vec3 v) const {
        vec4 const looked =
            // TODO #speed: cache view_proj
            view_proj() % vec4 {v.x(), v.y(), v.z(), 1.0};
        return (looked / looked.w()).to<vec3>();
    }
};
