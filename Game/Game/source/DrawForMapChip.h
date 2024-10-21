#pragma once
#include "appframe.h"

class DrawForMapChip
{
public:
	// �}�b�v�`�b�v�P�ʂł̕`��
	// @param mView �r���[�s��
	// @param position �}�b�v�`�b�v���W
	// @param zoom �g�嗦
	// @param angle ��]�p�x
	// @param width �`��T�C�Y ��
	// @param height �`��T�C�Y ����
	// @param cgHandle �摜�n���h��
	// @param turn �摜�̌��� 0:���̂܂� 0�ȊO:���E���]
	static void MyDrawModiGraph(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);
	static void MyDrawModiGraph(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);

	// �}�b�v�`�b�v�P�ʂŉ�ʃT�C�Y�䗦�ɍ��킹�ĕ`��
	// @param mView �r���[�s��
	// @param position �}�b�v�`�b�v���W
	// @param zoom �g�嗦
	// @param angle ��]�p�x
	// @param width �`��T�C�Y ��
	// @param height �`��T�C�Y ����
	// @param cgHandle �摜�n���h��
	// @param turn �摜�̌��� 0:���̂܂� 0�ȊO:���E���]
	static void MyDrawModiGraphWithScreenSize(MATRIX mView, VECTOR position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);
	static void MyDrawModiGraphWithScreenSize(MATRIX mView, Vector3 position, float zoom, float angle, int width, int height, int cgHandle, int turn = 0);
};