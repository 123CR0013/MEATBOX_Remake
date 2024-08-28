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
	std::unordered_map<std::string, int>	mapLabelData;	// ���x�����i���x�������j
	std::vector<char>	vScript;	// �X�N���v�g�������ݐ�
	ClearScript();

	// pass = 0 : #���x�� �̎擾�ƁA���x���ԍ��̌���
	// pass = 1 : '���x��' �̃��x���ԍ��ւ̕ϊ��A���x���ʒu�̌���A�f�[�^�̏o��
	for (pass = 0; pass < 2; pass++) {
		int flgLineBreak = 1;		// ���s�t���O
		cnt = 0;
		cntLabel = 0;
		while (cnt < size) {
			int csize = StringUtility::Check(src[cnt]);
			if (csize > 1) {
				// �S�p�����̏ꍇ�A���̂܂܏���
				if (pass == 1) {
					for (int i = 0; i < csize; i++) {
						vScript.push_back(src[cnt + i]);
					}
				}
				cnt += csize;
				flgLineBreak = 0;
			}
			else {
				// ���p�����̏ꍇ�A��͂ɓ���
				bool write = true;
				if (src[cnt + 0] == '/' && src[cnt + 1] == '/') {
					// �u//�v������s�܂ł��R�����g�Ƃ��Ĕ�΂�
					cnt += 2;
					while (StringUtility::CheckLineBreak(&src[cnt]) == 0) {
						cnt += StringUtility::Check(src[cnt]);
					}
				}
				else if (src[cnt + 0] == '/' && src[cnt + 1] == '*') {
					// �u/*�v����u*/�v�܂ł��R�����g�Ƃ��Ĕ�΂�
					cnt += 2;
					while (!(src[cnt + 0] == '*' && src[cnt + 1] == '/')) {
						cnt += StringUtility::Check(src[cnt]);
					}
					cnt += 2;
				}
				else if (flgLineBreak && src[cnt] == '#') {
					// �s����#���������̂ŁA���x���ƌ��Ȃ�
					cnt++;
					if (pass == 0) {
						// �ŏ�(pass1)�B���x�����𓾂ăJ�E���^��i�߂�
						std::string strLabel = "";
						cnt += StringUtility::GetString(&src[cnt], "\x0a\x0d\0", strLabel);	// ���s�}�[�N�܂ł̕�������擾����
						mapLabelData[strLabel] = cntLabel;
					}
					else {
						// 2���(pass2)�B���x���̂���ꏊ���W�����v��Ƃ��ĕۑ�����
						_vJumpAdr.push_back((int)vScript.size());
						while (StringUtility::CheckLineBreak(&src[cnt]) == 0) {
							cnt += StringUtility::Check(src[cnt]);
						}
					}
					cntLabel++;
				}
				else if (src[cnt] == '\"') {
					// �u"�v���������̂ŁA��������u"�v�܂ł����̂܂܏�������
					if (pass == 1) { vScript.push_back(src[cnt]); }		// �J�n��"
					cnt++;		// " ���X�L�b�v
					std::string str;
					cnt += StringUtility::GetString(&src[cnt], "\"", str);	// "�܂ł̕�������擾����
					if (pass == 1) {
						for (int i = 0; i < strlen(str.c_str()); i++) { vScript.push_back(str[i]); }
					}
					if (pass == 1) { vScript.push_back(src[cnt]); }		// �I����"
					cnt++;
					write = false;
				}
				else if (src[cnt] == '\'') {
					// �u'�v���������̂ŁA��������u'�v�܂ł����x���ƔF�����Ēu���B
					cnt++;		// ' ���X�L�b�v
					// ���x�����𓾂�
					std::string strLabel;
					cnt += StringUtility::GetString(&src[cnt], "'", strLabel);	// '�܂ł̕�������擾����
					cnt++;		// ' ���X�L�b�v
					write = false;
					if (pass == 1) {
						// 2��ڂ̂�(pass2)�B���x�������T�[�`
						if (mapLabelData.count(strLabel) > 0) {
							// ���������̂Œu��������
							std::string str = "";
							str += std::to_string(mapLabelData[strLabel]);
							for (int i = 0; i < str.length(); i++) { vScript.push_back(str[i]); }
						}
						else {
							// ������Ȃ������A�G���[
							cout << "error: label not found. " << strLabel << "\n";
							success = false;
						}
					}
				}
				// ���s�`�F�b�N
				if (StringUtility::CheckLineBreak(&src[cnt]) > 0) {
					// ���s�͏������܂Ȃ�
					flgLineBreak = 1; cnt += StringUtility::CheckLineBreak(&src[cnt]);
				}
				else {
					if (write) {
						// �R���g���[���R�[�h���H
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
		// �������Ă�����A�X�N���v�g�o�̓������ɏ�������
		_nScriptSize = (int)vScript.size();
		_pScript = new char[_nScriptSize + 1];
		for (int i = 0; i < _nScriptSize; i++) {
			_pScript[i] = vScript[i];
		}
		_pScript[_nScriptSize] = '\0';		// �f�o�b�O���Ƀ����������₷���悤�ɏI�[�L��
	}

	return success;
}


VariableItem::TYPE	ScriptEngine::GetDataType(const char* src) {
	int cnt = 0;
	cnt += StringUtility::SkipSpace(&src[cnt]);
	if (src[cnt] == '$') {
		// �ŏ��̕ϐ��̃^�C�v�Ńf�[�^�^�C�v�𔻒f
		cnt++;		// $
		std::string strVarName;
		cnt += _varSys.PickupVariableName(&src[cnt], strVarName);
		return _varSys.GetType(strVarName);
	}
	if (src[cnt] == '\"') {
		// ������
		return VariableItem::TYPE::STRING;
	}
	// ����ȊO�͐��l
	return VariableItem::TYPE::NUM;
}

int		ScriptEngine::GetString(const char* src, std::string& strOut) {
	int cnt = 0;
	strOut = "";
	cnt += StringUtility::SkipSpace(&src[cnt]);
	while (1) {
		if (src[cnt] == '\"') {
			// ""�Ɉ͂܂ꂽ������
			cnt++;		// �J�n�� "
			std::string str;
			cnt += StringUtility::GetString(&src[cnt], "\"", str);	// "�܂ł̕�������擾����
			cnt++;		// �I���� "
			strOut += str;
		}
		else if (src[cnt] == '$') {
			// �ϐ��f�[�^
			cnt++;		// $
			std::string strVarName;
			cnt += _varSys.PickupVariableName(&src[cnt], strVarName);
			strOut += _varSys.GetString(strVarName);
		}
		else {
			// ���l�f�[�^
			RevPolishCalc::ItemType num;
			cnt += GetNum(&src[cnt], num);
			strOut += std::to_string(num);
		}

		// ������͘A�����邩�H
		if (src[cnt] != '.') { break; }
		cnt++;		// �A���� .
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
			// �����R�[�h�Ȃ̂ł��̂܂ܐi�߂�
			_pnt += csize;
		}
		else {
			// ���p�����Ȃ̂ŃR�}���h����������
			if (_pScript[_pnt] == ':') {
				// �R�}���h�}�[�N�L
				_pnt++;
				if (_pScript[_pnt] == '$') {
					// �ϐ����
					std::string strVarName;
					_pnt++;
					_pnt += _varSys.PickupVariableName(&_pScript[_pnt], strVarName);	// �ϐ����̎擾
					_pnt += StringUtility::SkipString(&_pScript[_pnt], "=");			// = �܂ŃX�L�b�v
					_pnt++;		// = ���΂�
					// �E�ӂ̎擾
					switch (GetDataType(&_pScript[_pnt])) {
					case VariableItem::TYPE::NUM:
					{	// ���l�ϐ�
						RevPolishCalc::ItemType ans;
						_pnt += GetNum(&_pScript[_pnt], ans);	// �f�[�^�̎擾
						_varSys.Set(strVarName, ans);			// �ϐ��ւ̑��
						break;
					}
					case VariableItem::TYPE::STRING:
					{	// �����ϐ�
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);	// �f�[�^�̎擾
						_varSys.Set(strVarName, str);				// �ϐ��ւ̑��
						break;
					}
					}
				}
				else {
					// �R�}���h����
				// �܂��A�R�}���h��������擾����i�R�}���h������͕K��A-Za-z�ŁA�擾���ɑ啶��������j
					std::string strCmd = "";
					while (('A' <= _pScript[_pnt] && _pScript[_pnt] <= 'Z')
						|| ('a' <= _pScript[_pnt] && _pScript[_pnt] <= 'z')) {
						strCmd += toupper(_pScript[_pnt]); _pnt++;
					}

					if (strCmd == "PRINT") {
						// :PRINT�́A��ʂɕ������o��
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);
						//cout << str << "\n";
						_strPrint += str;
						// �����\�����ߎ��͕`��ɔ�����
						return false;
					}
					else if (strCmd == "BGM") {
						std::string str;
						_pnt += GetString(&_pScript[_pnt], str);
						PlaySoundFile(str.c_str(), DX_PLAYTYPE_LOOP);
					}
					else if (strCmd == "IF") {
						while (1) {
							// ���莮���擾
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
					if (StringUtility::CompareMemStrNoCase(&_pScript[_pnt], "ELSE")) {		// elseif, else�ǂ��������������
						// skip
						_pnt += SkipKakko(&_pScript[_pnt]);
					}
					else {
						break;
					}
				}
			}
			else {
				// �R�}���h�łȂ��̂�1byte�i�߂�
				_pnt++;
			}
		}
	}

	// �X�N���v�g���s�I��
	return true;
}

