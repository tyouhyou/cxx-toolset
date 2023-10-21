/* *
 * stopwatch for performance measuring purpose.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
 * */

#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace zb
{

    class stopwatch
    {
    public:
        static std::shared_ptr<stopwatch> g_sw()
        {
            static std::shared_ptr<stopwatch> sw;
            static std::mutex mtx;
            std::lock_guard<std::mutex> lock(mtx);
            if (!sw)
            {
                sw = std::make_shared<stopwatch>();
            }
            return sw;
        }

        stopwatch &start()
        {
            mark_list.clear();
            startpoint = std::chrono::high_resolution_clock::now();
            lastpoint = startpoint;
            return *this;
        }

        inline long long elaspsed()
        {
            return during<std::chrono::nanoseconds>(
                startpoint,
                std::chrono::high_resolution_clock::now());
        }

        inline long long elaspsed_ms()
        {
            return during<std::chrono::milliseconds>(
                startpoint,
                std::chrono::high_resolution_clock::now());
        }

        inline long long wrap()
        {
            auto temppoint = std::chrono::high_resolution_clock::now();
            auto dur = during<std::chrono::nanoseconds>(lastpoint, temppoint);
            lastpoint = temppoint;
            return dur;
        }

        inline long long wrap_ms()
        {
            auto temppoint = std::chrono::high_resolution_clock::now();
            auto dur = during<std::chrono::milliseconds>(lastpoint, temppoint);
            lastpoint = temppoint;
            return dur;
        }

        inline void mark(const std::string &m)
        {
            mark_list[m] = std::chrono::high_resolution_clock::now();
        }

        inline long long measure(const std::string &mark)
        {
            return during<std::chrono::nanoseconds>(
                mark_list[mark],
                std::chrono::high_resolution_clock::now());
        }

        inline long long measure_ms(const std::string &mark)
        {
            return during<std::chrono::milliseconds>(
                mark_list[mark],
                std::chrono::high_resolution_clock::now());
        }

        inline long long measure(const std::string &mark1, const std::string &mark2)
        {
            return during<std::chrono::nanoseconds>(
                mark_list[mark1],
                mark_list[mark2]);
        }

        inline long long measure_ms(const std::string &mark1, const std::string &mark2)
        {
            return during<std::chrono::milliseconds>(
                mark_list[mark1],
                mark_list[mark2]);
        }

        stopwatch &reset()
        {
            return start();
        }

        ~stopwatch()
        {
            mark_list.clear();
        }

    private:
        std::chrono::high_resolution_clock::time_point startpoint;
        std::chrono::high_resolution_clock::time_point lastpoint;
        std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> mark_list;

        template <class T>
        inline long long during(
            const std::chrono::time_point<std::chrono::high_resolution_clock> &d1,
            const std::chrono::time_point<std::chrono::high_resolution_clock> &d2)
        {
            return std::chrono::duration_cast<T>(d2 - d1).count();
        }
    };

} // namespace zb