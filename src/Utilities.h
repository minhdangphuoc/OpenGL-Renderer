#pragma once
#define __UTILITIES_H__
#ifdef __UTILITIES_H__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


static std::string readFile(const std::string& path) {
	std::string sourceCode;
    std::ifstream file;

	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		sourceCode = fileStream.str();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return sourceCode;
}

#endif // __UTILITIES_H__