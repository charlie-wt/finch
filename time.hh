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

using Duration = std::chrono::duration<double>;

// tiny helper, just to return the duration as a
// double (since by default it's an int).
inline Duration since (Time bef, Time aft) {
    return aft - bef;
}

// tiny helper, just to return the duration as a
// double (since by default it's an int).
inline Duration since (Time bef) {
    return since(bef, now());
}

struct LoopState {
    double avg_effective_fps () const {
        return 1. / avg_frametime.count();
    }

    std::wstring speed_report () const {
        int64_t const fps = avg_effective_fps();
        return std::to_wstring(fps) +
               L" / " +
               std::to_wstring(avg_frametime.count());
    }

    Duration avg_frametime;
};

// Note: the function given to this struct should
// take three params:
// * double t (total time elapsed)
// * double dt (time elapsed since last update)
// * int64_t count (number of updates elapsed)
// * LoopState const &
// ...and return a boolean of whether the looping
// should now stop.
template<typename Fn>
struct UpdateLoop {
    UpdateLoop(double fps, Fn const &fn)
        : fps(fps)
        , count(0)
        , t_start()
        , t_prev()
        , body(fn)
        , state() {}

    void start () {
        count = 0;
        t_start = now();
        t_prev = t_start;
        auto const spf = 1. / fps;

        while (true) {
            if (count > 0 &&
                since(t_prev).count() < spf)
                continue;

            auto const this_t = now();

            bool const should_stop = body(
                since(t_start, this_t).count(),
                since(t_prev, this_t).count(),
                count, state);

            count++;

            state.avg_frametime *= count - 1;
            state.avg_frametime += since(this_t);
            state.avg_frametime /= count;

            if (should_stop)
                break;

            t_prev = now();
        }
    }

    double fps;
    uint64_t count;
    Time t_start;
    Time t_prev;

    Fn body;

    LoopState state;
};
