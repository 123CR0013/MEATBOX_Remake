#pragma once
#include <string>
#include <vector>

class Variable;

// �X�^�b�N
template <typename T>
class RevPolishStack {
public:
	RevPolishStack() {}
	virtual ~RevPolishStack() { _v.clear(); }
	int		Size() { return (int)_v.size(); }
	void	Push(T n) { _v.push_back(n); }
	T		Pop() { T n = Back(); _v.pop_back(); return n; }
	T		Back() { return _v.back(); }
	void	Clear() { _v.clear(); }
private:
	std::vector<T>	_v;
};

// �����񎮌v�Z�N���X
class RevPolishCalc {
public:
	// ���̍��̌^���w��
	typedef	double	ItemType;
	//typedef	int	ItemType;

	// �v�Z��������̎w��
	static int Math(const char* math, ItemType* pAnswer, Variable* varSys = NULL);
	static int Math(std::string math, ItemType* pAnswer, Variable* varSys = NULL) {
		return Math(math.c_str(), pAnswer, varSys);
	}

protected:
	static int ToRevPolish(const char* str, Variable* varSys);	// �����񎮁��t�|�[�����h�f�[�^�ɕϊ�
	static ItemType CalcRevPolish();							// �t�|�[�����h�f�[�^���v�Z

	// �D�揇��
	enum class PRIORITY {	// ��̕����D�揇�ʒႢ
		NONE,
		CLOSE,
		Q_AND_OR,
		Q_EQU_NOT,
		PAU_AND_OR,
		ADD_SUB,
		MUL_DIV,
		DOUBLE,
		NOT,
		NUM,
		OPEN,
	};
	// �L���ԍ�
	enum class TYPE {
		NONE,
		NUM,		// 0 �` 9
		CLOSE,		// )
		OR,			// |
		AND,		// &
		PAU,		// %
		SUB,		// -
		ADD,		// +
		DIV,		// /
		MUL,		// *
		DOUBLE,		// ^
		NOT,		// !
		OPEN,		// (
		MIN,		// <
		OVER,		// >
		OR_OR,		// ||     ��������2�����L��
		AND_AND,	// &&
		MIN_EQU,	// <=
		OVER_EQU,	// >=
		NOT_EQU,	// !=
		EQU_EQU,	// ==
	};
	// ���f�[�^
	typedef struct {
		PRIORITY	prio;
		TYPE		type;
		ItemType	item;
	} REVPOLISH_DATA;
	static std::vector<REVPOLISH_DATA>		_vRepData;		// �t�|�[�����h�f�[�^

	// �����񂩂獀�̎�ނ𔻒�
	static REVPOLISH_DATA	GetPriority(const char* str, int& length, Variable* varSys);

	// ���l���擾
	static int GetNum(const char* str, ItemType& item, Variable* varSys);
};
