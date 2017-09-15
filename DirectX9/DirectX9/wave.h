#pragma once

//using namespace std;
//stringを使用するときにstd::を省略できるがヘッダー内でusing namespace std を使用するのはあまりよくないので使用しない
//よくない理由はこのヘッダーで読み込んだファイル全てで有効になるから

#pragma comment(lib,"dsound.lib")//ダイレクトサウンド
#include<dsound.h>//ダイレクトサウンド

#include<tchar.h>
#include<string>

//一つのWave形式の音声データファイルを読み込んで保持するクラス
class WaveFile
{
	//SoundBufferクラスからこのクラスのprivateなメンバにアクセスできるようにする
	friend class SoundBuffer;
private:
	//読み込んだ　(読み込もうとしている)
	//Waveファイルの記憶のされ方
	WAVEFORMATEX waveFormat;

	int dataSize;//波形データのデータ量
	byte* pWaveData;//波形データ(音声データ)

public:
	//コンストラクタ
	WaveFile();
	//デストラクタ
	~WaveFile();

	//解放処理
	void Release();

	//ファイルロード
	bool Load(TCHAR*filePath);
	bool Load(std::string filePath);
};