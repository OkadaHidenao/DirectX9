#include "Direct3D.h"

#include"Sprite.h"
#include"Texture.h"
#include"MeshX.h"

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

//�f�o�C�X�̍쐬�����݂�
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
	//�o�b�N�o�b�t�@(�����)�̂����������v�Z
	//�E�B���h�E�̃N���C�A���g�̈�̂��������ɍ��킹��

	//�N���C�A���g�̈�̂��������𒲂ׂ�
	RECT rect;
	GetClientRect(hWnd, &rect);

	//�o�b�N�o�b�t�@�̕��ƍ���
	int width  = rect.right  - rect.left;
	int height = rect.bottom - rect.top;

	//Direct3D9�I�u�W�F�N�g�̍쐬
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//�f�B�X�v���C�����擾
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//�X���b�v�`�F�C���ׂ̈̐ݒ�
	//�X���b�v�`�F�C���@
	//�X�N���[���̃o�b�N�o�b�t�@��
	//�t�����g�o�b�t�@�ɒu�������鏈���̂���

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;				//�o�b�N�o�b�t�@�̕��@�N���C�A���g�̈�Ɠ���
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
													//�`��̍ہ@���łɎ�O�̂��̂�������Ă����
													//���̂��̂̕`����Ȃ����߂̏��@
													//�X�N���[���ɐF�ł͂Ȃ����̃s�N�Z���ɏ����ꂽ���̂�
													//�J��������̋����������Ă���ƃC���[�W����Ɨǂ�
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;	//�[�x�X�e���V���̃t�H�[�}�b�g
	D3DParam.Flags = 0;								//�ݒ�̃t���O
	D3DParam.FullScreen_RefreshRateInHz = 0;		//�E�B���h�E���[�h�͎g�p���Ȃ��̂�0

	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//�A�_�v�^�̃��t���b�V�����[�g��present���������s���郌�[�g�̊֌W

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
		//1�ڂ̐ݒ�Ŏ��s������
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//��ڂ̐ݒ肩�� HARDWARE -> MIXED
			&D3DParam, &pDevice3D)
		))
		{
			//2�ڂ̐ݒ�Ŏ��s������
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,		//��ڂ̐ݒ肩�� HAL -> REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)
			))
			{
				//3�ڂ̐ݒ�Ŏ��s������
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//3�ڂ̐ݒ肩�� HARDWARE -> MIXED
					&D3DParam, &pDevice3D)
				))
				{
					//4�S�Ă̐ݒ�ō쐬�����s������

					//�������������
					//���s�����Ŋ֐����I����
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}
	//�����ɗ����Ƃ������Ƃ͂ǂꂩ�̐ݒ�Ńf�o�C�X�̍쐬����������
	//�E�B���h�E�̃n���h�����o���Ă���
	this->hWnd = hWnd;
	
	return true;

}

//�`��̐ݒ�
void Direct3D::SetRenderState(RENDERSTATE state)
{
	//�f�o�C�X���쐬����Ă��Ȃ��Ȃ璆�f
	if (pDevice3D == nullptr)return ;

	switch (state)
	{
		case RENDER_DEFAULT:
			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);//���e�X�g����
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//���u�����h����

			break;
		case RENDER_ALPHATEST:
			//���������̐؂蔲��
			//���l�ł��̃s�N�Z����`�悷�邩�𔻒f����
			//���f��̃��l�͂��炩���ߌ��߂Ă���
			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, true);//���e�X�g�L��
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//���u�����h����
			pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);//���f��l
			pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//��l�ȏ�ŕ`��

			break;
		case RENDER_ALPHABLEND:
			//�������`��

			pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);//���e�X�g����
			pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//���u�����h�L��

			//�u�����h�W���@������`�悷��s�N�Z���F�ɑ΂������
			pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		
			//�u�����h�W��	���Ƀo�b�N�o�b�t�@�ɏ�����Ă���s�N�Z���F�ɑ΂������
			pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
			
			//�ŏI�I�ȕ`��F�@���@
			//	SRC�F�~�u�����f�B���O�W���iSRC)�@�{�@DEST�F�~�u�����f�B���O�W���iDEST�j

			break;	
	}


}

