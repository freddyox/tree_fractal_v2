#include "Tree.hh"
#include "Box.hh"
#include "TApplication.h"
#include "TGResourcePool.h"
#include "TGButton.h"
#include "TStyle.h"
#include "TCanvas.h"
#include <cmath>
#include <iostream>

Tree::Tree(const TGWindow *p,UInt_t w,UInt_t h){
  xhat = TVector2(1.0,0.0);
  yhat = TVector2(0.0,1.0);
  fOrigin = TVector2(0.5,0.4);  
  deg2rad = TMath::Pi()/180.0;

  // Parameters:
  fIter = 0;            // iteration number
  fTheta = 23;          // Fundamental fractal angle
  fBranchFactor = 0.6; // a^n fLength, n=fIter
  fLength = 0.35;       // trunk length
  fThickness = 0.03;    // trunk thickness
  fTopThickRatio = 0.5; // do we want a trapezoid effect?
  fOffsetY = fLength;   // Global offset
  fSigma = 0.0;         // Branch starting offset 
  fAsymmAngle = 0.0;    // Bias L/R angles
  fSplitFact = 2;       // # of branches to be generated 
  fColorScheme = 1;     // change color palette
  fBranchGrad = 54;

  fUseSliders = false;        // Sliders or numberentries -- fixed, but
                             // format is stupid

  // GUI BUTTON STATES
  fColorState = true;         // Black vs color palette 
  fReverseDrawState = false;  // Draw the poly vector backwards
  fDarkState = false;         // Background is white/black
  fThickState = false;
  fRandomState = false;
  
  fRand = new TRandom(0); // need this for flat random
  fGaus = new TRandom(0); // use this for gaussian random
 
  // Initialize the frames, and the canvas.
  fMain = new TGMainFrame(p,w,h);
  fCanvas = new TRootEmbeddedCanvas( "Canvas", fMain,w,h );
  fMain->AddFrame( fCanvas, new TGLayoutHints(kLHintsExpandX |
					      kLHintsExpandY, 10,10,10,20));

  ////////////////////////////////////////////////////////////////////////////////
  // CHANGE HERE TO ADD GUI
  //
  // ALL THESE ARRAYS NEED TO BE IN SYNC, a map will be built in the
  // gui loops
  //
  const char* numlabels[] = {"Theta", "Length", "Iterations", "AsymmAng",
			     "Offset","BranchRatio","Splits", "Palette",
			     "BranchWidth", "LevelGrad", "BranchGrad"};
  int IDs[] = {0,1,2,3,4,5,6,7,8,9,10};                                           // gui ID
  int NNumEntries = sizeof(numlabels) / sizeof*(numlabels);                // N guis
  int min[] = {  0, 1,    0,   0,  0,   5,   1,  0,   1, 1, 1};                        // min val
  int max[] = {360, 500, 16, 100, 50,  100, 10, 15, 100, 100, 100};                        // max val
  int initial[] = {fTheta,int(fLength*100), fIter, int(fAsymmAngle),
		   int(fSigma), int(fBranchFactor*100), fSplitFact, fColorScheme,
		   int(fThickness*600), fBranchGrad, int(fTopThickRatio*100)};
  
  TGLayoutHints *fBly = new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5);
  TGLayoutHints *fBfly1 = new TGLayoutHints(kLHintsTop | kLHintsCenterX, 5, 5, 5, 5);
  TGLayoutHints *fBfly2 = new TGLayoutHints(kLHintsTop | kLHintsLeft,    5, 5, 5, 5);
  TGLayoutHints *fBfly3 = new TGLayoutHints(kLHintsTop | kLHintsRight,   5, 5, 5, 5);
  
  TGHorizontalFrame *fHframe0, *fHButtons, *fHButtons1;
  TGVerticalFrame *fVframe0;

  // fHButtons = new TGHorizontalFrame(fHframe0,60,20);
  
  if( fUseSliders ){
    fHframe0 = new TGHorizontalFrame(fMain,60,35);
    for( int i=0; i<NNumEntries; i++ ){
      TGGroupFrame *fGframe = new TGGroupFrame(fHframe0, numlabels[i]);
      TGHSlider* slider = new TGHSlider(fGframe,150,kSlider1|kScaleDownRight,IDs[i]);
      fGui_labels[IDs[i]] = numlabels[i];
      slider->SetPosition(initial[i]);
      slider->SetRange(min[i],max[i]);
      slider->Connect("PositionChanged(Int_t)", "Tree", this, "HandleSliders()");
      fSliders[numlabels[i]] = slider;
      fGframe->AddFrame( slider,  new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5) );
      fHframe0->AddFrame( fGframe, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5) );
    }
    fHframe0->Resize(100,100);
    fMain->AddFrame( fHframe0, fBly);
  } else {
    fHframe0 = new TGHorizontalFrame(fMain,60,35);
    fVframe0 = new TGVerticalFrame(fHframe0,60,20);
    fHButtons = new TGHorizontalFrame(fVframe0,60,20);
    fHButtons1 = new TGHorizontalFrame(fVframe0,60,20);
    for( int i=0; i<NNumEntries; i++ ){
      fGui_labels[IDs[i]] = numlabels[i];
      TGGroupFrame *fGframe = new TGGroupFrame(fHframe0, numlabels[i]);
      TGNumberEntry* temp = new TGNumberEntry(fGframe, 0, 3, IDs[i], TGNumberFormat::kNESInteger,
					      TGNumberFormat::kNEAAnyNumber,
					      TGNumberFormat::kNELLimitMinMax,min[i],max[i]);
      temp->SetNumber( initial[i] );
      temp->Connect("ValueSet(Long_t)", "Tree", this, "HandleNumEntries()");
      (temp->GetNumberEntry())->Connect("ReturnPressed()", "Tree", this,
					    "HandleNumReturn()");
      fNumEntries[numlabels[i]] = temp;
      fGframe->AddFrame( temp,  new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5) );
      fHframe0->AddFrame( fGframe, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5) );
    }
    fDarken = new TGCheckButton(fHButtons, "Black Bkgrd",1);
    fDarken->Connect("Toggled(Bool_t)", "Tree", this,
		     "HandleCheckButton()");
    fControlColor = new TGCheckButton(fHButtons, "Color Tree",2);
    fControlColor->Connect("Toggled(Bool_t)", "Tree", this,
			   "HandleCheckButton()");
    fControlColor->SetOn();
    fThickTree = new TGCheckButton(fHButtons, "Add Thickness",3);
    fThickTree->Connect("Toggled(Bool_t)", "Tree", this,
			"HandleCheckButton()");
    fRandomize = new TGCheckButton(fHButtons1, "Randomize",4);
    fRandomize->Connect("Toggled(Bool_t)", "Tree", this,
			"HandleCheckButton()");
    fDrawOrder = new TGCheckButton(fHButtons1, "DrawOrder",4);
    fDrawOrder->Connect("Toggled(Bool_t)", "Tree", this,
			"HandleCheckButton()");
    // fill our smaller frames
    fHButtons->AddFrame(fDarken, fBfly2 );
    fHButtons->AddFrame(fControlColor, fBfly2 );
    fHButtons->AddFrame(fThickTree, fBfly2 );
    fHButtons1->AddFrame(fRandomize, fBfly2 );
    fHButtons1->AddFrame(fDrawOrder, fBfly2 );
    // add to main horizontal frame
    fVframe0->AddFrame(fHButtons, fBfly2);
    fVframe0->AddFrame(fHButtons1,fBfly2);
    fHframe0->AddFrame(fVframe0,  fBfly2 );
    fHframe0->Resize(100,100);
    fMain->AddFrame(fHframe0, fBly); // add to main program
  }
  // initialize:
  ColorPalette();
  GenerateTree();
  
  // Draw parent + children
  fMain->SetCleanup(kDeepCleanup);
  fMain->SetWindowName("Tree fractal generator");
  fMain->MapSubwindows();
  fMain->Resize(fMain->GetDefaultSize());
  fMain->MapWindow();
}

