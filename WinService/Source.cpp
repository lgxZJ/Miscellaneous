#include <Windows.h>

#include <iostream>

using namespace std;

#define SAFE_CALL(FuncCall, ErrorCode)		                        \
	if (FuncCall == ErrorCode) {			                        \
		cout << #FuncCall " error, code:" << GetLastError()         \
             << " ,line:" << __LINE__ << "\n"; 		                \
		exit(-1);							                        \
	}

wstring getExeFullFilename()
{
	static wchar_t buffer[1024];

	SAFE_CALL(GetModuleFileNameW(NULL, buffer, 1024), 0);
	return wstring(buffer);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void installService()
{
	auto scmHandle = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	SAFE_CALL(scmHandle, NULL);

	auto serviceHandle = CreateServiceW(scmHandle,
										L"lgxZJ::Service",
										L"lgxZJ::Service", 
										SERVICE_ALL_ACCESS,
										SERVICE_WIN32_OWN_PROCESS,
										SERVICE_AUTO_START,
										SERVICE_ERROR_NORMAL,
										getExeFullFilename().c_str(),
										NULL, NULL, L"", NULL, L"");
	SAFE_CALL(serviceHandle, NULL);

	CloseServiceHandle(scmHandle);
	CloseServiceHandle(serviceHandle);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void uninstallService()
{
	auto scmHandle = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SAFE_CALL(scmHandle, NULL);

	auto serviceHandle = OpenServiceW(	scmHandle,
										L"lgxZJ::Service",
										SERVICE_ALL_ACCESS);
	SAFE_CALL(serviceHandle, NULL);

	SERVICE_STATUS serviceStatus;
	SAFE_CALL(QueryServiceStatus(serviceHandle, &serviceStatus), 0);
	if (serviceStatus.dwCurrentState == SERVICE_RUNNING) {
		SAFE_CALL(ControlService(serviceHandle, SERVICE_CONTROL_STOP, &serviceStatus), 0);
		SAFE_CALL(serviceStatus.dwCurrentState, NO_ERROR);

		do {
			SAFE_CALL(QueryServiceStatus(serviceHandle, &serviceStatus), 0);
            Sleep(1000);
		} while (serviceStatus.dwCurrentState != SERVICE_STOPPED);
	}

	SAFE_CALL(DeleteService(serviceHandle), FALSE);

	CloseServiceHandle(scmHandle);
	CloseServiceHandle(serviceHandle);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void startService()
{
	auto scmHandle = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SAFE_CALL(scmHandle, NULL);

	auto serviceHandle = OpenServiceW(	scmHandle,
										L"lgxZJ::Service",
										SERVICE_ALL_ACCESS);
	SAFE_CALL(serviceHandle, NULL);

	SERVICE_STATUS serviceStatus;
	SAFE_CALL(QueryServiceStatus(serviceHandle, &serviceStatus), 0);
	if (serviceStatus.dwCurrentState == SERVICE_START && 
		serviceStatus.dwCurrentState != SERVICE_START_PENDING)
		return;

	SAFE_CALL(StartServiceW(serviceHandle, 0, NULL), FALSE);

	CloseServiceHandle(scmHandle);
	CloseServiceHandle(serviceHandle);
}

SERVICE_STATUS_HANDLE g_serviceStatusHandle = NULL;

void setServiceStatus(DWORD status)
{
     SERVICE_STATUS serviceStatus;
    serviceStatus.dwServiceType              = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwWin32ExitCode            = NO_ERROR;
    serviceStatus.dwServiceSpecificExitCode  = 0;
    serviceStatus.dwWaitHint                 = 2000;
    serviceStatus.dwCheckPoint               = 0;
    serviceStatus.dwControlsAccepted         =  SERVICE_ACCEPT_PAUSE_CONTINUE |
                                                SERVICE_ACCEPT_SHUTDOWN |
                                                SERVICE_ACCEPT_STOP;

    serviceStatus.dwCurrentState = status;
    SAFE_CALL(SetServiceStatus(g_serviceStatusHandle, &serviceStatus), 0);
}

VOID WINAPI ServiceHandler(DWORD controlCode)
{
    switch (controlCode)
    {
        case SERVICE_CONTROL_CONTINUE:
            setServiceStatus(SERVICE_START_PENDING);    break;
        case SERVICE_CONTROL_INTERROGATE:
                                                        break;
        case SERVICE_CONTROL_PAUSE:
            setServiceStatus(SERVICE_PAUSED);           break;
        case SERVICE_CONTROL_SHUTDOWN:
            setServiceStatus(SERVICE_STOPPED);          break;
        case SERVICE_CONTROL_STOP:
            setServiceStatus(SERVICE_STOPPED);          break;
        default:
            break;
    }
}

VOID WINAPI ServiceMain(DWORD argc, LPWSTR *argv)
{
    g_serviceStatusHandle = RegisterServiceCtrlHandlerW(L"lgxZJ::Service", &ServiceHandler);
    if (g_serviceStatusHandle == 0)
    {
        cout << "RegisterServiceCtrlHandlerW error, code:" << GetLastError()
            << " ,line:" << __LINE__ << "\n";
        exit(-1);
    }

    setServiceStatus(SERVICE_START_PENDING);
    setServiceStatus(SERVICE_RUNNING);
}

void runService()
{
    const SERVICE_TABLE_ENTRYW serviceTable[] = {
        { L"", ServiceMain },
        { NULL, NULL }
    };

    SAFE_CALL(StartServiceCtrlDispatcherW(&serviceTable[0]), 0);
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc == 1)                              runService();
    else if (argc == 2) {
        if (argv[1] == wstring(L"-install"))    installService(), startService(); 
	    if (argv[1] == wstring(L"-uninstall"))  uninstallService();
    } 
    else                                        std::cout << "usage: a.exe [-install/-uninstall]";

	return 0;
}
