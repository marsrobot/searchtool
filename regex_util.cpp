#include <string.h>
#include <string>
#include <vector>
#include "pcre.h"

#include "macro.h"
#include "regex_util.h"

std::vector<std::vector<std::string> > regex_match(std::string& regex_i, std::string string_i, int case_sensitive)
{
	const char* regex = regex_i.c_str();
	const char* str = string_i.c_str();
	int len = int(strlen(str));
	const char* error = nullptr;
	int erroffset;
	static constexpr int N = 8192;
	int ovector[N];
	pcre* re = nullptr;

	re = pcre_compile(regex, /* the pattern */
		case_sensitive ? PCRE_MULTILINE : PCRE_CASELESS | PCRE_MULTILINE,
		&error, /* for error message */
		&erroffset, /* for error offset */
		0); /* use default character tables */
	if (!re) {
		fprintf(stdout, "pcre_compile failed (offset: %ld), %s\n", erroffset, error);
		LOG(regex_i.size());
		LOG(std::string(regex_i));
		fprintf(stderr, "crash");
		exit(1);
		return std::vector<std::vector<std::string> >();
	}

	std::vector<std::vector<std::string> > vec;
	for (int offset = 0; offset < len;) {
		int rc = pcre_exec(re, 0, str, len, offset, 0, ovector, N);
		if (rc <= 0)
			break;
		std::vector<std::string> vs;
		for (int64_t i = 0; i < rc; ++i) {
			std::string s(str + ovector[2 * i], ovector[2 * i + 1] - ovector[2 * i]);
			vs.push_back(s);
		}
		offset = ovector[1];
		vec.push_back(vs);
	}

	return vec;
}

std::string string_colorize(std::string& s, std::string& regex_str)
{
	std::string s_format;
	std::string s_new;

	std::vector<std::vector<std::string> > vec = regex_match(regex_str, s, true);
	std::set<std::string> matches;
	for (int64_t i = 0; i < int64_t(vec.size()); i++)
		matches.insert(vec[i][0]);

	s_format = s;
	for (auto& match : matches) {
		std::string ttt = COLOR_RED;

		std::string new_part = std::string("") + COLOR_ERROR + match + COLOR_DEFAULT;

		string_replace(s_format.c_str(), match.c_str(), new_part.c_str(), true, true, &s_new);
		s_format = s_new;
	}
	return s_format;
}





