//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scExtControls"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(TimeOut), this, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::scImage1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
DWORD WINAPI TimeOut(LPVOID lParam)
{
	TForm2 *pForm = (TForm2 *)lParam;
	Sleep(3000);
	pForm->Close();

	return 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Image1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

