#include "variable.h"
#include "StringUtil.h"
#include "RevPolishCalc.h"


VariableItem::~VariableItem() {
	clear();
}
void VariableItem::clear() {
	switch (_type) {
	case TYPE::ARRAY:
	{
		for (auto ite = _array->begin(); ite != _array->end(); ite++) {
			(*ite).clear();
		}
		_array->clear();
		break;
	}
	}
	_type = TYPE::NONE;
}

RevPolishCalc::ItemType		VariableItem::GetNum() {
	switch (_type) {
	case TYPE::NUM:
		return _num;
	case TYPE::STRING:
		return std::stod(_string);
	case TYPE::ARRAY:
		break;
	}
	return 0;
}
std::string					VariableItem::GetString() {
	switch (_type) {
	case TYPE::NUM:
		return std::to_string(_num);
	case TYPE::STRING:
		return _string;
	case TYPE::ARRAY:
		break;
	}
	return "";
}
std::vector<VariableItem>* VariableItem::GetArray() {
	switch (_type) {
	case TYPE::NUM:
		break;
	case TYPE::STRING:
		break;
	case TYPE::ARRAY:
		return _array;
	}
	return NULL;
}


Variable::Variable() {
	_map.clear();
}
Variable::~Variable() {
	Clear();
}

void Variable::Clear() {
	for (auto ite = _map.begin(); ite != _map.end(); ite++) {
		(*ite).second.clear();
	}
	_map.clear();
}

// 配列番号を含めた変数名を取得する
int		Variable::PickupVariableName(const char* p, std::string& str) {
	int cnt = 0;
	str = "";
	cnt += StringUtility::SkipSpace(&p[cnt]);
	// $がついていたらその変数の値を変数名とする
	if (p[cnt] == '$') {
		std::string strVarName;
		cnt++;
		cnt += PickupVariableName(&p[cnt], strVarName);
		str = GetString(strVarName);
		return cnt;
	}
	// 変数名のスタートはA-Za-z_のみ
	if (('A' <= p[cnt] && p[cnt] <= 'Z')
		|| ('a' <= p[cnt] && p[cnt] <= 'z')
		|| (p[cnt] == '_')
		) {
		// 変数名に使えるのはA-Za-z0-9_
		while (('A' <= p[cnt] && p[cnt] <= 'Z')
			|| ('a' <= p[cnt] && p[cnt] <= 'z')
			|| ('0' <= p[cnt] && p[cnt] <= '9')
			|| (p[cnt] == '_')
			) {
			str += p[cnt]; cnt++;
		}
		// 変数名の後に[がついていたら、配列の値を取得
		while (p[cnt] == '[') {
			str += p[cnt]; cnt++;	// [
			RevPolishCalc::ItemType ans;
			cnt += RevPolishCalc::Math(&p[cnt], &ans, this);
			str += std::to_string((int)ans);
			str += p[cnt]; cnt++;	// ]
		}
	}

	return cnt;
}

// 変数があるか？
bool	Variable::Is(std::string varName) {
	std::string varArray;
	IsArray(varName, varArray);
	if (_map.count(varName) > 0) {
		return true;
	}
	return false;
}

// 変数名は配列タイプか？（[]がついていれば配列と見なす）
bool	Variable::IsArray(std::string& varName, std::string& varArray) {
	for (int i = 0; i < varName.length(); i++) {
		if (varName[i] == '[') {
			// 変数タイプ。ここから名前を分離する
			std::string varOrignal = varName;
			varName = varOrignal.substr(0, i);
			varArray = varOrignal.substr(i);
			return true;
		}
	}
	return false;
}

//[n][m]の文字列を元に位置リストを返す
std::vector<int>	Variable::GetArrayPos(std::string varArray) {
	std::vector<int> v;
	const char* p = varArray.c_str();
	int cnt = 0;
	while (p[cnt] == '[') {
		cnt++;
		RevPolishCalc::ItemType ans;
		cnt += RevPolishCalc::Math(&p[cnt], &ans, this);
		v.push_back((int)ans);
		cnt += StringUtility::SkipString(&p[cnt], "]");
		cnt++;
	}
	return v;
}

