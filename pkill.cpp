// pkill.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#pragma comment (lib, "Psapi.lib" )

void ErrorOut()
{
  DWORD Error= GetLastError();
  LPSTR messageBuffer = NULL;
  size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
  NULL, Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
  printf("%s", messageBuffer);
  LocalFree(messageBuffer);
}

_TCHAR* target = NULL;

void ps(DWORD ProcessID, HANDLE hProcess, LPCTSTR szFile)
{
  _tprintf( _T("%08X %s\n"), ProcessID, szFile );
}
void pgrep(DWORD ProcessID, HANDLE hProcess, LPCTSTR szFile)
{
  if( _tcscmp(target,szFile ) ) return;
  _tprintf( _T("%08X\n"), ProcessID );
}
void pkill(DWORD ProcessID, HANDLE hProcess, LPCTSTR szFile)
{
  if( _tcscmp(target,szFile ) ) return;
  if( ! TerminateProcess(hProcess, 0) ) ErrorOut();
}

enum mode { PS, PGREP, PKILL } _mode;
void (*_func[3])(DWORD , HANDLE , LPCTSTR ) = {
  ps, pgrep, pkill
};

int _tmain(int argc, _TCHAR* argv[])
{
  DWORD dwSize;
  DWORD ID[ 1024 ];
  HMODULE modules[ 1024 ] = { 0 };
  TCHAR   szFile[ 1024 ] = { 0 };
  int nID;
  int i;
  
  HANDLE hProcess;
  
  TCHAR* p = _tcsrchr(argv[0], '\\');
  if(p) p++;
  else p = argv[0];
  
  if( _tcsncmp(p,"ps", 2) == 0 ) _mode = PS;
  else if( _tcsncmp(p,"pgrep", 5) == 0 ) _mode = PGREP;
  else if( _tcsncmp(p,"pkill", 5) == 0 ) _mode = PKILL;

  target = argv[1];
  _func[PS] = ps;
  DWORD Error;
  _func[PGREP] = pgrep;
  _func[PKILL] = pkill;
  if (argc != 2 && _mode != PS ) return 0;
  
  EnumProcesses( ID, sizeof(ID), &dwSize );
  nID = dwSize/sizeof(DWORD);

  for (i = 0; i < nID; i++ ){
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ID[i]);
    if (! hProcess ) continue;
    if (! EnumProcessModules(hProcess, modules,sizeof(modules), &dwSize) ) continue;
    
    GetModuleFileNameEx( hProcess, modules[0], szFile, sizeof(szFile) );
    _func[_mode](ID[i], hProcess, szFile);
    
    CloseHandle( hProcess );
  }

return 0;
}
