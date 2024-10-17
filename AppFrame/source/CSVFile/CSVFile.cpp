#include"CSVFile.h"

#include <iostream>
#include<sstream>
#include<Windows.h>

//num�ڂ�seach������̏ꏊ��Ԃ�
size_t Mysseek(std::string target, std::string search, size_t num = 0)
{
	size_t count = 0;
	size_t pos = target.find(search);

	while (pos != std::string::npos)
	{
		if (count++ == num)
		{
			return pos;
		}

		size_t size = target.substr(pos + 1).find(search);

		if (size == std::string::npos)return std::string::npos;
		pos += size + 1;
	}
	return std::string::npos;
}

CSVFile::CSVFile(const std::string& filename,Type type)
	:CFile(filename)
	,_type(type)
{
	if (!Success())return;
	std::string data((char*)Data());
	Parse(data, _type, _data);
}

CSVFile::CSVFile(const std::string& filename, Cell& cellData, Type type)
	:CFile(filename,Format(cellData,type))
{

}

CSVFile::~CSVFile()
{
}

void CSVFile::Parse(std::string data, Type type,Cell &output)
{
	if (data.empty())return;
	switch (type)
	{
	case Type::LINE:
	{
		//2�����z��̗v�f���́A�L�[�̐�������
		int keyEleNum = std::count(data.begin(), data.end(), '\n');
		keyEleNum = data.back() == '\n' ? keyEleNum : keyEleNum + 1;
		output.resize(keyEleNum);

		size_t subStartPos = 0;										//�؂蔲���f�[�^�̐擪�ʒu
		size_t subEndPos = data.substr(subStartPos).find(",");		//�؂蔲���f�[�^�̖����ʒu

		for (int i = 0; i < keyEleNum; ++i) {

			size_t lineBreak = Mysseek(data, "\n", i);
			lineBreak = lineBreak == std::string::npos ? data.length() : lineBreak;

			while (subStartPos + subEndPos + 1 < lineBreak)
			{
				output[i].emplace_back(data.substr(subStartPos, subEndPos));
				subStartPos += subEndPos + 1;
				subEndPos = data.substr(subStartPos).find(",");
				if (subEndPos == std::string::npos)break;
			}

			size_t breakMrkSize = 1;
			subEndPos = data.substr(subStartPos).find("\n");
			if (data.substr(subStartPos, subEndPos).find("\r") != std::string::npos)
			{
				breakMrkSize = 2;
				subEndPos --;
			}

			if (subEndPos != std::string::npos) {
				output[i].emplace_back(data.substr(subStartPos, subEndPos));
				subStartPos += subEndPos + breakMrkSize;
				subEndPos = data.substr(subStartPos).find(",");
			}
			else { output[i].emplace_back(data.substr(subStartPos));}

			if (output[i].empty())continue;

			//��������̃f�[�^�A���s�R�[�h�A�J���}�Ȃ�A�폜����
			while (output[i].back().GetType() == TextData::Type::Empty
				|| output[i].back().GetStr() == "\r"
				|| output[i].back().GetStr() == "\n"
				|| output[i].back().GetStr() == "\r\n"
				|| output[i].back().GetStr() == ","
				)
			{
				output[i].pop_back();
			}
		}
		break;
	}
	case Type::VERTICAL:
	{
		int keyEleNum = 0;
		{
			size_t firstLineBreak = data.find("\n");

			//�f�[�^��1�񂵂��Ȃ��ꍇ�A�����������s�}�[�N���Ȃ��̂ŁA�f�[�^�̒����ɂ���
			firstLineBreak = firstLineBreak != std::string::npos ? firstLineBreak : data.length();

			std::string firstLine = data.substr(0, firstLineBreak);
			keyEleNum = std::count(firstLine.begin(), firstLine.end(), ',') + 1;
			output.resize(keyEleNum);
		}

		//�f�[�^�̍ő吔(1�����z��̍ő�v�f��)
		int maxDataEleNum = std::count(data.begin(), data.end(), '\n') + 1;

		for (size_t i = 0; i < keyEleNum; ++i)
		{
			for (int j = 0; j < maxDataEleNum; ++j)
			{

				//�T���J�n�ʒu
				size_t startSearchPos = j == 0 ? 0 : Mysseek(data, "\n", static_cast<size_t>(j - 1)) + 1;

				//���s�ʒu
				size_t lineBreakPos = data.substr(startSearchPos).find("\n");
				lineBreakPos = lineBreakPos != std::string::npos ? lineBreakPos : data.length();

				std::string lineData = data.substr(startSearchPos, lineBreakPos);

				//�ŏ�(i == 0)�́A0(�f�[�^�̐擪)����
				size_t dataStartPos = i == 0 ? 0 : Mysseek(lineData, ",", i - 1) + 1;
				size_t dataEndPos = Mysseek(lineData, ",", i);

				//��̖����̃f�[�^�ɂ́A�u,�v�ł͂Ȃ��A���s�}�[�N�u\n�v�����ɂ��Ă���
				//������Ȃ���΁A�f�[�^�̖����Ƃ������ƂɂȂ�
				if (dataEndPos == std::string::npos)
				{
					dataEndPos = lineData.find("\r");
					dataEndPos = dataEndPos != std::string::npos ? dataEndPos : lineData.length() - dataStartPos;
				}
				output[i].emplace_back(data.substr(startSearchPos + dataStartPos, dataEndPos - dataStartPos));
			}

			if (output[i].empty())continue;

			//��������̃f�[�^�A���s�R�[�h�A�J���}�Ȃ�A�폜����
			while (output[i].back().GetType() == TextData::Type::Empty
				|| output[i].back().GetStr() == "\r"
				|| output[i].back().GetStr() == "\n"
				|| output[i].back().GetStr() == "\r\n"
				|| output[i].back().GetStr() == ","
				)
			{
				output[i].pop_back();
			}
		}
		break;
	}
	default:
		break;
	}
	output.CreateKey();
}

std::string CSVFile::Format(Cell& cellData,Type type)
{
	std::string output;

	switch (type)
	{
	case Type::LINE:
	{
		for (size_t i = 0; i < cellData.size(); ++i)
		{
			for (size_t j = 0; j < cellData[i].size(); ++j)
			{
				output += cellData[i][j]._data + ",";
			}
			output.back() = '\n';
		}
	}
		break;
	case Type::VERTICAL:
	{
		size_t keyEleNum = cellData.size();
		size_t maxEleSize = 0;
		for (auto&& lineCell : cellData)
		{
			maxEleSize = maxEleSize < lineCell.size() ? lineCell.size() : maxEleSize;
		}

		for (size_t i = 0; i < maxEleSize; ++i)
		{
			for (size_t j = 0; j < keyEleNum; ++j)
			{
				if (i >= cellData[j].size())
				{
					output += ",";
					continue;
				}
				output += cellData[j][i].GetStr() + ",";
			}
			output.back() = '\n';
		}
	}
		break;

	default:
		break;
	}

	return output;
}

MyShallowVector<TextData> CSVFile::operator[](const std::string& key)
{
	return _data[key];
}

std::vector<TextData>& CSVFile::operator [](size_t num)
{
	return _data[num];
}