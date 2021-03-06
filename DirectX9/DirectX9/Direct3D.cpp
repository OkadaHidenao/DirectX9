#include "Direct3D.h"

#include"Sprite.h"
#include"Texture.h"
#include"MeshX.h"
#include"OrientedBoundingBox.h"

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

//デバイスの作成を試みる
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
	//バックバッファ(裏画面)のおおきさを計算
	//ウィンドウのクライアント領域のおおきさに合わせる

	//クライアント領域のおおきさを調べる
	RECT rect;
	GetClientRect(hWnd, &rect);

	//バックバッファの幅と高さ
	int width  = rect.right  - rect.left;
	int height = rect.bottom - rect.top;

	//Direct3D9オブジェクトの作成
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	//ディスプレイ情報を取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//スワップチェインの為の設定
	//スワップチェイン　
	//スクリーンのバックバッファを
	//フロントバッファに置き換える処理のこと

	D3DPRESENT_PARAMETERS D3DParam;
	D3DParam.BackBufferWidth = width;				//バックバッファの幅　クライアント領域と同じ
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
													//描画の際　すでに手前のものが書かれていると
													//奥のものの描画を省くための情報　
													//スクリーンに色ではなくそのピクセルに書かれたものの
													//カメラからの距離が入っているとイメージすると良い
	D3DParam.AutoDepthStencilFormat = D3DFMT_D24S8;	//深度ステンシルのフォーマット
	D3DParam.Flags = 0;								//設定のフラグ
	D3DParam.FullScreen_RefreshRateInHz = 0;		//ウィンドウモードは使用しないので0

	D3DParam.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//アダプタのリフレッシュレートとpresent処理を実行するレートの関係

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
		//1つ目の設定で失敗したら
		if (FAILED(pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
			//一つ目の設定から HARDWARE -> MIXED
			&D3DParam, &pDevice3D)
		))
		{
			//2つ目の設定で失敗したら
			if (FAILED(pD3D9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,		//一つ目の設定から HAL -> REF
				D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
				&D3DParam, &pDevice3D)
			))
			{
				//3つ目の設定で失敗したら
				if (FAILED(pD3D9->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					D3DParam.hDeviceWindow,
					D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
					//3つ目の設定から HARDWARE -> MIXED
					&D3DParam, &pDevice3D)
				))
				{
					//4つ全ての設定で作成が失敗したら

					//解放処理をして
					//失敗扱いで関数を終える
					pD3D9->Release();
					pD3D9 = NULL;
					pDevice3D = NULL;

					return false;
				}
			}
		}
	}
	//ここに来たということはどれかの設定でデバイスの作成が成功した
	//ウィンドウのハンドルを覚えておく
	this->hWnd = hWnd;
	
	return true;

}

//描画の設定
void Direct3D::SetRenderState(RENDERSTATE state)
{
	//デバイスが作成されていないなら中断
	if (pDevice3D == nullptr)return;

	switch (state)
	{
	case RENDER_DEFAULT:
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);//αテスト無効
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//αブレンド無効

		break;
	case RENDER_ALPHATEST:
		//透明部分の切り抜き
		//α値でそのピクセルを描画するかを判断する
		//判断基準のα値はあらかじめ決めておく
		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, true);//αテスト有効
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);//αブレンド無効
		pDevice3D->SetRenderState(D3DRS_ALPHAREF, 0x80);//判断基準値
		pDevice3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);//基準値以上で描画

		break;
	case RENDER_ALPHABLEND:
		//半透明描画

		pDevice3D->SetRenderState(D3DRS_ALPHATESTENABLE, false);//αテスト無効
		pDevice3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//αブレンド有効

		//ブレンド係数　今から描画するピクセル色に対するもの
		pDevice3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//ブレンド係数	既にバックバッファに書かれているピクセル色に対するもの
		pDevice3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//最終的な描画色　＝　
		//	SRC色×ブレンディング係数（SRC)　＋　DEST色×ブレンディング係数（DEST）

		break;
	case RENDER_MESH_X:
		//メッシュX描画の設定
			{
				SetProjectionMatrix();

				//カリングモード
				//渡された頂点配列の順番でポリゴンそれぞれを描画する際
				//時計周りに並んでいる面を表とするのか			CCWを設定
				//反時計回りに並んでいる面を表とするのかの設定　CWを設定
				//また、裏面の描画はしない　両面描画するのはD3DCULL_NONE
				//CW 時計回り
				//CCW 反時計回り
				//カリング自体の意味は裏面の描画を省く機能なので消すほうを設定
				pDevice3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

				//ライトを有効にするか
				pDevice3D->SetRenderState(D3DRS_LIGHTING, TRUE);
				//奥行きを有効にするか
				//カメラから遠いオブジェクトを描画するとき
				//手前にあるオブジェクトによって一部ないし全体が隠れている場合に
				//その部分の描画を省くという設定
				pDevice3D->SetRenderState(D3DRS_ZENABLE, TRUE);

				//(別に専用の関数を設けるのが無難ではあるが)
				//ライトの設定
				D3DLIGHT9 light;
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;//ライトの種類
				light.Diffuse.r = 1.0f;//ライトの色　赤　成分
				light.Diffuse.g = 1.0f;//			 緑
				light.Diffuse.b = 1.0f;//			 青
				light.Direction = D3DXVECTOR3(-0.5f, -1.0f, 0.5f);//ライトの方向
				light.Range = 1000.0f;//ライトの距離　Directionはあまり関係なかったはず

				pDevice3D->SetLight(0, &light);//ライトの設定　0　はライトの管理番号だと思っておけばOK

											   //向きの違うライトをもう一つ用意しておく
				ZeroMemory(&light, sizeof(D3DLIGHT9));
				light.Type = D3DLIGHT_DIRECTIONAL;
				light.Diffuse.r = 1.0f;
				light.Diffuse.g = 1.0f;
				light.Diffuse.b = 1.0f;
				light.Direction = D3DXVECTOR3(0.5f, -1.0f, 0.5f);
				light.Range = 1000.0f;
				pDevice3D->SetLight(1, &light);//ライトの設定　管理番号　1

				pDevice3D->LightEnable(0, TRUE);//0番のライトを有効にする
				pDevice3D->LightEnable(1, TRUE);//1番のライトを有効にする

												//アンビエント
												//環境光　
												//（説明が正確でないので注意）
												//ベタ塗用の光
												//上で設定した光の影響が全くない状態でも描画するための最低限の光
				pDevice3D->SetRenderState(D3DRS_AMBIENT, 0x00444444);
			}

	break;
	}
}

