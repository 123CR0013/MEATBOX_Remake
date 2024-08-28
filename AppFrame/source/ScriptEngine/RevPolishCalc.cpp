#include	"RevPolishCalc.h"
#include	"StringUtil.h"
#include	"Variable.h"


// static�ϐ��̎���
std::vector<RevPolishCalc::REVPOLISH_DATA>		RevPolishCalc::_vRepData;		// �t�|�[�����h�f�[�^

// ���l���擾
int RevPolishCalc::GetNum(const char* str, ItemType& item, Variable* varSys) {
	int cnt = 0;
	ItemType mul = 1;	// �v���X���}�C�i�X��
	int	n = 0, ncnt = 1;	// �����_����p
	int m = 0, mflg = 0;	// �����_�ȉ��p
	cnt += StringUtility::SkipSpace(&str[cnt]);	// �X�y�[�X��TAB���΂�

	// �����̍ŏ���+-�����Ă���ꍇ
	switch (str[cnt]) {
	case '+': { cnt++; break; }
	case '-': { mul = -1; cnt++; break; }
	}

	if (varSys && str[cnt] == '$') {
		// �ϐ��V�X�e��������ꍇ�A�ϐ��}�[�N$��ϐ��擾�Ƃ���
		std::string strVarName;
		cnt++;
		cnt += varSys->PickupVariableName(&str[cnt], strVarName);
		item = varSys->GetNum(strVarName);
		item = item * mul;
		return cnt;
	}
	else {
		// ���������o���i10�i��, �����_�Ή��j
		while (1) {
			if ('0' <= str[cnt] && str[cnt] <= '9') {
				n = n * 10 + (str[cnt] - '0');
				ncnt = ncnt * 10;
			}
			else if (str[cnt] == '.') {
				m = n; mflg = 1; ncnt = 1; n = 0;
			}
			else {
				break;
			}
			cnt++;
		}
	}

	// �߂�l�̍쐬
	if (mflg) {
		// �����_�ȉ���������
		item = (m + ((ItemType)(n) / ncnt)) * mul;
	}
	else {
		// �����_�ȉ��͂Ȃ�����
		item = (ItemType)n * mul;
	}

	return cnt;
}


RevPolishCalc::REVPOLISH_DATA	RevPolishCalc::GetPriority(const char* str, int& length, Variable* varSys) {
	typedef struct {
		char		data[4];
		PRIORITY	prio;
		TYPE		type;
	} TBL;
	const TBL tbl[] = {
		// �����̒������ɕ��ׂ�
		{ "==",	PRIORITY::Q_EQU_NOT,	TYPE::EQU_EQU },
		{ "!=",	PRIORITY::Q_EQU_NOT,	TYPE::NOT_EQU },
		{ ">=",	PRIORITY::Q_EQU_NOT,	TYPE::OVER_EQU },
		{ "<=",	PRIORITY::Q_EQU_NOT,	TYPE::MIN_EQU },
		{ "&&",	PRIORITY::Q_AND_OR,		TYPE::AND_AND },
		{ "||",	PRIORITY::Q_AND_OR,		TYPE::OR_OR },
		{ ">",	PRIORITY::Q_EQU_NOT,	TYPE::OVER },
		{ "<",	PRIORITY::Q_EQU_NOT,	TYPE::MIN },
		{ "(",	PRIORITY::OPEN,			TYPE::OPEN },
		{ ")",	PRIORITY::CLOSE,		TYPE::CLOSE },
		{ "",	PRIORITY::NUM,			TYPE::NUM },		// ���l�͓���A�R�[�h�Ŕ���
		{ "!",	PRIORITY::NOT,			TYPE::NOT },
		{ "^",	PRIORITY::DOUBLE,		TYPE::DOUBLE },
		{ "*",	PRIORITY::MUL_DIV,		TYPE::MUL },
		{ "/",	PRIORITY::MUL_DIV,		TYPE::DIV },
		{ "+",	PRIORITY::ADD_SUB,		TYPE::ADD },
		{ "-",	PRIORITY::ADD_SUB,		TYPE::SUB },
		{ "%",	PRIORITY::PAU_AND_OR,	TYPE::PAU },
		{ "&",	PRIORITY::PAU_AND_OR,	TYPE::AND },
		{ "|",	PRIORITY::PAU_AND_OR,	TYPE::OR },
	};
	REVPOLISH_DATA ret = { PRIORITY::NONE, TYPE::NONE, 0 };
	for (int i = 0; i < sizeof(tbl) / sizeof(tbl[0]); i++) {
		int f = 0;
		if (tbl[i].type == TYPE::NUM) {
			if ('0' <= str[0] && str[0] <= '9') { f = 1; }
			if (varSys && str[0] == '$') { f = 1; }		// �ϐ��V�X�e��������ꍇ�A�ϐ��}�[�N$�����l�Ƃ��Ĉ���
		}
		else if (memcmp(str, tbl[i].data, strlen(tbl[i].data)) == 0) { f = 1; }
		if (f) {
			ret.prio = tbl[i].prio;
			ret.type = tbl[i].type;
			length = (int)strlen(tbl[i].data);
			break;
		}
	}
	return ret;
}


