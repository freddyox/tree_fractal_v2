#include "Box.hh"
#include <iostream>

Box::Box(TVector2 xo, TVector2 xf, double w, double fac=1.0) :
  fS(xo), fF(xf), fBWidth(w), fFac(fac) {
  fLength = (fF-fS).Mod();
  TVector2 npar  = (fF-fS).Unit();
  TVector2 nperp( -npar.Y(), npar.X() );
  nperp.Unit();

  if( fBWidth < 0.001 ) fBWidth = 0.001;
  
  fTWidth = fac*fBWidth;
  
  TVector2 BL = fS - 0.5*fBWidth*nperp;
  TVector2 TL = fS + npar*fLength - 0.5*fTWidth*nperp;
  TVector2 TR = fS + npar*fLength + 0.5*fTWidth*nperp;
  TVector2 BR = fS + 0.5*fBWidth*nperp;

  double x[] = {BL.X(), TL.X(), TR.X(), BR.X()};
  double y[] = {BL.Y(), TL.Y(), TR.Y(), BR.Y()};
  
  fBox = new TPolyLine(4,x,y);
}

void Box::SetFillColor(int col){
  fBox->SetFillColor(col);
}

void Box::SetLineColor(int col){
  fBox->SetLineColor(col);
}

void Box::SetLineWidth(int wid){
  fBox->SetLineWidth(wid);
}
