#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include "WavFile.h"

#include <string>

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

class WavPlayer
{
public:
    WavPlayer();
    ~WavPlayer();

    void setFile(std::wstring filepath, HWND windowHandle);

    inline bool     isPlaying() const { return m_isPlaying; }
    inline unsigned getAudioTotalTime() const { return m_wavFile.getAudioTime(); }

    void play		();
    void stop		();
    void resume		();
	void playFrom	(unsigned seconds);

    void setAudioEndsNotify(IAudioEndNotify* outerNotify);

private:
    void openDefaultAudioDevice(HWND windowHandle);
    void createBufferOfSeconds(unsigned seconds);
    void fillDataIntoBuffer();
	void startDataFillingThread();
	void setNotifyEvent(HANDLE* notifyHandles, DWORD* bufferOffsets, unsigned num);
    void cleanResources();
	void sendProgressUpdatedSignal();
	void sendAudioEndsSignal();
	unsigned getBufferIndexFromNotifyIndex(unsigned notifyIndex);

    inline bool fileSet() const    { return m_directSound8 != nullptr; }

	static DWORD WINAPI dataFillingThread(LPVOID param);
	static bool	WINAPI	tryToFillNextBuffer(WavPlayer* wavPlayer, unsigned bufferSliceIndex);
	static void WINAPI	lockAndFillData(WavPlayer* wavPlayer, char* dataPtr, DWORD dataSizeInBytes, unsigned bufferSliceIndex);
	

private:
    WavFile         m_wavFile;

	unsigned				m_bufferSliceCount;
    IDirectSound8*          m_directSound8;
    IDirectSoundBuffer8*    m_soundBufferInterface;

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

	static const unsigned m_prefilledBufferSliceCount = 1;
};

//	notify map to data fill notify

#endif // WAVPLAYER_H
