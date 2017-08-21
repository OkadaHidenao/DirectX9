#include"DirectInput.h"

DirectInput* DirectInput::pInstance = nullptr;

DirectInput::DirectInput()
{
	pDInput = nullptr;

	pKeyDevice = nullptr;
	pMouseDevice = nullptr;

	//配列変数の初期化
	//すべてゼロ
	ZeroMemory(KeyInput, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(KeyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);

}

DirectInput::~DirectInput()
{
	//Release();
}

HRESULT DirectInput::Init(HWND hWnd)
{
	this->hWnd = hWnd;

	if (initialized == false)
	{
		//DirectInputオブジェクトの作成
		HRESULT hresult
			= DirectInput8Create
			(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
				IID_IDirectInput8, (VOID**)&pDInput, NULL);

		if (FAILED(hresult))
		{
			return hresult;
		}

		//オブジェクトができたのでキーボード用のデバイスを初期化
		hresult = InitKey();

		if (FAILED(hresult))
		{
			return hresult;
		}

		//すべての初期化が終了
		//初期化済みとする
		initialized = true;
	}
	//初期化成功
	return S_OK;
}

HRESULT DirectInput::InitKey()
{
	HRESULT hresult;


	//CreateDeviceの第一引数が作りたいデバイスのタイプが入る
	//第二引数が作ったデバイスの保存先が入るポインタ
	//タイプにかかわらず第二引数の型は
	//LPDIRECTINPUTDEVICE8*　であることに注意
	hresult = pDInput
		->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);

	if (FAILED(hresult))
	{
		return hresult;
	}

	//データフォーマットの設定　キーボードを選ぶ
	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//協調モードの設定
	//OSやほかのアプリケーションとのデバイス共有のための設定
	//デフォルト設定
	hresult = pKeyDevice->SetCooperativeLevel
	(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

	if (FAILED(hresult))
	{
		return hresult;
	}

	//入力デバイスへのアクセス権を取得
	pKeyDevice->Acquire();

	return S_OK;
}

HRESULT DirectInput::Update()
{
	//初期化が済んでいなかったら実行するのに問題がいろいろと出るので
	//関数をすぐに中断
	if (initialized == false)
	{
		return S_FALSE;
	}

	//キーボードの状態を取得して
	//メンバ変数の更新

	//アクセス権取得
	HRESULT hresult = pKeyDevice->Acquire();

	//S_FALSE以外の失敗は通さない
	if (hresult == DI_OK || hresult == S_FALSE)
	{
		//前回のフレームのキー情報をpreviewのほうにコピー
		memcpy(KeyInputPreview,
			KeyInput,
			sizeof(int)*INPUT_BUFFER_SIZE);

		//状態　まずデバイスから現在のON OFF情報を取得
		//t tmporaly 一時の
		//work 作業用の
		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		//デバイスからキー状態(On Off)の取得
		pKeyDevice->
			GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE,
				&tKeyInputWork);

		//キーのOnOffから
		//何フレーム押され続けているかという情報に加工
		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])//キーが押されている
			{
				if (KeyInput[i] < 2147483647)
				{
					//押され続けているフレーム数を加算
					KeyInput[i] ++;;
				}
			}
			else
			{
				//押されていない場合は　連続フレームっ数を０にする
				KeyInput[i] = 0;
			}
		}
	}
	return S_OK;
}

//現在押されているか
bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//離されている状態が０なので０より大きいと押されていることがわかる
		return KeyInput[code] > 0;
	}

	//範囲外はfalse
	return false;
}

//このフレームがこのキーの押された瞬間であるか
bool DirectInput::KeyJustPressed(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//0から１に切り替わった瞬間が押された瞬間を示している
		return KeyInput[code] ==1;
	}

	//範囲外はfalse
	return false;
}

//このフレームがキーの離された瞬間か
bool DirectInput::KeyJustReleased(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		//現在は0だけど
		//1フレーム前は押されていた
		return (KeyInput[code] == 0 &&
			KeyInputPreview[code] > 0);
	}

	//範囲外はfalse
	return false;
}

//押され続けているフレーム数を返す
int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return KeyInput[code];
	}

	//範囲外はfalse
	return false;
}