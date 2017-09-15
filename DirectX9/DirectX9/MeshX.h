#pragma once

#include"Direct3D.h"

#include<string>

//.x�`����3D���f���t�@�C���@���ێ�����N���X
class MeshX
{
	friend class Direct3D;
private:
	LPD3DXMESH pMesh;//���b�V��
	DWORD	   numMaterials;//�}�e���A���̌�

	LPDIRECT3DTEXTURE9* ppTexture;//�e�N�X�`���̔z��
	D3DMATERIAL9*		pMaterials;//�}�e���A���̔z��

public:
	MeshX();
	~MeshX();

	//�`��
	//MATRIX �s��
	void Draw(D3DXMATRIXA16& matTransform, //���W
		D3DXMATRIXA16& matRotation,		  //��]
		D3DXMATRIXA16& matScale);		  //�g��k��

	bool Load(TCHAR* path);
	bool Load(std::string path);
};