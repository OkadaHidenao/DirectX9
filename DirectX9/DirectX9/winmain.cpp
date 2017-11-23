
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>


#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include "DirectSound.h"
#include "Wave.h"
#include "SoundBuffer.h"

#include "Camera.h"
#include "MeshX.h"
#include"OrientedBoundingBox.h"

//ウィンドウプロシージャ
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//ウィンドウのハンドル
	UINT msg,	//メッセージ
	WPARAM wParam,
	LPARAM lParam
)
{
	//メッセージ処理のための関数
	//DispatchMessageによって呼び出される

	//ウィンドウクラスの登録時に
	//各ウィンドウに設定される

	//アプリケーションがメッセージを
	//取得したら呼び出される

	//メッセージの例

	//WM_DESTROY ウィンドウが破棄された
	//			 ×ボタンが押された時など
	//WM_MOVE    ウィンドウが移動
	//WM_SIZE    ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた
	//処理を行う

	//取りあえず WM_DESTROYが来た時の
	//終了処理のみ定義

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		//メッセージキューに
		//新しくWM_QUITメッセージを送る

		//いずれメッセージキューから
		//取り出され
		//メインループが終了する
		//(メインループの終了条件を
		//そのように作る)
		break;
	}

	//独自の処理を行ったら
	//デフォルトのウィンドウプロシージャを実行

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ウィンドウ設定の名前
static const TCHAR* WC_BASIC = _T("Basic");

//作りたいウィンドウの設定を
//登録する関数
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//ウィンドウの設定を保存する構造体
	WNDCLASSEX wcex;

	//wcexの先頭アドレスから
	//WNDCLASSEXの長さ分のメモリを
	//全て0で埋める
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//つまりはwcexが使用しているメモリ領域を
	//全て0で初期化する処理

	//ウィンドウの設定

	//構造体のサイズ
	wcex.cbSize = sizeof(WNDCLASSEX);

	//ウィンドウの設定情報に追加する捕捉を付けないので
	//0で固定
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Windowのスタイル
	//それぞれがビット単位のフラグであるため
	//オア演算で合成したものがスタイルの最終的な設定
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW 横サイズが変わったときウィンドウ全体の再描画
	//   VREDRAW 縦サイズが変わったときウィンドウ全体の再描画
	//   DBLCLKS ダブルクリックが発生したことをウィンドウに通知

	//WinMainのインスタンスハンドル
	wcex.hInstance = hInstance;

	//ウィンドウプロシージャのアドレス
	//関数ポインタの設定
	//ウィンドウプロシージャをOSが呼び出せるようにする
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//背景色
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//メニュー　なし
	wcex.lpszMenuName = NULL;

	//ウィンドウクラスの名前　このウィンドウ設定の名前
	wcex.lpszClassName = WC_BASIC;

	//アイコン　デフォルトのを使う
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//カーソル デフォルトのものを使う
	wcex.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//設定したウィンドウクラスを登録
	//登録したウィンドウ設定でないと
	//ウィンドウを作ることができない
	return RegisterClassEx(&wcex);
	//HRESULT型の実行結果が戻される
}


//登録した設定を基にウィンドウを作成する
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMainの識別子
	HWND &refHWnd,	//ウィンドウの識別子
					//正しくウィンドウの作成ができたら
					//この変数に識別子を代入する
	int width = 800,	//クライアント領域の幅
	int height = 600)	//クライアント領域の高さ
						//クライアント領域はウィンドウ全体から
						//外枠やメニューの部分を除いた物と今は思っておけばOK
{
	//クライアント領域のおおきさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//クライアント領域を保持するのに必要な
	//ウィンドウのおおきさを計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//正しく実行されるとrectの値が更新される

	//WS_OVERLAPPEDWINDOWはウィンドウのスタイルの一つ

	//ウィンドウのハンドル
	//ウィンドウの識別に使用する
	HWND hWnd;

	hWnd = CreateWindowEx
	(0,						//ウィンドウ拡張スタイル
		WC_BASIC,				//作りたいウィンドウクラス
								//あらかじめ登録されたもの
		_T("タイトル"),			//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,	//ウィンドウのスタイル
		CW_USEDEFAULT,			//位置x座標 デフォルトの値
		CW_USEDEFAULT,			//位置y座標 デフォルトの値
		rect.right - rect.left,	//ウィンドウ全体の幅
		rect.bottom - rect.top, //ウィンドウ全体の高さ
		NULL,					//親ウィンドウのハンドル
		NULL,					//メニューのハンドル
		hInstance,				//WinMainのハンドル

		NULL);					//ウィンドウ作成データ
								//ウィンドウに好きな32ビットのデータを
								//付属させることができる
								//追加情報構造体へのポインタなど
								//を付けるのが無難な使い方か
								//今回は使わない

								//ウィンドウが正しく作れたかをチェック
								//正しく作れたらhWndにNULL以外が入るはず
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT型　失敗を表す識別子の一つ
	}

	//ウィンドウをアクティブにして表示
	//hWndでウィンドウの識別をしていることに注目
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//クライアント領域の更新
	UpdateWindow(hWnd);

	//参照渡しになっている引数に
	//ウィンドウの識別ハンドルをコピーする
	//これによって今作ったウィンドウの識別ハンドルを
	//呼び出し元のプログラムで得ることができる
	refHWnd = hWnd;

	//HRESULT型　成功を表す識別子の一つ
	return S_OK;
}



