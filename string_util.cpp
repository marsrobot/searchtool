#include <string.h>
#include <stdarg.h>

#include "string_util.h"

int string_to_lower(const char* str, std::string* res)
{
	//std::string newstr = boost::to_upper_copy<std::string>(str);
	std::string newstr(str);
	std::locale loc;
	const int64_t N = strlen(str);
	for (int64_t i = 0; i < N; i++) {
		newstr[i] = std::tolower(str[i], loc);
	}
	*res = newstr;
	return 0;
}

std::string string_to_lower(std::string s)
{
	std::string res;
	string_to_lower(s.c_str(), &res);
	return res;
}

int string_is_ascii(const char* str)
{
	int64_t n = strlen(str);
	for (int64_t i = 0; i < n; i++) {
		if (str[i] >= '\x7f')
			return false;
	}
	return true;
}

//int begin_with(const char* str, const char* delim) {
//return Utility().begin_with(str, delim);
//}
int string_contain(const char* str, const char* p_delim)
{
	//assert( delim.size() > 0 );
	//return( str.find(delim, 0) != std::string::npos );
	// std::string delim(p_delim);
	return (strstr(str, p_delim) != nullptr);
}



int string_split_by_many(const char* p_s, std::vector<char> vc, std::vector<std::string>* res)
{
	std::string s(p_s);

	const int64_t n_c = vc.size();
	const int64_t n_s = s.size();
	std::vector<std::string> vs;
	int64_t start = -1;
	int64_t end = -1;
	for (int64_t i = 0; i < n_s; i++) {
		if (start == -1) {
			int flag = true;
			for (int64_t j = 0; j < n_c; j++)
				flag = flag && (s[i] != vc[j]);
			if (flag)
				start = i;
		}
		else {
			for (int64_t j = 0; j < n_c; j++)
				if (s[i] == vc[j]) {
					end = i;
					vs.push_back(std::string(s.data() + start, end - start));
					start = -1;
				}
		}
	}
	if (start != -1)
		vs.push_back(std::string(s.data() + start, n_s - start));
	*res = vs;
	return 0;
}

int string_trim(std::string* p_str, StringPosition pos, std::string* res)
{
	std::string str(*p_str);

	int64_t N = str.size();
	int64_t begin = 0x7fffffff;
	int64_t end = 0x0;
	for (int64_t i = 0; i < N; i++) {
		if (!isspace(str[i])) {
			begin = i;
			break;
		}
	}
	if (begin > N) {
		*res = "";
		return 0;
	}

	for (int64_t i = N - 1; i >= 0; i--) {
		if (!isspace(str[i])) {
			end = i;
			break;
		}
	}

	switch (pos) {
	case StringPositionFront: {
		*res = std::string(str.begin() + begin, str.end());
		return 0;
	}
	case StringPositionBack: {
		*res = std::string(str.begin(), str.begin() + end + 1);
		return 0;
	}
	default: {
		*res = std::string(str.begin() + begin, str.begin() + (end + 1));
		return 0;
	}
	}
}

// replace_all: replace every instance of x by y
// from_beginning: find instances from beginning. If replace_all is true,
// whether from_beginning is true or false should not matter.
// marsrobot, 12:28am 11/23/2017
int string_replace(const char* string_input, const char* x, const char* y, const int replace_all, const int from_beginning, std::string* res)
{
	if (from_beginning) {
		return string_replace_impl(string_input, x, y, replace_all, res);
	}
	else {
		std::string r_i = string_input;
		std::string r_x = x;
		std::string r_y = y;
		std::reverse(r_i.begin(), r_i.end());
		std::reverse(r_x.begin(), r_x.end());
		std::reverse(r_y.begin(), r_y.end());
		std::string r_o;
		string_replace_impl(r_i.c_str(), r_x.c_str(), r_y.c_str(), replace_all, &r_o);
		std::reverse(r_o.begin(), r_o.end());
		*res = r_o;
		return 0;
	}
}

int string_replace_impl(const char* p_str, const char* p_x, const char* p_y, const int replace_all, std::string* res)
{
	uint64_t pos = 0;
	uint64_t found;
	std::string str(p_str);
	std::string x(p_x);
	std::string y(p_y);

	std::string rv;
	for (;;) {
		found = str.find(x, pos);
		if (found != std::string::npos) {
			rv = rv + str.substr(0, found) + y;
			str = str.substr(found + x.size());
		}
		if (!(found != std::string::npos) || !replace_all)
			break;
	}
	rv = rv + str;
	*res = rv;
	return 0;
}



int StringCaseInsensitiveComparison(const std::string& first, const std::string& second)
{
	uint64_t i = 0;
	while ((i < first.length()) && (i < second.length())) {
		if (std::tolower(first[i]) < std::tolower(second[i]))
			return true;
		else if (std::tolower(first[i]) > std::tolower(second[i]))
			return false;
		++i;
	}
	if (first.length() < second.length())
		return true;
	else
		return false;
}