Tree::~Tree() { 
  fMain->Cleanup(); 
  delete fMain; 
}

void Tree::HandleCheckButton(){
  int col = -1;
  if( fDarken->GetState() ){
    col = kBlack;
  } else {
    col = kWhite;
  }
  if( col >= 0 ){
    fCanvas->GetCanvas()->SetFillColor(col);
    fCanvas->GetCanvas()->Clear();
    fCanvas->GetCanvas()->Modified();
    fCanvas->GetCanvas()->Update();
  }

  // handle states
  fColorState = fControlColor->GetState() ? true : false;
  fReverseDrawState = fDrawOrder->GetState() ? true : false;
  fDarkState = fDarken->GetState() ? true : false;
  fThickState = fThickTree->GetState() ? true : false;
  fRandomState = fRandomize->GetState() ? true : false;

  // redo the tree
  GenerateTree();
}

void Tree::HandleNumReturn(){ // no idea why but this works...
  ;
}

void Tree::HandleNumEntries(){
  // Note: Each time an event is handled, we need to generate the tree.
  //       The user can add more branches, but I control how many iterations
  //       or else too many lines are drawn. When iterations are controled,
  //       the colorpalette routine needs to be called and the tree needs to
  //       be regenerated.
  TGFrame *frm = (TGFrame *) gTQSender;
  TGNumberEntry *sl = (TGNumberEntry*) frm;
  int id = sl->WidgetId();
  switch( id ){
  case 0 :
    if( sl->GetButtonUp()->IsDown() && fTheta==360 ){
      fNumEntries[fGui_labels[id]]->SetNumber(0);
    }
    if( sl->GetButtonDown()->IsDown() && fTheta==0 ){
      fNumEntries[fGui_labels[id]]->SetNumber(360);
    }
    fTheta = fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber();
    std::cout << fGui_labels[id] << " " << fTheta << std::endl;
    break;
  case 1 :
    fLength = double( fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber() / 100.0);
    std::cout << fGui_labels[id] << " " << fLength << std::endl;
    break;
  case 2 :
    fIter = fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber();
    ColorPalette();
    std::cout << fGui_labels[id] << " " << fIter << std::endl;
    break;
  case 3 :
    fAsymmAngle = double( fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber());
    std::cout << fGui_labels[id] << " " << fAsymmAngle << std::endl;
    break;
  case 4 :
    fSigma = double( fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber())/100.0;
    std::cout << fGui_labels[id] << " " << fSigma << std::endl;
    break;
  case 5 :
    fBranchFactor = double( fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber())/100.0;
    std::cout << fGui_labels[id] << " " << fBranchFactor << std::endl;
    break;
  case 6 :
    fSplitFact =  fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber();
    std::cout << fGui_labels[id] << " " << fSplitFact << std::endl;
    if( fSplitFact >= 3 && fIter >= 11 ) fIter = 11;
    if( fSplitFact >= 4 && fIter >= 8 ) fIter = 8;
    if( fSplitFact >= 5 && fIter >= 7 ) fIter = 7;
    if( fSplitFact >= 6 && fIter >= 5 ) fIter = 5;
    fNumEntries["Iterations"]->SetNumber(fIter);
    ColorPalette();
    break;
  case 7 :
    if( sl->GetButtonUp()->IsDown() && fColorScheme==15 ){
      fNumEntries[fGui_labels[id]]->SetNumber(0);
    }
    if( sl->GetButtonDown()->IsDown() && fColorScheme==0 ){
      fNumEntries[fGui_labels[id]]->SetNumber(15);
    }
    fColorScheme = fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber();
    ColorPalette();
    break;
  case 8 :
    fThickness = double(fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber())/600.0;
    std::cout << fGui_labels[id] << " " << fThickness << std::endl;
    break;
  case 9 :
    fBranchGrad = fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber();
    std::cout << fGui_labels[id] << " " << fBranchGrad << std::endl;
    break;
  case 10 :
    fTopThickRatio = double(fNumEntries[fGui_labels[id]]->GetNumberEntry()->GetIntNumber())/100.0;
    std::cout << fGui_labels[id] << " " << fTopThickRatio << std::endl;
    break;
  default :
    break;
  }
  GenerateTree();
}

