#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <utility>


inline double deg2rad (double deg)
    { return deg * (M_PI / 180.); }

inline double rad2deg (double rad)
    { return rad * (180. / M_PI); }

template<typename T, size_t N>
struct vec : std::array<T, N> {
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

    void norm () {
        *this /= this->length();
    }

    template<typename S>
    T dot (vec<S, N> const &other) {
        T res = (*this)[0] * other[0];
        for (size_t i = 1; i < N; i++) {
            res += (*this)[i] * other[i];
        }
        return res;
    }

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

template<typename T, typename S, size_t N>
T dot (vec<T, N> v, vec<S, N> const &other)
    { v.dot(other); return v; }

template<typename T, size_t N>
vec<T, N> operator+ (vec<T, N> v, T val)
    { v += val; return v; }
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

template<typename T, size_t N>
vec<T, N> operator* (vec<T, N> v, T val)
    { v *= val; return v; }
template<typename T, typename S, size_t N>
vec<T, N> operator* (vec<T, N> v,
                     vec<S, N> const &other)
    { v *= other; return v; }

template<typename T, size_t N>
vec<T, N> operator/ (vec<T, N> v, T val)
    { v /= val; return v; }
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

template<size_t N>
using vecf = vec<double, N>;

using vec2 = vec<double, 2>;
using pixel = vec<int64_t, 2>;
using vec3 = vec<double, 3>;
using vec4 = vec<double, 4>;

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
                     vec<T, A> const &rhs) {
    auto res = vec<T, A>::zeros();
    for (size_t a = 0; a < A; a++) {
        for (size_t b = 0; b < B; b++) {
            res[a] += lhs[a][b] * rhs[a];
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

using mat2 = mat<double, 2, 2>;
using mat3 = mat<double, 3, 3>;
using mat4 = mat<double, 4, 4>;

inline mat3 rotation (vec3 axis, double degs) {
    auto const a = deg2rad(degs);
    auto const sa = sin(a);
    auto const ca = cos(a);
    auto const ica = 1 - ca;

    axis.norm();
    auto const ux = axis[0];
    auto const uy = axis[1];
    auto const uz = axis[2];

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

template<typename Canvas>
vec3 projected (vec3 v, Canvas canvas,
                double fov, double viewer_dist) {
    auto const factor = fov / (viewer_dist + v[2]);
    v[0] *= factor;
    v[1] *= factor;
    return v;
}
