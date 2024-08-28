#pragma once
#include <string>

class StringUtilityCodeBase {
public:
	virtual int Check(const unsigned char uc) = 0;
	virtual int Check(const char c) { return Check((const unsigned char)c); }
};

class StringUtilityCodeSJIS : public StringUtilityCodeBase {
public:
	virtual int Check(const unsigned char uc);
};

class StringUtilityCodeUTF8 : public StringUtilityCodeBase {
public:
	virtual int Check(const unsigned char uc);
};



class StringUtility {
public:
	StringUtility();
	virtual ~StringUtility();

	static int UTF8IsBOM(const char* up);
	static int SkipSpace(const char* str);
	static int SkipString(const char* str, const char* stopChar);			// 指定の文字が出るまでスキップする
	static int GetString(const char* str, const char* stopChar, std::string& getStr);	// 指定の文字が出るまでの文字を取得する

	static int CheckLineBreak(const char* str);		// 改行マークのチェック
	static int Check(const char c) { return _cb->Check(c); }

	static int CompareMemNoCaseLength(const char* p1, const char* p2, int length);
	static int CompareMemStrNoCase(const char* mem, std::string str);
private:
	static StringUtilityCodeBase* _cb;

};
