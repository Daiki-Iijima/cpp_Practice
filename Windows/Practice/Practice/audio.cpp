#include <stdio.h>
#include <alc.h>
#include <al.h>
#include <time.h>

#include "audio.h"

#pragma comment(lib,"OpenAL32.lib")			//	openalのライブラリをインポート

ALuint buffers[AUDIO_WAVEFORM_PULSE_MAX];	//	波形を保存するバッファ

//static float freqStart = DEFAULT_FREQ;	//	再生開始時の音階

typedef struct {
	ALuint sid;							//	音源ID
	int waveform;						//	波形の種類
	unsigned int length;				//	音の長さ
	unsigned int start;					//	音が鳴り始めた時間
	float decay;						//	音の減衰率
	float startGain;					//	開始時の音量
	float gain;							//	現在の音量
	float sweep;						//	周波数の変化率
	float freqStart;					//	再生開始時の音階
	float freq;							//	現在の音階
	float freqEnd;						//	目標(上限下限)の音階
}CHANNEL;

CHANNEL channels[AUDIO_CHANNEL_MAX];

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


	alGenBuffers(								//	波形データを管理するバッファを作成
		AUDIO_WAVEFORM_PULSE_MAX,				//	バッファーを作成する数
		buffers);								//	バッファーのポインターを返す

	//	==========	パルス波を作成 ==========
	{
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
				size);									//	周波数(440:ラの音)
		}
	}
	//	=====================================


	//	==========	三角波を作成 ==========
	{
		unsigned char triangle[] = {
			0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
			0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
		};

		alBufferData(								//	三角波データのセット
			buffers[AUDIO_WAVEFORM_TRIANGLE],		//	波形を保存するバッファ
			AL_FORMAT_MONO8,						//	フォーマット
			triangle,								//	波形データ
			sizeof triangle,						//	波形データのサイズ
			sizeof triangle);						//	周波数(音の高さ)
	}
	//	===================================

	//	===	長周期ノイズの波形データを作成する ===
	{
		const int len = 32767;									//	長周期ノイズの周期

		unsigned char noise[len];								//	8bitの符号なし整数

		int shiftReg = 1 << 14;									//	シフトレジスター作成(1~14の長さ)
		for (int i = 0; i < len; i++)
		{
			int result = (shiftReg ^ (shiftReg >> 1)) & 1;		//	0bit目と1bit目をXOR演算
			shiftReg >>= 1;										//	レジスター内の値を右に1ビットシフト
			shiftReg |= result << 14;							//	XOR演算で求めた値をシフトレジスターの最後に代入する

			noise[i] = 0xff * result;							//	0xff=255 ,result=(0 or 1)なので、0か255が書き込まれる
		}

		alBufferData(								//	長周期ノイズデータのセット
			buffers[AUDIO_WAVEFORM_NOISE_LONG],		//	波形を保存するバッファ
			AL_FORMAT_MONO8,						//	フォーマット
			noise,									//	波形データ
			sizeof noise,							//	波形データのサイズ
			1);										//	周波数(音の高さ)は後で決めるので1

	}
	//	====================================

	//	===	短周期ノイズの波形データを作成する ===
	{
		const int len = 93;										//	短周期ノイズの周期

		unsigned char noise[len];								//	8bitの符号なし整数

		int shiftReg = 1 << 14;									//	シフトレジスター作成(1~14の長さ)
		for (int i = 0; i < len; i++)
		{
			int result = (shiftReg ^ (shiftReg >> 6)) & 1;		//	0bit目と1bit目をXOR演算
			shiftReg >>= 1;										//	レジスター内の値を右に1ビットシフト
			shiftReg |= result << 14;							//	XOR演算で求めた値をシフトレジスターの最後に代入する

			noise[i] = 0xff * result;							//	0xff=255 ,result=(0 or 1)なので、0か255が書き込まれる
		}

		alBufferData(								//	長周期ノイズデータのセット
			buffers[AUDIO_WAVEFORM_NOISE_SHORT],	//	波形を保存するバッファ
			AL_FORMAT_MONO8,						//	フォーマット
			noise,									//	波形データ
			sizeof noise,							//	波形データのサイズ
			1);										//	周波数(音の高さ)は後で決めるので1

	}
	//	====================================

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
	{
		audioGain(i, AUDIO_DEFAULT_GAIN);	//	音量をデフォルト音量で初期化

		alGenSources(						//	音の再生をするためのソースを作成
			1,								//	ソースの数
			&channels[i].sid);				//	ソースのIDアドレス

		alSourcei(							//	ループを許可する
			channels[i].sid,				//	上で作ったsidを設定
			AL_LOOPING,						//	パラメーター(AL_LOOPING : ループするか)
			true);							//	ループを許可
	}

	//	=== 各チャンネルを初期化 ===
	{
		audioWaveform(AUDIO_CHANNEL_PULSE0, AUDIO_WAVEFORM_PULSE_12_5);
		audioWaveform(AUDIO_CHANNEL_PULSE1, AUDIO_WAVEFORM_PULSE_12_5);
		audioWaveform(AUDIO_CHANNEL_TRIANGLE, AUDIO_WAVEFORM_TRIANGLE);
		audioWaveform(AUDIO_CHANNEL_NOISE, AUDIO_WAVEFORM_NOISE_LONG);

		audioFreq(AUDIO_CHANNEL_PULSE0, AUDIO_DEFAULT_FREQ);
		audioFreq(AUDIO_CHANNEL_PULSE1, AUDIO_DEFAULT_FREQ);
		audioFreq(AUDIO_CHANNEL_TRIANGLE, AUDIO_DEFAULT_FREQ);
		audioFreq(AUDIO_CHANNEL_NOISE, audioIndexToFreq(8));
	}
	//	=============================

	return 0;
}