int string_split_with_brackets(const char* p_s, const char* p_delim, std::vector<char> brackets, int nparts, std::vector<std::string>* res)
{
	std::string delim(p_delim);

	std::vector<std::string> arr;
	const int64_t quote_count = brackets.size();
	assert(quote_count == 0 || quote_count == 2 || quote_count == 4);
	int use_quote = quote_count > 0;
	std::vector<int64_t> quotes;
	for (int64_t i = 0; i < int64_t(brackets.size() / 2); i++)
		quotes.push_back(0);

	int64_t start = 0;
	std::string s(p_s);
	int64_t n = s.size();

	for (int64_t i = 0; i < n; i++) {
		int64_t pos = -1;
		if (use_quote) {
			for (int64_t j = 0; j < int64_t(brackets.size()); j = j + 2) {
				if (s[i] == brackets[2 * j + 0])
					quotes[j / 2]++;
				if (quote_count > 1) {
					if (s[i] == brackets[2 * j + 1])
						quotes[j / 2]--;
				}
				else {
					if (s[i] == brackets[2 * j + 0])
						quotes[j / 2]--;
				}
			}
		}
		if (s.substr(i, delim.size()) == delim) {
			if (use_quote == true) {
				//if(brackets % 2 == 0)
				int64_t total_quotes = 0;
				for (int64_t k = 0; k < int64_t(quotes.size()); k++) {
					total_quotes += quotes[k];
				}
				if (total_quotes == 0)
					pos = i;
			}
			else
				pos = i;
		}
		if (pos != -1) {
			arr.push_back(s.substr(start, pos - start));
			start = pos + delim.size();
			if (int64_t(arr.size()) >= nparts - 1)
				break;
		}
	}

	arr.push_back(s.substr(start));
	*res = arr;
	return 0;
}


// if output is returned, this will be a bug. '\0' will be kept in the std::string and
// std::string stream.str() does not work any more as it truncates at '\0'.
int string_printf(std::string* res, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	uint64_t size = vsnprintf(nullptr, 0, format, args);
	va_start(args, format);
	std::string output(size + 1, '_');
	vsprintf(&output[0], format, args);
	va_end(args);
	std::string filtered(output.c_str());
	*res = filtered;
	return 0;
}


int string_begin_with(const char* p_str, const char* p_delim)
{
	std::string delim(p_delim);

	assert(delim.size() > 0);
	std::string str(p_str);
	return (str.find(delim, 0) == 0);
}



int string_end_with(const char* s_input, const char* suffix_input)
{
	const char* str = s_input;
	const char* suffix = suffix_input;
	if (!str || !suffix)
		return 0;
	uint64_t lenstr = strlen(str);
	uint64_t lensuffix = strlen(suffix);
	if (lensuffix > lenstr)
		return 0;
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

//std::string string_replace(const char* string_input, const char* x, const char* y, const int replace_all = true, const int from_beginning = true) {
//return Utility().replace(string_input, x, y, replace_all, from_beginning);
//}
//std::string case_insensitive_replace(const char* input, const char* a, const char* b) {
//return Utility().case_insensitive_replace(input, a, b);
//}



int string_case_insensitive_replace(const char* input, const char* p_a, const char* b, std::string* res)
{
	std::string s(input);
	std::string a(p_a);
	auto pos = std::search(s.begin(), s.end(), a.begin(), a.end(),
		[](const char c1, const char c2) {
			return (std::tolower(c1)) == (std::tolower(c2));
		});
	if (pos == s.end()) {
		*res = "";
		return 0;
	}
	auto pos2 = pos;
	std::advance(pos2, a.size());
	s.replace(pos, pos2, b);
	*res = s;
	return 0;
}

int string_split_impl(const char* p_str, const char* p_delim, std::vector<std::string>* res)
{
	std::string str(p_str);
	std::string delim(p_delim);

	std::vector<std::string> array;
	uint64_t pos = 0;
	uint64_t found;

	for (;;) {
		found = str.find(delim, pos);
		std::string substr = str.substr(pos, found - pos);
		if (substr.size())
			array.push_back(substr);
		if (found != std::string::npos)
			pos = found + delim.size();
		else
			break;
	}
	*res = array;
	return 0;
}


int string_split_two_delim(const char* p_str, char delim_0, char delim_1, std::vector<std::string>* res)
{
	std::string string_input(p_str);

	std::vector<std::string> strs;
	string_split_impl(string_input.c_str(), std::to_string(delim_0).c_str(), &strs);
	std::vector<std::string> array;
	for (int64_t i = 0, m = int64_t(strs.size()); i < m; i++) {
		std::vector<std::string> v;
		string_split_impl(strs[i].c_str(), std::to_string(delim_1).c_str(), &v);
		for (int64_t j = 0, n = int64_t(v.size()); j < n; j++)
			array.push_back(v[j]);
	}
	*res = array;
	return 0;
}

int string_reverse_trim(const char* p_str, int64_t* len, std::string* res) // remove trailing spaces
{
	char* str = new char[strlen(p_str) + 1];
	strcpy(str, p_str);
	for (; *len > 0 && isspace(str[*len - 1]); --(*len)) {
		;
	}
	str[*len] = '\0';
	*res = str;
	delete[] str;
	return 0;
}

int string_trim(std::string* p_str, int pos, std::string* res)
{
	std::string str(*p_str);

	int64_t N = str.size();
	int64_t begin = 0x7fffffff;
	int64_t end = 0x0;
	for (int64_t i = 0; i < N; i++) {
		if (!isspace(str[i])) {
			begin = i;
			break;
		}
	}
	if (begin > N) {
		*res = "";
		return 0;
	}

	for (int64_t i = N - 1; i >= 0; i--) {
		if (!isspace(str[i])) {
			end = i;
			break;
		}
	}

	switch (pos) {
	case StringPositionFront:
	{
		*res = std::string(str.begin() + begin, str.end());
		return 0;
	}
	case StringPositionBack:
	{
		*res = std::string(str.begin(), str.begin() + end + 1);
		return 0;
	}
	default:
	{
		*res = std::string(str.begin() + begin, str.begin() + (end + 1));
		return 0;
	}
	}
}
