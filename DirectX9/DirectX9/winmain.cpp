
#include<Windows.h>
#pragma comment(lib,"winmm.lib")

#include<tchar.h>

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

	//ウィンドウクラスの登録時に各ウィンドウに設定される

	//アプリケーションがメッセージを取得したら呼び出される

	//メッセージの例

	//WM_DESTROY　ウィンドウが破棄された
	//			  ×ボタンが押されたときなど
	//WM_MOVE     ウィンドウが移動
	//WM_SIZE	  ウィンドウサイズの変更
	//等

	//特別な処理を必要とする場合
	//または自分で独自の処理を定義したい場合
	//ここでメッセージの種類に応じた処理を行う

	//とりあえず　WM_DESTROYが来た時の終了処理のみ定義

	switch (msg)
	{
	case WM_DESTROY:
		
		PostQuitMessage(0);

		//メッセージキューに新しくWM_QUITメッセージを送る

		//いずれメッセージキューから取り出されメインループが終了する
		//(メインループの終了条件をそのように作る)



		break;
	}

	//独自の処理を行ったらデフォルトのウィンドウプロシージャ

	return DefWindowProc(hWnd, msg, wParam, lParam);

}


//エントリーポイント
//プログラムの開始関数

int _stdcall WinMain
(HINSTANCE hInstace,		//プログラムを識別するハンドル
	HINSTANCE hPrevInstance,//アプリケーションの前のインスタンスハンドル
							//win32だと常にNULL
	LPSTR lpCmdLine			//コマンドライン引数
	,int nCmdShow)			//ウィンドウの表示状態
{
	//メッセージボックス
	MessageBox(NULL,			//ウィンドウのハンドル
		TEXT("テスト"),			//本文
		TEXT("テスト‐タイトル"),//タイトル
		MB_OK);					//メッセージボックスのタイプ
								//MB_OK okのボタンが表示
}