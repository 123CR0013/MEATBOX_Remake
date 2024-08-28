#pragma once
#include <string>
#include <vector>

#include "RevPolishCalc.h"
#include "Variable.h"


class ScriptEngine {
public:
	ScriptEngine();
	virtual ~ScriptEngine();

	void ClearScript();
	bool LoadScript(std::string filename);
	bool SetScript(const char *p, const int size);
	bool Run();

	char* GetScriptBinary() { return _pScript; }
	int GetScriptBinarySize() { return _nScriptSize; }
	std::vector<int>* GetScriptBinaryJumpAdr() { return &_vJumpAdr; }

	VariableItem::TYPE	GetDataType(const char* src);		// スクリプトのデータタイプを得る
	int		GetString(const char* src, std::string& strOut);			// スクリプトから文字列を得る
	int		GetNum(const char* src, RevPolishCalc::ItemType& numOut);	// スクリプトから数値を得る

	std::string	GetPrintString() { return _strPrint; }
	void		ClearPrintString() { _strPrint = ""; }

private:
	Variable		_varSys;		// 変数システム

	bool Compiler(const char* src, int size);
	std::vector<int>	_vJumpAdr;			// ラベルアドレス

	char*	_pScript = NULL;	// スクリプト本体（コンパイル済み）
	int		_nScriptSize = 0;	// スクリプトサイズ

	int		_pnt = 0;	// スクリプト実行ポインタ

	RevPolishStack<int>		_vGosubStack;		// GSスタック
	std::string		_strPrint = "";				// PRINT文の文字列

	int		SkipKakko(const char* src);		// {}内をスキップする
};
