#include"Direct3D.h"

//�X�^�e�B�b�N�ȃ����o�ϐ��̏�����
Direct3D* Direct3D::pInstance = nullptr;

//�R���X�g���N�^
Direct3D::Direct3D()
{
	//�|�C���^�ɒl�������邾��
	//�܂��ǂ̃I�u�W�F�N�g���w���Ȃ�
	pD3D9 = NULL;
	pDevice3D = NULL;
}

//�f�X�g���N�^
Direct3D::~Direct3D()
{
	Release();
}

//����̊֐�
void Direct3D::Release()
{
	if (pDevice3D != NULL)
	{
		pDevice3D->Release();
		pD3D9->Release();

		pDevice3D = NULL;
		pD3D9 = NULL;
	}
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (pDevice3D != nullptr)
	{
		//���łɍ쐬����Ă���̂Ŏ��s
		return false;
	}

	//�쐬�̊֐����Ă�
	if (!Create(hWnd))
	{
		return false;
	}

	//�쐬����
	return true;
}

//�f�o�C�X�̍쐬
//TryCreate�֐�����Ă�
bool Direct3D::Create(HWND hWnd)
{
	//�o�b�N�o�b�t�@�i����ʁj�̑傫�����v�Z
	//�E�B���h�E�̃N���C�A���g�̈�̑傫���ɍ��킹��

	//�N���C�A���g�̈�̑傫���𒲂ׂ�
	RECT rect;
	GetClientRect(hWnd, &rect);

	//�o�b�N�o�b�t�@�̕��ƍ���
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;

	//Direct3D9�I�u�W�F�N�g�̍쐬
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
}