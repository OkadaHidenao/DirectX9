#define _CRT_SECURE_NO_WARNINGS
//fopenでの警告をなくす
//この警告があるとコンパイルできなくなる

#include"wave.h"

using namespace std;

//コンストラクタ
WaveFile::WaveFile()
{
	//WaveFormatの中身全てをゼロに初期化

	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	pWaveData = nullptr;
	dataSize = 0;

}

//デストラクタ
WaveFile::~WaveFile()
{
	Release();
}

//解放処理 メンバ変数をコンストラクタ実行時と同じ状態に戻す
void WaveFile::Release()
{
	if (pWaveData)//ロード済みか
	{
		delete[]pWaveData;
		pWaveData = nullptr;
	}
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));
	dataSize = 0;
}
bool WaveFile::Load(TCHAR *filepath)
{
	//.wav 形式のデータの保存のされ方
	//3つのチャンクに分かれている
	//チャンク　データを区切った塊

	//RIFFヘッダーチャンク　ファイルの種類を表している
	//フォーマットチャンク　音声データの記録のされ方
	//データチャンク　　　　音声(波形)データ

	FILE *fp = nullptr;

	//ファイルのオープン
	//バイナリ形式の読み込みモードで開く
	fp = _tfopen(filepath, _T("rb"));//r read, b binary

	if (fp == nullptr)
	{
		//ファイルオープンに失敗
		return false;
	}

	//ファイルのオープンに成功している

	//チャンクに関する情報を入れる変数
	char chunkID[5]{};
	char type[5]{};
	unsigned int chunkSize = 0;

	//RIFFヘッダーチャンクを読み込み

	//RIFFヘッダーの構成-------------------------------------------------------------
	//riff 4byte"RIFF"の４文字
	//size 4byte 総ファイルサイズ-8(byte 単位)
	//			 引かれた8はriffとsizeの容量分
	//           (つまりsize　以降の残りのデータ量)
	//type 4byte "WAVE"の４文字(音声データであることを示す)
	//-------------------------------------------------------------------------------

	//riff=="RIFF" type="WAVE"
	//の場合のみ、その後の読み込みを行う

	//fread関数
	//chunkIDのアドレスにfpから読みだしたデータをコピーする
	//データ量char４個分
	//1 (件数)	配列変数にまとめて複数件データを読み込みたいときに利用
	fread(chunkID, sizeof(char) * 4, 1, fp);

	//fread行うと　読みだした後　fpの次回の読み出し開始位置が
	//読みだしたデータ量分だけ後ろへずれる

	//ファイルデータ量の読み込み
	//ファイルの総容量自体は以降利用しないがfpの読み込み開始時点をずらすために実行
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	//typeの読み出し
	fread(type, sizeof(char)*4, 1, fp);

	//文字列の等価比較がやりやすいように
	//string型に文字列をコピーする
	string chunkID_st = chunkID;
	string type_st = type;

	//正しいWAVEファイルかどうかを調べる
	if (!(chunkID_st == "RIFF"&&type_st == "WAVE"))
	{
		return false;//条件から外れているので失敗
	}

	//それぞれのチャンクを読みだしたか
	bool fmtChunk = false;
	bool dateChunk = false;


	//チャンクごとの呼び出し
	//フォーマットチャンクとデータチャンクが両方読み出せたらループから抜ける
	while (!(fmtChunk&&dateChunk))
	{
		//どのチャンクもchunkIDとchunkのデータ容量からデータがスタートする
		fread(chunkID, sizeof(char) * 4, 1, fp);
		fread(&chunkSize, sizeof(unsigned int), 1, fp);
		//チャンクサイズはチャンク全体から-8
		//(idとsizeの容量分)されたもの

		chunkID_st = chunkID;

		if (chunkID_st == "fmt ")//fmtの後ろにスペース
		{
			//読み込もうとしているのがフォーマットチャンク

			//フォーマットチャンク-----------------------------------------------
			//id				4byte		チャンク識別子
			//size				4byte		idとizeを除くチャンクサイズ
			//format			2byte		波形データのフォーマット
			//channels			2byte		チャンネル数(モノラルorステレオ)
			//samplerate		4byte		サンプリング周波数
			//bytepersec		4byte		平均データの割合
			//blockalign		2byte		データのブロックサイズ
			//bitswidth			2byte		1サンプルあたりのビット数
			//eztended_size		2byte		拡張データのサイズ
			//extended			Nbyte		拡張データ
			//--------------------------------------------------------------------
			//これらのデータをまとめてWAVEFORMATEXの変数に読み込む
			if (chunkSize >= sizeof(WAVEFORMATEX))
			{
				//チャンクサイズがWAVEFORMATEXより大きい場合
				//おそらく拡張データがある場合
				//その場合は拡張分を無視

				fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);

				//チャンクの残りの領域の大きさの計算
				int diff = chunkSize - sizeof(WAVEFORMATEX);
				//diff difference　差

				//次のチャンクの先頭までシーク
				//(読み飛ばす)
				fseek(fp, diff, SEEK_CUR);//CURは現在の位置から
			}
			else
			{
				//チャンクサイズがWAVEFORMATEXの大きさに満たない場合
				//チャンクの終わりまで読む
				//とりあえずある分だけ読む
				fread(&waveFormat,chunkSize, 1, fp);
			}
			//フォーマットチャンク読み込みフラグ
			fmtChunk = true;
		}
		else if (chunkID_st == "data")//データチャンク
		{
			//データチャンク---------------------------------------------
			//id				4byte	チャンクの識別子
			//size				4byte	idとsizeを除くチャンクのサイズ
			//							(そのまま波形データのデータ量)
			//waveformatDate	Nbyte	波形データ
			//-----------------------------------------------------------

			//データ領域の確保
			dataSize = chunkSize;
			pWaveData = new byte[dataSize];//動的にデータ確保

			//データの読み込み
			//freadの戻り値は正しく読み込めた件数
			//dateSize件分読む
			unsigned long readCount
				= fread(pWaveData, sizeof(byte), dataSize, fp);

			if (readCount != dataSize)
			{
				//すべて読めなかった
				fclose(fp);//ファイル閉じる

				Release();//データリセットする

				return false;
			}

			dateChunk = true;
		}
		else
		{
			//fmtでもdateでもないチャンクは読み飛ばし
			//(JUNKとか)

			fseek(fp, chunkSize, SEEK_CUR);
		}

	}
	fclose(fp);
	return true;//成功
	
}


bool WaveFile::Load(string filepath)
{
	const TCHAR* tp = _T(filepath. c_str());

	return Load(tp);
}