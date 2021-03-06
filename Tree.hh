#include "TGClient.h"  
#include "RQ_OBJECT.h" 
#include "TString.h"
#include "TRootEmbeddedCanvas.h" 
#include "TGLabel.h"
#include "TGNumberEntry.h"
#include "TGButton.h"
#include "TLine.h"
#include "TMath.h"
#include "TVector2.h"
#include "TGSlider.h"
#include "TPolyLine.h"
#include "TRandom.h"
#include <vector>
#include <set>
#include <map>

class Tree{
  RQ_OBJECT("Tree") 
  private:
  // GUI RELATED
  bool fUseSliders;
  TGMainFrame *fMain;                             
  TGLayoutHints *fL1, *fL2;
  TRootEmbeddedCanvas *fCanvas;

  std::map<int, const char*> fGui_labels;
  std::map<const char*, TGHSlider*> fSliders;
  std::map<const char*, TGNumberEntry*> fNumEntries;
  TGCheckButton *fDarken, *fControlColor, *fThickTree;
  TGCheckButton *fRandomize, *fDrawOrder;
  
  bool fColorState, fReverseDrawState, fDarkState;
  bool fThickState, fRandomState;
  
  TRandom *fRand, *fGaus;

  double fSigma;
  double fAsymmAngle;
  double fChangeBranchFactor;
  
  double deg2rad;
  double fDisx, fDisy;
  int fIter;
  int fSplitFact;
  int fColorScheme;
  int fBranchGrad;
  double fLength;
  double fBranchFactor;
  TVector2 fOrigin;
  double fOffsetY;

  int fTheta;
  TVector2 yhat;
  TVector2 xhat;

  std::vector<TLine*>::iterator fVit;
  std::vector<TLine*> fBranches;
  std::vector<TLine*> fStorage, fTemp;
  std::vector<int> fPalette;

  //Box* boxtemp;
  std::vector<TPolyLine*>::iterator fPVit_G;
  std::vector<TPolyLine*> fPBranchesGlobal;
  double fThickness, fTopThickRatio;

public:
  Tree(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~Tree();
  void Draw();
  void GenerateTree();
  void HandleSliders();
  void HandleCheckButton();
  void HandleNumEntries();
  void HandleNumReturn();
  void Clean();
  void ColorPalette();
  void GetTheta();
  void PDraw();
};
