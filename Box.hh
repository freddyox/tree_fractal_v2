#include "TPolyLine.h"
#include "TVector2.h"

class Box{
  private:
  double fFac;
  double fBWidth, fLength, fTWidth;
  TPolyLine *fBox;
  TVector2 fS, fF;
  
  public:
  Box(TVector2,TVector2,double,double);
  ~Box(){;}
  void SetFillColor(int);
  void SetLineColor(int);
  void SetLineWidth(int);
  TPolyLine *GetBox(){return fBox;}
  double Length()   {return fLength;}
  double BotWidth() {return fBWidth;}
  double TopWidth() {return fTWidth;}
  TVector2 GetStart() {return fS;}
  TVector2 GetFinal() {return fF;}
};
