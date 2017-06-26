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

	//�f�B�X�v���C�����擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//�X���b�v�`�F�C���̂��߂̐ݒ�
	//�X���b�v�`�F�C��
	//�X�N���[���̃o�b�N�o�b�t�@���t�����g�ɒu�������鏈���̂���

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;				//�o�b�N�o�b�t�@�̕��@�N���C�A���g�̈�Ɠ�����
	D3DParam.BackBufferHeight = height;				//�o�b�N�o�b�t�@�̍���
	D3DParam.BackBufferFormat = Display.Format;		//�o�b�N�o�b�t�@�̃t�H�[�}�b�g
													//�f�B�X�v���C�����
	D3DParam.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;	//�}���`�T���v�����O�̐ݒ�
													//�Ȃ�
	D3DParam.MultiSampleQuality = 0;				//�}���`�T���v�����O�̃N�I���e�B
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�X���b�v�`�F�C���̕��@
													//Direct3D�C��
	D3DParam.hDeviceWindow = hWnd;					//�`�悷��E�B���h�E�̎��ʃn���h��
	D3DParam.Windowed = TRUE;						//�E�B���h�E���[�h
	D3DParam.EnableAutoDepthStencil = TRUE;			//�[�x�X�e���V���o�b�t�@�@Z�o�b�t�@
													//�`��̍ہ@���łɎ�O�̂��̂�������Ă���Ɖ��̂��̂̕`����Ȃ����߂̏��
													//�X�N���[���ɐF�ł͂Ȃ����̃s�N�Z���ɏ����ꂽ���̂�
													//�J��������̋����������Ă���ƃC���[�W����Ƃ悢
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;	//�[�x�X�e���V���̃t�H�[�}�b�g
	D3DParam.Flags = 0;								//�ݒ�̃t���O
	D3DParam.FullScreen_RefreshRateInHz = 0;		//�E�B���h�E���[�h�͎g�p���Ȃ��̂�0
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//�A�_�v�^�̃��t���b�V�����[�g��present���������s���背�[�g�̊֌W

	//�������̐ݒ�Ńf�o�C�X�쐬�����݂�
	//HAL���[�h�ō쐬
	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)
	))
	{
		//��ڂ̐ݒ�Ŏ��s������
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//��ڂ̐ݒ肩��@HARDWEAR->MIXED
			&D3DParam, &pDevice3D)
		))
		{
			//��ڂ̐ݒ�Ŏ��s������
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,			//��ڂ̐ݒ肩��HAL->REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)
			))
			{
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//�O�ڂ̐ݒ肩��HARDWARE->SOFTWARE
					&D3DParam, &pDevice3D)
				))
				{
					//�S���ׂĂ̐ݒ�ō쐬�����s������

					//������������Ď��s�����Ŋ֐����I����
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}


	//�����ɗ����Ƃ������Ƃ͂ǂꂩ�̐ݒ�Ńf�o�C�X�̍쐬����������
	return true;
}

HRESULT Direct3D::BeginScene()
{
	if (pDevice3D != nullptr)
	{
		//Direct3D�f�o�C�X��BeginSene�����s
		return pDevice3D->BeginScene();
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT Direct3D::EndScene()
{
	if (pDevice3D != nullptr)
	{
		//Direct3D�f�o�C�X��EndSene�����s
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

//�o�b�N�o�b�t�@�̃N���A
HRESULT Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		//�w�i�̃N���A�E
		//�����`�悳��Ă��Ȃ���Ԃ��Ƃ��̐F����ʂɏo��
		D3DXCOLOR Color = D3DCOLOR_XRGB(0, 128, 0);

		//Clear�̎��s���ʂ�Ԃ�
		return pDevice3D->Clear(0, NULL,
			//�N���A�̃^�[�Q�b�g�ƂȂ�o�b�t�@���w��
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER
			, Color, 1.0f, 0);
	}
	else
	{
		return S_FALSE;
	}
}

//�o�b�N�o�b�t�@���t�����g�ɔ��f
HRESULT Direct3D::Present()
{
	if (pDevice3D != nullptr)
	{
		return pDevice3D->Present(NULL, NULL, NULL, NULL);
	}
	else
	{
		return S_FALSE;
	}
}