#pragma once

#if _MSC_VER == 1700
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "rexp_vc11-ugd.lib")
		#endif // ifdef _UNICODE
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "rexp_vc11-u.lib")
		#endif // ifdef _UNICODE
	#endif // ifdef _DEBUG
#else
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "rexpmud.lib")
		#endif // ifdef _UNICODE
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "rexpmu.lib")
		#endif // ifdef _UNICODE
	#endif // ifdef _DEBUG
#endif 
