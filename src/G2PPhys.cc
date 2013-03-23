#include <cstring>
#include <map>

#include "TROOT.h"
#include "TObject.h"
#include "TError.h"

#include "G2PPhysBase.hh"
#include "G2PPhysEl/G2PPhysEl.hh"
#include "G2PPhysPB/G2PPhysPB.hh"
#include "G2PPhysQFS/G2PPhysQFS.hh"

#include "G2PAppBase.hh"
#include "G2PGlobals.hh"
#include "G2PRunBase.hh"

#include "G2PPhys.hh"

using namespace std;

G2PPhys* G2PPhys::pG2PPhys = NULL;

G2PPhys::G2PPhys() :
    iSetting(1), iZ(1), iA(1), fTargetMass(0.0), iPID(11),
    fPars(NULL), nPars(0), pModel(NULL)
{
    // Nothing to do
}

G2PPhys::G2PPhys(const char *model) :
    iSetting(1), iZ(1), iA(1), fTargetMass(0.0), iPID(11),
    fPars(NULL), nPars(0), pModel(NULL)
{
    if (pG2PPhys) {
        Error("G2PPhys()", "Only one instance of G2PPhys allowed.");
        MakeZombie();
        return;
    }
    pG2PPhys = this;

    map<string, int> model_map;
    model_map["elastic"] = 1;
    model_map["pbosted"] = 11;
    model_map["qfs"] = 12;

    iSetting = model_map[model];
}

G2PPhys::~G2PPhys()
{
    if (pG2PPhys==this) pG2PPhys = NULL;
}

int G2PPhys::Init()
{
    static const char* const here = "Init()";

    if (G2PAppBase::Init()!=0) return fStatus;

    switch (iSetting) {
    case 1:
        pModel = new G2PPhysEl();
        break;
    case 11:
        pModel = new G2PPhysPB();
        break;
    case 12:
        pModel = new G2PPhysQFS();
        break;
    default:
        Error(here, "Cannot initialize, invalid setting.");
        return (fStatus = kINITERROR);
        break;
    }

    return (fStatus = kOK);
}

int G2PPhys::Begin()
{
    //static const char* const here = "Begin()";

    if (G2PAppBase::Begin()!=0) return fStatus;

    iZ = gG2PRun->GetTargetZ();
    iA = gG2PRun->GetTargetA();
    fTargetMass = gG2PRun->GetTargetMass();
    iPID = gG2PRun->GetParticlePID();

    pModel->SetTarget(iZ, iA);
    pModel->SetParticle(iPID);
    pModel->SetPars(fPars, nPars);

    return (fStatus = kOK);
}

double G2PPhys::GetXS(double Ei, double Ef, double theta)
{
    return pModel->GetXS(Ei, Ef, theta);
}