// �����񎮁��t�|�[�����h�f�[�^�ɕϊ�
int RevPolishCalc::ToRevPolish(const char* str, Variable* varSys) {
	int cnt = 0;
	RevPolishStack<REVPOLISH_DATA>	vStack;
	int flgKigou = 1;			// �O�񂪉��Z�L���Ȃ�1
	_vRepData.clear();		// �t�|�[�����h�f�[�^�̏�����

	// ���ڎ擾���[�v
	while (1) {
		// ���ڂ̗D�揇�ʎ擾
		REVPOLISH_DATA itm;
		int itmLength;
		cnt += StringUtility::SkipSpace(&str[cnt]);				// �X�y�[�X��TAB���΂�
		itm = GetPriority(&str[cnt], itmLength, varSys);		// ���ڂ̗D�揇�ʂ𓾂�
		if (itm.prio == PRIORITY::NONE) { break; }		// ���ڂ��Ȃ������犮��
		// �O�񂪉��Z�L���Ȃ̂ɍ��� + or - ������
		if (flgKigou && itm.prio == PRIORITY::ADD_SUB) {
			itm.type = TYPE::NUM;			// ���l�Ƃ��Ĕ���
		}

		// �X�^�b�N�̓������o��
		REVPOLISH_DATA	stackTop = { PRIORITY::NONE };
		if (vStack.Size() > 0) { stackTop = vStack.Back(); }

		// �X�^�b�N�̓��ƁA���ڂ̗D�揇�ʂ��r
		if (stackTop.type == TYPE::OPEN && itm.type == TYPE::CLOSE) {
			// �X�^�b�N�� ( ���A�Q�ƋL���� ) �������ꍇ�̓���.
			// �ǂ�������E���������.
			vStack.Pop();
			cnt++;
		}
		else if (((int)stackTop.prio % (int)PRIORITY::OPEN) < (int)itm.prio || stackTop.prio == PRIORITY::NONE) {
			// �X�^�b�N�̓����A���ڂ̗D�揇�ʂ������B�X�^�b�N�ɐς�
			if (itm.type == TYPE::NUM) {
				// ���l�̏ꍇ�A�l�𓾂�
				cnt += GetNum(&str[cnt], itm.item, varSys);
				flgKigou = 0;
			}
			else {
				// �L���ł���
				cnt += itmLength;
				flgKigou = 1;
			}
			vStack.Push(itm);
		}
		else {
			// �X�^�b�N�̓��̕����A���ڂ��D�揇�ʂ������B�X�^�b�N����肾���A�t�|�[�����h�f�[�^�ɒǉ��B
			vStack.Pop();
			_vRepData.push_back(stackTop);
		}
	}
	// �X�^�b�N�ɂ��܂��Ă���f�[�^���A�t�|�[�����h�f�[�^�ɒǉ�
	while (vStack.Size() > 0) {
		_vRepData.push_back(vStack.Pop());
	}

	// �ǂݐi�񂾕�������Ԃ�
	return cnt;
}

