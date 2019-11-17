#include <stdio.h>
#include <alc.h>
#include <al.h>
#include <time.h>

#include "audio.h"

#pragma comment(lib,"OpenAL32.lib")			//	openal�̃��C�u�������C���|�[�g

#define DEFAULT_GAIN (.1f)	//	�f�t�H���g�̉���
#define DEFAULT_FREQ (440)	//	�f�t�H���g�̎��g��

static ALuint sid;							//	����ID
ALuint buffers[AUDIO_WAVEFORM_PULSE_MAX];	//	�g�`��ۑ�����o�b�t�@
static int waveform;

static unsigned int length;	//	���̒���
static unsigned int start;	//	������n�߂�����
static float gain;			//	���݂̉���
static float decay;			//	���̌�����
static float sweep;			//	���̃s�b�`�ω���

static float freqStart = DEFAULT_FREQ;	//	�Đ��J�n���̉��K
static float freq;						//	���݂̉��K
static float freqEnd;					//	�ڕW(�������)�̉��K




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
			size * 440);							//	���g��(440:���̉�)
	}
	//	=====================================


	//	==========	�O�p�g���쐬 ==========
	unsigned char triangle[] = {
		0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
	};

	alBufferData(								//	�O�p�g�f�[�^�̃Z�b�g
		buffers[AUDIO_WAVEFORM_TRIANGLE],		//	�g�`��ۑ�����o�b�t�@
		AL_FORMAT_MONO8,						//	�t�H�[�}�b�g
		triangle,								//	�g�`�f�[�^
		sizeof triangle,						//	�g�`�f�[�^�̃T�C�Y
		sizeof triangle * DEFAULT_FREQ);		//	���g��(���̍���)

	//	===================================

	alGenSources(		//	���̍Đ������邽�߂̃\�[�X���쐬
		1,				//	�\�[�X�̐�
		&sid);			//	�\�[�X��ID�A�h���X

	alSourcei(			//	���[�v��������
		sid,			//	��ō����sid��ݒ�
		AL_LOOPING,		//	�p�����[�^�[(AL_LOOPING : ���[�v���邩)
		true);			//	���[�v������

	alSourcef(			//	���ʂ𒲐�
		sid,			//	��ō����sid��ݒ�
		AL_GAIN,		//	�p�����[�^�[(AL_GAIN : ����)
		.1f);			//	0.1f


	return 0;
}

void audioWaveform(int _waveform)
{
	waveform = _waveform;
}

void audioLength(unsigned int _millis)
{
	length = _millis;
}

void audioDecay(float _decay)
{
	decay = _decay;
}

void audioSweep(float _sweep, float _freqEnd)
{
	sweep = _sweep;
	freqEnd = _freqEnd;
}

void audioFreq(float _freq)
{
	freqStart = _freq;
}

void audioPlay()
{
	alSourcef(							//	���ʂ𒲐�
		sid,							//	sid
		AL_GAIN,						//	�p�����[�^�[(AL_GAIN : ����)
		gain = DEFAULT_GAIN);			//	�f�t�H���g�l��ݒ肵��(0.1f)gain�ɂ��ŏ��̒l�Ƃ��ĕۑ�

	freq = freqStart;					//	�J�n���g����ݒ�

	alSourcef(							//	�s�b�`��ݒ�
		sid,							//	sid
		AL_PITCH,						//	�p�����[�^�[(AL_PITCH : �s�b�`)
		freq / DEFAULT_FREQ);			//	���g�� / �f�t�H���g���g�� = �s�b�`

	alSourcei(							//	�o�b�t�@�[���Z�b�g����
		sid,							//	sid
		AL_BUFFER,						//	�p�����[�^�[(AL_BUFFER : �o�b�t�@�[��ݒ肷��)
		buffers[waveform]);				//	waveform�̌��݂̔ԍ��̃p���X�g��ݒ�

	alSourcePlay(sid);					//	�Đ�
	start = clock();					//	�Đ�����������ۑ�
}

void audioStop()
{
	alSourceStop(sid);	//	��~
}

void audioUpdate()
{
	if ((length > 0) && (clock() - start >= length))
	{
		audioStop();
	}

	if ((decay != 00) && (decay < 1))
	{
		alSourcef(							//	���ʂ𒲐�
			sid,							//	��ō����sid��ݒ�
			AL_GAIN,						//	�p�����[�^�[(AL_GAIN : ����)
			gain *= decay);					//	���݂̉��ʂ��猸�������������l������
	}

	if (sweep != 0)
	{
		freq *= sweep;
		if (freqEnd != 0) {
			if (
				(sweep >= 1 && freq >= freqEnd) ||		//	�����Ȃ�ꍇ
				(sweep <= 1 && freq <= freqEnd)			//	�Ⴍ�Ȃ�ꍇ
				)
			{
				audioStop();
			}
		}
		alSourcef(							//	�s�b�`��ݒ�
			sid,							//	sid
			AL_PITCH,						//	�p�����[�^�[(AL_PITCH : �s�b�`)
			freq / DEFAULT_FREQ);			//	���g�� / �f�t�H���g���g�� = �s�b�`
	}
}