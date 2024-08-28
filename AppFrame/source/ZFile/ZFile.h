#pragma once
#include<string>

class ZFile
{
public:
	//ìWäJ
	ZFile(const std::string& fileName);

	//à≥èk
	ZFile(const std::string& fileName, char* data, size_t size);
	ZFile(const std::string& fileName, const std::string& data) :ZFile(fileName, (char*)data.c_str(), data.size())
	{

	}

	~ZFile();

	bool Success()const { return _success; }
	char* Data()const { return _data; }
	size_t Size()const { return _size; }
	std::string DataStr() { return std::string(_data); }
	std::string Filename()const { return _filename; }

private:
	std::string _filename;

	char* _data;
	size_t _size;

	bool _success;
};