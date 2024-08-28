// 独自計算ライブラリ
#include "Easing.h"
#include"../Math/MyMath/mymath.h"

// イージング
float Easing::Linear(float cnt, float start, float end, float frames) {
    return (end - start) * cnt / frames + start;
}
float Easing::InQuad(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return (end - start) * cnt * cnt + start;
}
float Easing::OutQuad(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return -(end - start) * cnt * (cnt - 2) + start;
}
float Easing::InOutQuad(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return (end - start) / 2.0 * cnt * cnt + start;
    }
    cnt--;
    return -(end - start) / 2.0 * (cnt * (cnt - 2) - 1) + start;
}
float Easing::InCubic(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return (end - start) * cnt * cnt * cnt + start;
}
float Easing::OutCubic(float cnt, float start, float end, float frames) {
    cnt /= frames;
    cnt--;
    return (end - start) * (cnt * cnt * cnt + 1) + start;
}
float Easing::InOutCubic(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return (end - start) / 2.0 * cnt * cnt * cnt + start;
    }
    cnt -= 2;
    return (end - start) / 2.0 * (cnt * cnt * cnt + 2) + start;
}
float Easing::InQuart(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return (end - start) * cnt * cnt * cnt * cnt + start;
}
float Easing::OutQuart(float cnt, float start, float end, float frames) {
    cnt /= frames;
    cnt--;
    return -(end - start) * (cnt * cnt * cnt * cnt - 1) + start;
}
float Easing::InOutQuart(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return (end - start) / 2.0 * cnt * cnt * cnt * cnt + start;
    }
    cnt -= 2;
    return -(end - start) / 2.0 * (cnt * cnt * cnt * cnt - 2) + start;
}
float Easing::InQuint(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return (end - start) * cnt * cnt * cnt * cnt * cnt + start;
}
float Easing::OutQuint(float cnt, float start, float end, float frames) {
    cnt /= frames;
    cnt--;
    return (end - start) * (cnt * cnt * cnt * cnt * cnt + 1) + start;
}
float Easing::InOutQuint(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return (end - start) / 2.0 * cnt * cnt * cnt * cnt * cnt + start;
    }
    cnt -= 2;
    return (end - start) / 2.0 * (cnt * cnt * cnt * cnt * cnt + 2) + start;
}
float Easing::InSine(float cnt, float start, float end, float frames) {
    return -(end - start) * cos(cnt / frames * (PIOVER2)) + end + start;
}
float Easing::OutSine(float cnt, float start, float end, float frames) {
    return (end - start) * sin(cnt / frames * PIOVER2) + start;
}
float Easing::InOutSine(float cnt, float start, float end, float frames) {
    return -(end - start) / 2.0 * (cos(PI * cnt / frames) - 1) + start;
}
float Easing::InExpo(float cnt, float start, float end, float frames) {
    return (end - start) * pow(2.0, 10 * (cnt / frames - 1)) + start;
}
float Easing::OutExpo(float cnt, float start, float end, float frames) {
    return (end - start) * (-pow(2.0, -10 * cnt / frames) + 1) + start;
}
float Easing::InOutExpo(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return (end - start) / 2.0 * pow(2.0, 10 * (cnt - 1)) + start;
    }
    cnt--;
    return (end - start) / 2.0 * (-pow(2.0, -10 * cnt) + 2) + start;
}
float Easing::InCirc(float cnt, float start, float end, float frames) {
    cnt /= frames;
    return -(end - start) * (sqrt(1 - cnt * cnt) - 1) + start;
}
float Easing::OutCirc(float cnt, float start, float end, float frames) {
    cnt /= frames;
    cnt--;
    return (end - start) * sqrt(1 - cnt * cnt) + start;
}
float Easing::InOutCirc(float cnt, float start, float end, float frames) {
    cnt /= frames / 2.0;
    if(cnt < 1) {
        return -(end - start) / 2.0 * (sqrt(1 - cnt * cnt) - 1) + start;
    }
    cnt -= 2;
    return (end - start) / 2.0 * (sqrt(1 - cnt * cnt) + 1) + start;
}

float Easing::InBack(float cnt, float start, float end, float frames) {
    const float c = 1.70158f;   //最大下限が変わる
    return  start + (end - start) * ((c + 1.f) * powf(cnt / frames, 3.f) - c * powf(cnt / frames, 2.f));
}

float Easing::OutBack(float cnt, float start, float end, float frames) {
    const float c = 1.70158f;   //最大上限が変わる
    return  start + (end - start) * (1.f - ((c + 1.f) * powf(1.f - cnt / frames, 3.f) - c * powf(1.f - cnt / frames, 2.f)));
}

float Easing::OutBounce(float cnt, float start, float end, float frames)
{
    float a = 7.5625f;
    float b = 2.75f;

    float x = cnt / frames;

    if (x < 1.0f / b)
    {
        return start + (end - start) * (a * x * x);
    }
    else if (x < 2.0f / b)
    {
        float c = (x - 1.5f / b);
        return start + (end - start) * (a * c * c + 0.75f);
    }
    else if (x < 2.5 / b)
    {
        float c = (x - 2.25f / b);
        return start + (end - start) * (a * c * c + 0.9375f);
    }
    else
    {
        float c = (x - 2.625f / b);
        return start + (end - start) * (a * c * c + 0.984375f);
    }
}

float Easing::OutElastic(float cnt, float start, float end, float frames)
{
    if (cnt == 0) {
        return start;
    }
    else if (cnt == frames) {
        return end;
    }
    else {
        float time = cnt / frames;
        float rad = (2.0f * PI) / 3.0f;
        float rate = pow(2, -10 * time) * sin((time * 10 - 0.75) * rad) + 1;

        return  start + (end - start) * rate;
    }
}

