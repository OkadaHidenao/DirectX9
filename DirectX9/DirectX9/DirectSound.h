#pragma once

#define DIRECTSOUND_VIRSION 0x800

#pragma comment(lib,"dsound.lib")

#include<dsound.h>

class DirectSound
{
private:
	//シングルトンにする
	static DirectSound* pInstance;

	IDirectSound8* pDirectSound8;

	void Release();

	//シングルトンにするためにクラス外から実行できないようにする
	DirectSound();//コンストラクタ
	~DirectSound();
	void operator=(const DirectSound& obj) {}//代入演算
	DirectSound(const DirectSound& obj) {}//コピーコンストラクタ
public:
	//インスタンスの取得
	static DirectSound* GetInstance()
	{
		if (pInstance == NULL)
		{
			pInstance = new DirectSound();
		}
		return pInstance;
	}

	//デバイスの作成
	bool Create(HWND hWnd);

	HRESULT CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer**ppBuffer);
};