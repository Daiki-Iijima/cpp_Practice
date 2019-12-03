#include <stdio.h>
#include <alc.h>
#include <al.h>
#include <time.h>

#include "audio.h"

#pragma comment(lib,"OpenAL32.lib")			//	openal�̃��C�u�������C���|�[�g

ALuint buffers[AUDIO_WAVEFORM_PULSE_MAX];	//	�g�`��ۑ�����o�b�t�@

//static float freqStart = DEFAULT_FREQ;	//	�Đ��J�n���̉��K

typedef struct {
	ALuint sid;							//	����ID
	int waveform;						//	�g�`�̎��
	unsigned int length;				//	���̒���
	unsigned int start;					//	������n�߂�����
	float decay;						//	���̌�����
	float startGain;					//	�J�n���̉���
	float gain;							//	���݂̉���
	float sweep;						//	���g���̕ω���
	float freqStart;					//	�Đ��J�n���̉��K
	float freq;							//	���݂̉��K
	float freqEnd;						//	�ڕW(�������)�̉��K
}CHANNEL;

CHANNEL channels[AUDIO_CHANNEL_MAX];

int audioInit()
{
	ALCdevice* device = alcOpenDevice(NULL);	//	�f�o�C�X���J���āA�f�o�C�X�̃|�C���^���擾
	if (device == NULL)
		return 1;								//	���܂������Ȃ������ꍇ1

	ALCcontext* context = alcCreateContext(		//	�R���e�L�X�g���쐬
		device,									//	���Ŏ擾�����f�o�C�X
		NULL);
	if (context == NULL)
		return 1;								//	���܂������Ȃ������ꍇ1

	alcMakeContextCurrent(context);				//	���݂̃R���e�L�X�g����ō쐬�����R���e�L�X�g�ɐݒ�


	alGenBuffers(								//	�g�`�f�[�^���Ǘ�����o�b�t�@���쐬
		AUDIO_WAVEFORM_PULSE_MAX,				//	�o�b�t�@�[���쐬���鐔
		buffers);								//	�o�b�t�@�[�̃|�C���^�[��Ԃ�

	//	==========	�p���X�g���쐬 ==========
	{
		unsigned char pulse[][8] = {						//	�p���X�g���쐬(8�Ȃ̂�8�r�b�g�Ŏg������)
			{0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00},		//	(1:7)12.5%�̃p���X�g (AUDIO_WAVEFORM_PULSE_12_5)
			{0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00},		//	(2:6)20%�̃p���X�g (AUDIO_WAVEFORM_PULSE_25)
			{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00},		//	(4:4)50%�̃p���X�g (AUDIO_WAVEFORM_PULSE_50)
			{0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00},		//	(4:7)75%�̃p���X�g (AUDIO_WAVEFORM_PULSE_75)
		};

		for (int i = AUDIO_WAVEFORM_PULSE_12_5; i <= AUDIO_WAVEFORM_PULSE_75; i++)
		{
			ALsizei size = sizeof pulse[0];				//	pulse�ϐ�1�̔z��̒������擾
			alBufferData(								//	�p���X�g�f�[�^�̃Z�b�g
				buffers[i],								//	�g�`��ۑ�����o�b�t�@
				AL_FORMAT_MONO8,						//	�t�H�[�}�b�g
				pulse[i],								//	�g�`�f�[�^
				size,									//	�g�`�f�[�^�̃T�C�Y
				size);									//	���g��(440:���̉�)
		}
	}
	//	=====================================


	//	==========	�O�p�g���쐬 ==========
	{
		unsigned char triangle[] = {
			0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
			0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
		};

		alBufferData(								//	�O�p�g�f�[�^�̃Z�b�g
			buffers[AUDIO_WAVEFORM_TRIANGLE],		//	�g�`��ۑ�����o�b�t�@
			AL_FORMAT_MONO8,						//	�t�H�[�}�b�g
			triangle,								//	�g�`�f�[�^
			sizeof triangle,						//	�g�`�f�[�^�̃T�C�Y
			sizeof triangle);						//	���g��(���̍���)
	}
	//	===================================

	//	===	�������m�C�Y�̔g�`�f�[�^���쐬���� ===
	{
		const int len = 32767;									//	�������m�C�Y�̎���

		unsigned char noise[len];								//	8bit�̕����Ȃ�����

		int shiftReg = 1 << 14;									//	�V�t�g���W�X�^�[�쐬(1~14�̒���)
		for (int i = 0; i < len; i++)
		{
			int result = (shiftReg ^ (shiftReg >> 1)) & 1;		//	0bit�ڂ�1bit�ڂ�XOR���Z
			shiftReg >>= 1;										//	���W�X�^�[���̒l���E��1�r�b�g�V�t�g
			shiftReg |= result << 14;							//	XOR���Z�ŋ��߂��l���V�t�g���W�X�^�[�̍Ō�ɑ������

			noise[i] = 0xff * result;							//	0xff=255 ,result=(0 or 1)�Ȃ̂ŁA0��255���������܂��
		}

		alBufferData(								//	�������m�C�Y�f�[�^�̃Z�b�g
			buffers[AUDIO_WAVEFORM_NOISE_LONG],		//	�g�`��ۑ�����o�b�t�@
			AL_FORMAT_MONO8,						//	�t�H�[�}�b�g
			noise,									//	�g�`�f�[�^
			sizeof noise,							//	�g�`�f�[�^�̃T�C�Y
			1);										//	���g��(���̍���)�͌�Ō��߂�̂�1

	}
	//	====================================

	//	===	�Z�����m�C�Y�̔g�`�f�[�^���쐬���� ===
	{
		const int len = 93;										//	�Z�����m�C�Y�̎���

		unsigned char noise[len];								//	8bit�̕����Ȃ�����

		int shiftReg = 1 << 14;									//	�V�t�g���W�X�^�[�쐬(1~14�̒���)
		for (int i = 0; i < len; i++)
		{
			int result = (shiftReg ^ (shiftReg >> 6)) & 1;		//	0bit�ڂ�1bit�ڂ�XOR���Z
			shiftReg >>= 1;										//	���W�X�^�[���̒l���E��1�r�b�g�V�t�g
			shiftReg |= result << 14;							//	XOR���Z�ŋ��߂��l���V�t�g���W�X�^�[�̍Ō�ɑ������

			noise[i] = 0xff * result;							//	0xff=255 ,result=(0 or 1)�Ȃ̂ŁA0��255���������܂��
		}

		alBufferData(								//	�������m�C�Y�f�[�^�̃Z�b�g
			buffers[AUDIO_WAVEFORM_NOISE_SHORT],	//	�g�`��ۑ�����o�b�t�@
			AL_FORMAT_MONO8,						//	�t�H�[�}�b�g
			noise,									//	�g�`�f�[�^
			sizeof noise,							//	�g�`�f�[�^�̃T�C�Y
			1);										//	���g��(���̍���)�͌�Ō��߂�̂�1

	}
	//	====================================

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++)
	{
		audioGain(i, AUDIO_DEFAULT_GAIN);	//	���ʂ��f�t�H���g���ʂŏ�����

		alGenSources(						//	���̍Đ������邽�߂̃\�[�X���쐬
			1,								//	�\�[�X�̐�
			&channels[i].sid);				//	�\�[�X��ID�A�h���X

		alSourcei(							//	���[�v��������
			channels[i].sid,				//	��ō����sid��ݒ�
			AL_LOOPING,						//	�p�����[�^�[(AL_LOOPING : ���[�v���邩)
			true);							//	���[�v������
	}

	//	=== �e�`�����l���������� ===
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

	alSourcei(												//	�g�`�f�[�^��ݒ�
		channels[_channel].sid,								//	sid
		AL_BUFFER,											//	�p�����[�^�[(AL_BUFFER : �o�b�t�@�[��ݒ肷��)
		buffers[channels[_channel].waveform]);				//	waveform�̌��݂̔ԍ��̃p���X�g��ݒ�
}

