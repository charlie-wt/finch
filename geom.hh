#pragma once

#include "camera.hh"

#include <array>
#include <cmath>
#include <iostream>
#include <sstream>
#include <utility>


inline double deg2rad (double deg)
    { return deg * (M_PI / 180.); }

inline double rad2deg (double rad)
    { return rad * (180. / M_PI); }

template<typename T, size_t N>
struct vec : std::array<T, N> {
    using type = T;
    inline static constexpr size_t size = N;

    using Vec = vec<T, N>;

    static constexpr Vec full (T val) {
        return full_vec(
            val, std::make_index_sequence<N>());
    }

    static constexpr Vec zeros ()
        { return full(0); }

    static constexpr Vec ones ()
        { return full(1); }

    T length () const {
        T res = 0;
        for (auto const v : *this)
            res += v * v;
        return std::sqrt(res);
    }

    T sum () const {
        T res;
        for (auto const v : *this)
            res += v;
        return res;
    }

    void norm (T length = 1.0) {
        auto const l = this->length();
        if (l)
            *this *= length/l;
    }

    template<typename S,
             std::enable_if_t<N != 0, bool> = true>
    T dot (vec<S, N> const &other) const {
        T res = (*this)[0] * other[0];
        for (size_t i = 1; i < N; i++)
            res += (*this)[i] * other[i];
        return res;
    }

    template<typename S,
             size_t M,
             std::enable_if_t<N >= M, bool> = true>
    vec<S, M> cast () const {
        vec<S, M> res;
        for (size_t i = 0; i < M; i++)
            res[i] = static_cast<S>((*this)[i]);
        return res;
    }

    template<typename S>
    vec<S, N> cast () const
        { return cast<S, N>(); }

    template<typename Q>
    Q to () const
        { return cast<typename Q::type, Q::size>(); }

    // need D simply because enable_if only works
    // based on template params for the method,
    // whereas the actual condition is only based
    // on template params for the class. just use
    // D for a simple `&& true` in the condition.
    template<typename D = T,
             std::enable_if_t<N >= 1 && std::is_same<D, T>::value, bool> = true>
    T& x () { return (*this)[0]; }
    template<typename D = T,
             std::enable_if_t<N >= 1 && std::is_same<D, T>::value, bool> = true>
    T const & x () const { return (*this)[0]; }
    template<typename D = T,
             std::enable_if_t<N >= 2 && std::is_same<D, T>::value, bool> = true>
    T& y () { return (*this)[1]; }
    template<typename D = T,
             std::enable_if_t<N >= 2 && std::is_same<D, T>::value, bool> = true>
    T const & y () const { return (*this)[1]; }
    template<typename D = T,
             std::enable_if_t<N >= 3 && std::is_same<D, T>::value, bool> = true>
    T& z () { return (*this)[2]; }
    template<typename D = T,
             std::enable_if_t<N >= 3 && std::is_same<D, T>::value, bool> = true>
    T const & z () const { return (*this)[2]; }
    T& w () { return (*this)[3]; }
    template<typename D = T,
             std::enable_if_t<N >= 4 && std::is_same<D, T>::value, bool> = true>
    T const & w () const { return (*this)[3]; }

    template<typename D = T,
             std::enable_if_t<N >= 1 && std::is_same<D, T>::value, bool> = true>
    T& r () { return (*this)[0]; }
    template<typename D = T,
             std::enable_if_t<N >= 1 && std::is_same<D, T>::value, bool> = true>
    T const & r () const { return (*this)[0]; }
    template<typename D = T,
             std::enable_if_t<N >= 2 && std::is_same<D, T>::value, bool> = true>
    T& g () { return (*this)[1]; }
    template<typename D = T,
             std::enable_if_t<N >= 2 && std::is_same<D, T>::value, bool> = true>
    T const & g () const { return (*this)[1]; }
    template<typename D = T,
             std::enable_if_t<N >= 3 && std::is_same<D, T>::value, bool> = true>
    T& b () { return (*this)[2]; }
    template<typename D = T,
             std::enable_if_t<N >= 3 && std::is_same<D, T>::value, bool> = true>
    T const & b () const { return (*this)[2]; }
    template<typename D = T,
             std::enable_if_t<N >= 4 && std::is_same<D, T>::value, bool> = true>
    T& a () { return (*this)[3]; }
    template<typename D = T,
             std::enable_if_t<N >= 4 && std::is_same<D, T>::value, bool> = true>
    T const & a () const { return (*this)[3]; }

