#include "DirectSoundBasic.h"

#include <iostream>

#include <QtGlobal>

///////////////////////////////////////////////////////////////////////

std::vector<std::wstring>	DirectSoundBasic::audioDeviceGuids_;
std::vector<std::wstring>	DirectSoundBasic::deviceDescriptions_;
std::vector<std::wstring>	DirectSoundBasic::deviceDriverModules_;
std::vector<DSCAPS>			DirectSoundBasic::deviceCapabilities_;
std::vector<DWORD>			DirectSoundBasic::deviceConfigurations_;
std::vector<bool>			DirectSoundBasic::deviceCertifiedFlags_;

////////////////////////////////////////////////////////////////////////

DirectSoundBasic::DirectSoundBasic()
{
	if (DirectSoundEnumerateW(enumerateCallback, nullptr) != DS_OK) {
		std::wcout << L"[error] DirectSoundEnumerateW call error!";

		audioDeviceGuids_.clear();
		deviceDescriptions_.clear();
		deviceDriverModules_.clear();
		deviceCapabilities_.clear();
		deviceConfigurations_.clear();
	}
}

BOOL CALLBACK DirectSoundBasic::enumerateCallback(LPGUID guid,
                                                  LPCWSTR deviceDescription,
                                                  LPCWSTR deviceDriverModule,
                                                  LPVOID context)
{
    Q_UNUSED(context);

	//	if primary device, skip it
	if (guid == nullptr)		return TRUE;

	IDirectSound8* directSound8;
	if (DirectSoundCreate8(guid, &directSound8, NULL) != DS_OK) {
		std::wcout << L"[error] DirectSoundCreate8 call error!";
		return TRUE;	//	if error, skip this device
	}

	//	here we use DSSCL_NORMAL, so the window which DS used to detect input focus change
	//	is not mattered
	if (directSound8->SetCooperativeLevel(GetDesktopWindow(), DSSCL_NORMAL) != DS_OK) {
		std::wcout << L"[error] SetCooperativeLevel call error!";
		return TRUE;
	}

	DSCAPS deviceCapability = { sizeof(deviceCapability) };
	if (directSound8->GetCaps(&deviceCapability) != DS_OK) {
		std::wcout << L"[error] GetCaps call error!";
		return TRUE;
	}

	DWORD deviceSpeakerConfiguration;
	if (directSound8->GetSpeakerConfig(&deviceSpeakerConfiguration) != DS_OK) {
		std::wcout << L"[error] GetSpeakerConfig call error!";
		return TRUE;
	}

	DWORD deviceCertified;
	if (directSound8->VerifyCertification(&deviceCertified) != DS_OK) {
		std::wcout << L"[error] VerifyCertification call error!";
		return TRUE;
	}

	//	store infomation as an atom operation
	LPOLESTR oleStr;
	if (StringFromCLSID(*guid, &oleStr) != DS_OK) {
		std::wcout << L"[error] StringFromCLSID call error!";
		return TRUE;
	}
	audioDeviceGuids_.emplace_back(oleStr);
	CoTaskMemFree(oleStr);

	deviceDescriptions_.emplace_back(deviceDescription);
	deviceDriverModules_.emplace_back(deviceDriverModule);

	deviceCapabilities_.emplace_back(deviceCapability);
	deviceConfigurations_.emplace_back(deviceSpeakerConfiguration);
	deviceCertifiedFlags_.push_back(deviceCertified == DS_CERTIFIED);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

unsigned DirectSoundBasic::deviceCount() const
{
	return audioDeviceGuids_.size();
}

std::wstring DirectSoundBasic::deviceGuid(unsigned index) const
{
	checkIndexRange(index);
	return audioDeviceGuids_[index];
}

std::wstring DirectSoundBasic::deviceDescription(unsigned index) const
{	
	checkIndexRange(index);
	return deviceDescriptions_[index];
}

std::wstring DirectSoundBasic::deviceDriverModule(unsigned index) const
{
	checkIndexRange(index);
	return deviceDriverModules_[index];
}

DSCAPS DirectSoundBasic::deviceCapabilities(unsigned index) const
{
	checkIndexRange(index);
	return deviceCapabilities_[index];
}

DWORD DirectSoundBasic::deviceConfiguration(unsigned index) const
{
	checkIndexRange(index);
	return deviceConfigurations_[index];
}

bool DirectSoundBasic::deviceCertifiedByDirectX(unsigned index) const
{
	checkIndexRange(index);
	return deviceCertifiedFlags_[index];
}

inline void DirectSoundBasic::checkIndexRange(unsigned index) const
{
	if (index >= deviceCount())
		throw std::exception("index range error");
}
