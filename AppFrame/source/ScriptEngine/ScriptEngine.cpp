#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;

#include <DxLib.h>
#include "../CFile/CFile.h"
#include "StringUtil.h"
#include "ScriptEngine.h"
#include "RevPolishCalc.h"




ScriptEngine::ScriptEngine() {
}
ScriptEngine::~ScriptEngine() {
	ClearScript();
}

void ScriptEngine::ClearScript() {
	if (_pScript) {
		delete[] _pScript;
		_pScript = NULL;
	}
	_nScriptSize = 0;
	_vJumpAdr.clear();
	_vGosubStack.Clear();
	_pnt = 0;
	_strPrint = "";
}

bool ScriptEngine::LoadScript(std::string filename) {
	CFile f(filename);
	if (!f.Success()) { return false; }
	if (!SetScript((const char*)f.Data(), f.Size())) { return false; }
	return true;
}

bool ScriptEngine::SetScript(const char* p, const int size) {
	if (!Compiler(p, size)) { return false; }
	return true;
}

bool ScriptEngine::Compiler(const char* src, int size) {
	int cnt, pass, cntLabel;
	bool success = true;
	std::unordered_map<std::string, int>	mapLabelData;	// ラベル情報（ラベル名つき）
	std::vector<char>	vScript;	// スクリプト書き込み先
	ClearScript();

	// pass = 0 : #ラベル の取得と、ラベル番号の決定
	// pass = 1 : 'ラベル' のラベル番号への変換、ラベル位置の決定、データの出力
	for (pass = 0; pass < 2; pass++) {
		int flgLineBreak = 1;		// 改行フラグ
		cnt = 0;
		cntLabel = 0;
		while (cnt < size) {
			int csize = StringUtility::Check(src[cnt]);
			if (csize > 1) {
				// 全角文字の場合、そのまま処理
				if (pass == 1) {
					for (int i = 0; i < csize; i++) {
						vScript.push_back(src[cnt + i]);
					}
				}
				cnt += csize;
				flgLineBreak = 0;
			}
			else {
				// 半角文字の場合、解析に入る
				bool write = true;
				if (src[cnt + 0] == '/' && src[cnt + 1] == '/') {
					// 「//」から改行までをコメントとして飛ばす
					cnt += 2;
					while (StringUtility::CheckLineBreak(&src[cnt]) == 0) {
						cnt += StringUtility::Check(src[cnt]);
					}
				}
				else if (src[cnt + 0] == '/' && src[cnt + 1] == '*') {
					// 「/*」から「*/」までをコメントとして飛ばす
					cnt += 2;
					while (!(src[cnt + 0] == '*' && src[cnt + 1] == '/')) {
						cnt += StringUtility::Check(src[cnt]);
					}
					cnt += 2;
				}
				else if (flgLineBreak && src[cnt] == '#') {
					// 行頭に#があったので、ラベルと見なす
					cnt++;
					if (pass == 0) {
						// 最初(pass1)。ラベル名を得てカウンタを進める
						std::string strLabel = "";
						cnt += StringUtility::GetString(&src[cnt], "\x0a\x0d\0", strLabel);	// 改行マークまでの文字列を取得する
						mapLabelData[strLabel] = cntLabel;
					}
					else {
						// 2回目(pass2)。ラベルのある場所をジャンプ先として保存する
						_vJumpAdr.push_back((int)vScript.size());
						while (StringUtility::CheckLineBreak(&src[cnt]) == 0) {
							cnt += StringUtility::Check(src[cnt]);
						}
					}
					cntLabel++;
				}
				else if (src[cnt] == '\"') {
					// 「"」があったので、そこから「"」までをそのまま書き込み
					if (pass == 1) { vScript.push_back(src[cnt]); }		// 開始の"
					cnt++;		// " をスキップ
					std::string str;
					cnt += StringUtility::GetString(&src[cnt], "\"", str);	// "までの文字列を取得する
					if (pass == 1) {
						for (int i = 0; i < strlen(str.c_str()); i++) { vScript.push_back(str[i]); }
					}
					if (pass == 1) { vScript.push_back(src[cnt]); }		// 終了の"
					cnt++;
					write = false;
				}
				else if (src[cnt] == '\'') {
					// 「'」があったので、そこから「'」までをラベルと認識して置換。
					cnt++;		// ' をスキップ
					// ラベル名を得る
					std::string strLabel;
					cnt += StringUtility::GetString(&src[cnt], "'", strLabel);	// 'までの文字列を取得する
					cnt++;		// ' をスキップ
					write = false;
					if (pass == 1) {
						// 2回目のみ(pass2)。ラベル名をサーチ
						if (mapLabelData.count(strLabel) > 0) {
							// 見つかったので置き換える
							std::string str = "";
							str += std::to_string(mapLabelData[strLabel]);
							for (int i = 0; i < str.length(); i++) { vScript.push_back(str[i]); }
						}
						else {
							// 見つからなかった、エラー
							cout << "error: label not found. " << strLabel << "\n";
							success = false;
						}
					}
				}
				// 改行チェック
				if (StringUtility::CheckLineBreak(&src[cnt]) > 0) {
					// 改行は書き込まない
					flgLineBreak = 1; cnt += StringUtility::CheckLineBreak(&src[cnt]);
				}
				else {
					if (write) {
						// コントロールコードか？
						if (src[cnt] > ' ') {
							if (pass == 1) { vScript.push_back(src[cnt]); }
						}
						cnt++;
					}
					flgLineBreak = 0;
				}
			}
		}
	}

	if (success) {
		// 成功していたら、スクリプト出力メモリに書き込む
		_nScriptSize = (int)vScript.size();
		_pScript = new char[_nScriptSize + 1];
		for (int i = 0; i < _nScriptSize; i++) {
			_pScript[i] = vScript[i];
		}
		_pScript[_nScriptSize] = '\0';		// デバッグ時にメモリを見やすいように終端記号
	}

	return success;
}


