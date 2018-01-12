#ifndef DIRECTSOUND_BASIC_H
#define DIRECTSOUND_BASIC_H

#include <dsound.h>

#include <vector>
#include <string>

class DirectSoundBasic
{
public:
	explicit DirectSoundBasic();

	unsigned deviceCount() const;

	std::wstring deviceGuid(unsigned index) const;
	std::wstring deviceDescription(unsigned index) const;
	std::wstring deviceDriverModule(unsigned index) const;

	DSCAPS deviceCapabilities(unsigned index) const;
	DWORD deviceConfiguration(unsigned index) const;
	bool deviceCertifiedByDirectX(unsigned index) const;

private:
	static BOOL CALLBACK enumerateCallback(LPGUID guid,
										   LPCWSTR deviceDescription,
										   LPCWSTR deviceDriverModule,
										   LPVOID context);

    void checkIndexRange(unsigned index) const;

private:
	static std::vector<std::wstring>	audioDeviceGuids_;
	static std::vector<std::wstring>	deviceDescriptions_;
	static std::vector<std::wstring>	deviceDriverModules_;

	static std::vector<DSCAPS>			deviceCapabilities_;
	static std::vector<DWORD>			deviceConfigurations_;
	static std::vector<bool>			deviceCertifiedFlags_;
};

#endif
