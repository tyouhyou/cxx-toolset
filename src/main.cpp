//#define _WLOGGER // just for test on windows

#include <locale>
#include <codecvt>
#include "cxxlog.hpp"
#include "stopwatch.hpp"
#include "strcvt.hpp"

using namespace zb;

void test_logger();
void test_wlogger();

int main()
{
    test_logger();
    test_wlogger();
}

void test_wlogger()
{
#ifdef _WLOGGER
    StopWatch sw;
    sw.start();

    SET_LOG_FILE(S2WL("test/��.txt"));
    SET_LOG_MAX_SIZE(0);

    DL << L"�Х�";
    IL << L"Info";
    WL << L"����";
    EL << L"����`";

    DF(S2WL("test/log.txt")) << L"�Х�";
    IF(L"test/log.txt") << L"Info";
    WF(L"test/log.txt") << L"����";
    EF(L"test/log.txt") << L"����`";

    std::cerr << "elapsed: " << sw.elaspsed() << " microseconds.";
#endif
}

void test_logger()
{
#ifndef _WLOGGER

    StopWatch sw;
    sw.start();

    SET_LOG_FILE(W2SL(L"test/��.txt"));
    SET_LOG_MAX_SIZE(20);

    DL << "Here you are, BUG.";
    IL << "Let me inform you.";
    WL << "Alert, paradise is falling";
    EL << "OMG, it crashed.";
    DL << "�Х�";
    IL << "Info";
    WL << "����";
    EL << "����`";

    IE << "The first group elasped: " << sw.wrap() << " microseconds";

    DF(W2SL(L"test/debug.log")) << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "The second group elasped: " << sw.wrap() << " microseconds";

    DF("test/log.txt") << "�Х�";
    IF("test/log.txt") << "Info";
    WF("test/log.txt") << "����";
    EF("test/log.txt") << "����`";

    IE << "The third group elasped: " << sw.wrap() << " microseconds";

    IE << "Totally: " << sw.elaspsed() << " microseconds.";

#endif
}