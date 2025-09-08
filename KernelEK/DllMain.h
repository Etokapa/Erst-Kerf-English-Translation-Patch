#undef  UNICODE // Make sure Unicode is not defined
#undef _UNICODE
#undef  WINVER
#define WINVER         0x0400 // Both Windows 95/NT
#define _WIN32_WINNT   0x0400 // Windows NT 4.0 - Avoid new APIs to support Win9X platform
#define _WIN32_WINDOWS 0x0400 // Windows 95
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "erst_kerf.h"

HWND WINAPI MyCreateWindowExA(
  DWORD dwExStyle,      // extended window style
  LPCTSTR lpClassName,  // registered class name
  LPTSTR lpWindowName,  // window name
  DWORD dwStyle,        // window style
  int x,                // horizontal position of window
  int y,                // vertical position of window
  int nWidth,           // window width
  int nHeight,          // window height
  HWND hWndParent,      // handle to parent or owner window
  HMENU hMenu,          // menu handle or child identifier
  HINSTANCE hInstance,  // handle to application instance
  LPVOID lpParam        // window-creation data
)
{
	RECT rcRect;
// Just do a simple centering and change the caption title with build date for the main drawing window
	if ( GetWindowRect(GetDesktopWindow(), &rcRect) ) {
		x = rcRect.left + (rcRect.right-rcRect.left-nWidth)/2;
		y = rcRect.left + (rcRect.bottom-rcRect.top-nHeight)/2;
	}
// Might as well make it a TOPMOST window
	return CreateWindowExA(dwExStyle, lpClassName, NewWindowTitle, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}


extern LPVOID __fastcall HookAPI(PSTR thismodule, PSTR module, PSTR api, LPVOID hookproc) {
	PIMAGE_THUNK_DATA ptaddr, ptname;
	PIMAGE_IMPORT_DESCRIPTOR pidesc;
	PIMAGE_IMPORT_BY_NAME piname;
	PIMAGE_NT_HEADERS pnth;
	ULONG_PTR base, rva;
	DWORD dwOldProtect;
	PSTR impmodule;
	LPVOID org;

	base = (ULONG_PTR)GetModuleHandleA(thismodule);
	if ( !base )
		return 0;
	pnth = (PIMAGE_NT_HEADERS)(base + (ULONG_PTR)((PIMAGE_DOS_HEADER)base)->e_lfanew);
	if ( !pnth )
		return 0;
	rva = pnth->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	if ( !rva )
		return 0;
	pidesc = (PIMAGE_IMPORT_DESCRIPTOR)(base + rva);
	for (;;) {
		if ( !pidesc->FirstThunk )
			return 0;
		impmodule = (PSTR)(base + pidesc->Name);
		if ( !lstrcmpiA(module, impmodule) )
			break;
		pidesc++;
	}
	ptaddr = (PIMAGE_THUNK_DATA)(base + pidesc->FirstThunk);
	ptname = (PIMAGE_THUNK_DATA)(base + pidesc->OriginalFirstThunk);
	for (;;) {
		if ( !ptaddr->u1.Function )
			return 0;
		if ( !IMAGE_SNAP_BY_ORDINAL(ptname->u1.Ordinal) ) {
			piname = (PIMAGE_IMPORT_BY_NAME)(base + ptname->u1.AddressOfData);
			if ( !lstrcmpiA(api, (LPSTR)piname->Name) )
				break;
		}
		ptaddr++;
		ptname++;
	}
	org = (LPVOID)((ULONG_PTR)ptaddr->u1.Function);
	if ( org == hookproc )
		return 0;
	if ( !VirtualProtect(&ptaddr->u1.Function, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect) )
		return 0;
	#pragma warning(disable:4244)
	ptaddr->u1.Function = (ULONG_PTR)hookproc;
	#pragma warning(default:4244)
	VirtualProtect(&ptaddr->u1.Function, 4, dwOldProtect, &dwOldProtect);
	FlushInstructionCache(GetCurrentProcess(), NULL, NULL);
	return org;
}