    /* TODO #enhancement: expression templates */
    Vec& operator+= (T val) {
        for (auto &e : *this)
            e += val;
        return *this;
    }
    template<typename S>
    Vec& operator+= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] += other[i];
        }
        return *this;
    }
    Vec& operator-= (T val) {
        for (auto &e : *this)
            e -= val;
        return *this;
    }
    template<typename S>
    Vec& operator-= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] -= other[i];
        }
        return *this;
    }
    Vec& operator*= (T val) {
        for (auto &e : *this)
            e *= val;
        return *this;
    }
    template<typename S>
    Vec& operator*= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] *= other[i];
        }
        return *this;
    }
    Vec& operator/= (T val) {
        for (auto &e : *this)
            e /= val;
        return *this;
    }
    template<typename S>
    Vec& operator/= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            (*this)[i] /= other[i];
        }
        return *this;
    }

private:
    template<size_t... Is>
    static constexpr vec<T, sizeof...(Is)>
    full_vec (T val, std::index_sequence<Is...>) {
        return {((void)Is, val)...};
    }
};

template<typename T, size_t N>
vec<T, N> norm (vec<T, N> v, T length = 1.0)
    { v.norm(length); return v; }

template<typename A, typename B>
auto cross (vec<A, 3> const &a,
            vec<B, 3> const &b) ->
        vec<decltype(std::declval<A>() *
                     std::declval<B>()),
            3> {
    return { a[1] * b[2] - a[2] * b[1],
             a[2] * b[0] - a[0] * b[2],
             a[0] * b[1] - a[1] * b[0] };
};

template<typename T, size_t N>
vec<T, N> operator+ (vec<T, N> v, T val)
    { v += val; return v; }
template<typename T, size_t N>
vec<T, N> operator+ (T val, vec<T, N> v)
    { return v + val; }
template<typename T, typename S, size_t N>
vec<T, N> operator+ (vec<T, N> v,
                     vec<S, N> const &other)
    { v += other; return v; }

template<typename T, size_t N>
vec<T, N> operator- (vec<T, N> v, T val)
    { v -= val; return v; }
template<typename T, typename S, size_t N>
vec<T, N> operator- (vec<T, N> v,
                     vec<S, N> const &other)
    { v -= other; return v; }

/* TODO #cleanup */
/* TODO #finish: the rest */
template<typename T, typename S, size_t N>
auto operator* (vec<T, N> const &v, S val) -> vec<decltype(v[0] * val), N> {
    vec<decltype(v[0] * val), N> res;
    for (size_t i = 0; i < N; i++)
        res[i] = v[i] * val;
    return res;
}
template<typename T, size_t N>
vec<T, N> operator* (T val, vec<T, N> v)
    { return v * val; }
template<typename T, typename S, size_t N>
vec<T, N> operator* (vec<T, N> v,
                     vec<S, N> const &other)
    { v *= other; return v; }

template<typename T, typename S, size_t N>
auto operator/ (vec<T, N> const &v, S val) -> vec<decltype(v[0] / val), N> {
    vec<decltype(v[0] / val), N> res;
    for (size_t i = 0; i < N; i++)
        res[i] = v[i] / val;
    return res;
}
/* template<typename T, size_t N> */
/* vec<T, N> operator/ (vec<T, N> v, T val) */
/*     { v /= val; return v; } */
template<typename T, typename S, size_t N>
vec<T, N> operator/ (vec<T, N> v,
                     vec<S, N> const &other)
    { v /= other; return v; }

template<typename T, size_t N>
std::ostream &operator<< (std::ostream &os,
                          vec<T, N> const &v) {
    os << "[";
    std::string sep = "";
    for (auto const &val : v) {
        os << sep << val;
        sep = ", ";
    }
    os << "]";
    return os;
}

template<typename T, size_t N>
std::string pr (vec<T, N> const &v) {
    std::stringstream ss;
    ss << v;
    return ss.str();
}

/* TODO #enhancement: nd `range` iterator, to eg.
 *                    get all pixels in a grid */

template<size_t N>
using vecf = vec<double, N>;

using vec2f = vecf<2>;
using vec3f = vecf<3>;
using vec4f = vecf<4>;

template<size_t N>
using veci = vec<int64_t, N>;

using vec2i = veci<2>;
using vec3i = veci<3>;
using vec4i = veci<4>;

using vec2 = vec2f;
using vec3 = vec3f;
using vec4 = vec4f;
using pixel = vec2i;

template<typename T, size_t R, size_t C>
struct mat {
    using Mat = mat<T, R, C>;
    using Data = std::array<vec<T, C>, R>;

    static constexpr size_t rows = R;
    static constexpr size_t cols = C;
    static constexpr size_t numel = R * C;

    static constexpr Mat full (T val) {
        auto const v = vec<T, C>::full(val);
        Data d;
        d.fill(v);
        return { d };
    }
    static constexpr Mat zeros ()
        { return full(0); }
    static constexpr Mat ones ()
        { return full(1); }
    static constexpr Mat eye () {
        auto res = zeros();
        size_t const smallest = R > C ? C : R;
        /* TODO #enhancement: not actually constexpr */
        for (size_t i = 0; i < smallest; i++)
            res[i][i] = 1;
        return res;
    }

