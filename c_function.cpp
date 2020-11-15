#include <stdarg.h>
#include <cassert>
#include <algorithm>
#include <boost/filesystem.hpp>

#include "c_function.h"


void perror(const char* s)
{
	fprintf(stderr, "%s", s);
}

void perror_exit(const char* s)
{
	perror(s);
	exit(-1);
}

void* malloc_check(int size)
{
	void* p = (char*)malloc(size);
	if (p == nullptr)
		perror_exit("malloc failed");
	return p;
}

void c_decode_utf8(const char* p_bytes, std::wstring& wstr)
{
	std::string bytes(p_bytes);
	::utf8::utf8to32(bytes.begin(), bytes.end(), std::back_inserter(wstr));
}

void c_encode_utf8(wchar_t const* p_wstr, std::string& bytes)
{
	std::wstring wstr(p_wstr);
	::utf8::utf32to8(wstr.begin(), wstr.end(), std::back_inserter(bytes));
}

int unicode_utf8_to_utf16(const char* content, std::wstring* res)
{
	try {
		std::wstring ws;
		c_decode_utf8(content, ws);
		*res = ws;
		return 0;
	}
	catch (...)
		//catch (boost::locale::conv::conversion_error e) // error: catching polymorphic type by value
	{
		fprintf(stderr, "Fail to convert from UTF-8 to UTF-16\n");
		*res = L"";
		return 0;
	}
}

int unicode_utf16_to_utf8(wchar_t const* p_wcontent, std::string* res)
{
	std::wstring wcontent(p_wcontent);

	std::string s;
	::utf8::utf16to8(wcontent.begin(), wcontent.end(), std::back_inserter(s));
	*res = s;
	return 0;
}

int string_split(const char* str, char delimiter, std::vector<std::string>* res)
{
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the str into a stream.
	std::string tok;

	while (getline(ss, tok, delimiter))
	{
		internal.push_back(tok);
	}
	*res = internal;
	return 0;
}


