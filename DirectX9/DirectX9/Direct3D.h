#pragma once

//インクルードファイル　Direct3D関系
#include<d3d9.h>
#include<d3dx9.h>

//ライブラリファイルの読み込み
//Direct3D関係
#pragma comment(lib,"d3d9lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

//インクルードファイル　windows
#include<Windows.h>

//ライブラリファイル　windows
#pragma comment(lib,"winmm.lib")

#include<tchar.h>


//シングルトンなクラス
//インスタンスが一つしか作られないことを保証するクラス
//コンストラクタや代入演算子をprivateにしてオブジェクトの作成の手段を制限
class Direct3D
{
private:

	//クラス外からはオブジェクトが増える処理をさせない
	Direct3D();
	~Direct3D();
	Direct3D(const Direct3D& obj) {};		//コピーコンストラクタ
	void operator=(const Direct3D& obj) {};//代入演算

	//唯一のオブジェクトを指すポインタ
	static Direct3D* pInstance = nullptr;

public:

	//唯一のインスタンスへの参照を取得する関数
	//最初の実行の時にインスタンスを作成する
	static Direct3D& GetInstance()
	{
		if (pInstance == nullptr)
		{
			pInstance = new Direct3D();
		}
		return *pInstance;//唯一のインスタンスへの参照を返す
	}

	//作成した唯一のインスタンスを破棄する
	static void DestroInstance();
	{
		if (pInstance != nullptr)
		{
			delete pInstance;
			pInstance = nullptr;
		}
	}

	//解放関数
	//コンストラクタ実行時と同じ状態に戻す
	void Release();

private:
	//Direct3Dのデバイス
	//描画をしたり画像をロードしたりするために使用するオブジェクト
	IDirect3DDevice9*pDevice3D;

	//Direct3Dデバイスの作成を行うためのオブジェクト
	IDirect3D9* pD3D9;

	//以下　メンバ関数の宣言
private:
	bool Create(HWND hWnd);

public:

	//Direct3Dデバイスの作成を試みる
	//失敗するとfalseが返るようにする
	bool TryCreate(HWND hWnd);

	

};