//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstance,	//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine,		//コマンドライン引数
	int nCmdShow)		//ウィンドウの表示状態
{
	//メッセージボックス
	MessageBox(NULL,		//ウィンドウのハンドル 
		TEXT("テスト"),		//本文
		TEXT("テスト-タイトル"),//タイトル
		MB_OK);				//メッセージボックスのタイプ
							//MB_OK  okのボタンが表示

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウクラス設定失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	//正しくウィンドウが作成されれば
	//hWndにウィンドウの識別ハンドルが入る
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("ウィンドウ作成失敗"),
			TEXT("テスト-タイトル"),
			MB_OK);
		return 0;
	}

	//Direct3Dを管理するクラス(シングルトン)への参照を取得
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDeviceの作成を試みる
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device作成成功"),
			TEXT("テスト-タイトル"),
			MB_OK);
	}

	//レンダーステートの設定  αブレンド
	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
	d3d.SetProjectionMatrix();

	{
		//ワールド座標系の座標をカメラ中心の座標系に変換する行列
		//カメラの位置と原点が重なるような座標系
		//座標系の各軸の方向はカメラの向きによって変わる
		D3DXMATRIXA16 mat;
		D3DXVECTOR3 eye(0.0f, 0.0f, -10.0f);	//視点ワールド座標
		D3DXVECTOR3 lookat(0.0f, 0.0f, 0.0f);	//注視点　eyeの位置から注視点を見るような回転を作る
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);		//ワールド座標上方向

												//例として
												//カメラが(0,0,300)の位置から
												//(0,0,200)の位置にある3Dオブジェクトを見てる
												// これをカメラの位置を(0,0,0)とみて
												//3Dオブジェクトの位置を(0,0,-100)としてみる 
												//(ただしカメラの回転によって(100,0,0)などそれぞれの軸の値はあらゆる値をとり得る)

												//ビュー行列を作る　LH LeftHandの略 　(RH　はRightHand)
		D3DXMatrixLookAtLH(&mat, &eye, &lookat, &up);
		//3Dの座標系には右手座標系と左手座標系がありその左手座標系を使う
		//両手をフレミングの左手の法則の様に広げて
		//親指　人差し指　中指がそれぞれの軸のプラス方向としてみると
		//右手と左手で親指の向きが違う(右手系と左手系のわかりやすい違い)

		//ビュー行列設定
		d3d.SetViewMatric(mat);
	}

	Camera camera;
	//カメラの位置を調整
	camera.Move(0, 0, -10);

	//スプライトのインスタンスを作成
	//パラメータは適当で
	Sprite sprite;
	sprite.SetAlpha(0.1);
	sprite.SetSize(100, 100);
	sprite.SetAngle(0);
	sprite.SetPos(200, 200);

	//テクスチャのインスタンスを作成
	Texture texture;
	texture.Load(_T("test.png"));//ファイルのロード
	texture.SetDivide(2, 1);

	DirectInput * pDi = DirectInput::GetInstansce();
	pDi->Init(hWnd);

	//ダイレクトサウンドのデバイス作成
	DirectSound* pDs = DirectSound::GetInstance();
	pDs->Create(hWnd);

	WaveFile waveFile;			//音声ファイルデータ
	SoundBuffer soundBuffer;	//再生用バッファ
	if (waveFile.Load("BGM.wav"))
	{
		soundBuffer.Create(waveFile);
		//soundBuffer.Play(true);		
	}
	else
	{
		int a = 0;
	}

	//メッシュインスタンスの作成
	MeshX meshX;
	meshX.Load("Mesh/iasel/iasel_brackboard.x");

	OrientedBoundingBox obb[2];
	obb[1].SetPosition(D3DXVECTOR3(5, 0, 0));

	float sita = 0.2f;
	D3DXVECTOR3 dir[3]
	{
		D3DXVECTOR3(-sin(sita),0,cos(sita)),
		D3DXVECTOR3(cos(sita),0,sin(sita)),
		D3DXVECTOR3(0,1,0)
	};

	obb[1].SetDirection(dir[0],dir[1],dir[2]);
	obb[1].SetLength(0.5, 1, 1);

	D3DXVECTOR4 BaseDir[4]
	{
		D3DXVECTOR4(0,0,1,1),
		D3DXVECTOR4(1,0,0,1),
		D3DXVECTOR4(0,1,0,1)
	};

	//メインループ
	//メッセージループ
	MSG msg = {};
	
	//quitメッセージが出てくるまでループを繰り返す
	//quitメッセージは上記のウィンドウプロシージャから送信
	//送信の条件などはウィンドウプロシージャを確認
	while (msg.message != WM_QUIT)
	{
		//PeekMessage
		//メッセージキューの中にメッセージがあるかを調べて
		//msgの中に書き込みtrueを返す
		//メッセージがなければfalseを返す
		//PM_REMOVEを設定しているとメッセージの書き込みのあとに
		//メッセージキューから元のメッセージを消す

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーメッセージを文字メッセージに変換し
			//それをメッセージキューにポストする
			TranslateMessage(&msg);

			//メッセージの割り当て
			//ウィンドウプロシージャでメッセージを処理
			DispatchMessage(&msg);
		}
		else
		{
			//メッセージキューにメッセージが無かったとき
			//こちらの処理
			//ここにゲーム処理を書き込んでいく

			pDi->Update();//キー状態の更新

			if (pDi->MouseButtonJustPressed(MOUSE_BUTTON_LEFT))
			{
				Vector2<int>  vec = pDi->MousePosition();
				int breakpoint = 0;
			}


			sprite.SetAngle(sprite.GetAngle_Rad() + 0.01f);//回転

			static int dir = 1;//α
			sprite.SetAlpha(sprite.GetAlpha() + (dir*0.01));
			switch (dir)
			{
			case 1:
				if (sprite.GetAlpha() >= 1)
				{
					dir = -1;
				}
				break;
			case -1:
				if (sprite.GetAlpha() <= 0)
				{
					dir = 1;
				}
				break;
			}

			//カメラの操作
			{
				//回転
				float pitch = 0;
				float yaw = 0;

				if (pDi->KeyState(DIK_RIGHT))
				{
					yaw -= 1;
				}
				if (pDi->KeyState(DIK_LEFT))
				{
					yaw += 1;
				}
				if (pDi->KeyState(DIK_UP))
				{
					pitch += 1;
				}
				if (pDi->KeyState(DIK_DOWN))
				{
					pitch -= 1;
				}

				camera.Rotate(yaw*0.01f, pitch*0.01f);

				//移動
				float x = 0;
				float y = 0;
				float z = 0;

				if (pDi->KeyState(DIK_D))x += 1;
				if (pDi->KeyState(DIK_A))x -= 1;
				if (pDi->KeyState(DIK_W))z += 1;
				if (pDi->KeyState(DIK_S))z -= 1;
				if (pDi->KeyState(DIK_R))y += 1;
				if (pDi->KeyState(DIK_F))y -= 1;
				camera.MoveBasedRotationFlatten(z*0.05, x*0.05, y*0.05);

				//スペースキーでカメラリセット
				if (pDi->KeyState(DIK_SPACE))
				{
					camera.SetPosition(0, 0, -10);
					camera.SetRotation(3.1415f*0.5f, 0.0f);
				}

				//ビュー行列の更新
				D3DXMATRIXA16 view = camera.CreateViewMatrix();
				d3d.SetViewMatric(view);
			}

			{
				static bool flag = true;
				if (flag)
				{
					static float f = 0;
					f += 0.01f;
					obb[1].SetPosition(D3DXVECTOR3(5 - f, 0, 0));
					if (OrientedBoundingBox::Collision(obb[1], obb[0]))
					{
						flag = false;
					}

					//mat1 2は回転行列
					D3DXMATRIXA16 mat1, mat2;
					D3DXMatrixRotationX(&mat1, f*5);	//x軸中心
					D3DXMatrixRotationZ(&mat2, f * 2);	//z軸中心
					D3DXVECTOR4 v4[3];					//基本の向きのベクトルに行列１，２の回転を適用した状態を計算して代入　
														//Vector4なのは行列の型が4*4なのでそれに合わせた
					D3DXVECTOR3 v3[3];

					for (int i = 0; i < 3; i++)
					{
						D3DXVec4Transform(&v4[i], &BaseDir[i], &mat1);	//行列１の回転で変換
						D3DXVec4Transform(&v4[i], &v4[i], &mat2);		//さらに行列２の回転で変換

						//vec4からvec3にデータを移す
						v3[i] = D3DXVECTOR3(v4[i].x, v4[i].y, v4[i].z);
					}

					//変換した向きをobbの向きとして設定
					obb[0].SetDirection(v3[0], v3[1], v3[2]);
				}

			}
			//描画処理
			if (SUCCEEDED(d3d.BeginScene()))
			{
				//バックバッファのクリア
				d3d.ClearScreen();

				static bool f = false;
				static D3DXMATRIXA16 matIdentify;//単位行列
				if (f == false)//最初の一回目だけ初期化
				{
					f = true;
					D3DXMatrixIdentity(&matIdentify);//初期化 単位行列
													 //座標(0,0,0)　無回転　拡大率100%
				}

				//スプライト描画の為のレンダーステート
				//d3d.SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
				//スプライトの描画
				//sprite.Draw(texture);

				//メッシュの描画
				//メッシュ描画の為のレンダーステート
				d3d.SetRenderState(RENDERSTATE::RENDER_MESH_X);
				//描画　位置回転拡大　全て単位行列
				meshX.Draw(matIdentify, matIdentify, matIdentify);
				for (int i = 0; i < 2; i++)
				{
					d3d.CallDrawFunc(obb[i]);
				}
				//描画終了の合図
				d3d.EndScene();
				//バックバッファをフロントへ反映
				d3d.Present();
			}
		}
	}



	return 0;
}