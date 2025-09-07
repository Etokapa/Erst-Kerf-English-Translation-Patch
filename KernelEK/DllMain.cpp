//#define __DEBUG
#include "DllMain.h"
// By: Nicolas Livaditis (NightWolve)
// For Erst Kerf localization project

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, PVOID lpReserved) {
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			HookAPI(NULL, "USER32.dll", "CreateWindowExA", &MyCreateWindowExA);		
			break;

		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

// Memory Bug Fix!!!!!
// The game expects pretty close to the exact Japanese string sizes
// so making them longer without adjusting hardcoded size values will
// cause crashes when it copies them to new buffers using that old size!
// Fix: Just return larger >256 buffers for any little string upon request!
// If the game code requested only 16-64 bytes, return 384, no big deal!!

LPVOID WINAPI MyHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {
	if ( dwBytes < 256 )
		dwBytes = 384;
	return HeapAlloc(hHeap, dwFlags, dwBytes);
}

// This is our intercepted version of lstrcpyA() in kernel32
// It's done via the module def file under exports
// lstrcpyA=MylstrcpyA
// So any exe loaded with this DLL now calls MylstrcpyA()
// instead of the real lstrcpyA() so we can mod its behavior!

LPSTR WINAPI MylstrcpyA(LPSTR lpString1, LPSTR lpString2) {

// If item description detected, replace lpString2 address with that of longer version!
	if ( lpString2 )
		if ( lstrcmpiA(lpString2, "F: 0 M: 0 None.") == 0 ) {
			lpString2 = "F: 0 M: 0 None. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 1 M: 0 Rapid-fire shots.") == 0 ) {
			lpString2 = "F: 1 M: 0 Rapid-fire shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 2 M: 1 2x rapid-fire shots.") == 0 ) {
			lpString2 = "F: 2 M: 1 2x rapid-fire shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 4 M: 1 Fan pattern shots.") == 0 ) {
			lpString2 = "F: 4 M: 1 Fan pattern shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 3 M: 2 8-way homing shots.") == 0 ) {
			lpString2 = "F: 3 M: 2 8-way homing shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 4 M: 2 Hi-power, close range. Deflects.") == 0 ) {
			lpString2 = "F: 4 M: 2 Hi-power, close range. Deflects. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 4 M: 2 Powerful fast shots.") == 0 ) {
			lpString2 = "F: 4 M: 2 Powerful fast shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 5 M: 3 Explosive shots.") == 0 ) {
			lpString2 = "F: 5 M: 3 Explosive shots. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 2 M: 2 Slow piercing disks.") == 0 ) {
			lpString2 = "F: 2 M: 2 Slow piercing disks. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 6 M: 8 Hi-power shots that erase bullets.") == 0 ) {
			lpString2 = "F: 6 M: 8 Hi-power shots that erase bullets. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 5 M: 5 Stationary Mine.") == 0 ) {
			lpString2 = "F: 5 M: 5 Stationary Mine. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 3 M: 3 Generates a shockwave.") == 0 ) {
			lpString2 = "F: 3 M: 3 Generates a shockwave. Extended String working!";
		} else if ( lstrcmpiA(lpString2, "F: 3 M: 3 Ricochet shots.") == 0 ) {
			lpString2 = "F: 3 M: 3 Ricochet shots.";
		} else if ( lstrcmpiA(lpString2, "F: 6 M: 6 High speed dash. Clears gaps.") == 0 ) {
			lpString2 = "F: 6 M: 6 High speed dash. Clears gaps.";
		} else if ( lstrcmpiA(lpString2, "F: 7 M: 5 Deploys Auto-Turrent.") == 0 ) {
			lpString2 = "F: 7 M: 5 Deploys Auto-Turrent.";
		} else if ( lstrcmpiA(lpString2, "F: 5 M: 5 Stone Pillar Shield.") == 0 ) {
			lpString2 = "F: 5 M: 5 Stone Pillar Shield.";
		} else if ( lstrcmpiA(lpString2, "F: 3 M: 3 Creates a barrier.") == 0 ) {
			lpString2 = "F: 3 M: 3 Creates a barrier.";
		} else if ( lstrcmpiA(lpString2, "F: 0 M: 3 More Attack. Less Defense.") == 0 ) {
			lpString2 = "F: 0 M: 3 More Attack. Less Defense.";
		} else if ( lstrcmpiA(lpString2, "F: 9 M: 9 Restore HP.") == 0 ) {
			lpString2 = "F: 9 M: 9 Restore HP.";
		} else if ( lstrcmpiA(lpString2, "F: 9 M: 0 Restore MP.") == 0 ) {
			lpString2 = "F: 9 M: 0 Restore MP.";
		}

// We still call the real function at the end so everything else continues normally
	return lstrcpyA(lpString1, lpString2);
}