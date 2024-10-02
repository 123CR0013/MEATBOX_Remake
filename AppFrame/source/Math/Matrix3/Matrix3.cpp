#include"Matrix3.h"

static float m3Ident[3][3] =
{
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};
const Matrix3 Matrix3::Identity(m3Ident);

Matrix3 Matrix3::Invert(Matrix3 in)
{
	Matrix3 inv_a(m3Ident); //�����ɋt�s�񂪓���
	float buf; //�ꎞ�I�ȃf�[�^��~����
	int i, j, k; //�J�E���^
	int n = 3;  //�z��̎���

	//�|���o���@
	for (i = 0; i < n; i++) {
		buf = 1 / in.mat[i][i];
		for (j = 0; j < n; j++) {
			in.mat[i][j] *= buf;
			inv_a.mat[i][j] *= buf;
		}
		for (j = 0; j < n; j++) {
			if (i != j) {
				buf = in.mat[j][i];
				for (k = 0; k < n; k++) {
					in.mat[j][k] -= in.mat[i][k] * buf;
					inv_a.mat[j][k] -= inv_a.mat[i][k] * buf;
				}
			}
		}
	}
	return inv_a;
}

Matrix3 Matrix3::Invert()
{
	return Invert(*this);
}

void Matrix3::Inverted()
{
	*this = Invert(*this);
}