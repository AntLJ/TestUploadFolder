Option Strict Off
Option Explicit On
Module mdlDeclare
	Public Structure PROCESS_INFORMATION
		Dim hProcess As Integer
		Dim hThread As Integer
		Dim dwProcessId As Integer
		Dim dwThreadId As Integer
	End Structure
	
	Public Structure STARTUPINFO
		Dim cb As Integer
		Dim lpReserved As String
		Dim lpDesktop As String
		Dim lpTitle As String
		Dim dwX As Integer
		Dim dwY As Integer
		Dim dwXSize As Integer
		Dim dwYSize As Integer
		Dim dwXCountChars As Integer
		Dim dwYCountChars As Integer
		Dim dwFillAttribute As Integer
		Dim dwFlags As Integer
		Dim wShowWindow As Short
		Dim cbReserved2 As Short
		Dim lpReserved2 As Integer
		Dim hStdInput As Integer
		Dim hStdOutput As Integer
		Dim hStdError As Integer
	End Structure
	
	'UPGRADE_WARNING: 構造体 PROCESS_INFORMATION に、この Declare ステートメントの引数としてマーシャリング属性を渡す必要があります。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"' をクリックしてください。
	'UPGRADE_WARNING: 構造体 STARTUPINFO に、この Declare ステートメントの引数としてマーシャリング属性を渡す必要があります。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="C429C3A5-5D47-4CD9-8F51-74A1616405DC"' をクリックしてください。
	'UPGRADE_ISSUE: パラメータ 'As Any' の宣言はサポートされません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"' をクリックしてください。
	'UPGRADE_ISSUE: パラメータ 'As Any' の宣言はサポートされません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"' をクリックしてください。
	'UPGRADE_ISSUE: パラメータ 'As Any' の宣言はサポートされません。 詳細については、'ms-help://MS.VSCC.v90/dv_commoner/local/redirect.htm?keyword="FAE78A8D-8978-4FD4-8208-5B7324A8F795"' をクリックしてください。
    'Public Declare Function CreateProcess Lib "kernel32"  Alias "CreateProcessA"(ByVal lpApplicationName As String, ByVal lpCommandLine As String, ByRef lpProcessAttributes As Any, ByRef lpThreadAttributes As Any, ByVal bInheritHandles As Integer, ByVal dwCreationFlags As Integer, ByRef lpEnvironment As Any, ByVal lpCurrentDriectory As String, ByRef lpStartupInfo As STARTUPINFO, ByRef lpProcessInformation As PROCESS_INFORMATION) As Integer
	
    'Public Declare Function OpenProcess Lib "kernel32.dll" (ByVal dwAccess As Integer, ByVal fInherit As Short, ByVal hObject As Integer) As Integer
	
    'Public Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Integer, ByVal dwMilliseconds As Integer) As Integer
	
    'Public Declare Function TerminateProcess Lib "kernel32" (ByVal hProcess As Integer, ByVal uExitCode As Integer) As Integer
	
    'Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Integer) As Integer
	
	Public Const SYNCHRONIZE As Integer = 1048576
	Public Const NORMAL_PRIORITY_CLASS As Integer = &H20
	Public Const PROCESS_TERMINATE As Integer = &H1
	Public Const INFINITE As Integer = &HFFFF '  Infinite timeout
	Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Integer)
	
    Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
	
	Public Declare Function GetPrivateProfileInt Lib "kernel32"  Alias "GetPrivateProfileIntA"(ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal nDefault As Integer, ByVal lpFileName As String) As Integer
	
    Public Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpString As String, ByVal lpFileName As String) As Integer
	
	Public Declare Function MeshToLL Lib "MeshToLL.dll" (ByVal nMeshCode As Integer, ByVal nX As Integer, ByVal nY As Integer, ByRef dLon As Double, ByRef dLat As Double) As Short
End Module