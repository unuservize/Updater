//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TMain.h"
#include "Unit1.h"
#include "fstream.h"
#include "conio.h"
#include <Tlhelp32.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMain::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

char sPath[MAX_PATH]="C:\\Webclient";

int n=0;
int k=0;      //������������� � ������ ������� md5
int m=0;      //������ ���������� GetFolder


__fastcall TMain::TMain(bool CreateSuspended)
        : TThread(CreateSuspended)
{
    //------------------- ���� ������������� -----------------//
	Log=new TLogClass("TMain");
    if (Log)
        Log->Write("//-------------- "+DateTimeToStr(Now())+" -------------------------"); //������� � ���� ������� ���� � �����
    percentage = 0; //������� ������������ ������
	MDCL = new TMD5Class(); //��������� MD5 �����
	slDirs = new TStringList;
	TPrBr = new TProgressBar(true); //����������� �������� ���������� � ������������
    TPrBr->Resume();
	TFoLi = new TFolderList(); //������ �����, ������� ��������� �� ����� 
	download = false; //����� ��������� ���������� ��� ������ ��������� (��-��������� ������� ��� � ��� ���������� ������)
    NewFile=false; //������ "�����" ���� ��� ���
	TBaUp = new TBackup();   //��������� ����������� ������� ������
    CurBackUp=TBaUp->BUDir(); //������ ���� ����� ��� ������

    //------------------- ������������ �� FTP -----------------//

try{
    int i;
    string s;
    AnsiString xmlstring;
                ifstream input("C:\\WebClient\\config\\config.xml");  //������ ��� � ��� �������� ��������� �������
                Log->Write("Configuration XML has been opened ... ");
                while (getline(input, s))
                        {
                        if(!s.length())
                        {
                                 break;
                        }
                         xmlstring=xmlstring+s.c_str();
                        }                                     
xmlCfg = LoadXMLData(xmlstring);
Log->Write("Try to get Configuretion info...");
IXMLNode *nodRoot = xmlCfg->DocumentElement;

IXMLNode *nodElement = nodRoot->ChildNodes->FindNode(L"ftpconnect");
IXMLNode *nodSubElement = nodElement->ChildNodes->FindNode(L"host");
Log->Write("host: "+nodSubElement->NodeValue+".....OK");
IXMLNode *nodSubElement2 = nodElement->ChildNodes->FindNode(L"username");
Log->Write("user: "+nodSubElement2->NodeValue+"......OK");
IXMLNode *nodSubElement3 = nodElement->ChildNodes->FindNode(L"password");
Log->Write("password: "+nodSubElement3->NodeValue+".....OK");

if( nodSubElement == NULL ){
                Log->Write("|notupdate| : There is no node with that name. Check your XML file");
                TExit();
        }
else if( nodSubElement != NULL ){
                thost = nodSubElement->NodeValue;
                username = nodSubElement2->NodeValue;
                password = nodSubElement3->NodeValue;
                Log->Write("Connecting to: "+thost+" | "+username+" | "+password+" | ... OK ");
                StopProc("DPSKiosk.exe");
                StopProc("DPSCom.exe");
        }
}
catch(...){
Log->Write("Something wrong with initialization...Check \"config.xml\" file");
Application->Terminate();
}
}

