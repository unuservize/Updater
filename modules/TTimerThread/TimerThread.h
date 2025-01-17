//---------------------------------------------------------------------------

#ifndef TimerThreadH
#define TimerThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "LogClass.h"
#include "TCancelUpdate.h"
//---------------------------------------------------------------------------
class TThreadTimer : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        __fastcall TThreadTimer(bool CreateSuspended);
        void __fastcall TThreadTimer::OutTimer();
        void __fastcall TThreadTimer::ShowTimer();
        int timecount,timecountmin;
        AnsiString outtime, outtimemin;
//---------------------------------------------------------------------------
        TLogClass *Log;
        TCancelUpdate *TCaUp;
};
//---------------------------------------------------------------------------
#endif
