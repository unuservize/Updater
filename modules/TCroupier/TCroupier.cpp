//---------------------------------------------------------------------------


#pragma hdrstop

#include "TCroupier.h"

//---------------------------------------------------------------------------

TCroupier::TCroupier()
{
    Log = new TLogClass("TCroupier");
    if (Log){
     Log->Write("TCroupier started ...");
    }
}

TCroupier::~TCroupier(){};


//����� ��� �������� �����
void TCroupier::RestoreFolders(AnsiString FS)
{
Log->Write("RestoreFolders started ...");
char *FolderStruct = FS.c_str();
if (DirectoryExists(AnsiString(FolderStruct)))
     {
        Log->Write("Struct is correct");
     }
else if ( (!DirectoryExists(AnsiString(FolderStruct))) && (FolderStruct!="C:\\") )
     {
                 if ( CreateDirectory(FolderStruct,NULL) ){
                      Log->Write(AnsiString(FolderStruct)+ " - CREATED");
                 }
                 else{
                           AnsiString ToCreate = FolderStruct;
                           FolderStruct[strlen(FolderStruct)-strlen(strrchr(FolderStruct,'\\'))]='\0';
                           Log->Write("Start cheking: "+AnsiString(AnsiString(FolderStruct)));
                           RestoreFolders(FolderStruct);
                           Log->Write(AnsiString(FolderStruct)+" - CREATED");
                           Log->Write("Remember ToCreate: " + ToCreate);
                           CreateDirectory(ToCreate.c_str(),NULL);
                           }
     }
else {
        Log->Write("Error on checking structure directory: "+AnsiString(FolderStruct));
     }
}

//����� ��������� ��� ����� � �������� �� ����� ���������� � ������
void TCroupier::FileDistribution(LPTSTR sPath) {

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
   if(pFILEDATA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
     FileDistribution(strcat(sPath,pFILEDATA.cFileName));
     sPath[strlen(sPath) - strlen(pFILEDATA.cFileName)-1] = '\0';
     Application->ProcessMessages();
     }

   else{
      //��������� �� ����������� sExt ���������� pFILEDATA.cFileName
       char sDistrPath[MAX_PATH]="C:\\WebClient";            //���������� ���������� ��� "�������" ������ (����������-����������)
       Log->Write("sPath: " + AnsiString(sPath));            //������� ���� � �������� �����(�����)
       string sPathErased=sPath;
       char *sBackUpErase = "C:\\DPSUpdate\\Webclient\\";    //������� �������, ����� �� ����� �������� ��� ����������� �����
       sPathErased.erase(0,strlen(sBackUpErase)-1);
       char *mychar = new char[sPathErased.length()+1];
       strcpy(mychar, sPathErased.c_str());
       strcat(sDistrPath,mychar);                          //��, ���� �� ����� ���������� ��� ����
       Log->Write("sPathErased:" + AnsiString(sDistrPath));
       AnsiString FS = sDistrPath;
       RestoreFolders(FS);                         //�������� ��������� ����������� �����, ���� ����� ����
       strcat(sPath, pFILEDATA.cFileName);
       strcat(sDistrPath, pFILEDATA.cFileName);
       Log->Write("Copy: "+AnsiString(sPath));
       Log->Write("Send to: "+AnsiString(sDistrPath));
       try{
          int CopyOK = CopyFile(sPath,sDistrPath,false);                       //�������� � �����������, ���� �����������
          if (CopyOK == 0){
                int Error = GetLastError();
                Log->Write("CopyFile error: " + AnsiString(Error));
          }

       }
       catch (Exception &ex)
          {
           Log->Write("CopyFile error exception: " + AnsiString(ex.Message));
          }
       sPath[strlen(sPath) - strlen(pFILEDATA.cFileName)] = '\0';
   }
  }
  while (FindNextFile(hFile,&pFILEDATA));
 }
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