void audioGain(int _channel, float _gain)
{
	channels[_channel].gain = channels[_channel].startGain = _gain;

	alSourcef(													//	���ʂ𒲐�
		channels[_channel].sid,									//	sid
		AL_GAIN,												//	�p�����[�^�[(AL_GAIN : ����)
		channels[_channel].gain);								//	��Őݒ肵�����ʂ�ݒ�

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

	alSourcef(													//	�s�b�`��ݒ�
		channels[_channel].sid,									//	sid
		AL_PITCH,												//	�p�����[�^�[(AL_PITCH : �s�b�`)
		channels[_channel].freq);								//	���g�� / �f�t�H���g���g�� = �s�b�`
}

float audioIndexToFreq(int _index)
{
	int divisorTable[] = {										//	�t�@�~�R���̏����e�[�u��(16)
		4,8,16,32,64,96,128,160,202,254,380,508,762,1016,2034,4068
	};

	return 1789772.5f / divisorTable[_index];					//	���g����Ԃ�(1789772.5f = �t�@�~�R����CPU�̎��g��)
}

void audioPlay(int _channel)
{
	channels[_channel].gain = channels[_channel].startGain;		//	���݂̉��ʂ��J�n���ʂŏ�����

	alSourcef(													//	���ʂ𒲐�
		channels[_channel].sid,									//	sid
		AL_GAIN,												//	�p�����[�^�[(AL_GAIN : ����)
		channels[_channel].gain);								//	�f�t�H���g�l��ݒ肵��(0.1f)gain�ɂ��ŏ��̒l�Ƃ��ĕۑ�

	channels[_channel].freq = channels[_channel].freqStart;		//	�J�n���g����ݒ�

	alSourcef(													//	�s�b�`��ݒ�
		channels[_channel].sid,									//	sid
		AL_PITCH,												//	�p�����[�^�[(AL_PITCH : �s�b�`)
		channels[_channel].freq);								//	���g�� / �f�t�H���g���g�� = �s�b�`

	alSourcePlay(channels[_channel].sid);						//	�Đ�
	channels[_channel].start = clock();							//	�Đ�����������ۑ�
}

