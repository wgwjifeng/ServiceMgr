## Windows 服务管理程序

工具主要使用了一系列简单的 Windows 服务管理的 API 对 Windows 服务进行控制。

采用 Visual Studio 2013 Update 5 编写。

### OpenSCManager

```
SC_HANDLE WINAPI OpenSCManager(
  _In_opt_ LPCTSTR lpMachineName,
  _In_opt_ LPCTSTR lpDatabaseName,
  _In_     DWORD   dwDesiredAccess
);
```

### EnumServiceHandle

```
BOOL WINAPI EnumServicesStatus(
  _In_        SC_HANDLE             hSCManager,
  _In_        DWORD                 dwServiceType,
  _In_        DWORD                 dwServiceState,
  _Out_opt_   LPENUM_SERVICE_STATUS lpServices,
  _In_        DWORD                 cbBufSize,
  _Out_       LPDWORD               pcbBytesNeeded,
  _Out_       LPDWORD               lpServicesReturned,
  _Inout_opt_ LPDWORD               lpResumeHandle
);
```

### OpenService

```
SC_HANDLE WINAPI OpenService(
  _In_ SC_HANDLE hSCManager,
  _In_ LPCTSTR   lpServiceName,
  _In_ DWORD     dwDesiredAccess
);
```

### ControlService

```
BOOL WINAPI ControlService(
  _In_  SC_HANDLE        hService,
  _In_  DWORD            dwControl,
  _Out_ LPSERVICE_STATUS lpServiceStatus
);
```

### CloseServiceHandle

```
BOOL WINAPI CloseServiceHandle(
  _In_ SC_HANDLE hSCObject
);
```