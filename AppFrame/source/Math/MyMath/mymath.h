#pragma once
#include "dxlib.h"
#include <math.h>
#include <iostream>

// �~����
#define	PI				(3.141592653589793f)
#define TWOPI			(PI * 2.f)
#define PIOVER2			(PI / 2.f)

namespace MyMath
{
	// degree��radian 
	inline float DegToRad(float deg) {
		return deg * DX_PI / 180.0f;		// �p�x��degree����radian�ɕϊ�
	}

	// radian��degree
	inline float RadToDeg(float rad) {
		return rad * 180.0f / DX_PI;		// �p�x��radian����degree�ɕϊ�
	}

	inline float Max(float left, float right) {
		return left > right ? left : right;
	}

	inline float Min(float left, float right) {
		return left < right ? left : right;
	}

	inline float Clamp(float min_n, float max_n, float n) {
		if (n <= min_n) { return min_n; }
		if (n >= max_n) { return max_n; }
		return n;
	}

	template <typename T>
	void Swap(T& x, T& y)
	{
		T temp = std::move(x);
		x = std::move(y);
		y = std::move(temp);
	}

	inline float Cot(float angle) {
		return  1.0f / tanf(angle);
	}

	inline float Lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	inline bool NearZero(float val, float epsilon = 0.001f)
	{
		return fabs(val) <= epsilon;
	}

	//0:x 1:y 2:z
	VECTOR NormRotMatrix(VECTOR norm, float rotation, int type);

	VECTOR MatrixToVector(MATRIX matrix, int index);

	float LenSegOnSeparateAxis(VECTOR Sep, VECTOR e1, VECTOR e2, VECTOR e3 = VGet(0, 0, 0));

	MATRIX MMultXYZ(float dir_x, float dir_y, float dir_z);

	MATRIX MMultXYZToMatrix(float dir_x, float dir_y, float dir_z);


	bool SetMatrixToVector(VECTOR& vec_x, VECTOR& vec_y, VECTOR& vec_z, const MATRIX matrix);

	//�s�p���ǂ���
	bool CheckAcuteAngle(VECTOR p1, VECTOR p2, VECTOR p3);

	//���s���ǂ���
	bool CheckParallelRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	//�������ǂ���
	bool CheckVerticalRelation(VECTOR line_1_start, VECTOR line_1_end, VECTOR line_2_start, VECTOR line_2_end);

	// 2�̃x�N�g���̂Ȃ��p���v�Z����
	// �߂�l: �p�x(radian) 
	float CalcVectorAngle(VECTOR v1, VECTOR v2);

	// 3D���f��������vForward�̕��������ʂɂȂ�悤�ɉ�]������
	void SetModelForward_RotationY(int modelHandle, VECTOR vForward);
};