void audioStop(int _channel)
{
	alSourceStop(channels[_channel].sid);						//	��~
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
			alSourcef(										//	���ʂ𒲐�
				channels[i].sid,							//	��ō����sid��ݒ�
				AL_GAIN,									//	�p�����[�^�[(AL_GAIN : ����)
				channels[i].gain *= channels[i].decay);		//	���݂̉��ʂ��猸�������������l������
		}

		if (channels[i].sweep != 0)
		{
			channels[i].freq *= channels[i].sweep;
			if (channels[i].freqEnd != 0) {
				if (
					(channels[i].sweep >= 1 && channels[i].freq >= channels[i].freqEnd) ||		//	�����Ȃ�ꍇ
					(channels[i].sweep <= 1 && channels[i].freq <= channels[i].freqEnd)			//	�Ⴍ�Ȃ�ꍇ
					)
				{
					audioStop(i);
				}
			}
			alSourcef(										//	�s�b�`��ݒ�
				channels[i].sid,							//	sid
				AL_PITCH,									//	�p�����[�^�[(AL_PITCH : �s�b�`)
				channels[i].freq);							//	���g�� / �f�t�H���g���g�� = �s�b�`
		}

	}

	ALenum error = alGetError();							//	�G���[�̎擾
	if (error != AL_NO_ERROR)								//	�G���[���������ꍇ(AL_NO_ERROR : �G���[���Ȃ�)
	{
		printf("%s\n", alGetString(error));
	}
}