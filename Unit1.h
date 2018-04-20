//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
#include "scAppPager.hpp"
#include "scControls.hpp"
#include "scExtControls.hpp"
#include "scGPVertPagers.hpp"
#include "scImageCollection.hpp"
#include "scModernControls.hpp"
#include "scOpenFileDialog.hpp"
#include "scShellControls.hpp"
#include "scStyledForm.hpp"
#include "scStyleManager.hpp"
#include "SkinCtrls.hpp"
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
#include <boost/scoped_array.hpp>
#include <boost/program_options.hpp>
#include <boost/timer.hpp>

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include "scControls.hpp"
#include "scShellControls.hpp"
#include "scModernControls.hpp"
#include "SkinCtrls.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "scStyleManager.hpp"
#include "scStyledForm.hpp"
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include "scAppPager.hpp"
#include "scExtControls.hpp"
#include "scGPVertPagers.hpp"
#include <Vcl.Mask.hpp>
#include "scOpenFileDialog.hpp"
#include "scImageCollection.hpp"
#include "scToolPager.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <pe_bliss.h>
#include <pe_bliss_resources.h>

#include "../../lzo-2.06/include/lzo/lzo1z.h"

#include "structs.h"

#include "parameters.h"
#include "unpacker.h"

using namespace pe_bliss;

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TscShellListView *scShellListView1;
	TscShellTreeView *scShellTreeView1;
	TscPanel *scPanel1;
	TscStyledForm *scStyledForm1;
	TscButton *scButton1;
	TscPanel *scPanel2;
	TImageList *ImageList1;
	TPopupMenu *PopupMenu1;
	TMenuItem *New1;
	TMenuItem *Open1;
	TMenuItem *Close1;
	TMenuItem *N2;
	TMenuItem *Save1;
	TMenuItem *SaveAs1;
	TMenuItem *N1;
	TMenuItem *Exit1;
	TImageList *ImageList2;
	TscAppPager *scAppPager1;
	TscAppPagerPage *scAppPagerPage1;
	TscAppPagerPage *scAppPagerPage2;
	TscAppPagerPage *scAppPagerPage3;
	TscButton *scButton3;
	TscStyleManager *scStyleManager1;
	TscStatusBar *scStatusBar1;
	TscButton *scButton5;
	TscPanel *scPanel4;
	TscGPVertTabControl *scGPVertTabControl1;
	TscLabel *scLabel1;
	TscEdit *scEdit1;
	TscPageViewer *scPageViewer1;
	TscPageViewerPage *scPageViewerPage1;
	TscPageViewerPage *scPageViewerPage2;
	TscPageViewerPage *scPageViewerPage3;
	TscButton *scButton2;
	TscOpenDialog *scOpenDialog1;
	TscLabel *scLabel2;
	TscEdit *scEdit2;
	TscLabel *scLabel3;
	TscMemo *scMemo1;
	TscImageCollection *scImageCollection1;
	TscPanel *scPanel3;
	TImageList *ImageList3;
	TImageList *ImageList4;
	TImageList *ImageList5;
	TscButton *scButton4;
	TscButton *scButton6;
	TscCheckBox *scCheckBox1;
	TscGroupBox *scGroupBox1;
	TscGroupBox *scGroupBox2;
	TscComboBox *scComboBox1;
	TscLabel *scLabel4;
	TscButton *scButton7;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall scButton3Click(TObject *Sender);
	void __fastcall scButton4Click(TObject *Sender);
	void __fastcall scButton5Click(TObject *Sender);
	void __fastcall scGPVertTabControl1ChangeTab(TObject *Sender);
	void __fastcall scButton2Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall scShellListView1DblClick(TObject *Sender);
	void __fastcall scButton6Click(TObject *Sender);
	void __fastcall scCheckBox1Click(TObject *Sender);
	void __fastcall scButton7Click(TObject *Sender);
	void __fastcall New1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
private:	// User declarations
	int __fastcall Packing(UnicodeString in_filename, int argc, char *argv[]);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
