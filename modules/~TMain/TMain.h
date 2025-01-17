//---------------------------------------------------------------------------

#ifndef TMainH
#define TMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "LogClass.h"
#include "TMD5Class.h"
#include "XMLDoc.hpp"
//#include "XMLIntf.hpp"
#include "TBackup.h"  
#include "TFolderList.h"
#include "TProgressBar.h"
#include "TCroupier.h"
#include "TFileMap.h"
//---------------------------------------------------------------------------
class TMain : public TThread
{
private:
	   int i; //�������� ��� ������ � �.�.
protected:
        void __fastcall Execute();
public:
        __fastcall TMain(bool CreateSuspended);
        void __fastcall Progress(int Point, int Count);
        void __fastcall TStartDownload(); //�������� md5 ��� ������� ������ � ������ md5 ����������
		void GetFolder(AnsiString CurFolder, char* SaveFolder); // ����� ������� ���������� ��� ����� �� ����� ��������� ���������� � ������
		bool __fastcall ExceptionList(LPTSTR checkPath); //��������� ����������� �� �������� ����� ��� �� ������, ������� ��� ������������ �� ������
		void __fastcall TGet();//��������� ����
		void __fastcall TDisconnect(); //����������� �� �������
        void __fastcall TExit();
        //void __fastcall Extract7z();              //������������� ���������� �����
	//bool __fastcall StartProgram(AnsiString ApplicationName); //��������� DPSKiosk
	void __fastcall StopProc(char *exeName); // ������������� �������
        TProgressBar *TPrBr;
        TLogClass *Log;
        //--------------
        int percentage; //������� ������������ ������

        //------------------- ������������ �� FTP -----------------//
        String thost;
        String username;
        String password;
        int times; //���������� ������� �������
        _di_IXMLDocument  xmlCfg;
		
	//------------------- �������� ���-�� ������ � ��� ��� ������� � ������� --------------//
	int GM1C; //�������� ���-�� ������
	int GM2C;

        String Memo1[10000]; //������ ������ �� �������(��������� ���������� ������ md5)
	String Memo2[10000]; //������ ������, � ��������� ����� (���������� ���������� ���������� ������ md5 � ��������� �����)
	
	bool NewFile; //������� ����� ���� ��� ���
	bool download; //������� ����� �������� ��� ���
        bool notries; //������� ������� �� ������� �� ���������� (���� "true" - �������)
	
	String toreload[5000]; //�����, ������� ����� ���������
	
	/*�� ����� ��������*/
	AnsiString FF; //������:
        AnsiString FP; //����:
	
	//------------------- ��� ������ � ������������ --------------//
	TStringList *slDirs; //"������ ����������" �� �������
        TCroupier *cr; //"�������" ����� ���������� ����� ����������

	AnsiString sa; //�����, ������� ������������� � ������ ������ (�� ��������)	
	
	String ftpfold[5000];    //������ ���� �� ftp
	String drivefold[5000]; //������ ���� �� ��������� �����
		
	//------------------- ��� ���������� MD5 --------------//
	TMD5Class *MDCL;	
	
	//------------------- ������ �����, ������� ��������� �� ����� --------------//
	TFolderList *TFoLi;
	
	//------------------- ��������� �����������(Backup) --------------//
	TBackup *TBaUp;
	char *CurBackUp;

    //------------------- ��� �������� ��� ���������� --------------//
    HANDLE UpdateHandle;
    DWORD UpdateProcessId;
	
    HANDLE StartProgram(AnsiString ApplicationName); //��������� DPSKiosk

    //------------------- ��� ����������� �������������� �������������� --------------//
    TFileMap *FileMap;
    CLogHolder TLog;





};
//---------------------------------------------------------------------------
#endif
