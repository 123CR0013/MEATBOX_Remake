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

// �z��ԍ����܂߂��ϐ������擾����
int		Variable::PickupVariableName(const char* p, std::string& str) {
	int cnt = 0;
	str = "";
	cnt += StringUtility::SkipSpace(&p[cnt]);
	// $�����Ă����炻�̕ϐ��̒l��ϐ����Ƃ���
	if (p[cnt] == '$') {
		std::string strVarName;
		cnt++;
		cnt += PickupVariableName(&p[cnt], strVarName);
		str = GetString(strVarName);
		return cnt;
	}
	// �ϐ����̃X�^�[�g��A-Za-z_�̂�
	if (('A' <= p[cnt] && p[cnt] <= 'Z')
		|| ('a' <= p[cnt] && p[cnt] <= 'z')
		|| (p[cnt] == '_')
		) {
		// �ϐ����Ɏg����̂�A-Za-z0-9_
		while (('A' <= p[cnt] && p[cnt] <= 'Z')
			|| ('a' <= p[cnt] && p[cnt] <= 'z')
			|| ('0' <= p[cnt] && p[cnt] <= '9')
			|| (p[cnt] == '_')
			) {
			str += p[cnt]; cnt++;
		}
		// �ϐ����̌��[�����Ă�����A�z��̒l���擾
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

// �ϐ������邩�H
bool	Variable::Is(std::string varName) {
	std::string varArray;
	IsArray(varName, varArray);
	if (_map.count(varName) > 0) {
		return true;
	}
	return false;
}

// �ϐ����͔z��^�C�v���H�i[]�����Ă���Δz��ƌ��Ȃ��j
bool	Variable::IsArray(std::string& varName, std::string& varArray) {
	for (int i = 0; i < varName.length(); i++) {
		if (varName[i] == '[') {
			// �ϐ��^�C�v�B�������疼�O�𕪗�����
			std::string varOrignal = varName;
			varName = varOrignal.substr(0, i);
			varArray = varOrignal.substr(i);
			return true;
		}
	}
	return false;
}

//[n][m]�̕���������Ɉʒu���X�g��Ԃ�
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

// �z��̃A�C�e���ʒu���擾
VariableItem* Variable::GetArrayItem(std::string varName, std::string varArray, bool set) {
	std::vector<int> vArray = GetArrayPos(varArray);
	// �������ݑz��ŁA�ϐ��͂��邩�H
	if (set && Is(varName)) {
		// ���łɂ���B�z��^�C�v���H
		if (GetType(varName) != VariableItem::TYPE::ARRAY) {
			// �Ⴄ�B���̕ϐ����폜
			Erace(varName);
		}
	}
	// �V�K�쐬���H
	std::vector<VariableItem>* startArray;
	if (!Is(varName)) {
		// �V�K�쐬
		std::vector<VariableItem>* a = new std::vector<VariableItem>();
		startArray = a;
		for (int n = 0; n < vArray.size(); n++) {
			a->resize(vArray[n] + 1);
			if (n + 1 < vArray.size()) {
				// �r���Ȃ̂Ŏ����z��
				std::vector<VariableItem>* na = new std::vector<VariableItem>();
				VariableItem* v = new VariableItem(na);
				a->at(vArray[n]) = *v;
				a = na;
			}
		}
		// �������Ȃ��ƁA�f�X�g���N�^��array���������Ă��܂�
		VariableItem* v = new VariableItem(startArray);
		_map[varName] = *v;
	}
	// �z���H��
	{
		std::vector<VariableItem>* a = _map[varName].GetArray();
		if (a) {
			for (int n = 0; n < vArray.size(); n++) {
				// ���̑w�͔z��̐�������Ă��邩�H
				if (a->size() <= vArray[n]) {
					// �T�C�Y�g��
					a->resize(vArray[n] + 1);
				}
				if (n + 1 < vArray.size()) {
					// ���̑w�͔z�񂩁H
					if (a->at(vArray[n]).GetType() != VariableItem::TYPE::ARRAY) {
						// �z��
						a->at(vArray[n]).clear();
						std::vector<VariableItem>* na = new std::vector<VariableItem>();
						VariableItem* v = new VariableItem(na);
						a->at(vArray[n]) = *v;
					}
					a = a->at(vArray[n]).GetArray();
				}
				else {
					// �Ō�̑w
					return &a->at(vArray[n]);
				}
			}
		}
	}

	return NULL;
}


// �ϐ��̃f�[�^�^�C�v��Ԃ�
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


// ���l�ϐ��Ƃ��ăf�[�^�Z�b�g
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

// �����ϐ��Ƃ��ăf�[�^�Z�b�g
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

// ���l�ϐ���Ԃ�
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

// ������ϐ���Ԃ�
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
