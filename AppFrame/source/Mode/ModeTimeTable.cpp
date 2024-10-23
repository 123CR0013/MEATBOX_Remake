#include"ModeTimeTable.h"
#include"ModeServer.h"

ModeTimeTable* ModeTimeTable::_lpInstance = nullptr;
std::string ModeTimeTable::_name = "TimeTable";

ModeTimeTable::ModeTimeTable()
	:_frameCount(0)
{
	//これは、間違ってまた自クラスのインスタンスを生成した
	if (_lpInstance)
	{
		//タスクをコピー
		_tasks = _lpInstance->_tasks;
		ModeServer::GetInstance()->Del(_lpInstance);
	}

	_lpInstance = this;
}

ModeTimeTable::~ModeTimeTable()
{
	_tasks.clear();
	_lpInstance = nullptr;
}

bool ModeTimeTable::Process()
{
	if (!ModeBase::Process())return false;

	//実行する時間が遅い順に並んでいるため、実行できるタスクがない場合は処理を止める
	for (auto iter = _tasks.begin();iter != _tasks.end();iter)
	{
		if ((*iter)._frameTaskExe <= _frameCount)
		{
			(*iter)._funcTaskExe();
			iter = _tasks.erase(iter);
			continue;
		}
		else break;
		++iter;
	}
	++_frameCount;

	return true;
}

void ModeTimeTable::Add(std::function<void()>func, UINT afterFrame)
{
	if (!_lpInstance)return;

	UINT frame = _lpInstance->_frameCount + afterFrame;

	if (_lpInstance->_tasks.empty())
	{
		Task task = { func, frame };
		_lpInstance->_tasks.emplace_back(task); return;
	}

	auto iter = _lpInstance->_tasks.begin();
	for (iter; iter != _lpInstance->_tasks.end(); ++iter)
	{
		if ((*iter)._frameTaskExe > frame)
		{
			break;
		}
	}

	_lpInstance->_tasks.insert(iter, { func, frame });
}