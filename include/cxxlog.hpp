/* *
 * A simple log utility for c++ projects (c++11 and later).
 * 
 * @author  tyouhyou    github.com/tyouhyou
 * */

#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif

#pragma region : includes

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <locale>
#include <algorithm>

#pragma endregion

/**
 * _WLOGGER macro is for using with std::wstring / std::wofstream on windows.
 * Do not define this macro on linux, otherwise compile error may occur.
 * I do not think to use wchar on linux is a good idea, so left this issue unfixed.
 * */
#if defined(_WLOGGER)
#define __t_string std::wstring
#define __t_ifstream std::wifstream
#define __t_ofstream std::wofstream
#define __t_stringstream std::wstringstream
#define __t(str) L##str
#else
#define __t_string std::string
#define __t_ifstream std::ifstream
#define __t_ofstream std::ofstream
#define __t_stringstream std::stringstream
#define __t(str) str
#endif

#define __sd __t("[DEBUG]")
#define __si __t("[INFO ]")
#define __sw __t("[WARN ]")
#define __se __t("[ERROR]")

#pragma region : log leve defines.set LOG_LEVEL macro during compilation or before use log macros, if needed.

#define _LOG_ALL 0
#define _LOG_DEBUG 1
#define _LOG_INFO 2
#define _LOG_WARN 3
#define _LOG_ERROR 4
#define _LOG_NONE 9

#ifndef LOG_LEVEL
#if defined(DEBUG) || defined(_DEBUG)
#define LOG_LEVEL _LOG_ALL
#else
#define LOG_LEVEL _LOG_INFO
#endif
#endif

#pragma endregion

#pragma region : do not use macros define in the region.

#if defined(_MSC_VER)
#define __PATH_SEP_STR__ "\\"
#define __PATH_SEP_CHR__ '\\'
#else
#define __PATH_SEP_STR__ "/"
#define __PATH_SEP_CHR__ '/'
#endif

#define __FILENAME__ strrchr(__PATH_SEP_STR__ __FILE__, __PATH_SEP_CHR__) + 1
#define __CODE_INFO__ __t("[") << __FILENAME__ << __t("(") << __LINE__ << __t(")") << __t("::") << __func__ << __t("]")

#define _V(lg, lv, lvstr) \
    if (LOG_LEVEL > lv)   \
    {                     \
    }                     \
    else                  \
        lg << lvstr << __t(" ")
#define _VC(lg, lv, lvstr) \
    if (LOG_LEVEL > lv)    \
    {                      \
    }                      \
    else                   \
        lg << lvstr << __CODE_INFO__ << __t(" ")

#pragma endregion

#pragma region : macros outputting logs.USE THE MACROS DEFINED IN THIS REGION ONLY.

#define SET_LOG_FILE(f) th_util::logger::set_g_log_file(f)
#define SET_LOG_MAX_SIZE(lz) th_util::logger::get_log_max_size(lz)

#define DL _VC((*th_util::logger::get_g_logger()), _LOG_DEBUG, __sd)
#define IL _V((*th_util::logger::get_g_logger()), _LOG_INFO, __si)
#define WL _V((*th_util::logger::get_g_logger()), _LOG_WARN, __sw)
#define EL _VC((*th_util::logger::get_g_logger()), _LOG_ERROR, __se)

#define DF(f) _VC((th_util::logger(f)), _LOG_DEBUG, __sd)
#define IF(f) _V((th_util::logger(f)), _LOG_INFO, __si)
#define WF(f) _V((th_util::logger(f)), _LOG_WARN, __sw)
#define EF(f) _VC((th_util::logger(f)), _LOG_ERROR, __se)

#define DE _VC((th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()), _LOG_DEBUG, __sd)
#define IE _V((th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()), _LOG_INFO, __si)
#define WE _V((th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()), _LOG_WARN, __sw)
#define EE _VC((th_util::ender(std::cerr), std::cerr << th_util::util::get_cur_datetime()), _LOG_ERROR, __se)

#pragma endregion

#pragma region : Definitions of classes backing the log output macros.Do not use them directly.

namespace th_util
{
    class util
    {
    public:
        static __t_string get_cur_datetime()
        {
            auto dat = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm buf;
#if defined(_MSC_VER)
            localtime_s(&buf, &dat);
#else
            localtime_r(&dat, &buf);
#endif
            __t_stringstream ss;
            ss << std::put_time(&buf, __t("[%Y-%m-%d, %H:%M:%S]"));
            return ss.str();
        }
    };

    class logger
    {
    public:
        static void set_g_log_file(const __t_string &file)
        {
            if (file.empty())
                return;

            get_g_logger(&file);
        }

        static uint get_log_max_size(const uint &max_size = 0)
        {
            static uint _g_log_file_size = 0;
            if (max_size > 0)
            {
                _g_log_file_size = max_size;
            }
            return _g_log_file_size;
        }

        static std::shared_ptr<logger>
        get_g_logger(const __t_string *file = nullptr)
        {
            static __t_string _g_log_file;
            static std::shared_ptr<std::mutex> _g_log_locker;

            if (nullptr != file && !file->empty())
            {
                _g_log_file = *file;
                _g_log_locker = std::make_shared<std::mutex>();

                return (std::shared_ptr<logger>)nullptr;
            }

            auto lg = std::make_shared<logger>(_g_log_file, _g_log_locker);
            return lg;
        }

        logger(const __t_string &logfile, const std::shared_ptr<std::mutex> &locker = nullptr)
            : log_file{logfile}, log_lock{locker}
        {
            std::locale::global(std::locale(""));
            // TODO ? check if folder exist. If not, empty string to log file path .
        }

        template <typename T>
        logger &operator<<(const T &s)
        {
            if (ss.bad())
                return *this;
            if (ss.fail())
            {
                ss.clear();
                return *this;
            }

            ss << s;
            return *this;
        }

        ~logger()
        {
            if (log_file.empty())
                return;

            if (log_lock)
                log_lock->lock();
            try
            {
                auto max_size = get_log_max_size();
                __t_stringstream iss;
                if (max_size > 0)
                {
                    __t_ifstream ifs(log_file);
                    auto linesize = std::count(std::istreambuf_iterator<char>(ifs),
                                               std::istreambuf_iterator<char>(), '\n');
                    ifs.seekg(0);
                    __t_string ln;
                    for (int i = 0; i < linesize - max_size + 1; i++)
                    {
                        std::getline(ifs, ln);
                    }
                    iss << ifs.rdbuf();
                    ifs.close();
                }

                iss << util::get_cur_datetime()
                    << ss.str()
                    << std::endl;

                __t_ofstream ofs;
                ofs.open(log_file, std::ofstream::out | std::ofstream::trunc | std::ios::binary);
                ofs.imbue(std::locale());
                ofs << iss.rdbuf();
                ofs.close();
            }
            catch (...)
            {
                // DO NOTHING
            }
            if (log_lock)
                log_lock->unlock();
        }

    private:
        logger() = default;
        logger(const logger &) = delete;
        logger(const logger &&) = delete;
        logger &operator=(const logger &) = delete;
        logger &operator=(const logger &&) = delete;

        __t_string log_file;
        __t_stringstream ss;
        std::shared_ptr<std::mutex> log_lock;
    };

    class ender
    {
    public:
        ender(const std::ostream &os)
        {
            this->os = std::make_shared<std::ostream>(os.rdbuf());
        }
        ~ender()
        {
            *os << std::endl;
        }

    private:
        std::shared_ptr<std::ostream> os;
    };

} // namespace th_util

#pragma endregion

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
