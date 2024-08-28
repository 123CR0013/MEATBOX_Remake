#include "StringUtil.h"


// SHIFT-JISの1文字目を確認し、
// 1byte(半角)か2byte(全角)かをチェック
int StringUtilityCodeSJIS::Check(const unsigned char uc) {
	if (uc < 0x81) { return 1; }
	if (uc < 0xa0) { return 2; }
	if (uc < 0xe0) { return 1; }
	if (uc < 0xff) { return 2; }
	return 1;
}

// UTF-8の1文字目を確認し、
// 1byte(半角)か 複数byte(全角)かをチェック
int StringUtilityCodeUTF8::Check(const unsigned char uc) {
	if ((uc & 0x80) == 0x00) { return 1; }	// 0*** ****
	if ((uc & 0xe0) == 0xc0) { return 2; }	// 110* ****
	if ((uc & 0xf0) == 0xe0) { return 3; }	// 1110 ****
	if ((uc & 0xf8) == 0xf0) { return 4; }	// 1111 0***
	if ((uc & 0xfc) == 0xf8) { return 5; }	// 1111 10**
	if ((uc & 0xfe) == 0xfc) { return 6; }	// 1111 110*
	return 0;	// 1文字目じゃない	
}


StringUtilityCodeBase* StringUtility::_cb = new StringUtilityCodeSJIS();
//StringUtilityCodeBase* StringUtility::_cb = new StringUtilityCodeUTF8();

StringUtility::StringUtility() {
}
StringUtility::~StringUtility() {
}

// utf-8 bomかチェック
int StringUtility::UTF8IsBOM(const char* p) {
	const unsigned char* up = (const unsigned char*)p;
	if (up[0] == 0xef && up[1] == 0xbb && up[2] == 0xbf) { return 3; }
	return 0;
}

// 空白やコントロールコードのスキップ
int	StringUtility::SkipSpace(const char* str) {
	int cnt = 0;
	while ('\0' < str[cnt] && str[cnt] <= ' ') { cnt++; }
	return cnt;
}

// 指定の文字が出るまでスキップする
int StringUtility::SkipString(const char* str, const char* stopChar) {
	int cnt = 0, flg = 1, flgQuote = 0;
	while (flg) {
		if (!flgQuote) {
			for (int i = 0; i < strlen(stopChar);) {
				int c = _cb->Check(stopChar[i]);
				if (memcmp(&str[cnt],&stopChar[i], c) == 0) { flg = 0; }
				i += c;
			}
		}
		if (flg) {
			if (str[cnt] == '"') { flgQuote = !flgQuote; }
			cnt+= StringUtility::Check(str[cnt]);
		}
	}
	return cnt;
}

// 指定の文字が出るまでの文字を取得する
int StringUtility::GetString(const char* str, const char* stopChar, std::string& getStr) {
	int n = SkipString(str, stopChar);
	std::string s = "";
	for (int i = 0; i < n; i++) {
		s += str[i];
	}
	getStr = s;
	return n;
}

// 改行マークがあるかの確認
int StringUtility::CheckLineBreak(const char* str) {
	// Windows改行マーク
	if (str[0] == 0x0d && str[1] == 0x0a) { return 2; }
	// UNIX改行マーク
	if (str[0] == 0x0a) { return 1; }
	// mac改行マーク
	if (str[0] == 0x0d) { return 1; }
	// 改行無し
	return 0;
}

// メモリ同士を比較。ただし大文字小文字は比較しない
int StringUtility::CompareMemNoCaseLength(const char* p1, const char* p2, int length) {
	int i = 0;
	while (i < length) {
		int csize = StringUtility::Check(p1[i]);
		if (csize == 1) {
			if (toupper(p1[i]) != toupper(p2[i])) { return 0; }
		}
		else {
			for (int n = 0; n < csize; n++) {
				if (p1[i + n] != p2[i + n]) { return 0; }
			}
		}
		i += csize;
	}
	return 1;
}

// メモリ内にstr文字列があるか(strの長さまでのみ比較)
int StringUtility::CompareMemStrNoCase(const char* mem, std::string str) {
	return CompareMemNoCaseLength(mem, str.c_str(), (int)str.length());
}

