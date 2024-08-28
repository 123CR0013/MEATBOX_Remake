#pragma once
#include <string>
#include <vector>

class Variable;

// スタック
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

// 文字列式計算クラス
class RevPolishCalc {
public:
	// 式の項の型を指定
	typedef	double	ItemType;
	//typedef	int	ItemType;

	// 計算式文字列の指定
	static int Math(const char* math, ItemType* pAnswer, Variable* varSys = NULL);
	static int Math(std::string math, ItemType* pAnswer, Variable* varSys = NULL) {
		return Math(math.c_str(), pAnswer, varSys);
	}

protected:
	static int ToRevPolish(const char* str, Variable* varSys);	// 文字列式→逆ポーランドデータに変換
	static ItemType CalcRevPolish();							// 逆ポーランドデータを計算

	// 優先順位
	enum class PRIORITY {	// 上の方が優先順位低い
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
	// 記号番号
	enum class TYPE {
		NONE,
		NUM,		// 0 ～ 9
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
		OR_OR,		// ||     ここから2文字記号
		AND_AND,	// &&
		MIN_EQU,	// <=
		OVER_EQU,	// >=
		NOT_EQU,	// !=
		EQU_EQU,	// ==
	};
	// 項データ
	typedef struct {
		PRIORITY	prio;
		TYPE		type;
		ItemType	item;
	} REVPOLISH_DATA;
	static std::vector<REVPOLISH_DATA>		_vRepData;		// 逆ポーランドデータ

	// 文字列から項の種類を判定
	static REVPOLISH_DATA	GetPriority(const char* str, int& length, Variable* varSys);

	// 数値を取得
	static int GetNum(const char* str, ItemType& item, Variable* varSys);
};