void audioWaveform(int _channel, int _waveform)
{
	channels[_channel].waveform = _waveform;

	alSourcei(												//	波形データを設定
		channels[_channel].sid,								//	sid
		AL_BUFFER,											//	パラメーター(AL_BUFFER : バッファーを設定する)
		buffers[channels[_channel].waveform]);				//	waveformの現在の番号のパルス波を設定
}

void audioGain(int _channel, float _gain)
{
	channels[_channel].gain = channels[_channel].startGain = _gain;

	alSourcef(													//	音量を調整
		channels[_channel].sid,									//	sid
		AL_GAIN,												//	パラメーター(AL_GAIN : 音量)
		channels[_channel].gain);								//	上で設定した音量を設定

}

void audioLength(int _channel, unsigned int _millis)
{
	channels[_channel].length = _millis;
}

void audioDecay(int _channel, float _decay)
{
	channels[_channel].decay = _decay;
}

void audioSweep(int _channel, float _sweep, float _freqEnd)
{
	channels[_channel].sweep = _sweep;
	channels[_channel].freqEnd = _freqEnd;
}

void audioFreq(int _channel, float _freq)
{
	channels[_channel].freqStart = _freq;

	alSourcef(													//	ピッチを設定
		channels[_channel].sid,									//	sid
		AL_PITCH,												//	パラメーター(AL_PITCH : ピッチ)
		channels[_channel].freq);								//	周波数 / デフォルト周波数 = ピッチ
}

float audioIndexToFreq(int _index)
{
	int divisorTable[] = {										//	ファミコンの除数テーブル(16)
		4,8,16,32,64,96,128,160,202,254,380,508,762,1016,2034,4068
	};

	return 1789772.5f / divisorTable[_index];					//	周波数を返す(1789772.5f = ファミコンのCPUの周波数)
}

void audioPlay(int _channel)
{
	channels[_channel].gain = channels[_channel].startGain;		//	現在の音量を開始音量で初期化

	alSourcef(													//	音量を調整
		channels[_channel].sid,									//	sid
		AL_GAIN,												//	パラメーター(AL_GAIN : 音量)
		channels[_channel].gain);								//	デフォルト値を設定しつつ(0.1f)gainにも最初の値として保存

	channels[_channel].freq = channels[_channel].freqStart;		//	開始周波数を設定

	alSourcef(													//	ピッチを設定
		channels[_channel].sid,									//	sid
		AL_PITCH,												//	パラメーター(AL_PITCH : ピッチ)
		channels[_channel].freq);								//	周波数 / デフォルト周波数 = ピッチ

	alSourcePlay(channels[_channel].sid);						//	再生
	channels[_channel].start = clock();							//	再生した時刻を保存
}

void audioStop(int _channel)
{
	alSourceStop(channels[_channel].sid);						//	停止
}

void audioUpdate()
{
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
	{
		if ((channels[i].length > 0) && (clock() - channels[i].start >= channels[i].length))
		{
			audioStop(i);
		}

		if ((channels[i].decay != 00) && (channels[i].decay < 1))
		{
			alSourcef(										//	音量を調整
				channels[i].sid,							//	上で作ったsidを設定
				AL_GAIN,									//	パラメーター(AL_GAIN : 音量)
				channels[i].gain *= channels[i].decay);		//	現在の音量から減衰率をかけた値を入れる
		}

		if (channels[i].sweep != 0)
		{
			channels[i].freq *= channels[i].sweep;
			if (channels[i].freqEnd != 0) {
				if (
					(channels[i].sweep >= 1 && channels[i].freq >= channels[i].freqEnd) ||		//	高くなる場合
					(channels[i].sweep <= 1 && channels[i].freq <= channels[i].freqEnd)			//	低くなる場合
					)
				{
					audioStop(i);
				}
			}
			alSourcef(										//	ピッチを設定
				channels[i].sid,							//	sid
				AL_PITCH,									//	パラメーター(AL_PITCH : ピッチ)
				channels[i].freq);							//	周波数 / デフォルト周波数 = ピッチ
		}

	}

	ALenum error = alGetError();							//	エラーの取得
	if (error != AL_NO_ERROR)								//	エラーがあった場合(AL_NO_ERROR : エラーがない)
	{
		printf("%s\n", alGetString(error));
	}
}