void Tree::Clean(){
  fBranches.clear();
  fStorage.clear();
  fTemp.clear();
}

void Tree::GenerateTree(){
  double offset = 0.0;
  if( fTheta>=180 ) {
    offset = (fTheta-180.)/360.0;
    if(offset>=0.4) offset=0.4;
    std::cout << offset << std::endl;
  }else{
    offset=0.0;
  } 

  std::vector<Box*>::iterator fPVit;
  std::vector<Box*> fPBranches;
  std::vector<Box*> fPStorage,fPTemp;

  double yi= fOrigin.Y() - fLength/2.0 - fOffsetY;
  if(yi<0.0)yi=0.0;
  TVector2 xi(fOrigin.X(),yi);
  TVector2 xf(fOrigin.X(), fOrigin.Y() + fLength/2.0 - fOffsetY + offset);
  TLine *trunk = new TLine( xi.X(), xi.Y(), xf.X(), xf.Y() );
  if( fColorState ) {
    trunk->SetLineColor(fPalette[0]);
  } else {
    if( fDarkState ) trunk->SetLineColor(kWhite);
    else trunk->SetLineColor(kBlack);
  }
 
  int dummy = 0; 

  Box *ptrunk = new Box(xi,xf, fThickness, fTopThickRatio);
  if( fColorState ) {
    ptrunk->SetFillColor(fPalette[0]);
  } else {
    if( fDarkState ) ptrunk->SetFillColor(kWhite);
    else ptrunk->SetFillColor(kBlack);
  }
  
  if( fThickState ) {
    fPBranches.clear();
    fPBranchesGlobal.clear();
    fPBranches.push_back( ptrunk );
    fPBranchesGlobal.push_back( (TPolyLine*)ptrunk->GetBox() );
    fPStorage.clear();
    fPTemp.clear();
    fPStorage = fPBranches;
  } else {
    fBranches.clear();
    fBranches.push_back( trunk );
    fStorage.clear();
    fTemp.clear();
    fStorage = fBranches;
  }
  
  bool didwesplit = false;
  while( dummy < fIter ) {
    
    if( dummy > 0 ){
      if( fThickState ) {
	fPStorage.clear();
	fPStorage = fPTemp;
	fPTemp.clear();
      } else {
	fStorage.clear();
	fStorage = fTemp;
	fTemp.clear();
      }
    }
    if( fThickState ){
      for( fPVit=fPStorage.begin(); fPVit!=fPStorage.end(); fPVit++ ){
      	TVector2 bot = (*fPVit)->GetStart();
      	TVector2 top = (*fPVit)->GetFinal();
      	double length = (*fPVit)->Length();
      	double next_length = length*fBranchFactor;
      	TVector2 nhat = (top-bot).Unit();
      	double ang_wrt_x = (TMath::ACos(nhat*xhat))/deg2rad;
      	TVector2 nstart = top;

	if( fRandomState ) {
	  if( fRand->Integer(10)==0 ){
	    fSplitFact++;
	    fNumEntries[fGui_labels[6]]->SetNumber(fSplitFact);
	    didwesplit=true;
	  }
	}
	
      	for( int i=0; i<fSplitFact; i++){
   
      	  double tempang = fTheta*2.0 / fSplitFact;
      	  double fact = (fSplitFact-1) / 2.0;
      	  double ang = (ang_wrt_x - fact*tempang) + i*tempang;
      	  if( nhat*yhat < 0 ) { ang = -ang_wrt_x - fact*tempang + i*tempang;}
      	  ang_wrt_x -= fAsymmAngle;

	  nstart = nstart - (fSigma*next_length)*nhat;

	  // if random flag is ON, assign a random branch to ignore.
	  if( fRandomState ) {
	    if( fRand->Integer(10)==5 ) continue;
	    if( fRand->Integer(5)==0  ) { 
	      next_length += fGaus->Gaus(0.0,0.1*next_length);
	    }
	    if( fRand->Integer(3)==0 ){
	      ang += fGaus->Gaus(0.0, 10.0);
	    }
	    if( fRand->Integer(4)==0 ){
	      nstart = nstart - fabs(fGaus->Gaus(0.0,0.1*next_length))*nhat;
	    }
	  }
	 
      	  TVector2 next_nhat( cos(ang*deg2rad), sin(ang*deg2rad) );
      	  TVector2 end = nstart + next_length*next_nhat;
	  double width = fThickness * (fBranchGrad/100.0) / ( dummy + 1 );
      	  Box *branch = new Box(nstart,end,width,fTopThickRatio);
      	  if( fColorState ) {
	    branch->SetFillColor(fPalette[dummy+1]);
	  } else {
	    if( fDarkState ) branch->SetFillColor(kWhite);
	    else branch->SetFillColor(kBlack);
	  }
      	  fPBranches.push_back( branch );
	  fPBranchesGlobal.push_back( (TPolyLine*)branch->GetBox() );
      	  fPTemp.push_back(branch);
      	}
	// NOTE CONFUSING!!!!!!!!!!!!!!!!!!!!!
	// need to reset the global split factor if we randomly split
	// otherwise it will randomly walk to higher values
	if( didwesplit ) {
	  fSplitFact--;
	  fNumEntries[fGui_labels[6]]->SetNumber(fSplitFact);
	  didwesplit = false;
	}
	/////////////////////////////////////////////////////////
      }
    } else {
      for( fVit=fStorage.begin(); fVit!=fStorage.end(); fVit++ ){
	// Grab Coordinates:
	TVector2 bot( (*fVit)->GetX1(), (*fVit)->GetY1() );
	TVector2 top( (*fVit)->GetX2(), (*fVit)->GetY2() );
      
	double length = (top-bot).Mod();

	// Lets generate two more with these dimensions:
	double next_length = length*fBranchFactor;

	// Build a "normal"
	TVector2 nhat = (top-bot).Unit();
	double ang_wrt_x = (TMath::ACos(nhat*xhat))/deg2rad;

	// Next starting point
	TVector2 nstart = top;
	
	if( fRandomState ) {
	  if( fRand->Integer(10)==0 ){
	    fSplitFact++;
	    fNumEntries[fGui_labels[6]]->SetNumber(fSplitFact);
	    didwesplit=true;
	  }
	}
	
	for( int i=0; i<fSplitFact; i++){
	  double tempang = fTheta*2.0 / fSplitFact;
	  double fact = (fSplitFact-1) / 2.0;
	  double ang = (ang_wrt_x - fact*tempang) + i*tempang;
	  if( nhat*yhat < 0 ) { ang = -ang_wrt_x - fact*tempang + i*tempang;}
	  ang_wrt_x -= fAsymmAngle;

	  if( fRandomState ) {
	    if( fRand->Integer(10)==5 ) continue;
	    if( fRand->Integer(5)==0  ) { 
	      next_length += fGaus->Gaus(0.0,0.5*next_length);
	    }
	    if( fRand->Integer(3)==0 ){
	      ang += fGaus->Gaus(0.0, 4.0);
	    }
	    if( fRand->Integer(4)==0 ){
	      nstart = nstart - fabs(fGaus->Gaus(0.0,0.1*next_length))*nhat;
	    }
	  }
	  
	  nstart = nstart - (fSigma*next_length)*nhat;
	  
	  TVector2 next_nhat( cos(ang*deg2rad), sin(ang*deg2rad) );
	  TVector2 end = nstart + next_length*next_nhat;
	  
	  TLine *branch = new TLine( nstart.X(), nstart.Y(), end.X(), end.Y() );
      	  if( fColorState ) {
	    branch->SetLineColor(fPalette[dummy+1]);
	  } else {
	    if( fDarkState ) branch->SetLineColor(kWhite);
	    else branch->SetLineColor(kBlack);
	  }
	  fBranches.push_back( branch );
	  fTemp.push_back( branch );
	}
	// NOTE CONFUSING!!!!!!!!!!!!!!!!!!!!!
	// need to reset the global split factor if we randomly split
	// otherwise it will randomly walk to higher values
	if( didwesplit ) {
	  fSplitFact--;
	  fNumEntries[fGui_labels[6]]->SetNumber(fSplitFact);
	  didwesplit = false;
	}
	/////////////////////////////////////////////////////////
      }
    }
    dummy++;
  }
  if( fThickState ){
    fCanvas->GetCanvas()->Clear();
    PDraw();
    fCanvas->GetCanvas()->Modified();
    fCanvas->GetCanvas()->Update();
  } else {
    fCanvas->GetCanvas()->Clear();
    Draw();
    fCanvas->GetCanvas()->Modified();
    fCanvas->GetCanvas()->Update();
  }
}

