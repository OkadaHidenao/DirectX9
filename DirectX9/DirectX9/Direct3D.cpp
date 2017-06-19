#include"Direct3D.h"

//スタティックなメンバ変数の初期化
Direct3D* Direct3D::pInstance = nullptr;

//コンストラクタ
Direct3D::Direct3D()
{
	//ポインタに値を代入するだけ
	//まだどのオブジェクトも指さない
	pD3D9 = NULL;
	pDevice3D = NULL;
}

//デストラクタ
Direct3D::~Direct3D()
{
	Release();
}

//解放の関数
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
		//すでに作成されているので失敗
		return false;
	}

	//作成の関数を呼ぶ
	if (!Create(hWnd))
	{
		return false;
	}

	//作成成功
	return true;
}

//デバイスの作成
//TryCreate関数から呼ぶ
bool Direct3D::Create(HWND hWnd)
{
	//バックバッファ（裏画面）の大きさを計算
	//ウィンドウのクライアント領域の大きさに合わせる

	//クライアント領域の大きさを調べる
	RECT rect;
	GetClientRect(hWnd, &rect);

	//バックバッファの幅と高さ
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;

	//Direct3D9オブジェクトの作成
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
}