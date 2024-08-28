#include "SoundServer.h"

SoundServer::SoundServer() {
	_isUpdate = false;
	_cntOneShot = 0;

	_volBGM = 255;
	_volSE = 255;
	_volVOICE = 255;
}
SoundServer::~SoundServer() {
	Clear();
}

void SoundServer::Clear() {
	for (auto&& e : _v) {
		delete e.second;
	}
	_v.clear();
}

void SoundServer::Add(SoundItemOneShot* snd) {
	std::string name = "__ONESHOT_SOUND_" + std::to_string(_cntOneShot);
	_cntOneShot++;
	Add(name, snd);
	snd->Play();		// OneShot�͓o�^�����u�ԍĐ�����
}

void SoundServer::Add(std::string name, SoundItemBase* snd) {
	if (_isUpdate) {
		_vAdd[name] = snd;
	}
	else {
		_v[name] = snd;
	}
	snd->SetSoundServer(this);
}

bool SoundServer::Del(SoundItemBase* snd) {
	for (auto&& e : _v) {
		if (e.second == snd) {
			if (_isUpdate) {
				_vDel[e.first] = e.second;
			}
			else {
				std::string name = e.first;
				delete _v[name];
				_v.erase(name);
			}
			return true;
		}
	}
	return false;
}

bool SoundServer::Del(std::string name) {
	if (_v.count(name) > 0) {
		return Del(_v[name]);
	}
	return false;
}

SoundItemBase* SoundServer::Get(std::string name) {
	if (_v.count(name) > 0) { return _v[name]; }
	return NULL;
}

void SoundServer::StopType(SoundItemBase::TYPE type) {
	for (auto&& e : _v) {
		if (e.second->GetType() == type) {
			e.second->Stop();
		}
	}
}

void SoundServer::ChangeVolume(SoundItemBase::TYPE type, float percent)
{
	for (auto&& e : _v) {
		if (e.second->GetType() != type)continue;

		switch (e.second->GetType())
		{
		case SoundItemBase::TYPE::BGM:
			_volBGM = static_cast<int>(255.f * percent);
			e.second->SetVolume(_volBGM);
			break;

		case SoundItemBase::TYPE::SE:
			_volSE = static_cast<int>(255.f * percent);
			e.second->SetVolume(_volSE);
			break;
		case SoundItemBase::TYPE::VOICE:
			_volVOICE = static_cast<int>(255.f * percent);
			e.second->SetVolume(_volVOICE);
			break;
		default:
			break;
		}
	}
}


void SoundServer::Update() {
	// �ǉ����X�g�̂��̂�ǉ�
	for (auto&& e : _vAdd) { Add(e.first, e.second); }
	_vAdd.clear();

	// Update����
	_isUpdate = true;
	for (auto&& e : _v) {
		e.second->Update();
	}
	_isUpdate = false;

	// �폜���X�g�̂��̂��폜
	for (auto&& e : _vDel) { Del(e.second); }
	_vDel.clear();
}

void SoundServer::Play(const std::string& name, int volume) {

	auto snd = Get(name);

	if (snd) {

		if (volume > 0)
			{
			snd->SetVolume(volume);
			snd->Play();
		}
		else {

			switch (snd->GetType())
			{
			case SoundItemBase::TYPE::BGM:
				snd->SetVolume(_volBGM);
				break;

			case SoundItemBase::TYPE::SE:
				snd->SetVolume(_volSE);
				break;
			case SoundItemBase::TYPE::VOICE:
				snd->SetVolume(_volVOICE);
				break;
			default:
				break;
			}
			snd->Play();
		}
	}

}