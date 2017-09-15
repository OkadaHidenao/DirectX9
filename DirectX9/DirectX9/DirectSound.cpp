#include"DirectSound.h"

//静的メンバ変数の初期化
DirectSound* DirectSound::pInstance = nullptr;

//コンストラクタ
DirectSound::DirectSound()
{
	pDirectSound8 = NULL;
}

//デストラクタ
DirectSound::~DirectSound()
{
	Release();
}

//解放処理 コンストラクタ実行時と同じ状態へ戻す
void DirectSound::Release()
{
	if (pDirectSound8 != NULL)
	{
		pDirectSound8->Release();
		pDirectSound8 = NULL;
	}
}

bool DirectSound::Create(HWND hWnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
	{
		//失敗
		Release();
		return false;
	}

	//協調レベルの設定
	//また音を鳴らすウィンドウの指定
	if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		//PRIORITY 優先
			//ほかのアプリケーションのサウンドとも同時に再生できる
		//EXCUSIVE 排他
			//ほかのアプリケーションのサウンドを無音にするような
	{
		//失敗
		Release();
		return false;
	}

	return true;
}

HRESULT DirectSound::CreateSoundBuffer(DSBUFFERDESC*pDesc, IDirectSoundBuffer**ppBuffer)
{
	if (pDirectSound8 == NULL)
	{
		//デバイスの作成がされていないためバッファの作成ができない
		return E_FAIL;
	}
	//サウンドバッファの作成
	HRESULT hr =
		pDirectSound8->
		CreateSoundBuffer(pDesc, ppBuffer, NULL);

	return hr;
}