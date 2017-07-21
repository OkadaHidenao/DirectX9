#include"Texture.h"

//コンストラクタ
Texture::Texture()
{
	pTexture = nullptr;
}

//デストラクタ
Texture::~Texture()
{
	Release();

	//分割数は最低１
	divU = divV = 1;

	//分割中の何個目かを表す数値は０から分割数−１までとする
	numU = numV = 0;
}

//解放処理
void Texture::Release()
{
	if (pTexture != nullptr)
	{
		//IDirect3DTexture9は専用の変数でメモリを確保してDirect3D側で管理している

		//解放する際も専用の関数を実行する
		pTexture->Release();

		pTexture = nullptr;


		divU = divV = 1;
		numU = numV = 0;
	}
}

bool Texture::Load(TCHAR* FilePath)
{
	Direct3D &d3d = Direct3D::GetInstance();

	//Loadの処理にDirect3Dのデバイスが必要なのでDirect3D側にロードの処理の本体を記述している

	//ロードの実行と値のリターン
	return d3d.LoadTexture((*this), FilePath);

}

//分割の設定(左右上下いくつに等分するか)
void Texture::SetDivide(unsigned int DivU,
	unsigned int DivV)
{
	//分割数は1以上
	divU = (DivU != 0) ? DivU : 1;
	divV = (DivV != 0) ? DivV : 1;

	//現在設定されているnumUが新しく設定したdivUを超えてしまった場合の対処
	numU = (numU > divU) ? divU - 1 : numU;
	//numVのほうも同様
	numV = (numV > divV) ? divV - 1 : numV;
}

//分割中の何個目を選択しているか
void Texture::SetNum(unsigned int NumU,
	unsigned int NumV)
{
	numU = (numU > divU) ? divU-1 : numU;
	numV = (numV > divV) ? divV-1 : numV;
}