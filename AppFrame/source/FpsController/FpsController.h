#pragma once

//FLAME fps �ɂȂ�悤��fps���v�Z�E����
class FpsController {
public:
	FpsController();
	~FpsController();

	bool WaitFps();
	bool DrawFps(int x, int y);

protected:
	//fps
#define FLAME 60

//fps�̃J�E���^�A60�t���[����1���ƂȂ鎞�����L�^����ϐ�
	int fps_count, count0t;
	//���ς��v�Z���邽��60���1�����Ԃ��L�^
	int f[FLAME];
	//����fps
	double ave;
};