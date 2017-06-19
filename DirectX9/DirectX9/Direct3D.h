#pragma once

//�C���N���[�h�t�@�C���@Direct3D�֌n
#include<d3d9.h>
#include<d3dx9.h>

//���C�u�����t�@�C���̓ǂݍ���
//Direct3D�֌W
#pragma comment(lib,"d3d9lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

//�C���N���[�h�t�@�C���@windows
#include<Windows.h>

//���C�u�����t�@�C���@windows
#pragma comment(lib,"winmm.lib")

#include<tchar.h>


//�V���O���g���ȃN���X
//�C���X�^���X�����������Ȃ����Ƃ�ۏ؂���N���X
//�R���X�g���N�^�������Z�q��private�ɂ��ăI�u�W�F�N�g�̍쐬�̎�i�𐧌�
class Direct3D
{
private:

	//�N���X�O����̓I�u�W�F�N�g�������鏈���������Ȃ�
	Direct3D();
	~Direct3D();
	Direct3D(const Direct3D& obj) {};		//�R�s�[�R���X�g���N�^
	void operator=(const Direct3D& obj) {};//������Z

	//�B��̃I�u�W�F�N�g���w���|�C���^
	static Direct3D* pInstance = nullptr;

public:

	//�B��̃C���X�^���X�ւ̎Q�Ƃ��擾����֐�
	//�ŏ��̎��s�̎��ɃC���X�^���X���쐬����
	static Direct3D& GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Direct3D();
		}
		return *pInstance;//�B��̃C���X�^���X�ւ̎Q�Ƃ�Ԃ�
	}

	//�쐬�����B��̃C���X�^���X��j������
	static void DestroInstance();
	{
		if (pInstance != nullptr)
		{
			delete pInstance;
			pInstance = nullptr;
		}
	}

	//����֐�
	//�R���X�g���N�^���s���Ɠ�����Ԃɖ߂�
	void Release();

private:
	//Direct3D�̃f�o�C�X
	//�`���������摜�����[�h�����肷�邽�߂Ɏg�p����I�u�W�F�N�g
	IDirect3DDevice9*pDevice3D;

	//Direct3D�f�o�C�X�̍쐬���s�����߂̃I�u�W�F�N�g
	IDirect3D9* pD3D9;

	//�ȉ��@�����o�֐��̐錾
private:
	bool Create(HWND hWnd);

public:

	//Direct3D�f�o�C�X�̍쐬�����݂�
	//���s�����false���Ԃ�悤�ɂ���
	bool TryCreate(HWND hWnd);

	

};