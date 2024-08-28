#include	"RevPolishCalc.h"
#include	"StringUtil.h"
#include	"Variable.h"


// static変数の実体
std::vector<RevPolishCalc::REVPOLISH_DATA>		RevPolishCalc::_vRepData;		// 逆ポーランドデータ

// 数値を取得
int RevPolishCalc::GetNum(const char* str, ItemType& item, Variable* varSys) {
	int cnt = 0;
	ItemType mul = 1;	// プラスかマイナスか
	int	n = 0, ncnt = 1;	// 小数点より上用
	int m = 0, mflg = 0;	// 小数点以下用
	cnt += StringUtility::SkipSpace(&str[cnt]);	// スペースやTABを飛ばす

	// 数字の最初に+-がついている場合
	switch (str[cnt]) {
	case '+': { cnt++; break; }
	case '-': { mul = -1; cnt++; break; }
	}

	if (varSys && str[cnt] == '$') {
		// 変数システムがある場合、変数マーク$を変数取得とする
		std::string strVarName;
		cnt++;
		cnt += varSys->PickupVariableName(&str[cnt], strVarName);
		item = varSys->GetNum(strVarName);
		item = item * mul;
		return cnt;
	}
	else {
		// 数字を取り出す（10進数, 小数点対応）
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

	// 戻り値の作成
	if (mflg) {
		// 小数点以下があった
		item = (m + ((ItemType)(n) / ncnt)) * mul;
	}
	else {
		// 小数点以下はなかった
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
		// 文字の長い順に並べる
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
		{ "",	PRIORITY::NUM,			TYPE::NUM },		// 数値は特例、コードで判定
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
			if (varSys && str[0] == '$') { f = 1; }		// 変数システムがある場合、変数マーク$も数値として扱う
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


// 文字列式→逆ポーランドデータに変換
int RevPolishCalc::ToRevPolish(const char* str, Variable* varSys) {
	int cnt = 0;
	RevPolishStack<REVPOLISH_DATA>	vStack;
	int flgKigou = 1;			// 前回が演算記号なら1
	_vRepData.clear();		// 逆ポーランドデータの初期化

	// 項目取得ループ
	while (1) {
		// 項目の優先順位取得
		REVPOLISH_DATA itm;
		int itmLength;
		cnt += StringUtility::SkipSpace(&str[cnt]);				// スペースやTABを飛ばす
		itm = GetPriority(&str[cnt], itmLength, varSys);		// 項目の優先順位を得る
		if (itm.prio == PRIORITY::NONE) { break; }		// 項目がなかったら完了
		// 前回が演算記号なのに今回 + or - だった
		if (flgKigou && itm.prio == PRIORITY::ADD_SUB) {
			itm.type = TYPE::NUM;			// 数値として判定
		}

		// スタックの頭を取り出す
		REVPOLISH_DATA	stackTop = { PRIORITY::NONE };
		if (vStack.Size() > 0) { stackTop = vStack.Back(); }

		// スタックの頭と、項目の優先順位を比較
		if (stackTop.type == TYPE::OPEN && itm.type == TYPE::CLOSE) {
			// スタックに ( が、参照記号に ) が来た場合の特例.
			// どちらも相殺され消える.
			vStack.Pop();
			cnt++;
		}
		else if (((int)stackTop.prio % (int)PRIORITY::OPEN) < (int)itm.prio || stackTop.prio == PRIORITY::NONE) {
			// スタックの頭より、項目の優先順位が高い。スタックに積む
			if (itm.type == TYPE::NUM) {
				// 数値の場合、値を得る
				cnt += GetNum(&str[cnt], itm.item, varSys);
				flgKigou = 0;
			}
			else {
				// 記号である
				cnt += itmLength;
				flgKigou = 1;
			}
			vStack.Push(itm);
		}
		else {
			// スタックの頭の方が、項目より優先順位が高い。スタックを取りだし、逆ポーランドデータに追加。
			vStack.Pop();
			_vRepData.push_back(stackTop);
		}
	}
	// スタックにたまっているデータを、逆ポーランドデータに追加
	while (vStack.Size() > 0) {
		_vRepData.push_back(vStack.Pop());
	}

	// 読み進んだ文字数を返す
	return cnt;
}

// 逆ポーランドデータを計算
RevPolishCalc::ItemType	RevPolishCalc::CalcRevPolish() {
	RevPolishStack<REVPOLISH_DATA>	vStack;
	for (int i = 0; i < _vRepData.size(); i++) {
		if (_vRepData[i].type == TYPE::NUM) {
			// 数値ならそのままスタックに入れる
			vStack.Push(_vRepData[i]);
		}
		else {
			// 記号なら、スタックからデータを2つ取りだし、計算
			REVPOLISH_DATA b, a = { PRIORITY::NUM, TYPE::NUM, 0 };
			REVPOLISH_DATA c = { PRIORITY::NUM, TYPE::NUM, 0 };
			// 種類によって取り出すデータ数が違う
			switch (_vRepData[i].type) {
			case TYPE::NOT:	// 1つ取り出す
				b = vStack.Pop();
				break;
			default:		// 2つ取り出す
				b = vStack.Pop();
				a = vStack.Pop();
			}
			// 種類別の計算
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
			// 計算結果をスタックに入れる
			vStack.Push(c);
		}
	}

	// スタックに残っている答えを取りだす
	REVPOLISH_DATA ans = vStack.Pop();
	// 答えを返す
	return ans.item;
}


// 文字列式を計算する
int RevPolishCalc::Math(const char* math, ItemType* pAnswer, Variable* varSys) {
	int cnt = 0;
	cnt += ToRevPolish(&math[cnt], varSys);	// 文字列式→逆ポーランドデータに
	ItemType answer = CalcRevPolish();		// 逆ポーランドデータを計算し、答えを求める
	if (pAnswer) { *pAnswer = answer; }		// 答えを入れるポインタが指定されていたら、答えを入れる
	return cnt;								// 読み進んだ文字数を返す
}

