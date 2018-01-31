#include "WavFile.h"

#include <Windows.h>

#include <exception>

#include <QDebug>

WavFile::WavFile()
    : m_mmioHandle(NULL)
	, m_data(nullptr)
	, m_waveFormat()	//	default member bitset to 0
{
}

WavFile::~WavFile()
{
    clean();
}

void WavFile::clean()
{
    if (m_mmioHandle != NULL) {
        mmioClose(m_mmioHandle, 0);
        m_mmioHandle = NULL;
    }
    if (m_data != nullptr) {
        free(m_data);
		m_data = nullptr;
    }
}

void WavFile::open(std::wstring filePath)
{
    Q_ASSERT (!opened());

    //  convert to wchar since mmioOpen only accept non-const wchar_t*
    wchar_t tempFilePath[128] = {0};
    CopyMemory(tempFilePath,
               filePath.data(),
               sizeof(wchar_t) * filePath.size());

    HMMIO mmioHandle = mmioOpenW(tempFilePath, NULL,
                                MMIO_READ |
                                MMIO_DENYWRITE |
                                MMIO_ALLOCBUF);
    if (mmioHandle == NULL) {
        throw std::exception("mmioOpon error!");
    }

    MMCKINFO riffChunkInfo;
    riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    DWORD as;
    if ((as = mmioDescend(mmioHandle, &riffChunkInfo, NULL, MMIO_FINDRIFF))
            !=
        MMSYSERR_NOERROR) {

        throw std::exception("mmioDescend error!");
    }

    MMCKINFO fmtSubChunkInfo;
    fmtSubChunkInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(mmioHandle, &fmtSubChunkInfo, &riffChunkInfo, MMIO_FINDCHUNK)
            !=
        MMSYSERR_NOERROR) {

        throw std::exception("mmioDescend error!");
    }

    //  `fmt` chunk contains wave audio format described by WAVEFORMAT or WAVEFORMATEX
    if (mmioRead(mmioHandle, (HPSTR)&m_waveFormat, fmtSubChunkInfo.cksize)
            <= 0) {

        throw std::exception("mmioRead read fmt chunk error!");
    }
    if (mmioAscend(mmioHandle, &fmtSubChunkInfo, 0) != MMSYSERR_NOERROR) {
        throw std::exception("mmioAscend error, cannot ascend out of `fmt` chunk");
    }

    //  in a valid `wav` file, after calling mmioDescend, the file position should be
    //  at the beginning of `data` chunk. however, we should never rely on this.
    MMCKINFO dataSubChunkInfo;
    dataSubChunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(mmioHandle, &dataSubChunkInfo, &riffChunkInfo, MMIO_FINDCHUNK)
            !=
        MMSYSERR_NOERROR) {

        throw std::exception("cannot descend to `data` chunk!");
    }

    m_dataSize=  dataSubChunkInfo.cksize;
    m_data = malloc(m_dataSize);
    if (m_data == nullptr) {
        throw std::exception("cannot malloc data!");
    }

    if (mmioRead(mmioHandle, (HPSTR)m_data, m_dataSize) <= 0 ) {
        throw std::exception("cannot read data!");
    }
}
