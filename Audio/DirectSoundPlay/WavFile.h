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

    WAVEFORMATEX&	getWaveFormat()         { return m_waveFormat; }
    void*           getAudioData() const    { return m_data; }
	DWORD			getAudioSize() const	{ return m_dataSize; }
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
