#pragma once

#include"Direct3D.h"

//有向境界ボックス
//OBB (略称)
class OrientedBoundingBox
{
public:
	//箱の描画の際にD3Dに渡す頂点情報
	//FVFで書いたとおりのメンバ
	//順番も決まっている(マイクロソフトのリファレンスサイトを参照)
	struct Vertex
	{
		//三次元座標
		D3DXVECTOR3 pos;

		//法線
		//面に垂直なベクトル
		D3DXVECTOR3 normal;

		//色
		D3DCOLOR color;
	};
private:
	//頂点構造体の構成要素を示す情報
	static const DWORD FVF_TRIANGLE_LIST_VERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	//diffuse 拡散光色　：色


protected:
	D3DXVECTOR3 position;//ボックスの中心座標

	//ボックスの向き　(標準化ベクトル)
	//正面　右　上　の三種類
	//標準化ベクトルは向きのみのベクトル(長さが１)normalized
	D3DXVECTOR3 NormDirection[3];

	//各軸の長さ
	float length[3];

	//直方体の角の座標
	D3DXVECTOR3 vertexPos[8];

public:
	OrientedBoundingBox();
	virtual ~OrientedBoundingBox();

	//新しい中心座標の設定
	void SetPosition(D3DXVECTOR3 newPos);

	//新しい向きの設定
	void SetDirection(D3DXVECTOR3 newForward,//正面
		D3DXVECTOR3 newRight,				 //右
		D3DXVECTOR3 newUp);					 //上

	//各辺の長さ
	void SetLength(float forward, float right, float up);

	//バウンディングボックスの範囲の立方体の描画
	HRESULT Draw(IDirect3DDevice9* pDevice);

private:
	//分離軸に直方体を投影した時の長さを計算する
	//pSeq 分離軸(標準化ベクトル)
	//e1 e2 e3
	//ボックスの方向ベクトル(長さ含む)
	//外積軸に投影する際は2辺のみなのでe3のデフォルト引数はnull
	//(外積の計算に使った軸は外積軸と直行するため点として投影されるので　長さ０とみなす)
	static float LengthSegmentOnSepatateAxis
	(D3DXVECTOR3 *pSeq,
		D3DXVECTOR3 *e1, D3DXVECTOR3 *e2,
		D3DXVECTOR3 *e3 = NULL);

	//角の座標を更新
	//頂点座標や向きや長さが変更された際に呼び出す
	void UpdateVertexPos();

public:
	//二つのOBBが接触していればtrueを返す
	static bool Collision(OrientedBoundingBox &obb1, OrientedBoundingBox &obb2);



};