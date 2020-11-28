#include "cxxlog.hpp"
#include "stopwatch.hpp"

using namespace th_util;

int main()
{
    th_util::StopWatch sw;
    SET_LOGFILE("test/log.txt");

    sw.start();

    D << "Here you are, BUG.";
    I << "Let me inform you.";
    W << "Alert, paradise is falling";
    E << "OMG, it crashed.";

    IE << "first 4 logs elasped: " << sw.perf() << " microseconds";

    DF("test/debug.log") << "This is debug message.";
    IF("test/info.log") << "Information for you.";
    WF("test/warn.log") << "I warn you.";
    EF("test/error.log") << "An error message goes here.";

    IE << "second 4 logs elasped: " << sw.perf() << " microseconds";

    DF("test/debug2.log") << "This is debug message again.";
    IF("test/info2.log") << "Information for you again.";
    WF("test/warn2.log") << "I warn you again.";
    EF("test/error2.log") << "An error message goes here again.";

    IE << "third 4 logs elasped: " << sw.perf() << " microseconds";
    IE << "lap: " << sw.lap() << " microseconds.";
}