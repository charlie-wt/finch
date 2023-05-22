#pragma once

#include <chrono>
#include <thread>


using Time = std::chrono::time_point<std::chrono::system_clock>;

inline void sleep (int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline Time now () {
    return std::chrono::system_clock::now();
}

// tiny helper, just to return the duration as a
// double (since by default it's an int).
inline std::chrono::duration<double> since (Time bef, Time aft) {
    return aft - bef;
}

// Note: the function given to this struct should
// take three params:
// * double t (total time elapsed)
// * double dt (time elapsed since last update)
// * int64_t count (number of updates elapsed)
// ...and return a boolean of whether the looping
// should now stop.
template<typename Fn>
struct UpdateLoop {
    UpdateLoop(double fps, Fn const &fn)
        : fps(fps)
        , count(0)
        , t_start()
        , t_prev()
        , body(fn) {}

    void start () {
        count = 0;
        t_start = now();
        t_prev = t_start;
        auto const spf = 1. / fps;

        while (true) {
            if (count > 0 &&
                since(t_prev, now()).count() < spf)
                continue;

            auto const this_t = now();

            bool const should_stop = body(
                since(t_start, this_t).count(),
                since(t_prev, this_t).count(),
                count);

            if (should_stop)
                break;

            t_prev = now();
            count++;
        }
    }

    double fps;
    int64_t count;
    Time t_start;
    Time t_prev;

    Fn body;
};
