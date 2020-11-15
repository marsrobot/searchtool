#pragma once

#include <sstream>

#undef LOG_CODE
#undef LOG_DEBUG
#undef LOG

#define LOG_CODE() std::cout << "" << std::endl;
#define LOG_INFORMATION(X) std::cout << X << std::endl;
#define LOG_IMPL(X) SimpleLogger_log_impl(stdout, CODE_LOCATION, std::string(#X), X);

void SimpleLogger_timestamp(FILE* fout);
void SimpleLogger_code_wt_timestamp(FILE* fout, CodeLocation loc);

template <typename T>
void SimpleLogger_print(FILE* fout, std::string name, T x)
{
    SimpleLogger_timestamp(fout);

    /*
	if(g_init.is_stdout_terminal()) {
		fprintf(fout, " {%s%s%s=%s%s%s, %s%s%s=%s%s%s}",
			COLOR_VARIABLE,
			name.c_str(),
			COLOR_DEFAULT,

			COLOR_VALUE,
			(::json_encode( (x) ))..c_str(),
			COLOR_DEFAULT,

			COLOR_VARIABLE,
			"type",
			COLOR_DEFAULT,

			COLOR_VALUE,
			//typeid(x).name(),
			TypeName().name<T>(x).c_str(),
			COLOR_DEFAULT);
	} else {
	*/
    std::stringstream ssx;
    ssx << x;
    std::string s = ssx.str();
    fprintf(fout,
            " %s%s%s="
            "%s%s%s, "
            "%s%s%s="
            "%s%s%s",
            COLOR_VARIABLE, name.c_str(), COLOR_DEFAULT,
            COLOR_VALUE, s.c_str(), COLOR_DEFAULT,
            COLOR_VARIABLE, "type", COLOR_DEFAULT,
            //typeid(x).name(),
            // Removed dependence on Osdebugger_ marsrobot, 2020/05/16
            //COLOR_VALUE, TypeName().name<T>(x).c_str(), COLOR_DEFAULT);
            COLOR_VALUE, typeid(T).name(), COLOR_DEFAULT);
    //}
}

/*
template <typename T>
void SimpleLogger_log_impl(FILE* fout, CodeLocation loc, T x)
{
	std::stringstream ssx;
	ssx << x;
	std::string s = ssx.str();

	SimpleLogger_timestamp(fout);
	fprintf(fout, " %s%s%s, ",
		COLOR_VALUE,
		s.c_str(),
		COLOR_DEFAULT
		);
	SimpleLogger_code_wt_timestamp(fout, loc);
}
*/

template <typename T>
void SimpleLogger_log_impl(FILE* fout, CodeLocation loc, std::string name, T x)
{
    SimpleLogger_print<T>(fout, name, x);
    SimpleLogger_code_wt_timestamp(fout, loc);
}

#define LOG_FOREACH_1(WHAT, X) WHAT(X)
#define LOG_FOREACH_2(WHAT, X, ...) \
    WHAT(X)                         \
    LOG_FOREACH_1(WHAT, __VA_ARGS__)
#define LOG_FOREACH_3(WHAT, X, ...) \
    WHAT(X)                         \
    LOG_FOREACH_2(WHAT, __VA_ARGS__)
#define LOG_FOREACH_4(WHAT, X, ...) \
    WHAT(X)                         \
    LOG_FOREACH_3(WHAT, __VA_ARGS__)
#define LOG_FOREACH_5(WHAT, X, ...) \
    WHAT(X)                         \
    LOG_FOREACH_4(WHAT, __VA_ARGS__)

#define LOG_GET_MACRO(_1, _2, _3, _4, _5, NAME, ...) NAME
#define LOG_FOR_EACH(action, ...) \
    LOG_GET_MACRO(__VA_ARGS__, LOG_FOREACH_5, LOG_FOREACH_4, LOG_FOREACH_3, LOG_FOREACH_2, LOG_FOREACH_1)(action, __VA_ARGS__)

#define LOG(X)                             \
    {                                        \
        LOG_IMPL(X); \
    }

#define LOG_BENCHMARK(X)                                                                            \
    {                                                                                               \
        double __start_LOG_BENCHMARK = DateTime().now().to_seconds();                               \
        X;                                                                                          \
        double __end_LOG_BENCHMARK = DateTime().now().to_seconds();                                 \
        fprintf(stdout, "%s: time cost: %.06f\n", #X, __end_LOG_BENCHMARK - __start_LOG_BENCHMARK); \
        LOG_CODE();                                                                                 \
    }
