#include"DirectSound.h"
#include"wave.h"

#include"SoundBuffer.h"

//コンストラクタ
SoundBuffer::SoundBuffer()
{
	pBuffer = NULL;
}

//デストラクタ
SoundBuffer::~SoundBuffer()
{
	Release();
}

//解放処理
void SoundBuffer::Release()
{
	//バッファにデータがあるなら
	if (pBuffer != NULL)
	{
		//解放処理の前に再生を停止する
		Stop();

		pBuffer->Release();
		pBuffer = NULL;
	}
}

bool SoundBuffer::Create(WaveFile& waveFile)
{
	//バッファ作成用の設定　変数
	//desc : description
	DSBUFFERDESC desc = {};

	//チャンネル数で分岐
	//モノラル　１チャンネル
	//ステレオ　２チャンネル

	switch (waveFile.waveFormat.nChannels)
	{
	case 1:
		desc.dwFlags = 
			DSBCAPS_CTRL3D | 
			DSBCAPS_CTRLVOLUME | 
			DSBCAPS_CTRLFREQUENCY | 
			DSBCAPS_GETCURRENTPOSITION2 | 
			DSBCAPS_CTRLPOSITIONNOTIFY | 
			DSBCAPS_STATIC;

		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;

		break;

	default://ステレオ　３チャンネル以上の場合もある
		if (waveFile.waveFormat.nChannels >= 2)
		{
			desc.dwFlags =
				//DSBCAPS_CTRL3D |//モノラルの設定から抜く
				DSBCAPS_CTRLVOLUME |
				DSBCAPS_CTRLFREQUENCY |
				DSBCAPS_GETCURRENTPOSITION2 |
				DSBCAPS_CTRLPOSITIONNOTIFY |
				DSBCAPS_STATIC |
				DSBCAPS_CTRLPAN;
			//モノラルのフラグ設定から最後の一行が増えただけ

			desc.guid3DAlgorithm = GUID_NULL;
		}
		break;
	}

	//構造体のサイズ
	desc.dwSize = sizeof(DSBUFFERDESC);
	//波形データのサイズ
	desc.dwBufferBytes = waveFile.dataSize;
	//waveファイルフォーマット
	desc.lpwfxFormat = &(waveFile.waveFormat);

	//設定終わり

	//デバイスの取得
	DirectSound* pDirectSound = DirectSound::GetInstance();

	//サウンドバッファ
	//メンバ変数のほうのサウンドバッファと型が違う
	//型名としては８がついているかついていないか
	IDirectSoundBuffer* pBuffer_work = NULL;


	//バッファの作成をDirectSoundクラスにやっていもらう
	//waveFile.dataSizeが収まるようなデータ領域の確保
	if (pDirectSound->CreateSoundBuffer(&desc, &pBuffer_work))
	{
		return false;
		//渡されたwaveデータに問題がある
		//初期化されていない
		//ロード失敗の可能性
	}

	//サウンドバッファの変換
	//IDirectSoundBuffer*からIDirectSoundBuffer8に変換
	//第一引数で変換後の型を指定
	//第二引数で返還後のデータを受け取るポインタ
	//変換後の型を指定する性質上　第二引数の型が不安
	//なのでvoid**に変換して強引に引数に渡す
	pBuffer_work->
		QueryInterface(IID_IDirectSoundBuffer8, (void**)&pBuffer);

	//変換出来たら変換元は用済み
	pBuffer_work->Release();

	//データの書き込み
	unsigned char* block1 = nullptr;
	unsigned char* block2 = nullptr;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;

	//バッファをロード
	pBuffer->Lock(
		0, waveFile.dataSize,
		(void**)&block1, &blockSize1,
		(void**)&block2, &blockSize2,
		DSBLOCK_ENTIREBUFFER);

	//バッファに音データをコピー
	memcpy(block1, waveFile.pWaveData,waveFile.dataSize);

	pBuffer->Unlock(block1, blockSize1, block2, 0);
	return true;

}

//再生開始
void SoundBuffer::Play(bool loop)
{
	if (pBuffer != NULL)
	{
		DWORD loopFlag = (loop ? DSBPLAY_LOOPING : 0);

		//第一引数　reserved 予約済み　０でなければならない
		//第二引数　priority 優先順位
		//					 ハードウェアミキシングリソースを割り当てる際に使う
		//					 バッファを作成するときにDSBCAPS_LOCDEFERが
		//					 フラグとして設定されていない場合は０でなければならない
		pBuffer->Play(0, 0, loopFlag);
	}
}

//再生停止
void SoundBuffer::Stop()
{
	if (pBuffer != NULL)
	{
		pBuffer->Stop();
	}
}