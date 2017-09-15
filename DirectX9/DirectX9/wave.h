#pragma once

//using namespace std;
//string���g�p����Ƃ���std::���ȗ��ł��邪�w�b�_�[����using namespace std ���g�p����̂͂��܂�悭�Ȃ��̂Ŏg�p���Ȃ�
//�悭�Ȃ����R�͂��̃w�b�_�[�œǂݍ��񂾃t�@�C���S�ĂŗL���ɂȂ邩��

#pragma comment(lib,"dsound.lib")//�_�C���N�g�T�E���h
#include<dsound.h>//�_�C���N�g�T�E���h

#include<tchar.h>
#include<string>

//���Wave�`���̉����f�[�^�t�@�C����ǂݍ���ŕێ�����N���X
class WaveFile
{
	//SoundBuffer�N���X���炱�̃N���X��private�ȃ����o�ɃA�N�Z�X�ł���悤�ɂ���
	friend class SoundBuffer;
private:
	//�ǂݍ��񂾁@(�ǂݍ������Ƃ��Ă���)
	//Wave�t�@�C���̋L���̂����
	WAVEFORMATEX waveFormat;

	int dataSize;//�g�`�f�[�^�̃f�[�^��
	byte* pWaveData;//�g�`�f�[�^(�����f�[�^)

public:
	//�R���X�g���N�^
	WaveFile();
	//�f�X�g���N�^
	~WaveFile();

	//�������
	void Release();

	//�t�@�C�����[�h
	bool Load(TCHAR*filePath);
	bool Load(std::string filePath);
};