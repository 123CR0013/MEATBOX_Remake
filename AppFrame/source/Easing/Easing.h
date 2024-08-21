#pragma once	// .h�̐擪�ɋL�q�B#include�ł��̃t�@�C�������x�ǂݍ��݂��Ă��A1�x�����ǂݍ��܂Ȃ�
#include <math.h>
#define	PI (3.141592653589793)
#define TwoPI   (PI * 2.0f)
#define PIOver2 (PI / 2.0f)
// �l�X�ȃC�[�W���O
// https://game-ui.net/?p=835 ���Q�l��
namespace Easing
{
	float Linear(float cnt, float start, float end, float frames);
	float InQuad(float cnt, float start, float end, float frames);
	float OutQuad(float cnt, float start, float end, float frames);
	float InOutQuad(float cnt, float start, float end, float frames);
	float InCubic(float cnt, float start, float end, float frames);
	float OutCubic(float cnt, float start, float end, float frames);
	float InOutCubic(float cnt, float start, float end, float frames);
	float InQuart(float cnt, float start, float end, float frames);
	float OutQuart(float cnt, float start, float end, float frames);
	float InOutQuart(float cnt, float start, float end, float frames);
	float InQuint(float cnt, float start, float end, float frames);
	float OutQuint(float cnt, float start, float end, float frames);
	float InOutQuint(float cnt, float start, float end, float frames);
	float InSine(float cnt, float start, float end, float frames);
	float OutSine(float cnt, float start, float end, float frames);
	float InOutSine(float cnt, float start, float end, float frames);
	float InExpo(float cnt, float start, float end, float frames);
	float OutExpo(float cnt, float start, float end, float frames);
	float InOutExpo(float cnt, float start, float end, float frames);
	float InCirc(float cnt, float start, float end, float frames);
	float OutCirc(float cnt, float start, float end, float frames);
	float InOutCirc(float cnt, float start, float end, float frames);

	float OutElastic(float cnt, float start, float end, float frames);
};
