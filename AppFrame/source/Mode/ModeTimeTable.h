#pragma once
#include"ModeBase.h"
#include<list>
#include<functional>

//ƒVƒ“ƒOƒ‹ƒgƒ“
class ModeTimeTable final:public ModeBase
{
public:
	struct Task
	{
		std::function<void()>_funcTaskExe;
		UINT _frameTaskExe;
	};
public:

	ModeTimeTable();
	~ModeTimeTable();

	bool Process()override;
	static void Add(std::function<void()>func,UINT afterFrame);
	static std::string Name() { return _name; }

private:
	static ModeTimeTable* _lpInstance;
	static std::string _name;
	std::list<Task>_tasks;
	UINT _frameCount;
};