//---------------------------------------------------------------------------
void __fastcall TMain::Execute()
{
CoInitialize(NULL);
Synchronize(&TStartDownload); //�������� md5 ��� ������� ������ � ������ md5 ����������
AnsiString Txt;  //Txt � TTxt ����� ��� ������ ���-���� ���������� ���� � ������������ ������ ��������
AnsiString TTxt;
Log->Write("�������� ����������...started");
for (k=0;k<=GM1C; k++) //��������� ������ ��� �������� (����� �����/����� ���������� �� ����� ��������, ������ ��� ����� �� ������������ ������ ���������)
    {
     Progress(k,GM1C+1); //����������� �������� ���������� � ������������
     NewFile = true;  //��-��������� �������, ��� ���� �����
         for (i=0; i<=GM2C; i++)
                {
                //Application->ProcessMessages();
                AnsiString UpTxt = Memo2[i];   // ��������� �����
                AnsiString UpTxtBack = UpTxt;
                AnsiString ExTxt = Memo1[k];  //��������� �����
               //Log->Write("��������� ����: "+ExTxt);
               //Log->Write("��������� ����: "+UpTxt);
                AnsiString ExTxtBack = ExTxt;
                if (UpTxt == ExTxt){    //���� ������ ��������� - ���� �� ��������� � ����������
					  NewFile = false;
                                          break;
					 }
                else {
                                          if(ExTxt.Pos("document") !=0)
                                                Log->Write("�������: "+UpTxt+" � "+ExTxt);
					  NewFile = true;
					  Txt = ExTxt;
					  TTxt=UpTxt;
                                          //break;
                     }
                }
                if (NewFile)
                       {
                        download = true;   //���� ���� ���� �� ���� ����� ����, ��������� � ��������
							if (Txt !=""){
										  toreload[m] = Txt;
									      Log->Write("Added toreload[m]: "+toreload[m]);
							}
							else
								 Log->Write(Txt + "found");
                                                                 m++;
                        //break;
                       }
                NewFile = true;
    }
        GetFolder("/Webclient","C:\\Webclient\\"); //����������� "������ ����������"
	if (download)    {
                Log->Write("download: true");
                Log->Write("Check directory exists ...");
                if (DirectoryExists("C:\\Webclient"))
                {
                int l=1;
                int finish = m;
                while (m>0) //���-�� ������ ��������� ����������
                        {
                         m--;
                         Progress(l, finish);
                         //Application->ProcessMessages();
                         AnsiString FileToDownload = toreload[m];
                         string str = toreload[m].c_str(); //������� �� �������
                         FileToDownload[strlen(FileToDownload.c_str())-strlen(strrchr(FileToDownload.c_str(),'*')+2)]='\0';
                         AnsiString str2 = FileToDownload; 
                         str.erase(0,strlen(str2.c_str())); //�������� ���� � �����

                         AnsiString FinalFile = StringReplace(str.c_str(),'\\',"/", TReplaceFlags()<< rfReplaceAll); //������� �� �������
                         AnsiString FinalPath = "C:\\";
                         FinalPath+=str.c_str(); //�������� ������ ���� � ����� �� ����� �:
                         try{
							   TBaUp->BUCopyFile(FinalPath.c_str(),CurBackUp); //�� ������ ������ ������ �����
                               FF=FinalFile; //���� �� �������
                               FP=FinalPath; //���� "���������"

                               //Application->ProcessMessages();
                               if (!ExceptionList(FinalPath.c_str())){
                                         //Synchronize(&TGet);
                                         Form1->idFTP->Get("/"+FF,FP,TRUE);
                                         Log->Write("File created : "+FinalPath);
                                        }
                               else     {
                                         Log->Write("Exception for path: "+FinalPath+" . Please choose another way to update this file");
                                        }
                        }
                        catch(EIdProtocolReplyError &e) {
                                Log->Write("There is no "+FinalPath+ "on FTP");
                                }
                        l++;
                        }
                //Sleep(1000);
                }
       }
        else
       {
        TPrBr->ProgressBarFinish();
        Log->Write("download: false");
        Log->Write("�� ��������� ����������� ���������� ������");
        if (GM2C ==0)
        {
        Log->Write("There is no any folder");
        }
       //Synchronize(&TDisconnect);
       }
       ChDir("C:\\WebClient\\");
       Synchronize(&TDisconnect);
       Synchronize(&TExit);
}


void __fastcall TMain::Progress(int Point, int Count){
      percentage = Point*100/Count+1;
      TPrBr->FillProgressBar(percentage);
}


void __fastcall TMain::TStartDownload()
{
        Form1->idFTP->Host=thost;
        Form1->idFTP->Username=username;
        Form1->idFTP->Password=password;
        try{
        Form1->idFTP->Connect();
        }
         catch (Exception &ex)
        {
         Log->Write("Exception: "+AnsiString(ex.Message));
        }
        if (Form1->idFTP->Connected())
        {
        Log->Write("���������� � �������� ...�������");

        Form1->idFTP->Noop();
        Form1->Memo1->Clear();
        Form1->Memo2->Clear();

     Log->Write("Connected to: "+AnsiString(thost));

     /**************************************************************************
     ��������!: ������ �� ����� /Webclient/md5.txt �  C:\Webclient\webclient.txt
     **************************************************************************/
     try{
     Form1->idFTP->Get("/Webclient/md5.txt","C:\\Webclient\\webclient.txt",TRUE);
     Form1->Memo1->Lines->LoadFromFile("C:\\Webclient\\webclient.txt");
     for (int q=0; q<=Form1->Memo1->Lines->Count-1; q++)
     {
      Memo1[q]= Form1->Memo1->Lines->Strings[q];
      GM1C = q; //��� ��� ������� ���-�� ������
     }
     MDCL->GetFileList(sPath);
     Form1->Memo2->Lines->SaveToFile("C:\\Webclient\\md5.txt");
     for (int q=0; q<=Form1->Memo2->Lines->Count-1; q++)
     {
      Memo2[q]= Form1->Memo2->Lines->Strings[q];
      GM2C = q;
     }
     }
     catch (Exception &ex)
     {
    Log->Write("Exception: "+AnsiString(ex.Message));
     }
     // delete MDCL;
     // MDCL = new TMD5Class();
}

}


