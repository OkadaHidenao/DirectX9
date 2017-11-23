#pragma once

#include"Direct3D.h"

//�L�����E�{�b�N�X
//OBB (����)
class OrientedBoundingBox
{
public:
	//���̕`��̍ۂ�D3D�ɓn�����_���
	//FVF�ŏ������Ƃ���̃����o
	//���Ԃ����܂��Ă���(�}�C�N���\�t�g�̃��t�@�����X�T�C�g���Q��)
	struct Vertex
	{
		//�O�������W
		D3DXVECTOR3 pos;

		//�@��
		//�ʂɐ����ȃx�N�g��
		D3DXVECTOR3 normal;

		//�F
		D3DCOLOR color;
	};
private:
	//���_�\���̂̍\���v�f���������
	static const DWORD FVF_TRIANGLE_LIST_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	//diffuse �g�U���F�@�F�F


protected:
	D3DXVECTOR3 position;//�{�b�N�X�̒��S���W

	//�{�b�N�X�̌����@(�W�����x�N�g��)
	//���ʁ@�E�@��@�̎O���
	//�W�����x�N�g���͌����݂̂̃x�N�g��(�������P)normalized
	D3DXVECTOR3 NormDirection[3];

	//�e���̒���
	float length[3];

	//�����̂̊p�̍��W
	D3DXVECTOR3 vertexPos[8];

public:
	OrientedBoundingBox();
	virtual ~OrientedBoundingBox();

	//�V�������S���W�̐ݒ�
	void SetPosition(D3DXVECTOR3 newPos);

	//�V���������̐ݒ�
	void SetDirection(D3DXVECTOR3 newForward,//����
		D3DXVECTOR3 newRight,				 //�E
		D3DXVECTOR3 newUp);					 //��

	//�e�ӂ̒���
	void SetLength(float forward, float right, float up);

	//�o�E���f�B���O�{�b�N�X�͈̗̔͂����̂̕`��
	HRESULT Draw(IDirect3DDevice9* pDevice);

private:
	//�������ɒ����̂𓊉e�������̒������v�Z����
	//pSeq ������(�W�����x�N�g��)
	//e1 e2 e3
	//�{�b�N�X�̕����x�N�g��(�����܂�)
	//�O�ώ��ɓ��e����ۂ�2�ӂ݂̂Ȃ̂�e3�̃f�t�H���g������null
	//(�O�ς̌v�Z�Ɏg�������͊O�ώ��ƒ��s���邽�ߓ_�Ƃ��ē��e�����̂Ł@�����O�Ƃ݂Ȃ�)
	static float LengthSegmentOnSepatateAxis
	(D3DXVECTOR3 *pSeq,
		D3DXVECTOR3 *e1, D3DXVECTOR3 *e2,
		D3DXVECTOR3 *e3 = NULL);

	//�p�̍��W���X�V
	//���_���W������Ⓑ�����ύX���ꂽ�ۂɌĂяo��
	void UpdateVertexPos();

public:
	//���OBB���ڐG���Ă����true��Ԃ�
	static bool Collision(OrientedBoundingBox &obb1, OrientedBoundingBox &obb2);



};