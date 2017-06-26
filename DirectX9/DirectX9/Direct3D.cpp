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

	//ディスプレイ情報を取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//スワップチェインのための設定
	//スワップチェイン
	//スクリーンのバックバッファをフロントに置き換える処理のこと

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;				//バックバッファの幅　クライアント領域と同じ幅
	D3DParam.BackBufferHeight = height;				//バックバッファの高さ
	D3DParam.BackBufferFormat = Display.Format;		//バックバッファのフォーマット
													//ディスプレイ情報より
	D3DParam.BackBufferCount = 1;					//バックバッファの数
	D3DParam.MultiSampleType = D3DMULTISAMPLE_NONE;	//マルチサンプリングの設定
													//なし
	D3DParam.MultiSampleQuality = 0;				//マルチサンプリングのクオリティ
	D3DParam.SwapEffect = D3DSWAPEFFECT_DISCARD;	//スワップチェインの方法
													//Direct3D任せ
	D3DParam.hDeviceWindow = hWnd;					//描画するウィンドウの識別ハンドル
	D3DParam.Windowed = TRUE;						//ウィンドウモード
	D3DParam.EnableAutoDepthStencil = TRUE;			//深度ステンシルバッファ　Zバッファ
													//描画の際　すでに手前のものが書かれていると奥のものの描画を省くための情報
													//スクリーンに色ではなくそのピクセルに書かれたものの
													//カメラからの距離が入っているとイメージするとよい
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;	//深度ステンシルのフォーマット
	D3DParam.Flags = 0;								//設定のフラグ
	D3DParam.FullScreen_RefreshRateInHz = 0;		//ウィンドウモードは使用しないので0
	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//アダプタのリフレッシュレートとpresent処理を実行すりレートの関係

	//いくつかの設定でデバイス作成を試みる
	//HALモードで作成
	if (FAILED(pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&D3DParam, &pDevice3D)
	))
	{
		//一つ目の設定で失敗したら
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//一つ目の設定から　HARDWEAR->MIXED
			&D3DParam, &pDevice3D)
		))
		{
			//二つ目の設定で失敗したら
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,			//一つ目の設定からHAL->REF
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
					//三つ目の設定からHARDWARE->SOFTWARE
					&D3DParam, &pDevice3D)
				))
				{
					//４つすべての設定で作成が失敗したら

					//解放処理をして失敗扱いで関数を終える
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}


	//ここに来たということはどれかの設定でデバイスの作成が成功した
	return true;
}

HRESULT Direct3D::BeginScene()
{
	if (pDevice3D != nullptr)
	{
		//Direct3DデバイスのBeginSeneを実行
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
		//Direct3DデバイスのEndSeneを実行
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

//バックバッファのクリア
HRESULT Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		//背景のクリア職
		//何も描画されていない状態だとこの色が一面に出る
		D3DXCOLOR Color = D3DCOLOR_XRGB(0, 128, 0);

		//Clearの実行結果を返す
		return pDevice3D->Clear(0, NULL,
			//クリアのターゲットとなるバッファを指定
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER
			, Color, 1.0f, 0);
	}
	else
	{
		return S_FALSE;
	}
}

//バックバッファをフロントに反映
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