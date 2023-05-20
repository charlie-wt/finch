#include <chrono>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>


void sleep (int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

/* std::string pr (std::string const &str) { */
/*     return "\"" + str + "\""; */
/* } */

std::string pr (std::string_view str) {
    std::string res = "\"";
    res += str;
    return res +"\"";
}

std::string pr (bool const b) {
    return b ? "true" : "false";
}

std::string pr (char const c) {
    return std::string {'\'', c, '\''};
}

template<typename T,
         std::enable_if_t<std::is_arithmetic<T>::value &&
                          !std::is_same<T, bool>::value,
                          bool> = true>
std::string pr (T const t) {
    return std::to_string(t);
}

template<typename T,
         std::enable_if_t<std::is_convertible<T, std::string>::value &&
                          !std::is_same<T, std::string>::value,
                          bool> = true>
std::string pr (T const t) {
    return static_cast<std::string>(t);
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), std::string>::type
pr (std::tuple<Tp...> const &t) {
    if constexpr (sizeof... (Tp) == 0)
        return "()";
    return "";
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), std::string>::type
pr (std::tuple<Tp...> const &t) {
    std::string pref = "";
    if constexpr (I == 0)
        pref = "(";

    std::string suff = ", ";
    if constexpr (I == sizeof... (Tp) - 1)
        suff = ")";

    return pref + pr(std::get<I>(t)) + suff + pr<I + 1, Tp...>(t);
}

template<typename F, typename S>
std::string pr (std::pair<F, S> const &p) {
    return "(" + pr(p.first) + ", " + pr(p.second) + ")";
}

template<typename T>
std::string pr (std::vector<T> const &vec) {
    std::stringstream ss;
    ss << "[";
    std::string sep = "";
    for (auto const &v : vec) {
        ss << sep << pr(v);
        sep = ", ";
    }
    ss << "]";
    return ss.str();
}

namespace detail {

template<typename T>
struct is_maybe_ordered_map { static constexpr bool value = false; };
template<typename K, typename V>
struct is_maybe_ordered_map<std::map<K, V>> { static constexpr bool value = true; };
template<typename K, typename V>
struct is_maybe_ordered_map<std::unordered_map<K, V>> { static constexpr bool value = true; };

}

template<typename T,
         std::enable_if_t<detail::is_maybe_ordered_map<T>::value, bool> = true>
std::string pr (T const &m) {
    std::stringstream ss;
    ss << "{";
    std::string sep = "";
    for (auto const &[k, v] : m) {
        ss << sep << pr(k) << ": " << pr(v);
        sep = ", ";
    }
    ss << "}";
    return ss.str();
}