HRESULT Direct3D::BeginScene()
{	
	if (pDevice3D != nullptr)
	{
		//Direct3D�f�o�C�X��BegineScene�����s
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
		//Direct3D�f�o�C�X��EndScene�����s
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

//�o�b�N�o�b�t�@�̃N���A
HRESULT  Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		//�w�i�̃N���A�F
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

void Direct3D::DrawSprite(Sprite& sprite,Texture& tex)
{
	//�f�o�C�X���쐬����Ă��Ȃ����
	//�`��̏����ɓ��炸return����
	if (pDevice3D == nullptr)return;

	//�`��Ɏg�p����ϐ���sprite������o��
	D3DXVECTOR2 pos = sprite.GetCentrePos();
	float width = sprite.GetWidth();
	float height = sprite.GetHeight();
	float alpha = sprite.GetAlpha();

	char alphaC = static_cast<char>(255 * alpha);

	float u = tex.numU;
	float v = tex.numV;
	float du = tex.divU;
	float dv = tex.divV;

	//�l�p�`�Ȃ̂Œ��_4��
	//�g���C�A���O���X�g���b�v���g�p����̂�
	//vertex�Ɋi�[���鏇�Ԃ� �E��@�E���@����@����
	SpriteVertex vertex[4] =
	{
		//			�@x   y   z	 rhw	���_�F		�ԁ@�΁@�@�����@u	  v
		{ D3DXVECTOR3( width / 2 ,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,		v/dv },
		{ D3DXVECTOR3( width / 2 , height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,	 (v+1)/dv},
		{ D3DXVECTOR3(-width / 2 ,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	u/du,		v/dv },
		{ D3DXVECTOR3(-width / 2 , height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	u/du,	(v+1)/dv }
	};

	float angle = sprite.GetAngle_Rad();
	
	for (int i = 0; i < 4; i++)
	{
		//�O�p�֐��@���@�藝�𗘗p����4�̒��_�̍��W��ϊ�
		//���_�𒆐S�Ƃ���angle�̕�������
		float x = vertex[i].pos.x*cosf(angle) -
			vertex[i].pos.y*sinf(angle);

		float y = vertex[i].pos.x*sinf(angle) +
			vertex[i].pos.y*cosf(angle);

		//���S���W���������s�ړ�
		D3DXVECTOR3 vPos;
		vPos.x = pos.x + x;
		vPos.y = pos.y + y;
		vPos.z = 0;
		vertex[i].pos = vPos;


	}

	//�e�N�X�`���̎w��
	pDevice3D->SetTexture(0, tex.pTexture);

	//���_�\���̐錾
	pDevice3D->SetFVF(SPRITE_FVF);

	//�|���S���̕`��
	//�g���C�A���O���X�g���b�v�ŕ`��

	//���� (�`��^�C�v,
		//�O�p�`�̐�,
		//���_�\���̔z��̐擪�A�h���X,
		//�ЂƂ̒��_�\���̂̃T�C�Y);
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, 
		vertex, sizeof(SpriteVertex));

}

//�e�N�X�`���̃��[�h
bool Direct3D::LoadTexture(Texture& tex, TCHAR* FilePath)
{
	//�f�o�C�X���쐬����Ă��邩�ǂ����̊m�F
	if (pDevice3D != nullptr)
	{
		if (SUCCEEDED(
			D3DXCreateTextureFromFile(
				pDevice3D, FilePath, &(tex.pTexture)
			)//createFromFile
			)//succeeded
			)//if
		{
			//���[�h�ɐ�������
			return true;
		}
	}

	//�f�o�C�X�����݂��Ȃ�����
	//���݂������쐬�Ɏ��s����
	return false;//���s
}

//.x�`���̃��b�V���ǂݍ���
bool Direct3D::LoadMeshX(MeshX& mesh, TCHAR*path)
{
	//X�t�@�C���̃p�X�̎擾
	//�e�N�X�`���̓ǂݍ��ݎ��Ɏg�p
	CHAR  dir[_MAX_DIR];
	_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	//���b�V����ǂݍ��񂾌�}�e���A�����ꎞ�I�ɕۑ����Ă����ϐ�
	LPD3DXBUFFER pBufferMatrial;

	//�ǂݍ���
	if (D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice3D, NULL, &pBufferMatrial, NULL, &mesh.numMaterials, &mesh.pMesh)!=D3D_OK)
	{
		return false;
	}

	//�}�e���A���̏���
	if (mesh.numMaterials > 0)
	{
		//�e�N�X�`���ƃ}�e���A�������ꂼ��}�e���A���̌����m��
		//�����̃}�e���A���ň�̃e�N�X�`�������L����ꍇ���l�����疳�ʂ��o�Ă��܂�
		mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
		mesh.ppTexture = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

		//�o�b�t�@���}�e���A���̌`���ɃL���X�g
		D3DXMATERIAL* d3dxmaterials = (D3DXMATERIAL*)pBufferMatrial->GetBufferPointer();

		//�}�e���A�����ЂƂ��o�b�t�@����R�s�[
		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			mesh.pMaterials[i] = d3dxmaterials[i].MatD3D;

			//�����ɑ΂��锽�ˌW��(�F)���g�U�����˂̂��̂ƈꏏ�ɂ���
			mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

			mesh.ppTexture[i] = nullptr;

			//�e�N�X�`���̃t�@�C���������o���ă��[�h
			if (d3dxmaterials[i].pTextureFilename != nullptr)
			{
				//�e�N�X�`���̃t�@�C���p�X�̍쐬
				CHAR textureFilepath[1024];
				ZeroMemory(textureFilepath, sizeof(textureFilepath));

				//�t�@�C���p�X�Ƀ��b�V���t�@�C���܂ł̃o�X����������
				lstrcat(textureFilepath, dir);

				//�t�@�C���p�X�̖����Ƀe�N�X�`���̃t�@�C������ǉ�
				lstrcat(textureFilepath, d3dxmaterials[i].pTextureFilename);

				//������p�X��p���ăe�N�X�`���t�@�C���̃��[�h
				if (D3DXCreateTextureFromFile(pDevice3D, textureFilepath, &mesh.ppTexture[i]) != D3D_OK)
				{
					mesh.ppTexture[i] = nullptr;
				}
			}
		}
	}
	//�o�b�t�@�̉��
	pBufferMatrial->Release();

	return true;
}
//.x�`���̃��b�V���`��
void Direct3D::DrawMeshX(MeshX& mesh, D3DXMATRIXA16& matWorld)
{
	if (mesh.pMesh != nullptr)
	{
		//���b�V�������[�h�ς݂ł���
		//SetTransform�֐������Direct3D�̃f�o�C�X�Ƀ��[���h�ϊ��s���n��
		//���������s��̎��
		//D3DTS_WORLD�̏ꍇ�̓��b�V���̕ϊ��s��(���W�@��]�@�g�k)
		pDevice3D->SetTransform(D3DTS_WORLD, &matWorld);

		//���_�V�F�[�_(�ݒ肵�Ȃ��@�f�t�H���g���g�p)
		//�|���S���̊e���_���X�N���[���̂ǂ̈ʒu�ɂ��邩���v�Z�������
		pDevice3D->SetVertexShader(NULL);

		//���_�t�H�[�}�b�g
		//�|���S���̈�̒��_�̏����i�[�����\���̂��ǂ̂悤�ȍ\���ɂȂ��Ă��邩���f�o�C�X�ɓn��
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());

		//�}�e���A�����ƂɃ��b�V����`��
		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			//�}�e���A���Ƃ���ɑΉ�����e�N�X�`�����f�o�C�X�ɓn��
			pDevice3D->SetMaterial(&mesh.pMaterials[i]);//�}�e���A��
			//�e�N�X�`���X�e�[�W�͂O
			pDevice3D->SetTexture(0, mesh.ppTexture[i]);//�e�N�X�`��

			//i�Ԃ̃}�e���A�����K�p����Ă���|���S���O���[�v(�T�u�Z�b�g)��`��
			mesh.pMesh->DrawSubset(i);
		}
	}
}

void Direct3D::SetViewMatric(D3DXMATRIXA16& matView)
{
	//�r���[�s����f�o�C�X�ɓn��
	pDevice3D->SetTransform(D3DTS_VIEW, &matView);
}

void Direct3D::SetProjectionMatrix()
{
	//�N���C�A���g�̈�̕��ƍ�������A�X�y�N�g������߂�
	RECT clint;

	GetClientRect(hWnd, &clint);
	float w = clint.right - clint.left;
	float h = clint.bottom - clint.top;

	D3DXMATRIXA16 matProj;
	//�v���W�F�N�V�����s����쐬����
	D3DXMatrixPerspectiveFovLH(&matProj, 
		(float)(D3DX_PI / 4.0),//����p�@�X�O�x
		w / h,//�A�X�y�N�g��
		1.0f,//�`�拗���@��
		100.0f);//�`�拗���@��

	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);
}