VariableItem::TYPE	ScriptEngine::GetDataType(const char* src) {
	int cnt = 0;
	cnt += StringUtility::SkipSpace(&src[cnt]);
	if (src[cnt] == '$') {
		// 最初の変数のタイプでデータタイプを判断
		cnt++;		// $
		std::string strVarName;
		cnt += _varSys.PickupVariableName(&src[cnt], strVarName);
		return _varSys.GetType(strVarName);
	}
	if (src[cnt] == '\"') {
		// 文字列
		return VariableItem::TYPE::STRING;
	}
	// それ以外は数値
	return VariableItem::TYPE::NUM;
}

int		ScriptEngine::GetString(const char* src, std::string& strOut) {
	int cnt = 0;
	strOut = "";
	cnt += StringUtility::SkipSpace(&src[cnt]);
	while (1) {
		if (src[cnt] == '\"') {
			// ""に囲まれた文字列
			cnt++;		// 開始の "
			std::string str;
			cnt += StringUtility::GetString(&src[cnt], "\"", str);	// "までの文字列を取得する
			cnt++;		// 終了の "
			strOut += str;
		}
		else if (src[cnt] == '$') {
			// 変数データ
			cnt++;		// $
			std::string strVarName;
			cnt += _varSys.PickupVariableName(&src[cnt], strVarName);
			strOut += _varSys.GetString(strVarName);
		}
		else {
			// 数値データ
			RevPolishCalc::ItemType num;
			cnt += GetNum(&src[cnt], num);
			strOut += std::to_string(num);
		}

		// 文字列は連結するか？
		if (src[cnt] != '.') { break; }
		cnt++;		// 連結の .
	}

	return cnt;
}


int		ScriptEngine::GetNum(const char* src, RevPolishCalc::ItemType& numOut) {
	int cnt = 0;
	numOut = 0;
	while (1) {
		RevPolishCalc::ItemType ans;
		cnt += RevPolishCalc::Math(&src[cnt], &ans, &_varSys);
		numOut = ans;
		break;
	}
	return cnt;
}

int		ScriptEngine::SkipKakko(const char* src) {
	int cnt = 0;
	int cntKakko = 0;
	do {
		cnt += StringUtility::SkipString(&src[cnt], "{}");
		if (src[cnt] == '{') { cntKakko++; }
		if (src[cnt] == '}') { cntKakko--; }
		cnt++;
	} while (cntKakko);
	return cnt;
}

