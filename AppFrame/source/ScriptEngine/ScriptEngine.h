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

	VariableItem::TYPE	GetDataType(const char* src);		// �X�N���v�g�̃f�[�^�^�C�v�𓾂�
	int		GetString(const char* src, std::string& strOut);			// �X�N���v�g���當����𓾂�
	int		GetNum(const char* src, RevPolishCalc::ItemType& numOut);	// �X�N���v�g���琔�l�𓾂�

	std::string	GetPrintString() { return _strPrint; }
	void		ClearPrintString() { _strPrint = ""; }

private:
	Variable		_varSys;		// �ϐ��V�X�e��

	bool Compiler(const char* src, int size);
	std::vector<int>	_vJumpAdr;			// ���x���A�h���X

	char*	_pScript = NULL;	// �X�N���v�g�{�́i�R���p�C���ς݁j
	int		_nScriptSize = 0;	// �X�N���v�g�T�C�Y

	int		_pnt = 0;	// �X�N���v�g���s�|�C���^

	RevPolishStack<int>		_vGosubStack;		// GS�X�^�b�N
	std::string		_strPrint = "";				// PRINT���̕�����

	int		SkipKakko(const char* src);		// {}�����X�L�b�v����
};
