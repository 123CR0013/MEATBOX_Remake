
#include "ApplicationMain.h"
#include "ModeSoundLayer.h"


bool ModeSoundLayer::Initialize() {
	if (!base::Initialize()) { return false; }

	// 非同期読み込み設定
	SetUseASyncLoadFlag(TRUE);

	int timems = GetNowCount();
	_asyncLoadBaseTimeMs = timems;	// 非同期ロード開始時間
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

	//// 音データファイルを読み込む
	global._soundServer->Add("BGM_01", NEW SoundItemBGM("res/Sound/bgm/title_01.wav"));
	global._soundServer->Add("BGM_02", NEW SoundItemBGM("res/Sound/bgm/stageselect_01.wav"));
	global._soundServer->Add("BGM_03", NEW SoundItemBGM("res/Sound/bgm/mainstage_01.wav"));
	global._soundServer->Add("BGM_04", NEW SoundItemBGM("res/Sound/bgm/mainstage_02.wav"));
	global._soundServer->Add("BGM_05", NEW SoundItemBGM("res/Sound/bgm/mainstage_03.wav"));
	global._soundServer->Add("BGM_06", NEW SoundItemBGM("res/Sound/bgm/lightsout_01.wav"));
	global._soundServer->Add("BGM_07", NEW SoundItemBGM("res/Sound/bgm/gameover_01.wav"));
	global._soundServer->Add("BGM_08", NEW SoundItemBGM("res/Sound/bgm/result_01.wav"));
	global._soundServer->Add("SE_01", NEW SoundItemSE("res/Sound/se/footsteps_01.wav"));
	global._soundServer->Add("SE_02", NEW SoundItemSE("res/Sound/se/knifeattack_01.wav"));
	global._soundServer->Add("SE_03", NEW SoundItemSE("res/Sound/se/knifehit_01.wav"));
	global._soundServer->Add("SE_04", NEW SoundItemSE("res/Sound/se/falldown_01.wav"));
	global._soundServer->Add("SE_05", NEW SoundItemSE("res/Sound/se/detectionrise_01.wav"));
	global._soundServer->Add("SE_06", NEW SoundItemSE("res/Sound/se/gunshot_01.wav"));
	global._soundServer->Add("SE_07", NEW SoundItemSE("res/Sound/se/siren_01.wav"));
	global._soundServer->Add("SE_08", NEW SoundItemSE("res/Sound/se/decoy_01.wav"));
	global._soundServer->Add("SE_09", NEW SoundItemSE("res/Sound/se/lightsoutstart_01.wav"));
	global._soundServer->Add("SE_10", NEW SoundItemSE("res/Sound/se/lightsoutfinish_01.wav"));
	global._soundServer->Add("SE_11", NEW SoundItemSE("res/Sound/se/selection_01.wav"));
	global._soundServer->Add("SE_12", NEW SoundItemSE("res/Sound/se/decision_01.wav"));
	global._soundServer->Add("SE_13", NEW SoundItemSE("res/Sound/se/confirmation_01.wav"));
	global._soundServer->Add("SE_14", NEW SoundItemSE("res/Sound/se/pause_01.wav"));
	global._soundServer->Add("SE_15", NEW SoundItemSE("res/Sound/se/timer_01.wav"));
	global._soundServer->Add("SE_27", NEW SoundItemSE("res/Sound/se/owldeath_01.wav"));
	global._soundServer->Add("SE_28", NEW SoundItemSE("res/Sound/se/energy_01.wav"));

	// ModeGame
	global._soundServer->Add("se_move_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_move_01.mp3"));
	global._soundServer->Add("se_question_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_question_01.mp3"));
	global._soundServer->Add("se_attack_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_attack_01.mp3"));
	global._soundServer->Add("se_kill_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_kill_01.mp3"));
	global._soundServer->Add("se_sticky_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_sticky_01.mp3"));
	global._soundServer->Add("se_damage_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_damage_01.mp3"));
	global._soundServer->Add("se_enemymove_01", NEW SoundItemSE("res/Sound/se/ModeGame/se_enemymove_01.mp3"));

	// 読み込み時間
	_loadTimeMs = GetNowCount() - timems;

	// 同期読み込み設定
	SetUseASyncLoadFlag(FALSE);

	// 選択リスト用
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

	// サウンドサーバ更新
	global._soundServer->Update();

	// 非同期ロードは終わったか？
	if (GetASyncLoadNum() == 0) {
		if (_asyncLoadTimeMs == 0) {
			// 非同期ロードにかかった時間
			_asyncLoadTimeMs = GetNowCount() - _asyncLoadBaseTimeMs;
		}
	}
	return true;
}



bool ModeSoundLayer::Render() {
	base::Render();
	return true;
}

