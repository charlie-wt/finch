#include "util.hh"


std::string binary (uint8_t num) {
    std::string res = "";

    for (int i = 0; i < 8; i++) {
        res += num % 2
            ? '1'
            : '0';
        num >>= 1;
    }

    return {res.rbegin(), res.rend()};
}
