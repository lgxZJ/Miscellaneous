#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include "WavFile.h"

#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <memory>

#include <dsound.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
//	WARNING:
//		when the end notify position is the same same data filling notify position, the behaviour 
//	is not definded because i didn't test it through the code copes with this condition	!!!!
/////////////////////////////////////////////////////////////////////////////////////////////////

class IAudioEndNotify
{
public:
	virtual void wavPlayerAudioEnds() = 0;
    //  progress range is 0~totalTime
	virtual void wavPlayerProgressUpdated(unsigned currentPlayingTime) = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////

class EffectBase
{
public:
	virtual void usedForDynamic() {}
};

class EffectChorus : public EffectBase
{
public:
	enum LfoWaveform { Sin, Triangle };
    enum PhaseDifferential { Neg180, Neg90, Zero, Pos90, Pos180 };
	struct ValueRange
	{
		ValueRange(double min, double max)
			: m_min(min)
			, m_max(max)
		{}

		double m_min; double m_max;
	};

	using WaveformTypes = std::vector<LfoWaveform>;
	using PhaseTypes = std::vector<PhaseDifferential>;

	EffectChorus(void* unknown)
		: m_interfacePtr(reinterpret_cast<IDirectSoundFXChorus8*>(unknown))
	{
		assert( m_interfacePtr != nullptr );
        if (m_interfacePtr->GetAllParameters(&m_params) != DS_OK)
            throw std::exception("GetAllParameters");
	}
	
    /////////////////////////////////////////
    //	NOTE:	While change effects, the sound buffer must be stopped and not locked!!!
    /////////////////////////////////////////////////////////////////////////////////////
	static ValueRange		getWetDryRadioRange() 	{ return ValueRange( DSFXCHORUS_WETDRYMIX_MIN, DSFXCHORUS_WETDRYMIX_MAX ); }
	static ValueRange		getDepthRange() 		{ return ValueRange( DSFXCHORUS_DEPTH_MIN, DSFXCHORUS_DEPTH_MAX ); }
	static ValueRange		getFeedbackRange() 		{ return ValueRange( DSFXCHORUS_FEEDBACK_MIN, DSFXCHORUS_FEEDBACK_MAX ); }
	static ValueRange		getFrequencyRange() 	{ return ValueRange( DSFXCHORUS_FREQUENCY_MIN, DSFXCHORUS_FREQUENCY_MAX ); }
	static WaveformTypes	getWaveformTypes() 		{ return { Sin, Triangle }; }
	static ValueRange		getDelayRange() 		{ return ValueRange( DSFXCHORUS_DELAY_MIN, DSFXCHORUS_DELAY_MAX ); }
    static PhaseTypes		getPhaseTypes() 		{ return { Neg180, Neg90, Zero, Pos90, Pos180 }; }

	const DSFXChorus& getParams() const		{ return m_params; }

	void setParams( const DSFXChorus& params)
	{
		m_params = params;
		if (m_interfacePtr->SetAllParameters( &m_params ) != DS_OK)
			throw std::exception( "SetAllParameters" );
	}

private:
	IDirectSoundFXChorus8*	m_interfacePtr;
	DSFXChorus				m_params;
};

//////////////////////////////////////////////////////////////////////////////////////////

class WavPlayer
{
public:
	enum WavEffect {
		Chorus = 0, 
		Compression, 
		Distortion, 
		Echo, 
		EnvironmentReverberation, 
		Flange, 
		Gargle, 
		ParametricEqualizer, 
		WavesReverberation 
	};

    WavPlayer();
    ~WavPlayer();

	void setFile(std::wstring filepath, HWND windowHandle);
	void setAudioEndsNotify(IAudioEndNotify* outerNotify);

	/////////////////////////////////////////////////////////////////////////////
	//	WARNING:
	//		The all following functions must be called after `setFile()`.
	/////////////////////////////////////////////////////////////////////////////

    inline bool     isPlaying() const { return m_isPlaying; }
    inline unsigned getAudioTotalTime() const { return m_wavFile.getAudioTime(); }

    void play		();
    void stop		();
    void resume		();
	void playFrom	(unsigned seconds);