void Tree::PDraw(){
  if( fReverseDrawState ) {
    std::vector<TPolyLine*>::reverse_iterator rit;
    for( rit=fPBranchesGlobal.rbegin(); rit!=fPBranchesGlobal.rend(); rit++){
      (*rit)->Draw("f");
    }
  } else {
    for(fPVit_G=fPBranchesGlobal.begin(); fPVit_G!=fPBranchesGlobal.end(); fPVit_G++){
      (*fPVit_G)->Draw("f");
    }
  }
}

void Tree::Draw(){
  if( fReverseDrawState ){
    std::vector<TLine*>::reverse_iterator rit;
    for( rit=fBranches.rbegin(); rit!=fBranches.rend(); rit++){
      (*rit)->Draw("gl");
    }
  } else {
    for( fVit=fBranches.begin(); fVit!=fBranches.end(); fVit++){
      (*fVit)->Draw("gl");
    }
  }
}

// Run the Program - this structure is necessary
int main( int argc, char **argv ) {
  int w = 1500, h=900;
  TApplication theApp( "App", &argc, argv ); 
  new Tree(gClient->GetRoot(),w,h); 
  theApp.Run(); 
  return 0; 
}

////////////////////////////////////////////////////////////////////////////////
// Snagged the arrays from ROOT
// obviously it can be made very large
void Tree::ColorPalette(){
  fPalette.clear();
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};

  int FI;
  int N = fIter+1;
  if( fColorScheme==0 ){
    Double_t red[9]   = {  0./255.,  9./255., 13./255., 17./255., 24./255.,  32./255.,  27./255.,  25./255.,  29./255.};
    Double_t green[9] = {  0./255.,  0./255.,  0./255.,  2./255., 37./255.,  74./255., 113./255., 160./255., 221./255.};
    Double_t blue[9]  = { 28./255., 42./255., 59./255., 78./255., 98./255., 129./255., 154./255., 184./255., 221./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N );
  } else if( fColorScheme==1 ){
    Double_t red[9]   = {  0./255.,   5./255.,  15./255.,  35./255., 102./255., 196./255., 208./255., 199./255., 110./255.};
    Double_t green[9] = {  0./255.,  48./255., 124./255., 192./255., 206./255., 226./255.,  97./255.,  16./255.,   0./255.};
    Double_t blue[9]  = { 99./255., 142./255., 198./255., 201./255.,  90./255.,  22./255.,  13./255.,   8./255.,   2./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N );
  }else if( fColorScheme==2 ){
    Double_t red[9]   = { 242./255., 234./255., 237./255., 230./255., 212./255., 156./255., 99./255., 45./255., 0./255.};
    Double_t green[9] = { 243./255., 238./255., 238./255., 168./255., 101./255.,  45./255.,  0./255.,  0./255., 0./255.};
    Double_t blue[9]  = { 230./255.,  95./255.,  11./255.,   8./255.,   9./255.,   3./255.,  1./255.,  1./255., 0./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==3 ){
    Double_t red[9]   = {0./255.,  61./255.,  89./255., 122./255., 143./255., 160./255., 185./255., 204./255., 231./255.};
    Double_t green[9] = {0./255.,   0./255.,   0./255.,   0./255.,  14./255.,  37./255.,  72./255., 132./255., 235./255.};
    Double_t blue[9]  = {0./255., 140./255., 224./255., 144./255.,   4./255.,   5./255.,   6./255.,   9./255.,  13./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==4 ){
    Double_t red[9]   = { 0./255.,   0./255.,   0./255.,  70./255., 148./255., 231./255., 235./255., 237./255., 244./255.};
    Double_t green[9] = { 0./255.,   0./255.,   0./255.,   0./255.,   0./255.,  69./255.,  67./255., 216./255., 244./255.};
    Double_t blue[9]  = { 0./255., 102./255., 228./255., 231./255., 177./255., 124./255., 137./255.,  20./255., 244./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==5 ){
    Double_t red[9]   = { 0./255.,  4./255., 12./255.,  30./255.,  52./255., 101./255., 142./255., 190./255., 237./255.};
    Double_t green[9] = { 0./255., 40./255., 86./255., 121./255., 140./255., 172./255., 187./255., 213./255., 240./255.};
    Double_t blue[9]  = { 0./255.,  9./255., 14./255.,  18./255.,  21./255.,  23./255.,  27./255.,  35./255., 101./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==6 ){
    Double_t red[9]   = { 243./255., 243./255., 240./255., 240./255., 241./255., 239./255., 186./255., 151./255., 129./255.};
    Double_t green[9] = {   0./255.,  46./255.,  99./255., 149./255., 194./255., 220./255., 183./255., 166./255., 147./255.};
    Double_t blue[9]  = {   6./255.,   8./255.,  36./255.,  91./255., 169./255., 235./255., 246./255., 240./255., 233./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==7 ){
    Double_t red[9]   = { 22./255., 19./255.,  19./255.,  25./255.,  35./255.,  53./255.,  88./255., 139./255., 210./255.};
    Double_t green[9] = {  0./255., 32./255.,  69./255., 108./255., 135./255., 159./255., 183./255., 198./255., 215./255.};
    Double_t blue[9]  = { 77./255., 96./255., 110./255., 116./255., 110./255., 100./255.,  90./255.,  78./255.,  70./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==8 ){
    Double_t red[9]   = {  43./255.,  44./255., 50./255.,  66./255., 125./255., 172./255., 178./255., 155./255., 157./255.};
    Double_t green[9] = {  63./255.,  63./255., 85./255., 101./255., 138./255., 163./255., 122./255.,  51./255.,  39./255.};
    Double_t blue[9]  = { 121./255., 101./255., 58./255.,  44./255.,  47./255.,  55./255.,  57./255.,  44./255.,  43./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==9 ){
    Double_t red[9]   = { 49./255., 59./255., 72./255., 88./255., 114./255., 141./255., 176./255., 205./255., 222./255.};
    Double_t green[9] = { 78./255., 72./255., 66./255., 57./255.,  59./255.,  75./255., 106./255., 142./255., 173./255.};
    Double_t blue[9]  = { 78./255., 55./255., 46./255., 40./255.,  39./255.,  39./255.,  40./255.,  41./255.,  47./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==10 ){
    Double_t red[9]   = { 243./255., 222./255., 201./255., 185./255., 165./255., 158./255., 166./255., 187./255., 219./255.};
    Double_t green[9] = {  94./255., 108./255., 132./255., 135./255., 125./255.,  96./255.,  68./255.,  51./255.,  61./255.};
    Double_t blue[9]  = {   7./255.,  9./255.,   12./255.,  19./255.,  45./255.,  89./255., 118./255., 146./255., 118./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==11 ){
    Double_t red[9]   = {  5./255.,  18./255.,  45./255., 124./255., 193./255., 223./255., 205./255., 128./255., 49./255.};
    Double_t green[9] = { 48./255., 134./255., 207./255., 230./255., 193./255., 113./255.,  28./255.,   0./255.,  7./255.};
    Double_t blue[9]  = {  6./255.,  15./255.,  41./255., 121./255., 193./255., 226./255., 208./255., 130./255., 49./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==12 ){
    Double_t red[9]   = { 0./255., 48./255., 119./255., 173./255., 212./255., 224./255., 228./255., 228./255., 245./255.};
    Double_t green[9] = { 0./255., 13./255.,  30./255.,  47./255.,  79./255., 127./255., 167./255., 205./255., 245./255.};
    Double_t blue[9]  = { 0./255., 68./255.,  75./255.,  43./255.,  16./255.,  22./255.,  55./255., 128./255., 245./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==13 ){
    Double_t red[9]   = {  34./255.,  70./255., 129./255., 187./255., 225./255., 226./255., 216./255., 193./255., 179./255.};
    Double_t green[9] = {  48./255.,  91./255., 147./255., 194./255., 226./255., 229./255., 196./255., 110./255.,  12./255.};
    Double_t blue[9]  = { 234./255., 212./255., 216./255., 224./255., 206./255., 110./255.,  53./255.,  40./255.,  29./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==14 ){
    Double_t red[9]   = { 18./255.,  72./255.,   5./255.,  23./255.,  29./255., 201./255., 200./255., 98./255., 29./255.};
    Double_t green[9] = {  0./255.,   0./255.,  43./255., 167./255., 211./255., 117./255.,   0./255.,  0./255.,  0./255.};
    Double_t blue[9]  = { 51./255., 203./255., 177./255.,  26./255.,  10./255.,   9./255.,   8./255.,  3./255.,  0./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else if( fColorScheme==15 ){
    Double_t red[9]   = { 0./255., 13./255.,  30./255.,  44./255.,  72./255., 120./255., 156./255., 200./255., 247./255.};
    Double_t green[9] = { 0./255., 36./255.,  84./255., 117./255., 141./255., 153./255., 151./255., 158./255., 247./255.};
    Double_t blue[9]  = { 0./255., 94./255., 100./255.,  82./255.,  56./255.,  66./255.,  76./255., 131./255., 247./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N);
  }else {
    Double_t red[9]   = {  0./255.,   5./255.,  15./255.,  35./255., 102./255., 196./255., 208./255., 199./255., 110./255.};
    Double_t green[9] = {  0./255.,  48./255., 124./255., 192./255., 206./255., 226./255.,  97./255.,  16./255.,   0./255.};
    Double_t blue[9]  = { 99./255., 142./255., 198./255., 201./255.,  90./255.,  22./255.,  13./255.,   8./255.,   2./255.};
    FI = TColor::CreateGradientColorTable(9, stops, red, green, blue, N );
  }
  
  for (int i=0; i<N; i++) fPalette.push_back(FI+i);
}


void Tree::HandleSliders(){
  Int_t id;
  TGFrame *frm = (TGFrame *) gTQSender;
  TGSlider *sl = (TGSlider*) frm;
  id = sl->WidgetId();
  switch( id ){
  case 0 :
    fTheta = fSliders[fGui_labels[id]]->GetPosition();
    GenerateTree();
    std::cout << fGui_labels[id] << " " << fTheta << std::endl;
    break;
  case 1 :
    fLength = double(fSliders[fGui_labels[id]]->GetPosition() / 100.0);
    std::cout << fGui_labels[id] << " " << fLength << std::endl;
    GenerateTree();
    break;
  case 2 :
    fIter = fSliders[fGui_labels[id]]->GetPosition();
    ColorPalette();
    std::cout << fGui_labels[id] << " " << fIter << std::endl;
    GenerateTree();
    break;
  case 3 :
    fAsymmAngle = double(fSliders[fGui_labels[id]]->GetPosition());
    std::cout << fGui_labels[id] << " " << fAsymmAngle << std::endl;
    GenerateTree();
    break;
  case 4 :
    fSigma = double(fSliders[fGui_labels[id]]->GetPosition())/100.0;
    std::cout << fGui_labels[id] << " " << fSigma << std::endl;
    GenerateTree();
    break;
  case 5 :
    fBranchFactor = double(fSliders[fGui_labels[id]]->GetPosition())/100.0;
    std::cout << fGui_labels[id] << " " << fBranchFactor << std::endl;
    GenerateTree();
    break;
  case 6 :
    fSplitFact = fSliders[fGui_labels[id]]->GetPosition();
    std::cout << fGui_labels[id] << " " << fSplitFact << std::endl;
    if( fSplitFact >= 3 && fIter >= 11 ) fIter = 11;
    if( fSplitFact >= 4 && fIter >= 9 ) fIter = 9;
    if( fSplitFact >= 5 && fIter >= 7 ) fIter = 7;
    if( fSplitFact >= 6 && fIter >= 5 ) fIter = 5;
    fSliders["Iterations"]->SetPosition(fIter);
    ColorPalette();
    GenerateTree();
    break;
  case 7 :
    fColorScheme =  fSliders[fGui_labels[id]]->GetPosition();
    ColorPalette();
    GenerateTree();
    break;
  default :
    break;
  }
}
