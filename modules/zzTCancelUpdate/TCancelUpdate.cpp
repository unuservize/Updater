//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TCancelUpdate.h"
#include "Unit1.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TCancelUpdate::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TCancelUpdate::TCancelUpdate(bool CreateSuspended)
        : TThread(CreateSuspended)
{
 typedef unsigned char BYTE;
 CoInitialize(NULL);
  Log = new TLogClass("TCancelUpdate");
  if (Log)
        Log->Write("TCancelUpdate start ...");
 // UpTh = new UpdateThread(true);
  file = NULL;
  file2 = NULL;
  sPath = NULL;
  //BackUpDir = "";
  UpdateProcessId=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TCancelUpdate::Execute()
{
CoInitialize(NULL);
Application->ProcessMessages();
GetCurrentDirectory(MAX_PATH, BackUpDir);
Log->Write(BackUpDir);
GetCancleFileList(BackUpDir, sExt, sEXT);
BackUpDir[strlen(BackUpDir)-strlen(strrchr(BackUpDir,'\\'))]='\0';
Log->Write("Finishing BackUp operations . . . ");
ChDir("C:\\WebClient\\");
StartProgram("C:\\WebClient\\DPSKiosk.exe");
Sleep(5000);
Synchronize(Terminate);
}
//---------------------------------------------------------------------------


bool TCancelUpdate::StartProgram(AnsiString ApplicationName)
{
try
    {
    Log->Write("Starting "+ApplicationName+"...");
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.lpDesktop="WinSta0\\Default";

    DWORD lpExitCode;
    if (!CreateProcess((LPCTSTR)ApplicationName.c_str(), NULL,
            NULL,             // Process handle not inheritable.
            NULL,             // Thread handle not inheritable.
            true,            // Set handle inheritance to FALSE.
            0,//CREATE_NEW_CONSOLE|CREATE_DEFAULT_ERROR_MODE|CREATE_NEW_PROCESS_GROUP,                // No creation flags.
            NULL,             // Use parent's environment block.
            NULL,             // Use parent's starting directory.
            &si,              // Pointer to STARTUPINFO structure.
            &pi))
      {
      Log->Write("Starting program error");
      return false;
      }
      else
      {
      Log->Append("OK.");
      UpdateHandle=pi.hProcess;
      UpdateProcessId=pi.dwProcessId;
      return true;
      }
   }
catch(Exception& error)
   {
   Log->Write("StartProgram() Exception :" + AnsiString(error.Message));
   return false;
   }
}

//-------------------------------------------------------------------------------------

void TCancelUpdate::StopProc(char *exeName)
{

  HANDLE lSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

  PROCESSENTRY32 uProcess;
  uProcess.dwSize=sizeof(uProcess);

  BOOL rProcessFound=Process32First(lSnapshot,&uProcess);
  while ( rProcessFound ){
    if ( strstr( uProcess.szExeFile, exeName ) != NULL ){
      HANDLE myproc = OpenProcess( PROCESS_ALL_ACCESS, FALSE, uProcess.th32ProcessID );
      if ( myproc != NULL ){
        if ( TerminateProcess( myproc, NO_ERROR ) ){
          Log->Write("Terminate process "+AnsiString(exeName)+" success.");
        }
        else{
          Log->Write("Terminate process "+AnsiString(exeName)+" error:"+AnsiString(GetLastError()));
        }
      }
      CloseHandle(myproc);
    }
    rProcessFound = Process32Next( lSnapshot, &uProcess );
  }
  CloseHandle(lSnapshot);
}
//---------------------------------------------------------------------------

void TCancelUpdate::GetCancleFileList(LPTSTR sPath, LPTSTR sExt, LPTSTR sEXT)
{

 WIN32_FIND_DATA pFILEDATA;

 HANDLE hFile = FindFirstFile(strcat(sPath,"\\*.*"),&pFILEDATA);

 sPath[strlen(sPath) - strlen(strstr(sPath,"*.*"))] = '\0';
 if (hFile!=INVALID_HANDLE_VALUE)    {

  char * chBuf;
  do {
   if (strlen(pFILEDATA.cFileName) == 1 &&  strchr(pFILEDATA.cFileName,'.') !=NULL)
    if (FindNextFile(hFile,&pFILEDATA) == 0)
      break;
   if (strlen(pFILEDATA.cFileName) == 2 && strstr(pFILEDATA.cFileName,"..") !=NULL)
    if(FindNextFile(hFile,&pFILEDATA) == 0)
      break;
     //���� ����� ����������, ��������� � ��� ����������� �����
     if(pFILEDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
             {             //���� ����� �� �������� ����������� �� ������

                GetCancleFileList(strcat(sPath,pFILEDATA.cFileName), sExt, sEXT);
                delete sPath;
                delete pFILEDATA.cFileName;//Log->Write(AnsiString(sPath));



        //else Log->Write("================= "+AnsiString(sPath)+" execution ============================= ");
               // GetFileList(strcat(sPath,pFILEDATA.cFileName), sExt, sEXT);

//�������������� ����� ����
   // sPath[strlen(sPath) - strlen(pFILEDATA.cFileName)-1] = '\0';
  //  Application->ProcessMessages();
    }

   else {


       //��������� �� ����������� sExt ���������� pFILEDATA.cFileName
       //Log->Write(AnsiString(sPath)+AnsiString(pFILEDATA.cFileName));
       char sCancelPath[MAX_PATH]="C:\\WebClient";
       Log->Write("sPath: " + AnsiString(sPath));
       //Log->Write("BackUpDir: " + AnsiString(BackUpDir));
       string sPathErased=sPath;
       char *sBackUpErase = "C:\\DPSUpdate\\BackUp\\";
       sPathErased.erase(0,strlen(sBackUpErase));
       char *mychar = new char[sPathErased.length()+1];
       strcpy(mychar, strstr(sPathErased.c_str(),"\\"));
       strcat(sCancelPath,mychar);
       Log->Write("sPathErased:" + AnsiString(sCancelPath));
       strcat(sPath, pFILEDATA.cFileName);
       strcat(sCancelPath, pFILEDATA.cFileName);
       Log->Write("Copy: "+AnsiString(sPath));
       Log->Write("Send to: "+AnsiString(sCancelPath));
       CopyFile(sPath,sCancelPath,false);
      // Log->Write("Send to: "+AnsiString(sCancelPath));
       sPath[strlen(sPath) - strlen(pFILEDATA.cFileName)] = '\0';
   }
  }
  while (FindNextFile(hFile,&pFILEDATA));
 }
}