// 配列のアイテム位置を取得
VariableItem* Variable::GetArrayItem(std::string varName, std::string varArray, bool set) {
	std::vector<int> vArray = GetArrayPos(varArray);
	// 書き込み想定で、変数はあるか？
	if (set && Is(varName)) {
		// すでにある。配列タイプか？
		if (GetType(varName) != VariableItem::TYPE::ARRAY) {
			// 違う。この変数を削除
			Erace(varName);
		}
	}
	// 新規作成か？
	std::vector<VariableItem>* startArray;
	if (!Is(varName)) {
		// 新規作成
		std::vector<VariableItem>* a = new std::vector<VariableItem>();
		startArray = a;
		for (int n = 0; n < vArray.size(); n++) {
			a->resize(vArray[n] + 1);
			if (n + 1 < vArray.size()) {
				// 途中なので次も配列
				std::vector<VariableItem>* na = new std::vector<VariableItem>();
				VariableItem* v = new VariableItem(na);
				a->at(vArray[n]) = *v;
				a = na;
			}
		}
		// こうしないと、デストラクタでarrayが解放されてしまう
		VariableItem* v = new VariableItem(startArray);
		_map[varName] = *v;
	}
	// 配列を辿る
	{
		std::vector<VariableItem>* a = _map[varName].GetArray();
		if (a) {
			for (int n = 0; n < vArray.size(); n++) {
				// この層は配列の数が足りているか？
				if (a->size() <= vArray[n]) {
					// サイズ拡張
					a->resize(vArray[n] + 1);
				}
				if (n + 1 < vArray.size()) {
					// 次の層は配列か？
					if (a->at(vArray[n]).GetType() != VariableItem::TYPE::ARRAY) {
						// 配列化
						a->at(vArray[n]).clear();
						std::vector<VariableItem>* na = new std::vector<VariableItem>();
						VariableItem* v = new VariableItem(na);
						a->at(vArray[n]) = *v;
					}
					a = a->at(vArray[n]).GetArray();
				}
				else {
					// 最後の層
					return &a->at(vArray[n]);
				}
			}
		}
	}

	return NULL;
}


// 変数のデータタイプを返す
VariableItem::TYPE Variable::GetType(std::string varName) {
	if (!Is(varName)) { return VariableItem::TYPE::NONE; }
	std::string varArray;
	if (!IsArray(varName, varArray)) {
		return _map[varName].GetType();
	}
	else {
		VariableItem* item = GetArrayItem(varName, varArray, true);
		return item->GetType();
	}
}

bool	Variable::Erace(std::string varName) {
	if (!Is(varName)) { return false; }
	_map.erase(varName);
	return true;
}


// 数値変数としてデータセット
bool	Variable::Set(std::string varName, RevPolishCalc::ItemType num) {
	std::string varArray;
	if (!IsArray(varName, varArray)) {
		if (Is(varName)) { _map[varName].clear(); }
		_map[varName] = VariableItem(num);
	}
	else {
		VariableItem* item = GetArrayItem(varName, varArray, true);
		*item = VariableItem(num);
	}
	return true;
}

// 文字変数としてデータセット
bool	Variable::Set(std::string varName, std::string str) {
	std::string varArray;
	if (!IsArray(varName, varArray)) {
		if (Is(varName)) { _map[varName].clear(); }
		_map[varName] = VariableItem(str);
	}
	else {
		VariableItem* item = GetArrayItem(varName, varArray, true);
		*item = VariableItem(str);
	}
	return true;
}

// 数値変数を返す
RevPolishCalc::ItemType	Variable::GetNum(std::string varName) {
	if (!Is(varName)) { return 0; }
	std::string varArray;
	if (!IsArray(varName, varArray)) {
		return _map[varName].GetNum();
	}
	else {
		VariableItem* item = GetArrayItem(varName, varArray, false);
		return item->GetNum();
	}
}

// 文字列変数を返す
std::string	Variable::GetString(std::string varName) {
	if (!Is(varName)) { return ""; }
	std::string varArray;
	if (!IsArray(varName, varArray)) {
		return _map[varName].GetString();
	}
	else {
		VariableItem* item = GetArrayItem(varName, varArray, false);
		return item->GetString();
	}
}
