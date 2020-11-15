#pragma once

#include <stdio.h>
#include <time.h>
#include <ctime>
#include <chrono>
#include <string>
#include <sstream>
#include "macro.h"
#include "time_util.h"

void SimpleLogger_log_time(FILE* fout)
{
	std::chrono::time_point<std::chrono::system_clock> t;
	t = std::chrono::system_clock::now();
	t = std::chrono::time_point_cast<std::chrono::microseconds>(t);
	time_t time_t_x = std::chrono::system_clock::to_time_t(t);
	std::stringstream ssx;
	ssx << std::asctime(std::localtime(&time_t_x));
	std::string s = ssx.str();
	s.erase(int64_t(s.size()) - 1); // remove the line break
	fprintf(fout, "%s%s%s", COLOR_FILE, s.c_str(), COLOR_DEFAULT);
}
void SimpleLogger_flush(FILE* fout)
{
	fflush(fout);
	fflush(stdout);
	fflush(stdout);
}

void SimpleLogger_timestamp(FILE* fout)
{
	std::string ts;
	datetime_now_as_timestamp(&ts);
	fprintf(fout, "[%s]", ts.c_str());
	return;
}

void SimpleLogger_code_wt_timestamp(FILE* fout, CodeLocation loc)
{
	// if(g_debugger.is_stdout_terminal()) {
	//fprintf(fout, " %s%s:%ld %s%s%s\n",
	//COLOR_FILE,
	//loc.filename().c_str(),
	//loc.line(),
	//COLOR_FUNCTION,
	//loc.function().c_str(),
	//COLOR_DEFAULT);
	//} else {
	fprintf(fout,
		" %s%s:%ld "
		"%s%s%s\r\n",
		COLOR_FILE,
		loc.filename().c_str(), long(loc.line()),
		COLOR_FUNCTION, loc.function().c_str(), COLOR_DEFAULT);
	//}
	SimpleLogger_flush(fout);
}

void SimpleLogger_code(FILE* fout, CodeLocation loc)
{
	SimpleLogger_timestamp(fout);
	SimpleLogger_code_wt_timestamp(fout, loc);
}

void SimpleLogger_time(FILE* fout, CodeLocation loc)
{
	SimpleLogger_log_time(fout);
	fprintf(fout, "\n");
	return;
}

void SimpleLogger_progress(FILE* fout, CodeLocation loc, double progress)
{
	progress = std::max<double>(0.0, progress);
	while (progress < 1.0) {
		int64_t barWidth = 70;
		SimpleLogger_log_time(fout);
		fprintf(fout, "[");
		int64_t pos = int64_t(barWidth * progress);
		for (int64_t i = 0; i < barWidth; ++i) {
			if (i < pos)
				fprintf(fout, "=");
			else if (i == pos)
				fprintf(fout, ">");
			else
				fprintf(fout, " ");
		}
		fprintf(fout, "] %d%%\n", int(progress * 100.0));
		fflush(fout);
		progress += 0.16; // for demonstration only
	}
	SimpleLogger_flush(fout);
}

#if 0
#define __LOG() tcerr << "Function " << COLOR_FUNCTION << __FUNC__ << COLOR_DEF << std::endl \
                      << "File " << COLOR_FILE << __FILE__ << "\t" << __LINE__ << COLOR_DEF << std::endl
//<<"\t"<< __TIME__ <<" "<< __DATE__
#endif

void SimpleLogger_log_code_impl(FILE* fout, CodeLocation loc)
{
	SimpleLogger_timestamp(fout);
	SimpleLogger_code_wt_timestamp(fout, loc);
}
