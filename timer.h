#pragma once

#include <chrono>
#include <concepts>
#include <map>
#include <string_view>

using ns_ = std::nano;
using us_ = std::micro;
using ms_ = std::milli;
using seconds__ = std::ratio<1>;
using minutes__ = std::ratio<60>;
using hours__ = std::ratio<3600>;
template <class T = seconds__>
    requires                        //
    std::is_same_v<T, ns_> ||       //
    std::is_same_v<T, us_> ||       //
    std::is_same_v<T, ms_> ||       //
    std::is_same_v<T, seconds__> || //
    std::is_same_v<T, minutes__> || //
    std::is_same_v<T, hours__>      //

struct Timer {
#if defined(__gnu_linux__) || defined(__GNUC__)
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> t1;
#else
    const std::chrono::time_point<std::chrono::steady_clock> t1;
#endif
    const std::string_view stringView;
    static inline std::map<std::string_view, std::pair<size_t, double>> avgMap;

    constexpr Timer(std::string_view name, T = {})
        : t1 {std::chrono::high_resolution_clock::now()}
        , stringView {name} {
    }

    ~Timer() {
        using std::chrono::duration;
        using std::chrono::high_resolution_clock;

        duration<double, T> timeout {high_resolution_clock::now() - t1};

        auto& [ctr, avg] = avgMap[stringView];
        avg += timeout.count();
        ++ctr;

        /**/ if constexpr (std::is_same_v<T, ns_>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) nS", stringView.data(), timeout.count(), avg / ctr);
        else if constexpr (std::is_same_v<T, us_>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) uS", stringView.data(), timeout.count(), avg / ctr);
        else if constexpr (std::is_same_v<T, ms_>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) mS", stringView.data(), timeout.count(), avg / ctr);
        else if constexpr (std::is_same_v<T, seconds__>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) S", stringView.data(), timeout.count(), avg / ctr);
        else if constexpr (std::is_same_v<T, minutes__>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) M", stringView.data(), timeout.count(), avg / ctr);
        else if constexpr (std::is_same_v<T, hours__>)
            qDebug("\t%10s -> %1.3f (avg %1.3f) H", stringView.data(), timeout.count(), avg / ctr);
    }
};

template <class T>
Timer(std::string_view, T) -> Timer<T>;
