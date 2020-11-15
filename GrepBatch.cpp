#include "string.h"
#include "GrepBatch.h"

int GrepBatch::is_text_file(std::string filename)
{
	const char* included_types[] = {
		".action",
		".adp",
		".ashx",
		".asm",
		".asmx", // silverlight
		".asp", ".aspx",
		".asx",
		".atom",
		".awk",
		".axd",
		".bash", ".bashrc",
		".bat",
		".c", ".cc", ".cpp", ".cxx", ".cu", ".cuda",
		".cake",
		".ccss",
		".config", ".cfg",
		".cfm", ".cfml",
		".cgi",
		".chapel",
		".cjsx",
		".closure",
		".cmake",
		".cmd",
		".coffee",
		".cs", ".csharp",
		".csh",
		".cson",
		".css",
		".d",
		".dart",
		".def", // Win32 library definition file
		".dlg", // Win32 dialog resource file
		".do",
		".erb",
		".erlang",
		".for", ".f77", ".f95",
		".fsharp",
		".gcc",
		".ghc",
		".gnat",
		".go",
		".gpp",
		".h", ".hh", ".hpp", ".hxx",
		".hack",
		".hss",
		".htm", ".html", ".hta",
		".iced"
		".ifc",
		".ini",
		".java", ".jav", ".j",
		".mjs",
		".javascript", ".js", ".jse", //".json", // too many useless info when used in glimpse
		".jruby",
		".jsp", ".jspx",
		".jsx",
		".kotlin",
		".l", ".lex", ".ll", // lex source code
		".less",
		".log",
		".lua",
		".m", ".mm", // objective-c
		".makefile",
		".manifest",
		".md",
		".mk", // dmake makefile.
		".mod", "sbl", // BASIC module file.
		".node",
		".ocaml",
		".pascal", ".fpascal",
		".pcss",
		".perl", ".pl",
		".php", ".php3", ".php4", ".phtml",
		".pro", ".proj",
		".py", ".python", ".python3",
		".r",
		".racket",
		".rb", ".ruby",
		".res", ".resx", ".rc", // windows resource
		".rhtml",
		".rjs",
		".rss",
		".rust",
		".sass",
		".lisp", ".lsp", ".sbcl",
		".scheme", ".scm",
		".sh",
		".shtml",
		".svg",
		".swf",
		".swift",
		".tcl",
		".tex",
		".ts",
        ".typescript",
		".txt",
		".url",
		".vb",
		".vw",
		".wss",
		".xaml",
		".xdm", // mail message
		".xhtml",
		".xml",
		".xrb", // XML format file to generate Java properties in language translation.
		".y", // Yacc source code file.
		".yarv",
		".yaws",
		".yxx", // Bison source code file.
		"pri",

		//".ps",
		//".dll",
	};

	int flag = false;
	char* name = (char*)filename.c_str();

	for (int64_t j = 0; j < int64_t(sizeof(included_types) / sizeof(char*)); j++) {
		if (strends(string_to_lower(name).c_str(), included_types[j])) {
			flag = true;
			break;
		}
		if (strstr(name, "/node_modules/") != nullptr) { // contains
			flag = false;
			break;
		}
	}
	return flag;
}


std::map<std::string, GrepBucketType>& GrepBatch::get_words()
{
	return _words;
}
std::map<int, std::set<std::string> >& GrepBatch::get_buckets()
{
	return _buckets;
}


int GrepBatch::run(std::vector<std::string> args)
{
	GrepBucket bucket;

	/*
	std::vector<std::string> exts{ ".c", ".cc", ".cpp", ".cxx", ".hpp", ".h",
	   ".cs", ".java",
	   ".go",
	   ".py",
	   ".js", ".javascript", "jsx", ".ts",
	   ".f", ".f77", ".f90", ".for", ".fortran", ".f95",
	 };
	 */

	for (int64_t i = 0; i < int64_t(args.size()); i++) {
		std::string filename = args[i].c_str();
		std::string ext = string_to_lower(fs_extension_name(filename.c_str()));
		if (is_text_file(string_to_lower(filename))) {
			if (!fs_is_binary_file(filename.c_str())) {
				int a_bucket = bucket.get_bucket(filename);
				if (_buckets.find(a_bucket) == _buckets.end())
					_buckets[a_bucket] = std::set<std::string>();
				_buckets[a_bucket].insert(filename);

				std::vector<std::string> lines;
				try
				{
					fs_read_vector_string(filename.c_str(), &lines);
				}
				catch (std::runtime_error const& ex)
				{
					fprintf(stderr, "%s\n", ex.what());
					continue;
				}
				for (int64_t j = 0; j < int64_t(lines.size()); j++) {
					for (int64_t c = 0; c < int64_t(lines[j].size()); c++) {
						if (int(lines[j][c]) < 0 || int(lines[j][c]) > 255)
							lines[j][c] = ' ';
						else if (!std::isprint(lines[j][c]))
							lines[j][c] = ' ';
					}
					std::vector<std::string> splits;
					string_split_by_many(lines[j].c_str(), { ' ', '\t', '\n', '\r',
						/*'!', '|', '?', ';', '~', '%', '#', '*', '+', '^', */
						'&',
						'`', '\"', '\'',
						'(', ')', '[', ']', '{', '}',
						',', '=',
						/*'.', ':',*/
						'<', '>',
						'/',
						/*'\\', '-',*/
						},
						&splits);
					for (int64_t k = 0; k < int64_t(splits.size()); k++) {
						string_trim(&splits[k], StringPositionBoth, &splits[k]);
						if (splits[k].size() > 32)
							continue;

						if (_words.find(splits[k]) == _words.end()) {
							_words[splits[k]] = GrepBucketType();
						}
						_words[splits[k]].insert(a_bucket);
					}
				}
			}
		}
	}

	return 0;
}
