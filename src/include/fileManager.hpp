#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <boost/beast/http.hpp>
#include "misc.hpp"

using namespace boost::beast;

class FileManager
{
public:

	/**
	 * @brief static method to process file
	 */
	static void processFile(const http::request<http::string_body>& req, std::string& filename)
	{
		namespace fs = std::filesystem;
		std::string path = "/tmp";
		std::string fname;
		auto it = req.find("filename");
		if(it == req.end())
			throw std::runtime_error("request doesn't contain \"filename:\"");

		fname = it->value();
		filename = fname;
		
		path += "/" + fname;

		if(fs::exists(path))
				fs::remove(path);

		std::ofstream file;

		try
		{
			file.open(path, std::ios::out | std::ios::binary | std::ios::app);
			file.write(req.body().c_str(), req.body().size());
			file.close();
		}
		catch(const std::exception& e) { throw std::runtime_error(e.what()); }
	}

	/**
	 * @brief read processed image and store data in buffer
	 */
	static void ReadFile(const std::string& path, const std::string& filename, std::string& buffer)
	{
		try
		{
			std::ifstream file(path + "/" + filename, std::ios::in | std::ios::binary);
			buffer = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		}
		catch(const std::exception& e) { throw std::runtime_error("failed to read processed file"); }
	}
};  