#pragma once

#include <array>
#include <utility>


namespace detail {

template<typename T, size_t N>
struct vec;

template<typename T, size_t... Is>
constexpr vec<T, sizeof...(Is)>
full_vec (T val, std::index_sequence<Is...>) {
    return {((void)Is, val)...};
}

};

template<typename T, size_t N>
struct vec : std::array<T, N> {
    using Vec = vec<T, N>;
    /* using Data = std::array<T, N>; */

    /* T& operator[] (size_t idx) { */
    /*     return data[idx]; */
    /* } */

    /* T const& operator[] (size_t idx) const { */
    /*     return data[idx]; */
    /* } */

    static constexpr Vec full (T val) {
        return full_vec(
            val, std::make_index_sequence<N>());
    }

    static constexpr Vec zeros () {
        return full(0);
    }

    static constexpr Vec ones () {
        return full(1);
    }

    template<typename S>
    T dot (vec<S, N> const &other) {
        T res = this[0] * other[0];
        for (size_t i = 1; i < N; i++) {
            res += this[i] * other[i];
        }
        return res;
    }

    Vec& operator+= (T val) {
        for (auto const &e : *this)
            e += val;
        return *this;
    }
    template<typename S>
    Vec& operator+= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            this[i] += other[i];
        }
        return *this;
    }
    Vec& operator-= (T val) {
        for (auto const &e : *this)
            e -= val;
        return *this;
    }
    template<typename S>
    Vec& operator-= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            this[i] -= other[i];
        }
        return *this;
    }
    Vec& operator*= (T val) {
        for (auto const &e : *this)
            e *= val;
        return *this;
    }
    template<typename S>
    Vec& operator*= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            this[i] *= other[i];
        }
        return *this;
    }
    Vec& operator/= (T val) {
        for (auto const &e : *this)
            e /= val;
        return *this;
    }
    template<typename S>
    Vec& operator/= (vec<S, N> const &other) {
        for (size_t i = 0; i < N; i++) {
            this[i] /= other[i];
        }
        return *this;
    }

/* private: */
/*     std::array<T, N> data; */
};

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

template<typename T, typename S, size_t N>
T dot (vec<T, N> v, vec<S, N> const &other)
    { v.dot(other); return v; }

using vec2 = vec<double, 2>;
using pixel = vec<int64_t, 2>;
using vec3 = vec<double, 3>;
using vec4 = vec<double, 4>;

template<typename T, size_t R, size_t C>
struct mat {
    using Mat = mat<T, R, C>;
    using Data = std::array<std::array<T, C>, R>;

    static constexpr size_t rows = R;
    static constexpr size_t cols = C;
    static constexpr size_t numel = R * C;

    Mat& operator+= (T val) {
        for (auto &r : data) {
            for (auto &v : r)
                v += val;
        }
        return *this;
    }

    Mat& operator-= (T val) {
        for (auto &r : data) {
            for (auto &v : r)
                v -= val;
        }
        return *this;
    }

    Mat& operator*= (T val) {
        for (auto &r : data) {
            for (auto &v : r)
                v *= val;
        }
        return *this;
    }

    Mat& operator/= (T val) {
        for (auto &r : data) {
            for (auto &v : r)
                v /= val;
        }
        return *this;
    }

private:
    std::array<std::array<T, C>, R> data;
};

template<typename T, size_t R, size_t C>
mat<T, R, C> operator+ (mat<T, R, C> m, T val)
    { m += val; return m; }
/* template<typename T, typename S, size_t R, size_t C> */
/* mat<T, R, C> operator+ (mat<T, R, C> m, */
/*                         mat<S, R, C> const &other) */
/*     { m += other; return m; } */
template<typename T, size_t R, size_t C>
mat<T, R, C> operator- (mat<T, R, C> m, T val)
    { m -= val; return m; }
template<typename T, size_t R, size_t C>
mat<T, R, C> operator* (mat<T, R, C> m, T val)
    { m *= val; return m; }
template<typename T, size_t R, size_t C>
mat<T, R, C> operator/ (mat<T, R, C> m, T val)
    { m /= val; return m; }

using mat4 = mat<double, 4, 4>;
