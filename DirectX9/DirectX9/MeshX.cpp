#include"MeshX.h"
#include<tchar.h>

MeshX::MeshX()
{
	pMesh = nullptr;
	ppTexture = nullptr;
	pMaterials = nullptr;
	numMaterials = 0;
}

MeshX::~MeshX()
{
	//�e�N�X�`�������[�h�ς݂Ȃ�
	if (ppTexture)
	{
		for (unsigned int i = 0; i < numMaterials; i++)
		{
			//�e�N�X�`���̉��
			if (ppTexture[i])
			{
				ppTexture[i]->Release();
			}
		}
		delete[] ppTexture;
	}
	//�}�e���A���̉��
	if (pMaterials)
	{
		delete[] pMaterials;
	}
	//���b�V���̉��
	if (pMesh)
	{
		pMesh->Release();
	}
}

bool MeshX::Load(TCHAR* path)
{
	Direct3D& pD3d = Direct3D::GetInstance();
	
	return pD3d LoadMeshX(*this.path);
}

bool MeshX::Load(std::string path)
{
	const TCHAR* path_t = _T(path.c_str());

	return Load(path_t);
}

void MeshX::Draw(D3DXMATRIXA16& matTransform, D3DXMATRIXA16& matRotation, D3DXMATRIXA16& matScale)
{
	//�e�s��̍���

	//���[���h�ϊ��s��@���W(���s�ړ�)�Ɖ�]�Ɗg��̕ϊ������ׂĉ������s��
	D3DXMATRIXA16 matWorld;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIXA16 matTmp;//�v�Z�p�̈ꎞ�I�ȍs��
	D3DXMatrixIdentity(&matTmp);

	//�s��̍���
	//�s��̊|���Z�͂����鏇�Ԃɂ���Č��ʂ��ς��
	//�����鏇�Ԃ����܂��Ă���̂Œ���
	//���Ԃ͊g�k�~��]�~���s�ړ�

	//Multply�@�|���Z
	//�g�k�~��]�@���ꎞ�ϐ��ɑ��
	D3DXMatrixMultiply(&matTmp, &matScale, &matRotation);

	//�ꎞ�ϐ�(�g�k�~��])�~���s�ړ��@�����[���h�ϊ��ɑ��
	D3DXMatrixMultiply(&matWorld, &matTmp, &matTransform);

	//�f�o�C�X�Ǘ��N���X���擾
	Direct3D& d3d = Direct3D::GetInstance();

	//�`��
	//d3d.DrawMeshX(*this, matWorld);
}