bool ScriptEngine::Run() {

	while (_pnt < _nScriptSize) {
		int csize = StringUtility::Check(_pScript[_pnt]);
		if (csize > 1) {
			// 漢字コードなのでそのまま進める
			_pnt += csize;
		}
		else {
			// 半角文字なのでコマンド処理をする
			if (_pScript[_pnt] == ':') {
				// コマンドマーク有
				_pnt++;
				if (_pScript[_pnt] == '$') {
					// 変数代入
					std::string strVarName;
					_pnt++;
					_pnt += _varSys.PickupVariableName(&_pScript[_pnt], strVarName);	// 変数名の取得
					_pnt += StringUtility::SkipString(&_pScript[_pnt], "=");			// = までスキップ
					_pnt++;		// = を飛ばす
					// 右辺の取得
					switch (GetDataType(&_pScript[_pnt])) {
					case VariableItem::TYPE::NUM:
					{	// 数値変数
						RevPolishCalc::ItemType ans;
						_pnt += GetNum(&_pScript[_pnt], ans);	// データの取得
						_varSys.Set(strVarName, ans);			// 変数への代入
						break;
					}
					case VariableItem::TYPE::STRING:
					{	// 文字変数
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);	// データの取得
						_varSys.Set(strVarName, str);				// 変数への代入
						break;
					}
					}
				}
				else {
					// コマンド処理
				// まず、コマンド文字列を取得する（コマンド文字列は必ずA-Za-zで、取得時に大文字化する）
					std::string strCmd = "";
					while (('A' <= _pScript[_pnt] && _pScript[_pnt] <= 'Z')
						|| ('a' <= _pScript[_pnt] && _pScript[_pnt] <= 'z')) {
						strCmd += toupper(_pScript[_pnt]); _pnt++;
					}

					if (strCmd == "PRINT") {
						// :PRINTは、画面に文字を出す
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);
						//cout << str << "\n";
						_strPrint += str;
						// 文字表示命令時は描画に抜ける
						return false;
					}
					else if (strCmd == "BGM") {
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);
						PlaySoundFile(str.c_str(), DX_PLAYTYPE_LOOP);
					}
					else if (strCmd == "IF") {
						while (1) {
							// 判定式を取得
							RevPolishCalc::ItemType nMath;
							_pnt += GetNum(&_pScript[_pnt], nMath);
							if ((int)nMath != 0) {
								// ok.
								_pnt += StringUtility::SkipString(&_pScript[_pnt], "{");
								_pnt++;
								break;
							}
							else {
								// no.
								_pnt += SkipKakko(&_pScript[_pnt]);
								_pnt += StringUtility::SkipSpace(&_pScript[_pnt]);
								if (StringUtility::CompareMemStrNoCase(&_pScript[_pnt], "ELSE")) {
									_pnt += (int)strlen("ELSE");
									_pnt += StringUtility::SkipSpace(&_pScript[_pnt]);
									if (StringUtility::CompareMemStrNoCase(&_pScript[_pnt], "IF")) {
										_pnt += (int)strlen("IF");
									}
									else {
										_pnt += StringUtility::SkipString(&_pScript[_pnt], "{");
										_pnt++;
										break;
									}
								}
								else {
									break;
								}
							}
						}
					}
					else if (strCmd == "GO") {
						RevPolishCalc::ItemType nLabel;
						_pnt += GetNum(&_pScript[_pnt], nLabel);
						_pnt = _vJumpAdr[(int)nLabel];
					}
					else if (strCmd == "GS") {
						RevPolishCalc::ItemType nLabel;
						_pnt += GetNum(&_pScript[_pnt], nLabel);
						_vGosubStack.Push(_pnt);
						_pnt = _vJumpAdr[(int)nLabel];
					}
					else if (strCmd == "RT") {
						_pnt = _vGosubStack.Pop();
					}
					else
					{
						cout << "ERROR, Unknown Command: " << strCmd << "\n";
					}
				}
			}
			else if (_pScript[_pnt] == '}') {
				_pnt++;
				while (1) {
					_pnt += StringUtility::SkipSpace(&_pScript[_pnt]);
					if (StringUtility::CompareMemStrNoCase(&_pScript[_pnt], "ELSE")) {		// elseif, elseどちらも引っかける
						// skip
						_pnt += SkipKakko(&_pScript[_pnt]);
					}
					else {
						break;
					}
				}
			}
			else {
				// コマンドでないので1byte進める
				_pnt++;
			}
		}
	}

	// スクリプト実行終了
	return true;
}

