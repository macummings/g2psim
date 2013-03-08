#include <cstdlib>
#include <cmath>

#include "TROOT.h"
#include "TObject.h"
#include "TError.h"

#include "G2PGlobals.hh"
#include "G2PGunBase.hh"
#include "G2PRand.hh"

#include "G2PPointGun.hh"

G2PPointGun::G2PPointGun()
{
    // Nothing to do
}

G2PPointGun::~G2PPointGun()
{
    // Nothing to do
}

bool G2PPointGun::Shoot(double* V5beam_lab, double* V5react_tr, double* reserved)
{
    static const char* const here = "Shoot()";
    
    double X_lab = pRand->Gaus(fBeamX_lab, fSigmaPos_lab);
    double Y_lab = pRand->Gaus(fBeamY_lab, fSigmaPos_lab);
    double Z_lab = pRand->Gaus(fReactZLow_lab, fSigmaPos_lab);
    GetReactPoint(X_lab, Y_lab, Z_lab, V5beam_lab);

    double Xreact_tr, Yreact_tr, Zreact_tr;
    HCS2TCS(V5beam_lab[0], V5beam_lab[2], V5beam_lab[4], fHRSAngle, Xreact_tr, Yreact_tr, Zreact_tr);
    if (fDebug>2) {
        Info(here, "%10.3e %10.3e %10.3e", Xreact_tr, Yreact_tr, Zreact_tr);
    }
   
    V5react_tr[0] = Xreact_tr;
    V5react_tr[1] = pRand->Gaus(fTargetThLow_tr, fSigmaAng_tr);
    V5react_tr[2] = Yreact_tr;
    V5react_tr[3] = pRand->Gaus(fTargetPhLow_tr, fSigmaAng_tr);
    V5react_tr[4] = pRand->Gaus(fDeltaLow, fSigmaDelta);

    if (fDebug>1) {
        Info(here, "%10.3e %10.3e %10.3e %10.3e %10.3e", V5beam_lab[0], V5beam_lab[1], V5beam_lab[2], V5beam_lab[3], V5beam_lab[4]);
        Info(here, "%10.3e %10.3e %10.3e %10.3e %10.3e", V5react_tr[0], V5react_tr[1], V5react_tr[2], V5react_tr[3], V5react_tr[4]);
    }

    return true;
}

ClassImp(G2PPointGun)