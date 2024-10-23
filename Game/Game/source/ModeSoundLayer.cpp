
#include "ApplicationMain.h"
#include "ModeSoundLayer.h"


bool ModeSoundLayer::Initialize() {
	if (!base::Initialize()) { return false; }

	// �񓯊��ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(TRUE);

	int timems = GetNowCount();
	_asyncLoadBaseTimeMs = timems;	// �񓯊����[�h�J�n����
	_asyncLoadTimeMs = 0;

	CFile volumeFile("data/Volume.txt");
	if (volumeFile.Success())
	{
		size_t pos = 0;
		size_t to = volumeFile.DataStr().find(",");
		std::string param = volumeFile.DataStr().substr(pos, to);
		pos = to + 1;

		global._soundServer->SetSEVolume(std::stoi(param));

		to = volumeFile.DataStr().find(",");
		param = volumeFile.DataStr().substr(pos, to);
		pos = to + 1;

		global._soundServer->SetBGMVolume(std::stoi(param));

		to = volumeFile.DataStr().find(",");
		param = volumeFile.DataStr().substr(pos, to);
		pos = to + 1;

		global._soundServer->SetVOICEVolume(std::stoi(param));
	}

	//// ���f�[�^�t�@�C����ǂݍ���
	global._soundServer->Add("BGM_01", NEW SoundItemBGM("res/Sound/bgm/title_01.wav"));
	global._soundServer->Add("BGM_02", NEW SoundItemBGM("res/Sound/bgm/stageselect_01.wav"));
	global._soundServer->Add("BGM_03", NEW SoundItemBGM("res/Sound/bgm/mainstage_01.wav"));
	global._soundServer->Add("BGM_04", NEW SoundItemBGM("res/Sound/bgm/mainstage_02.wav"));
	global._soundServer->Add("BGM_05", NEW SoundItemBGM("res/Sound/bgm/mainstage_03.wav"));
	global._soundServer->Add("BGM_06", NEW SoundItemBGM("res/Sound/bgm/lightsout_01.wav"));
	global._soundServer->Add("BGM_07", NEW SoundItemBGM("res/Sound/bgm/gameover_01.wav"));
	global._soundServer->Add("BGM_08", NEW SoundItemBGM("res/Sound/bgm/result_01.wav"));
	global._soundServer->Add("SE_01", NEW SoundItemSE("res/Sound/se/se_select_01.mp3"));
	global._soundServer->Add("SE_02", NEW SoundItemSE("res/Sound/se/se_count_01.mp3"));
	global._soundServer->Add("SE_03", NEW SoundItemSE("res/Sound/se/se_decision_01.mp3"));
	global._soundServer->Add("SE_04", NEW SoundItemSE("res/Sound/se/se_display_01.mp3"));
	global._soundServer->Add("SE_05", NEW SoundItemSE("res/Sound/se/se_hidden_01.mp3"));
	global._soundServer->Add("SE_06", NEW SoundItemSE("res/Sound/se/se_rank_01.mp3"));


	// �ǂݍ��ݎ���
	_loadTimeMs = GetNowCount() - timems;

	// �����ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(FALSE);

	// �I�����X�g�p
	_listCur = 0;
	_listViewStart = 0;
	_listViewNum = 20;

	return true;
}

bool ModeSoundLayer::Terminate() {
	base::Terminate();
	return true;
}

bool ModeSoundLayer::Process() {
	base::Process();

	// �T�E���h�T�[�o�X�V
	global._soundServer->Update();

	// �񓯊����[�h�͏I��������H
	if (GetASyncLoadNum() == 0) {
		if (_asyncLoadTimeMs == 0) {
			// �񓯊����[�h�ɂ�����������
			_asyncLoadTimeMs = GetNowCount() - _asyncLoadBaseTimeMs;
		}
	}
	return true;
}



bool ModeSoundLayer::Render() {
	base::Render();
	return true;
}

