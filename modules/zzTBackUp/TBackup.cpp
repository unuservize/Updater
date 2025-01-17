//---------------------------------------------------------------------------


#pragma hdrstop

#include "TBackup.h"
//---------------------------------------------------------------------------
TBackup::TBackup()
{
 GetCurrentDirectory(MAX_PATH, CurDir); //������������� ������� ����������
 Log = new TLogClass("TBackup");
 if (Log)
 {
        Log->Write("TBackup initialise ...");
 }
 Backup = AnsiString(CurDir)+"\\Backup";
 bu=0; //����� ��������
}

TBackup::~TBackup()
{}

//---------------------------------------------------------------------------
//         ������� ���������� ��� ��������� ����� ��������� ���������
//---------------------------------------------------------------------------
char *TBackup::BUDir()
{
if (!FileExists(Backup))
                {
                CreateDirectory(Backup.c_str(),NULL);
                }
        else
                {
                printf("Directory %s existed \n", Backup);
                }
    time_t timer = time(NULL);
    struct tm *now = localtime( & timer );
    AnsiString s = String(now->tm_mday)+String(now->tm_mon + 1)+String(now->tm_year+1900)+" "+String(now->tm_hour)+"-"+String(now->tm_min)+"-"+String(now->tm_sec);
        if (CreateDirectory((Backup+="\\"+s).c_str(),NULL))
                {
                  printf("%s - created \n",s);
            }
        else printf("%s - failed creation \n",s);
 return Backup.c_str();
}



void TBackup::BUCopyFile(char *FileToCopy,char *CurBackUp)
{
        char BUbuff [MAX_PATH];
        strcpy(BUbuff,FileToCopy);
        char BUbuff2 [MAX_PATH];
        strcpy(BUbuff2,CurBackUp);
        strlen(BUbuff2);
        strcat(BUbuff2,strrchr(BUbuff,'\\'));
        Log->Write("New: "+AnsiString(BUbuff2));
        string CutFromString = strstr(FileToCopy,"Webclient");
        string str = "Webclient";
        CutFromString.erase(0,strlen(str.c_str()));
        CutFromString[strlen(CutFromString.c_str()) - strlen(strrchr(CutFromString.c_str(),'\\'))]='\0';
        CutFromString=CurBackUp+CutFromString;

        string ToCompare = CutFromString;
        if (ToCompare.c_str() !=CurBackUp && CheckFoldStruct(CutFromString))
        {

                                char ToCopy[MAX_PATH];
                                strcpy(ToCopy,CutFromString.c_str());
                                strcat(ToCopy,strrchr(BUbuff,'\\'));

                                CopyFile(FileToCopy,ToCopy,FALSE);
        }
        else if (ToCompare == CurBackUp)
        {
                      CopyFile(FileToCopy,BUbuff2,FALSE);
        }
        else
        {
          Log->Write("Cannot copy file: Incorrect copy directory");
        }
}

bool  TBackup::CheckFoldStruct(string FolderStruct)
{
Log->Write("CheckFoldStruct started ...");
if (DirectoryExists(FolderStruct.c_str()))                                                       
     {
        Log->Write("Struct is correct");
        return true;
     }
else if ( (!DirectoryExists(FolderStruct.c_str())) && (FolderStruct!="C:\\") )
     {
                 if ( CreateDirectory(FolderStruct.c_str(),NULL) ){
                      Log->Write(AnsiString(FolderStruct.c_str())+ " - CREATED");
                 }
                 else{
                           string ToCreate = FolderStruct;
                           FolderStruct[strlen(FolderStruct.c_str())-strlen(strrchr(FolderStruct.c_str(),'\\'))]='\0';
                           Log->Write("Start cheking: "+AnsiString(FolderStruct.c_str()));
                           CheckFoldStruct(FolderStruct);
                           Log->Write(AnsiString(FolderStruct.c_str())+" - CREATED");
                           CreateDirectory(ToCreate.c_str(),NULL);
                           return true;
                           }
     }
else {
        Log->Write("Error on checking structure directory: "+AnsiString(FolderStruct.c_str()));
        return false;
     }
}
  


#pragma package(smart_init)