// �t�|�[�����h�f�[�^���v�Z
RevPolishCalc::ItemType	RevPolishCalc::CalcRevPolish() {
	RevPolishStack<REVPOLISH_DATA>	vStack;
	for (int i = 0; i < _vRepData.size(); i++) {
		if (_vRepData[i].type == TYPE::NUM) {
			// ���l�Ȃ炻�̂܂܃X�^�b�N�ɓ����
			vStack.Push(_vRepData[i]);
		}
		else {
			// �L���Ȃ�A�X�^�b�N����f�[�^��2��肾���A�v�Z
			REVPOLISH_DATA b, a = { PRIORITY::NUM, TYPE::NUM, 0 };
			REVPOLISH_DATA c = { PRIORITY::NUM, TYPE::NUM, 0 };
			// ��ނɂ���Ď��o���f�[�^�����Ⴄ
			switch (_vRepData[i].type) {
			case TYPE::NOT:	// 1���o��
				b = vStack.Pop();
				break;
			default:		// 2���o��
				b = vStack.Pop();
				a = vStack.Pop();
			}
			// ��ޕʂ̌v�Z
			switch (_vRepData[i].type) {
			case TYPE::ADD:			{ c.item = a.item + b.item; break; }
			case TYPE::SUB:			{ c.item = a.item - b.item; break; }
			case TYPE::MUL:			{ c.item = a.item * b.item; break; }
			case TYPE::DIV:			{ c.item = a.item / b.item; break; }
			case TYPE::PAU:			{ c.item = (int)a.item % (int)b.item; break; }
			case TYPE::DOUBLE:		{ c.item = (ItemType)pow(a.item, b.item); break; }
			case TYPE::AND:			{ c.item = (int)a.item & (int)b.item; break; }
			case TYPE::OR:			{ c.item = (int)a.item | (int)b.item; break; }
			case TYPE::NOT:			{ c.item = !b.item; break; }
			case TYPE::EQU_EQU:		{ if (a.item == b.item) { c.item = 1; } break; }
			case TYPE::NOT_EQU:		{ if (a.item != b.item) { c.item = 1; } break; }
			case TYPE::OVER_EQU:	{ if (a.item >= b.item) { c.item = 1; } break; }
			case TYPE::MIN_EQU:		{ if (a.item <= b.item) { c.item = 1; } break; }
			case TYPE::OVER:		{ if (a.item > b.item) { c.item = 1; } break; }
			case TYPE::MIN:			{ if (a.item < b.item) { c.item = 1; } break; }
			case TYPE::AND_AND:		{ if (a.item && b.item) { c.item = 1; } break; }
			case TYPE::OR_OR:		{ if (a.item || b.item) { c.item = 1; } break; }
			}
			// �v�Z���ʂ��X�^�b�N�ɓ����
			vStack.Push(c);
		}
	}

	// �X�^�b�N�Ɏc���Ă��铚������肾��
	REVPOLISH_DATA ans = vStack.Pop();
	// ������Ԃ�
	return ans.item;
}


// �����񎮂��v�Z����
int RevPolishCalc::Math(const char* math, ItemType* pAnswer, Variable* varSys) {
	int cnt = 0;
	cnt += ToRevPolish(&math[cnt], varSys);	// �����񎮁��t�|�[�����h�f�[�^��
	ItemType answer = CalcRevPolish();		// �t�|�[�����h�f�[�^���v�Z���A���������߂�
	if (pAnswer) { *pAnswer = answer; }		// ����������|�C���^���w�肳��Ă�����A����������
	return cnt;								// �ǂݐi�񂾕�������Ԃ�
}

