#pragma once

#pragma comment(lib,"dsound.lib")

#include <dsound.h>	

class WaveFile;

class SoundBuffer
{
private:
	IDirectSoundBuffer *pBuffer;

public:
	//コンストラクタ
	SoundBuffer();
	//デストラクタ
	~SoundBuffer();

	bool Create(WaveFile& waveFile);

	//再生　停止
	void Play(bool loop);
	void Stop();

	//解放
	void Release();
};