HRESULT Direct3D::BeginScene()
{	
	if (pDevice3D != nullptr)
	{
		//Direct3DデバイスのBegineSceneを実行
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
		//Direct3DデバイスのEndSceneを実行
		return pDevice3D->EndScene();
	}
	else
	{
		return S_FALSE;
	}
}

//バックバッファのクリア
HRESULT  Direct3D::ClearScreen()
{
	if (pDevice3D != nullptr)
	{
		//背景のクリア色
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

void Direct3D::DrawSprite(Sprite& sprite,Texture& tex)
{
	//デバイスが作成されていなければ
	//描画の処理に入らずreturnする
	if (pDevice3D == nullptr)return;

	//描画に使用する変数をspriteから取り出す
	D3DXVECTOR2 pos = sprite.GetCentrePos();
	float width = sprite.GetWidth();
	float height = sprite.GetHeight();
	float alpha = sprite.GetAlpha();

	char alphaC = static_cast<char>(255 * alpha);

	float u = tex.numU;
	float v = tex.numV;
	float du = tex.divU;
	float dv = tex.divV;

	//四角形なので頂点4つ
	//トライアングルストリップを使用するので
	//vertexに格納する順番は 右上　右下　左上　左下
	SpriteVertex vertex[4] =
	{
		//			　x   y   z	 rhw	頂点色		赤　緑　青　透明　u	  v
		{ D3DXVECTOR3( width / 2 ,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,		v/dv },
		{ D3DXVECTOR3( width / 2 , height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),(u+1)/du,	 (v+1)/dv},
		{ D3DXVECTOR3(-width / 2 ,-height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	u/du,		v/dv },
		{ D3DXVECTOR3(-width / 2 , height / 2,0),1.0f,D3DCOLOR_RGBA(255,255,255,alphaC),	u/du,	(v+1)/dv }
	};

	float angle = sprite.GetAngle_Rad();
	
	for (int i = 0; i < 4; i++)
	{
		//三角関数　加法定理を利用して4つの頂点の座標を変換
		//原点を中心としてangleの分だけ回す
		float x = vertex[i].pos.x*cosf(angle) -
			vertex[i].pos.y*sinf(angle);

		float y = vertex[i].pos.x*sinf(angle) +
			vertex[i].pos.y*cosf(angle);

		//中心座標分だけ平行移動
		D3DXVECTOR3 vPos;
		vPos.x = pos.x + x;
		vPos.y = pos.y + y;
		vPos.z = 0;
		vertex[i].pos = vPos;


	}

	//テクスチャの指定
	pDevice3D->SetTexture(0, tex.pTexture);

	//頂点構造体宣言
	pDevice3D->SetFVF(SPRITE_FVF);

	//ポリゴンの描画
	//トライアングルストリップで描画

	//引数 (描画タイプ,
		//三角形の数,
		//頂点構造体配列の先頭アドレス,
		//ひとつの頂点構造体のサイズ);
	pDevice3D->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, 
		vertex, sizeof(SpriteVertex));

}

//テクスチャのロード
bool Direct3D::LoadTexture(Texture& tex, TCHAR* FilePath)
{
	//デバイスが作成されているかどうかの確認
	if (pDevice3D != nullptr)
	{
		if (SUCCEEDED(
			D3DXCreateTextureFromFile(
				pDevice3D, FilePath, &(tex.pTexture)
			)//createFromFile
			)//succeeded
			)//if
		{
			//ロードに成功した
			return true;
		}
	}

	//デバイスが存在しなかった
	//存在したが作成に失敗した
	return false;//失敗
}

//.x形式のメッシュ読み込み
bool Direct3D::LoadMeshX(MeshX& mesh, TCHAR*path)
{
	//Xファイルのパスの取得
	//テクスチャの読み込み時に使用
	CHAR  dir[_MAX_DIR];
	_splitpath_s(path, NULL, 0, dir, _MAX_DIR, NULL, 0, NULL, 0);

	//メッシュを読み込んだ後マテリアルを一時的に保存しておく変数
	LPD3DXBUFFER pBufferMatrial;

	//読み込み
	if (D3DXLoadMeshFromX(path, D3DXMESH_SYSTEMMEM, pDevice3D, NULL, &pBufferMatrial, NULL, &mesh.numMaterials, &mesh.pMesh)!=D3D_OK)
	{
		return false;
	}

	//マテリアルの準備
	if (mesh.numMaterials > 0)
	{
		//テクスチャとマテリアルをそれぞれマテリアルの個数分確保
		//複数のマテリアルで一つのテクスチャを共有する場合を考えたら無駄が出てしまう
		mesh.pMaterials = new D3DMATERIAL9[mesh.numMaterials];
		mesh.ppTexture = new LPDIRECT3DTEXTURE9[mesh.numMaterials];

		//バッファをマテリアルの形式にキャスト
		D3DXMATERIAL* d3dxmaterials = (D3DXMATERIAL*)pBufferMatrial->GetBufferPointer();

		//マテリアルをひとつずつバッファからコピー
		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			mesh.pMaterials[i] = d3dxmaterials[i].MatD3D;

			//環境光に対する反射係数(色)を拡散光反射のものと一緒にする
			mesh.pMaterials[i].Ambient = mesh.pMaterials[i].Diffuse;

			mesh.ppTexture[i] = nullptr;

			//テクスチャのファイル名を取り出してロード
			if (d3dxmaterials[i].pTextureFilename != nullptr)
			{
				//テクスチャのファイルパスの作成
				CHAR textureFilepath[1024];
				ZeroMemory(textureFilepath, sizeof(textureFilepath));

				//ファイルパスにメッシュファイルまでのバスを書き込む
				lstrcat(textureFilepath, dir);

				//ファイルパスの末尾にテクスチャのファイル名を追加
				lstrcat(textureFilepath, d3dxmaterials[i].pTextureFilename);

				//作ったパスを用いてテクスチャファイルのロード
				if (D3DXCreateTextureFromFile(pDevice3D, textureFilepath, &mesh.ppTexture[i]) != D3D_OK)
				{
					mesh.ppTexture[i] = nullptr;
				}
			}
		}
	}
	//バッファの解放
	pBufferMatrial->Release();

	return true;
}
//.x形式のメッシュ描画
void Direct3D::DrawMeshX(MeshX& mesh, D3DXMATRIXA16& matWorld)
{
	if (mesh.pMesh != nullptr)
	{
		//メッシュがロード済みである
		//SetTransform関数を介してDirect3Dのデバイスにワールド変換行列を渡す
		//第一引数が行列の種類
		//D3DTS_WORLDの場合はメッシュの変換行列(座標　回転　拡縮)
		pDevice3D->SetTransform(D3DTS_WORLD, &matWorld);

		//頂点シェーダ(設定しない　デフォルトを使用)
		//ポリゴンの各頂点がスクリーンのどの位置にくるかを計算するもの
		pDevice3D->SetVertexShader(NULL);

		//頂点フォーマット
		//ポリゴンの一つの頂点の情報を格納した構造体がどのような構成になっているかをデバイスに渡す
		pDevice3D->SetFVF(mesh.pMesh->GetFVF());

		//マテリアルごとにメッシュを描画
		for (unsigned int i = 0; i < mesh.numMaterials; i++)
		{
			//マテリアルとそれに対応するテクスチャをデバイスに渡す
			pDevice3D->SetMaterial(&mesh.pMaterials[i]);//マテリアル
			//テクスチャステージは０
			pDevice3D->SetTexture(0, mesh.ppTexture[i]);//テクスチャ

			//i番のマテリアルが適用されているポリゴングループ(サブセット)を描画
			mesh.pMesh->DrawSubset(i);
		}
	}
}

void Direct3D::SetViewMatric(D3DXMATRIXA16& matView)
{
	//ビュー行列をデバイスに渡す
	pDevice3D->SetTransform(D3DTS_VIEW, &matView);
}

void Direct3D::SetProjectionMatrix()
{
	//クライアント領域の幅と高さからアスペクト比を求める
	RECT clint;

	GetClientRect(hWnd, &clint);
	float w = clint.right - clint.left;
	float h = clint.bottom - clint.top;

	D3DXMATRIXA16 matProj;
	//プロジェクション行列を作成する
	D3DXMatrixPerspectiveFovLH(&matProj, 
		(float)(D3DX_PI / 4.0),//視野角　９０度
		w / h,//アスペクト比
		1.0f,//描画距離　近
		100.0f);//描画距離　遠

	pDevice3D->SetTransform(D3DTS_PROJECTION, &matProj);
}

HRESULT Direct3D::CallDrawFunc(OrientedBoundingBox &obb)
{
	return obb.Draw(pDevice3D);
}