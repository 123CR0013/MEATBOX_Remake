#pragma once
#include"../CFile/CFile.h"
#include"TextData.h"
#include"../MySTL/MyshallowVector.h"
#include<map>
#include<vector>

class Cell
{
public:
	void CreateKey()
	{
		for (size_t i = 0; i < _data.size(); ++i)
		{
			_key[_data[i].front().GetStr()] = i;
		}
	}

	bool FindKey(const std::string& key)const { return _key.find(key) != _key.end(); }

	std::vector<TextData>& operator[](size_t num) { return _data[num]; }
	MyShallowVector<TextData> operator[](const std::string& key) { return MyShallowVector<TextData>(&_data[_key[key]], 1); }


	MyIterator<std::vector<TextData>> begin() { return MyIterator<std::vector<TextData>>(_data.data(), 0); }
	MyIterator<std::vector<TextData>> end() { return MyIterator<std::vector<TextData>>(_data.data(), _data.size()); }

	size_t size()const { return _data.size(); }
	void resize(size_t num) { _data.resize(num); }
	
private:
	std::vector < std::vector<TextData> >_data;
	std::map<std::string, size_t>_key;
};

class CSVFile : public CFile
{
public:
	enum class Type
	{
		LINE,
		VERTICAL
	};


	CSVFile(const std::string& filename,Type type);
	CSVFile(const std::string& filename, Cell& cellData,Type type);
	virtual ~CSVFile();

	//データ解析
	static void Parse(std::string data, Type type, Cell &output);

	//csv形式のデータの作成
	static std::string Format(Cell& cellData,Type type);

	bool FindKey(const std::string& key) { return _data.FindKey(key); }

	MyShallowVector<TextData> operator [](const std::string& key);
	std::vector<TextData>& operator [](size_t num);

	Cell& CellData() { return _data; }

	MyIterator<std::vector<TextData>> begin() { return _data.begin(); }
	MyIterator<std::vector<TextData>> end() { return _data.end(); }

private:
	Cell _data;

	Type _type;
};