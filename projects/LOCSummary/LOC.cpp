#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <sstream>
#include <locale>
#include <iostream>
#include <iterator>
#include <fstream>
#include <vector>
#include <set>

template<class T>
std::string FormatWithCommas(T value)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

__int64 total = 0;
std::set<std::string> ignorefiles;

boost::filesystem::recursive_directory_iterator createRIterator(boost::filesystem::path path)
{
	try
	{
		return boost::filesystem::recursive_directory_iterator(path);
	}
	catch (boost::filesystem::filesystem_error& fex)
	{
		std::cout << fex.what() << std::endl;
		return boost::filesystem::recursive_directory_iterator();
	}
}

void dump(boost::filesystem::path path, int level)
{
	try
	{
		if (boost::filesystem::is_regular_file(path))
		{
			std::string file(path.filename().string());
			boost::replace_all(file, "\"", "");

			std::cout << file;

			for (size_t i = 0; i < 50 - file.size(); ++i)
				std::cout << " ";

			if (ignorefiles.find(file) == ignorefiles.end())
			{
				std::ifstream inFile(path.c_str());
				__int64 count = std::count(std::istreambuf_iterator<char>(inFile),
					std::istreambuf_iterator<char>(), '\n');

				total += count;
				std::cout << FormatWithCommas(count);
				
			}
			else
			{
				std::cout << "//IGNORE//";
			}
				
			std::cout << std::endl;
		}
	}
	catch (boost::filesystem::filesystem_error& fex)
	{
		std::cout << fex.what() << std::endl;
	}
}

void plainListTree(boost::filesystem::path path)
{
	dump(path, 0);

	boost::filesystem::recursive_directory_iterator it = createRIterator(path);
	boost::filesystem::recursive_directory_iterator end;

	while (it != end) 
	{
		dump(*it, it.level()); 

		if (boost::filesystem::is_directory(*it) && boost::filesystem::is_symlink(*it)) 
			it.no_push();

		try
		{
			++it;
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
			it.no_push();
			try { ++it; }
			catch (...) { std::cout << "!!" << std::endl; return; }
		}
	}
}

int main(int argc, char* argv[])
{
	std::ifstream is("./LOCSummary.ignore");
	if (is.good())
	{
		std::istream_iterator<std::string> start(is), end;
		std::vector<std::string> ignorelist(start, end);
		for (auto&i : ignorelist)
			ignorefiles.insert(i);

		plainListTree("./");
		std::cout << std::endl;
		for (size_t i = 0; i < 80; ++i)
			std::cout << "-";
		std::cout << std::endl;
		std::cout << "Total";
		for (size_t i = 0; i < 45; ++i)
			std::cout << " ";
		std::cout << FormatWithCommas(total) << std::endl;
		int i;
		std::cin >> i;
	}
	return 0;
}