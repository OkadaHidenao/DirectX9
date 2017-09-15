#pragma once

#include"Direct3D.h"

#include<string>

//.x形式の3Dモデルファイル　一つを保持するクラス
class MeshX
{
	friend class Direct3D;
private:
	LPD3DXMESH pMesh;//メッシュ
	DWORD	   numMaterials;//マテリアルの個数

	LPDIRECT3DTEXTURE9* ppTexture;//テクスチャの配列
	D3DMATERIAL9*		pMaterials;//マテリアルの配列

public:
	MeshX();
	~MeshX();

	//描画
	//MATRIX 行列
	void Draw(D3DXMATRIXA16& matTransform, //座標
		D3DXMATRIXA16& matRotation,		  //回転
		D3DXMATRIXA16& matScale);		  //拡大縮小

	bool Load(TCHAR* path);
	bool Load(std::string path);
};