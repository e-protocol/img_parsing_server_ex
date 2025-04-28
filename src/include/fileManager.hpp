#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include "misc.hpp"

class FileManager
{
public:
	/**
	 * @brief static method to process file
	 */
	static void processFile(http::request<http::string_body>&& req)
	{
		namespace fs = std::filesystem;
		std::string path = "/tmp";
		
		std::string filename;
		auto it = req.find("filename");

		if(it != req.end())
			filename = it->value();
		else
			throw std::runtime_error("Error: invalid filename field");
		
		RenameFile(filename, "_convert");

		if(!fs::exists(path))
			throw std::runtime_error("Error: /tmp folder");

		try
		{
			path += "/" + filename;

			if(fs::exists(path))
					fs::remove(path);
		}
		catch(const std::exception& e) { throw std::runtime_error(e.what()); }

		std::ofstream file;

		try
		{
			const char* begin = req.body().c_str();
			file.open(path, std::ios::out | std::ios::binary | std::ios::app);
			file.write(begin, req.body().size());
			file.close();
		}
		catch(const std::exception& e) { throw std::runtime_error(e.what()); }
	}

	/**
	 * @brief methof to rename file with prefix
	 */
	static bool RenameFile(std::string& filename, const std::string& prefix)
	{
		for(size_t i = 0; i < filename.size(); ++i)
			if(filename[i] == '.')
			{
				std::string str = filename.substr(0, i) + prefix + filename.substr(i, filename.size());
				filename = str;
				return true;
			}
		return false;
	}
};  