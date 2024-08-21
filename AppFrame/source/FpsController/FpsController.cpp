#include "DxLib.h"
#include "FpsController.h"

FpsController::FpsController()
{
    fps_count = 0;
    count0t = 0;
    for (int i = 0; i < FLAME; i++)
    {
        f[i] = 0;
    }
    ave = 0;
}

FpsController::~FpsController()
{

}

//FLAME fps �ɂȂ�悤��fps���v�Z�E����
bool FpsController::WaitFps() {
    int term, i, gnt;
    static int t = 0;
    if (fps_count == 0) {//60�t���[����1��ڂȂ�
        if (t == 0)//���S�ɍŏ��Ȃ�܂��Ȃ�
            term = 0;
        else//�O��L�^�������Ԃ����Ɍv�Z
            term = count0t + 1000 - GetNowCount();
    }
    else    //�҂ׂ�����=���݂���ׂ�����-���݂̎���
        term = (int)(count0t + fps_count * (1000.0 / FLAME)) - GetNowCount();

    if (term > 0)//�҂ׂ����Ԃ����҂�
        Sleep(term);

    gnt = GetNowCount();

    if (fps_count == 0)//60�t���[����1�x������
        count0t = gnt;
    f[fps_count] = gnt - t;//�P���������Ԃ��L�^
    t = gnt;
    //���όv�Z
    if (fps_count == FLAME - 1) {
        ave = 0;
        for (i = 0; i < FLAME; i++)
            ave += f[i];
        ave /= FLAME;
    }
    fps_count = (++fps_count) % FLAME;

    return true;
}

bool FpsController::DrawFps(int x, int y) {
    if (ave != 0) {
        DrawBox(x, y, x + 85, y + 20, GetColor(128, 128, 128), true);
        DrawFormatString(x + 2, y + 2, GetColor(255, 255, 255), "fps:%2.2lf", 1000 / ave); //double�^��%lf
    }
    return true;
}