#pragma once

#include <array>
#include <iostream>
#include <utility>


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
        /* TODO #verify: constexpr? */
        for (size_t i = 0; i < smallest; i++)
            res[i][i] = 1;
        return res;
    }

    template<typename S, size_t C2>
    mat<T, R, C2> operator% (mat<S, C, C2> const &other) const {
        auto res = mat<T, R, C2>::zeros();
        for (size_t r = 0; r < R; r++) {
            for (size_t c = 0; c < C; c++) {
                for (size_t c2 = 0; c2 < C2; c2++) {
                    res[r][c2] +=
                        data[r][c] * other[c][c2];
                }
            }
        }
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

template<typename T, typename S, size_t R, size_t C>
mat<T, R, C> operator+ (mat<T, R, C> m, S val)
    { m += val; return m; }
/* template<typename T, typename S, size_t R, size_t C> */
/* mat<T, R, C> operator+ (mat<T, R, C> m, */
/*                         mat<S, R, C> const &other) */
/*     { m += other; return m; } */
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
