#include"MeshX.h"
#include<tchar.h>

MeshX::MeshX()
{
	pMesh = nullptr;
	ppTexture = nullptr;
	pMaterials = nullptr;
	numMaterials = 0;
}

MeshX::~MeshX()
{
	//テクスチャがロード済みなら
	if (ppTexture)
	{
		for (unsigned int i = 0; i < numMaterials; i++)
		{
			//テクスチャの解放
			if (ppTexture[i])
			{
				ppTexture[i]->Release();
			}
		}
		delete[] ppTexture;
	}
	//マテリアルの解放
	if (pMaterials)
	{
		delete[] pMaterials;
	}
	//メッシュの解放
	if (pMesh)
	{
		pMesh->Release();
	}
}

bool MeshX::Load(TCHAR* path)
{
	Direct3D& pD3d = Direct3D::GetInstance();
	
	return pD3d LoadMeshX(*this.path);
}

bool MeshX::Load(std::string path)
{
	const TCHAR* path_t = _T(path.c_str());

	return Load(path_t);
}

void MeshX::Draw(D3DXMATRIXA16& matTransform, D3DXMATRIXA16& matRotation, D3DXMATRIXA16& matScale)
{
	//各行列の合成

	//ワールド変換行列　座標(平行移動)と回転と拡大の変換をすべて加えた行列
	D3DXMATRIXA16 matWorld;

	//単位行列に初期化
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIXA16 matTmp;//計算用の一時的な行列
	D3DXMatrixIdentity(&matTmp);

	//行列の合成
	//行列の掛け算はかける順番によって結果が変わる
	//かける順番が決まっているので注意
	//順番は拡縮×回転×平行移動

	//Multply　掛け算
	//拡縮×回転　を一時変数に代入
	D3DXMatrixMultiply(&matTmp, &matScale, &matRotation);

	//一時変数(拡縮×回転)×平行移動　をワールド変換に代入
	D3DXMatrixMultiply(&matWorld, &matTmp, &matTransform);

	//デバイス管理クラスを取得
	Direct3D& d3d = Direct3D::GetInstance();

	//描画
	//d3d.DrawMeshX(*this, matWorld);
}
