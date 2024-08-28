#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include <math.h>
#include<vector>
#include<functional>

// 様々なイージング
// https://game-ui.net/?p=835 を参考に
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
	float InBack(float cnt, float start, float end, float frames);
	float OutBack(float cnt, float start, float end, float frames);
	float OutBounce(float cnt, float start, float end, float frames);

	static std::vector<std::function<float(float, float, float, float)>>EasingFuncs = {
		Linear		,
		InQuad		,
		OutQuad		,
		InOutQuad		,
		InCubic		,
		OutCubic		,
		InOutCubic	,
		InQuart		,
		OutQuart		,
		InOutQuart	,
		InQuint		,
		OutQuint		,
		InOutQuint	,
		InSine		,
		OutSine		,
		InOutSine		,
		InExpo		,
		OutExpo		,
		InOutExpo		,
		InCirc		,
		OutCirc		,
		InOutCirc		,
		InBack		,
		OutBack		,
		OutBounce		,
		OutElastic
	};

	//上の配列の要素を参照するための番号
	constexpr unsigned char LINEAR = 0;
	constexpr unsigned char IN_QUAD = 1;
	constexpr unsigned char OUT_QUAD = 2;
	constexpr unsigned char INOUT_QUAD = 3;
	constexpr unsigned char IN_CUBIC = 4;
	constexpr unsigned char OUT_CUBIC = 5;
	constexpr unsigned char INOUT_CUBIC = 6;
	constexpr unsigned char IN_QUART = 7;
	constexpr unsigned char OUT_QUART = 8;
	constexpr unsigned char INOUT_QUART = 9;
	constexpr unsigned char IN_QUINT = 10;
	constexpr unsigned char OUT_QUINT = 11;
	constexpr unsigned char INOUT_QUINT = 12;
	constexpr unsigned char IN_SINE = 13;
	constexpr unsigned char OUT_SINE = 14;
	constexpr unsigned char INOUT_SINE = 15;
	constexpr unsigned char IN_EXPO = 16;
	constexpr unsigned char OUT_EXPO = 17;
	constexpr unsigned char INOUT_EXPO = 18;
	constexpr unsigned char IN_CIRC = 19;
	constexpr unsigned char OUT_CIRC = 20;
	constexpr unsigned char INOUT_CIRC = 21;
	constexpr unsigned char IN_BACK = 22;
	constexpr unsigned char OUT_BACK = 23;
	constexpr unsigned char OUT_BOUNCE = 24;
	constexpr unsigned char OUT_ELASTIC = 25;
};
