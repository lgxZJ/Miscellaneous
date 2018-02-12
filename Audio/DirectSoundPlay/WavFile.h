#ifndef WAVFILE_H
#define WAVFILE_H

#include <Windows.h>

#include <string>

class WavFile
{
public:
    WavFile();
    ~WavFile();

    void open(std::wstring filename);
    void clean();

	//	WARNING:
	//		These functions must be called after `open()`.
    WAVEFORMATEX&	getWaveFormat()         { return m_waveFormat; }
    void*           getAudioData() const    { return m_data; }
	DWORD			getAudioSize() const	{ return m_dataSize; }
	unsigned		getAudioTime() const	{ return m_dataSize / m_waveFormat.nAvgBytesPerSec; }
	char*			getDataEndPtr() const   { return (char*)m_data + m_dataSize; }

private:
    inline bool opened() const { return m_mmioHandle != NULL; }

private:
    HMMIO           m_mmioHandle;
    WAVEFORMATEX    m_waveFormat;
    DWORD           m_dataSize;
    void*           m_data;
};

#endif // WAVFILE_H
