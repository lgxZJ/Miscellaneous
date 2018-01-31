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
};

class WavPlayer
{
public:
    WavPlayer();
    ~WavPlayer();

    void setFile(std::wstring filepath, HWND windowHandle);

    inline bool isPlaying() const { return m_isPlaying; }

    void play();
    void stop();
    void resume();
	
	void setAudioEndsNotify(IAudioEndNotify* outerNotify);

private:
    void openDefaultAudioDevice(HWND windowHandle);
    void createBufferOfSeconds(unsigned seconds);
    void fillDataIntoBuffer();
	void startDataFillingThread();
	void setNotifyEvent(HANDLE* notifyHandles, DWORD* bufferOffsets, unsigned num);
    void cleanResources();

    inline bool fileSet() const    { return m_directSound8 != nullptr; }

	static DWORD WINAPI dataFillingThread(LPVOID param);
	static bool WINAPI waitToFillHalfBuffer(WavPlayer* wavPlayer, unsigned bufferSliceIndex);
	static void WINAPI lockAndFillData(WavPlayer* wavPlayer, char* dataPtr, DWORD dataSizeInBytes);

private:
    WavFile         m_wavFile;

	unsigned				m_bufferSliceCount;
    IDirectSound8*          m_directSound8;
    IDirectSoundBuffer8*    m_soundBufferInterface;

	bool			m_quitDataFillingThread;
    bool            m_isPlaying;
    unsigned        m_secondaryBufferSize;
    char*           m_nextDataToPlay;

	unsigned		m_notifyCount;
	HANDLE*			m_notifyHandles;
	DWORD*			m_notifyOffsets;

	bool			m_additionalEndNotify;
	HANDLE			m_endNotifyHandle;
	unsigned		m_endNotifyLoopCount;

	bool				m_dataFillingEnds;
	HANDLE				m_threadHandle;
	IAudioEndNotify*	m_outerNotify;
};

#endif // WAVPLAYER_H
