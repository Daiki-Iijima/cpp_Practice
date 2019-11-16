#include <stdio.h>
#include <alc.h>
#include <al.h>

#include "audio.h"

#pragma comment(lib,"OpenAL32.lib")			//	openalのライブラリをインポート

static ALuint sid;							//	音源ID
ALuint buffers[AUDIO_WAVEFORM_PULSE_MAX];	//	波形を保存するバッファー
static int waveform;

int audioInit()
{
	ALCdevice* device = alcOpenDevice(NULL);	//	デバイスを開いて、デバイスのポインタを取得
	if (device == NULL)
		return 1;								//	うまくいかなかった場合1

	ALCcontext* context = alcCreateContext(		//	コンテキストを作成
		device,									//	一つ上で取得したデバイス
		NULL);
	if (context == NULL)
		return 1;								//	うまくいかなかった場合1

	alcMakeContextCurrent(context);				//	現在のコンテキストを上で作成したコンテキストに設定


	alGenBuffers(								//	波形データを管理するバッファーを作成
		AUDIO_WAVEFORM_PULSE_MAX,				//	バッファーを作成する数
		buffers);								//	バッファーのポインターを返す

	//	==========	パルス波を作成 ==========
	unsigned char pulse[][8] = {						//	パルス波を作成(8なのは8ビットで使うから)
		{0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00},		//	(1:7)12.5%のパルス波 (AUDIO_WAVEFORM_PULSE_12_5)
		{0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00},		//	(2:6)20%のパルス波 (AUDIO_WAVEFORM_PULSE_25)
		{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},		//	(4:4)50%のパルス波 (AUDIO_WAVEFORM_PULSE_50)
		{0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00},		//	(4:7)75%のパルス波 (AUDIO_WAVEFORM_PULSE_75)
	};

	for (int i = AUDIO_WAVEFORM_PULSE_12_5; i <= AUDIO_WAVEFORM_PULSE_75; i++)
	{
		ALsizei size = sizeof pulse[0];				//	pulse変数1つの配列の長さを取得
		alBufferData(								//	パルス波データのセット
			buffers[i],								//	波形を保存するバッファ
			AL_FORMAT_MONO8,						//	フォーマット
			pulse[i],								//	波形データ
			size,									//	波形データのサイズ
			size * 440);							//	周波数(440:ラの音)
	}
	//	=====================================


	//	==========	三角波を作成 ==========
	unsigned char triangle[] = {
		0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
	};

	alBufferData(								//	三角波データのセット
		buffers[AUDIO_WAVEFORM_TRIANGLE],		//	波形を保存するバッファ
		AL_FORMAT_MONO8,						//	フォーマット
		triangle,								//	波形データ
		sizeof triangle,						//	波形データのサイズ
		sizeof triangle * 440);					//	周波数(440:ラの音)

	//	===================================

	alGenSources(		//	音の再生をするためのソースを作成
		1,				//	ソースの数
		&sid);			//	ソースのIDアドレス

	alSourcei(			//	ループを許可する
		sid,			//	上で作ったsidを設定
		AL_LOOPING,		//	パラメーター(AL_LOOPING : ループするか)
		true);			//	ループを許可

	alSourcef(			//	音量を調整
		sid,			//	上で作ったsidを設定
		AL_GAIN,		//	パラメーター(AL_GAIN : 音量)
		.1f);			//	0.1f


	return 0;
}

void audioWaveform(int _waveform)
{
	waveform = _waveform;	//	波形を設定
}

void audioPlay()
{
	alSourcei(					//	ソースにバッファーをセットする
		sid,					//	sidを設定
		AL_BUFFER,				//	パラメーター(AL_BUFFER : バッファーを設定する)
		buffers[waveform]);		//	waveformの現在の番号のパルス波を設定

	alSourcePlay(sid);	//	再生
}

void audioStop()
{
	alSourceStop(sid);	//	停止
}