#ifndef WAVEXCEPTION_H
#define	WAVEXCEPTION_H

#include <Windows.h>

#include <exception>
#include <string>

class WavException : public std::exception
{
public:
	WavException(const char* errorStr, HRESULT callResult) 
		: std::exception()
		, m_errorStr(errorStr)
		, m_callResult(callResult)
	{}

	virtual const char* what()
	{
		return (std::string(m_errorStr) + std::to_string(m_callResult)).c_str();
	}

private:
	const char*		m_errorStr;
	HRESULT			m_callResult;
};

#endif