    bool supportsEffect3D();
    void enableEffect3D(bool ifEnable);
    IDirectSound3DBuffer*		get3DSource()   { return m_3dSourceInterface; }
    IDirectSound3DListener8*	get3DListener() { return m_3dListenerInterface; }

    uint32_t getFrequencyMin();
    uint32_t getFrequencyMax();
    uint32_t getFrequency();
    void     setFrequency(uint32_t frequency);
    //	channel value range from -100(left) to 100(right)
    long     getChannel();
    void     setChannel(long channel);
	//	volume value range from 0 to 100
    long     getVolume();
    void     setVolume(long volume);

	//////////////////////////////////////////////////////////////////////
	//	WARNING:
	//		The following 9 functions must be called when one file is set
	//		and audio is not playing.
	//////////////////////////////////////////////////////////////////////
	void		addEffectOfType(WavEffect effect);
	void		applyEffects();
	EffectBase&	getEffectController(WavEffect effect);
	
private:
	enum CleanOption { CleanNoWav, CleanAll };

    void openDefaultAudioDevice(HWND windowHandle);
    void createBufferOfSeconds(unsigned seconds);
    void prefillDataIntoBuffer();
    void preparePlayResource();
	void startDataFillingThread(char* startDataPtr);
	void setNotifyEvent(HANDLE* notifyHandles, DWORD* bufferOffsets, unsigned num);
    void cleanResources(CleanOption option);
	void sendProgressUpdatedSignal();
	void sendAudioEndsSignal();
	unsigned getBufferIndexFromNotifyIndex(unsigned notifyIndex);
	void addEffectToAudio(GUID effectGuid);
	GUID mapEffectTypeToGuid(WavEffect effectType);
	void constructEffectObject( const GUID& guid, unsigned guidIndex );

    inline bool fileSet() const    { return m_directSound8 != nullptr; }

	static DWORD WINAPI dataFillingThread(LPVOID param);
	static bool	WINAPI	tryToFillNextBuffer(WavPlayer* wavPlayer, unsigned bufferSliceIndex);
	static void WINAPI	lockAndFillData(WavPlayer* wavPlayer, char* dataPtr, DWORD dataSizeInBytes, unsigned bufferSliceIndex);
	

private:
    WavFile         m_wavFile;

	HWND					m_windowHandle;
	unsigned				m_bufferSliceCount;
    IDirectSound8*          m_directSound8;
    IDirectSoundBuffer8*    m_soundBufferInterface;
    IDirectSound3DBuffer8*  m_3dSourceInterface;
    IDirectSound3DListener8* m_3dListenerInterface;

	bool			m_quitDataFillingThread;
    bool            m_isPlaying;
    unsigned        m_secondaryBufferSize;
    char*           m_nextDataToPlay;
	unsigned		m_currentPlayingTime;

	//	the notifies include:
	//		1) ends notify, 
	//		2) data filling notify, 
	//		3) exit notify
	unsigned		m_notifyCount;
	HANDLE*			m_notifyHandles;
	DWORD*			m_notifyOffsets;

	bool			m_additionalEndNotify;
	HANDLE			m_endNotifyHandle;
	unsigned		m_endNotifyLoopCount;
	unsigned		m_additionalNotifyIndex;

	bool				m_dataFillingEnds;
	HANDLE				m_threadHandle;
	IAudioEndNotify*	m_outerNotify;

	///////////////////////////////////////////////////////////////////////////////////////
	//	NOTE:
	//		Here we can use a vector to simulate an array(WinApi use arrays) because 
	//		c++11 standard says the element memory inside vector is allocated contiguously.
	std::vector<DSEFFECTDESC>				m_effects;
	std::vector<std::auto_ptr<EffectBase>>	m_effectControllers;

	static const unsigned s_prefilledBufferSliceCount = 1;
    static const unsigned s_secondsInBuffer = 4;
	static const long	  s_channelPercentageUnit = (DSBPAN_RIGHT - DSBPAN_LEFT) / 200;
	static const long	  s_volumeUnit = (DSBVOLUME_MAX - DSBVOLUME_MIN) / 100;
};

//	notify map to data fill notify

#endif // WAVPLAYER_H
