#pragma once
#include	<unordered_map>
#include	<string>
#include	<tchar.h>
#include	<vector>

// リソース管理サーバ
// すべて静的メンバで構成する
class ResourceServer {
public:
	static	void Init();
	static	void Release();

	static void ClearGraph();
	static int LoadGraph(const TCHAR* FileName);
	static int LoadGraph(const std::string FileName);

	static int LoadDivGraph(const TCHAR* FileName, int AllNum,
		int XNum, int YNum,
		int XSize, int YSize, int* HandleBuf);
	static int LoadDivGraph(const std::string FileName, int AllNum,
		int XNum, int YNum,
		int XSize, int YSize, int* HandleBuf);
	static int LoadDivGraph(const std::string FileName, int AllNum,
				int XNum, int YNum,
				int XSize, int YSize, std::vector<int>& HandleBuf);

	static int LoadSoundMem(const TCHAR* FileName);	
	static int LoadSoundMem(const std::string FileName);

private:
	static std::unordered_map<std::string, int>	_mapGraph;
	typedef struct {
		int		AllNum;
		int* handle;
	} DIVGRAPH;
	static std::unordered_map<std::string, DIVGRAPH>	_mapDivGraph;

	static std::unordered_map<std::string, int>	_mapSound;
};