    vec<T, C>& operator[] (size_t idx)
        { return data[idx]; }
    vec<T, C> const& operator[] (size_t idx) const
        { return data[idx]; }

    /* TODO #finish: iterators */

    template<typename S>
    Mat& operator+= (S val) {
        for (auto &r : data) {
            for (auto &v : r)
                v += val;
        }
        return *this;
    }

    template<typename S>
    Mat& operator-= (S val) {
        for (auto &r : data) {
            for (auto &v : r)
                v -= val;
        }
        return *this;
    }

    template<typename S>
    Mat& operator*= (S val) {
        for (auto &r : data) {
            for (auto &v : r)
                v *= val;
        }
        return *this;
    }

    template<typename S>
    Mat& operator/= (S val) {
        for (auto &r : data) {
            for (auto &v : r)
                v /= val;
        }
        return *this;
    }

    Data data;
};

template<typename T, typename S,
         size_t A, size_t B, size_t C>
mat<T, A, C> operator% (mat<T, A, B> const &lhs,
                        mat<S, B, C> const &rhs) {
    auto res = mat<T, A, C>::zeros();
    for (size_t a = 0; a < A; a++) {
        for (size_t b = 0; b < B; b++) {
            for (size_t c = 0; c < C; c++) {
                res[a][c] +=
                    lhs[a][b] * rhs[b][c];
            }
        }
    }
    return res;
}

template<typename T, typename S,
         size_t A, size_t B>
mat<T, 1, B> operator% (vec<T, A> const &lhs,
                        mat<S, A, B> const &rhs) {
    mat<T, 1, A> const lhs_m { { lhs } };
    return lhs_m % rhs;
}

template<typename T, typename S,
         size_t A, size_t B>
vec<T, A> operator% (mat<S, A, B> const &lhs,
                     vec<T, B> const &rhs) {
    auto res = vec<T, A>::zeros();
    for (size_t a = 0; a < A; a++) {
        for (size_t b = 0; b < B; b++) {
            res[a] += lhs[a][b] * rhs[b];
        }
    }
    return res;
}

template<typename T, typename S, size_t R, size_t C>
mat<T, R, C> operator+ (mat<T, R, C> m, S val)
    { m += val; return m; }
template<typename T, typename S, size_t R, size_t C>
mat<T, R, C> operator- (mat<T, R, C> m, S val)
    { m -= val; return m; }
template<typename T, typename S, size_t R, size_t C>
mat<T, R, C> operator* (mat<T, R, C> m, S val)
    { m *= val; return m; }
template<typename T, typename S, size_t R, size_t C>
mat<T, R, C> operator/ (mat<T, R, C> m, S val)
    { m /= val; return m; }

template<typename T, size_t R, size_t C>
std::ostream &operator<< (std::ostream &os,
                          mat<T, R, C> const &m) {
    os << "[";
    std::string pref = "";
    for (auto const &row : m.data) {
        os << pref << row;
        pref = ",\n ";
    }
    os << "]";
    return os;
}

template<size_t R, size_t C>
using matf = mat<double, R, C>;

using mat2 = matf<2, 2>;
using mat3 = matf<3, 3>;
using mat4 = matf<4, 4>;

inline mat3 rotation (vec3 axis, double degs) {
    auto const a = deg2rad(degs);
    auto const sa = sin(a);
    auto const ca = cos(a);
    auto const ica = 1 - ca;

    axis.norm();
    auto const ux = axis.x();
    auto const uy = axis.y();
    auto const uz = axis.z();

    return { {
        vec3 { ca + ux * ux * ica,
               ux * uy * ica - uz * sa,
               ux * uz * ica + uy * sa },
        vec3 { uy * ux * ica + uz * sa,
               ca + uy * uy * ica,
               uy * uz * ica - ux * sa },
        vec3 { uz * ux * ica - uy * sa,
               uz * uy * ica + ux * sa,
               ca + uz * uz * ica }
    } };
}

// turn world-space coord ((0, 0) is centre of
// screen, +y is up) to screen-space ((0, 0) is
// top-left, +y is down)
template<typename Frame,
         typename T,
         size_t N, std::enable_if_t<N >= 2, bool> = true>
vec<T, N> screen (vec<T, N> v,
                  Frame const &frame) {
    v.x() =  v.x() + frame.dims.x() / 2;
    v.y() = -v.y() + frame.dims.y() / 2;
    return v;
}

// perspective projection
inline vec3 perspective (vec3 v, Cam const &cam) {
    return v * cam.fov_degs / (cam.dist + v.z());
}

// perspective projection, & convert to
// screen-space.
template<typename Frame>
vec3 projected (vec3 v, Frame const &frame,
                Cam const &cam) {
    return screen(perspective(v, cam), frame);
}

vec3 barycentric (vec3 p,
                  vec3 tri1, vec3 tri2, vec3 tri3);
