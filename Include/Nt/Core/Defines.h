#pragma once

#ifndef NT_DLL
#	define NT_API  __declspec(dllimport)
#else
#	define NT_API __declspec(dllexport)
#endif

#define SAFE_DELETE(p) do { if (*p) { delete(*p); (*p) = nullptr; } } while (false)
#define SAFE_RELEASE(p) do { if (*p) { (*p)->Release(); delete(*p); (*p) = nullptr; } } while (false)

#define PI 3.1415926535897932384636433832795
#define RAD (PI / 180.0)
#define PIf 3.1415926535897932384636433832795f
#define RADf (PIf / 180.f)

// ============================================================================
//	Defines
// ----------------------------------------------------------------------------
#define ERROR_MSGW(Msg, Caption) MessageBoxW(nullptr, Msg, Caption, MB_ICONERROR)
#define INFO_MSGW(Msg, Caption) MessageBoxW(nullptr, Msg, Caption, MB_ICONINFORMATION)
#define WARNING_MSGW(Msg, Caption) MessageBoxW(nullptr, Msg, Caption, MB_ICONWARNING)

#define ERROR_MSGA(Msg, Caption) MessageBoxA(nullptr, Msg, Caption, MB_ICONERROR)
#define INFO_MSGA(Msg, Caption) MessageBoxA(nullptr, Msg, Caption, MB_ICONINFORMATION)
#define WARNING_MSGA(Msg, Caption) MessageBoxA(nullptr, Msg, Caption, MB_ICONWARNING)


#define ErrorBoxW ERROR_MSGW
#define InfoBoxW INFO_MSGW
#define WarningBoxW WARNING_MSGW

#define ErrorBoxA ERROR_MSGA
#define InfoBoxA INFO_MSGA
#define WarningBoxA WARNING_MSGA

#ifdef UNICODE
#	define ERROR_MSG ERROR_MSGW
#	define INFO_MSG ERROR_MSGW
#	define WARNING_MSG ERROR_MSGW

#	define ErrorBox ErrorBoxW
#	define InfoBox InfoBoxW
#	define WarningBox WarningBoxW
#else
#	define ERROR_MSG ERROR_MSGA
#	define INFO_MSG ERROR_MSGA
#	define WARNING_MSG ERROR_MSGA

#	define ErrorBox ErrorBoxA
#	define InfoBox InfoBoxA
#	define WarningBox WarningBoxA
#endif

#ifndef DCX_USESTYLE
#	define DCX_USESTYLE 0x00010000
#endif