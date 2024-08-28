#pragma once
#include "SoundItem.h"
#include <string>
#include <map>

class SoundServer
{
public:
	SoundServer();
	virtual ~SoundServer();

	void Clear();
	void Add(SoundItemOneShot* snd);
	void Add(std::string name, SoundItemBase* snd);
	bool Del(std::string name);
	bool Del(SoundItemBase* snd);
	SoundItemBase* Get(std::string name);

	void Play(const std::string& name, int volume = 0);

	std::map<std::string, SoundItemBase*>& GetVector() { return _v; }

	void StopType(SoundItemBase::TYPE type);

	//0.f~1.f;
	void ChangeVolume(SoundItemBase::TYPE type, float percent);

	void SetSEVolume(int vol) { _volSE = vol; }
	void SetBGMVolume(int vol) { _volBGM = vol; }
	void SetVOICEVolume(int vol) { _volVOICE = vol; }

	int GetSEVolume() { return _volSE; }
	int GetBGMVolume() { return _volBGM; }
	int GetVOICEVolume() { return _volVOICE; }

	int GetVolume(SoundItemBase::TYPE type)
	{
		switch (type)
		{
		case SoundItemBase::TYPE::BGM:
			return _volBGM;
		case SoundItemBase::TYPE::SE:
			return _volSE;
		case SoundItemBase::TYPE::VOICE:
			return _volVOICE;
		default:
			return -1;
		}
	}

	void Update();

protected:
	bool	_isUpdate;
	std::map<std::string, SoundItemBase*> _v;

	std::map<std::string, SoundItemBase*> _vAdd;
	std::map<std::string, SoundItemBase*> _vDel;

	int		_cntOneShot;							//ワンショットのカウント

	int _volSE;
	int _volBGM;
	int _volVOICE;


};