void TMain::GetFolder(AnsiString CurFolder, char* SaveFolder) // ����� ���������� "������ ����������", ���� �����-�� ����� ���������
{
try
{
//------------------- ���������� --------------//
    Log->Write("Create Directory: "+AnsiString(SaveFolder));
    CreateDirectory(SaveFolder,NULL);
    Log->Write("Directory created: "+AnsiString(SaveFolder));
    Form1->idFTP->Noop();
    Form1->Memo1->Clear();
    slDirs->Clear();
    Form1->idFTP->ChangeDir(CurFolder);
    Form1->idFTP->Noop();
    Log->Write("Have"+CurFolder);
//------------------- ������ --------------//
    Form1->idFTP->List(slDirs, ' ', False); //�������� ������ ���������� (� ����� ����� ��)
    Form1->Memo1->Lines->AddStrings(slDirs);
	int start_m = m; //���������� � ���� ������ ������
    for (int i=0; i<=(Form1->Memo1->Lines->Count-1); i++){
            sa="";
            sa = Form1->Memo1->Lines->Strings[i]; //�� ������ �������� � sa ��������� ���������� (��� ����, �.�. � ��� ��� � ������ ���� �������)
          if (! strstr(sa.c_str(),"."))  //���� ������� �� ����, �� �������� ���� � ���� ����������
          {
            AnsiString FolderLine = Form1->Memo1->Lines->Strings[i];
            AnsiString CF=CurFolder+'/'+FolderLine;    //������� ��� ��������
            AnsiString SF = StringReplace(CF,'/',"\\", TReplaceFlags()<< rfReplaceAll);
            SF = strstr(SF.c_str(),"\\Webclient");
            SF = "C:"+SF+"\\";
            ftpfold[m]=CF; //���� �� �������     <-- ������ � �������, ����� �� �������� ��� ����� ���������� ����������
            drivefold[m]=SF; //���� "���������"
            m++;
          }
	}
    while (m>start_m)
    {
	 Log->Write(AnsiString(m));
     SaveFolder = NULL;
     CurFolder = NULL;
     m--;

     GetFolder(ftpfold[m],drivefold[m].c_str());
    } 
    }
    catch (Exception &ex)
{
    Log->Write("Exception: "+AnsiString(ex.Message));
}
}


bool __fastcall TMain::ExceptionList(LPTSTR checkPath) //��������� ����������� �� �������� ����� ��� �� ������, ������� ��� ������������ �� ������
{
int n=0; //����� ������
string *notdirtxt=TFoLi->ListFolder("notupdate"); //�������� �����, ������� �� ����� ��������
        char dnotbuf [MAX_PATH];
        for(int k=0;k<6;k++)
        {
        strcpy(dnotbuf,notdirtxt[k].c_str());
        n=strlen(dnotbuf);
        if (!n==0){
                    if (strstr(checkPath,dnotbuf)){
                                Log->Write("ExceptionList: TRUE");
                                return true;
					}
					else{
                         //Log->Write("---------"+AnsiString(checkPath)+"------1s");
                         //Log->Write(notdirtxt[k].c_str());
                    }
		  }
        }
//Log->Write("ExceptionList: ends with false");
return false;
}

//--------------------------------------------------------------------------------
void __fastcall TMain::TGet() //��������� ����
{
  Form1->idFTP->Get("/"+FF,FP,TRUE);
}

//---------------------------------------------------------------------------

void __fastcall TMain::TDisconnect() //��������� �� �������
{
 Log->Write("Disconnect...");
 Form1->idFTP->Noop();
 StartProgram("C:\\WebClient\\DPSKiosk.exe");
 StartProgram("C:\\WebClient\\DPSCom.exe");
 Sleep(10000);
 Form1->idFTP->Disconnect();
}

//---------------------------------------------------------------------------

HANDLE TMain::StartProgram(AnsiString ApplicationName) //��������� DPSKiosk
{
  try{
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
		//Log->Append(ShowError("Starting program error"));
		return NULL;
		}
	else
		{
		Log->Append("OK.");
		return pi.hProcess;
		}
  }
  catch(Exception& error){
    Log->Write("StartProgram() Exception :" + AnsiString(error.Message));
    return NULL;
  }
  }


//-------------------------------------------------------------------------------------

void __fastcall TMain::StopProc(char *exeName) // ������������� �������
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

void __fastcall TMain::TExit()
{
Form1->Close();
}
















