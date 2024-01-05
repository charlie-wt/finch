#include "shader.hh"

/* TODO #temp */
#include "dbg.hh"


LitShader::LitShader (TermInfo const &t)
    : buf(t)
    , depth_buf(t), lol(false) {}

void LitShader::draw (XMesh<PosNorm> const &mesh,
                      Cam const &cam) {
    for (auto const &idx : mesh.indices) {
        std::array<PosNorm, 3> verts =
            { mesh.verts[idx[0]],
              mesh.verts[idx[1]],
              mesh.verts[idx[2]] };

        // apply origin & project
        for (auto &v : verts)
            v.pos = projected(
                v.pos + mesh.origin,
                buf, cam);

        draw(verts);
    }
}

void LitShader::draw (std::array<PosNorm, 3> verts) {
    // separate into above/below horizontal,
    // fill rows, lerp sides {in,out}wards
    std::sort(verts.begin(),
              verts.end(),
        [](PosNorm const &lhs, PosNorm const &rhs) {
            return lhs.pos.y() < rhs.pos.y();
        });

    pixel const low = verts[0].pos.to<pixel>();
    pixel const mid = verts[1].pos.to<pixel>();
    pixel const high = verts[2].pos.to<pixel>();

    double const dy = high.y() - low.y();
    int64_t const other_mid_x =
        (high.x() - low.x()) *
            ((mid.y() - low.y()) / dy)
        + low.x();
    auto const left = std::min(mid.x(), other_mid_x);
    auto const right = std::max(mid.x(), other_mid_x);

    vec3 const flow = verts[0].pos;
    vec3 const fmid = verts[1].pos;
    vec3 const fhigh = verts[2].pos;

    double const other_mid_z =
        (fhigh.z() - flow.z()) *
            ((fmid.y() - flow.y()) / dy)
        + flow.z();

    auto left_z = fmid.z();
    auto right_z = other_mid_z;
    if (left != mid.x()) {
        left_z = other_mid_z;
        right_z = fmid.z();
    }

    // low-mid
    if (low.y() != mid.y()) {
        auto const step = 1. / (mid.y() - low.y());
        auto const dl = left - low.x();
        auto const dr = right - low.x();

        auto const dlz = left_z - flow.z();
        auto const drz = right_z - flow.z();
        for (int64_t y = low.y(); y < mid.y(); y++) {
            auto const prog = (y - low.y()) * step;

            int64_t const lft = low.x() + dl * prog;
            int64_t const rgt = low.x() + dr * prog;

            auto const lftz = flow.z() + dlz * prog;
            auto const rgtz = flow.z() + drz * prog;
            auto const z_step = (rgtz - lftz) /
                (rgt - lft);
            for (int64_t x = lft; x < rgt; x++) {
                double const z =
                    lftz + (x - lft) * z_step;
                if (depth_buf.set(x, y, z)) {
                    /* buf.set(x, y); */
                    /* TODO #temp */
                    vec3 fpx { static_cast<double>(x),
                               static_cast<double>(y),
                               static_cast<double>(z) };
                    vec3 const bc = barycentric(
                        fpx, flow, fmid, fhigh);
                    vec3 const nm {
                        verts[0].norm * bc[0] +
                        verts[1].norm * bc[1] +
                        verts[2].norm * bc[2]
                    };
                    double lgt = fabs(nm.dot(vec3 {0,0,-1}));
                    if (!lol) {
                        dbg(std::to_wstring(lgt));
                        lol = true;
                    }
                    buf.set(x, y,
                            { lgt, lgt, lgt });
                }
            }
        }
    }

    // mid-high
    if (mid.y() != high.y()) {
        auto const step = 1. / (high.y() - mid.y());
        auto const dl = high.x() - left;
        auto const dr = high.x() - right;

        auto const dlz = fhigh.z() - left_z;
        auto const drz = fhigh.z() - right_z;
        for (int64_t y = mid.y(); y < high.y(); y++) {
            auto const prog = (y - mid.y()) * step;

            int64_t const lft = left + dl * prog;
            int64_t const rgt = right + dr * prog;

            auto const lftz = left_z + dlz * prog;
            auto const rgtz = right_z + drz * prog;
            auto const z_step = (rgtz - lftz) /
                (rgt - lft);
            for (int64_t x = lft; x < rgt; x++) {
                double const z =
                    lftz + (x - lft) * z_step;
                if (depth_buf.set(x, y, z)) {
                    /* buf.set(x, y); */
                    /* TODO #temp */
                    vec3 fpx { static_cast<double>(x),
                               static_cast<double>(y),
                               static_cast<double>(z) };
                    vec3 const bc = barycentric(
                        fpx, flow, fmid, fhigh);
                    vec3 const nm {
                        verts[0].norm * bc[0] +
                        verts[1].norm * bc[1] +
                        verts[2].norm * bc[2]
                    };
                    double lgt = fabs(nm.dot(vec3 {0,0,-1}));
                    buf.set(x, y,
                            { lgt, lgt, lgt });
                }
            }
        }
    }
}

void LitShader::clear () {
    buf.clear();
    depth_buf.clear();
    lol = false;
}
