#include "appframe.h"

#include <string>

// ���[�h
class ModeSoundLayer : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:
	/*void PrintSoundItem(int x, int y, std::string name, class SoundItemBase* snd);*/

protected:


	int		_loadTimeMs;		// ���[�h�o�^�ɂ�����������
	int		_asyncLoadBaseTimeMs;	// �񓯊����[�h�ɂ�����������(�v�Z�p)
	int		_asyncLoadTimeMs;		// �񓯊����[�h�ɂ�����������

	std::string		_msg;			// �������b�Z�[�W�\���p

	int		_listCur;			// �I�����X�g�J�[�\���ʒu
	int		_listViewStart, _listViewNum;	// �I�����X�g�\���֘A

}; 


