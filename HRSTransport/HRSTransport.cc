#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <map>

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"

#include "G2PTrans.hh"
#include "G2PTrans400016/G2PTrans400016.hh"
#include "G2PTrans484816/G2PTrans484816.hh"
#include "G2PTrans484816R00/G2PTrans484816R00.hh"
#include "G2PTrans484816R20/G2PTrans484816R20.hh"

#include "HRSTransport.hh"

using namespace std;

//#define DEBUG_HRS_FORWARD
//#define DEBUG_HRS_BACKWARD

ClassImp(HRSTransport);

const double cDeg = TMath::Pi()/180.0;

HRSTransport::HRSTransport()
    :iModelIndex(0), bIsLeftArm(true), fHRSAngle(5.767*cDeg),
     fModelAngle(5.767*cDeg), pModel(NULL)
{
    mModel.clear();
    mModelIndex.clear();

    RegisterModel();
}

HRSTransport::HRSTransport(const char* name)
    :iModelIndex(0), bIsLeftArm(true), fHRSAngle(5.767*cDeg),
     fModelAngle(5.767*cDeg), pModel(NULL)
{
    mModel.clear();
    mModelIndex.clear();

    RegisterModel();
    pModel = mModel[mModelIndex[name]];
    iModelIndex = mModelIndex[name];
    fModelAngle = pModel->GetAngle();
}

HRSTransport::HRSTransport(int setting)
    :iModelIndex(0), bIsLeftArm(true), pModel(NULL)
{
    mModel.clear();
    mModelIndex.clear();

    RegisterModel();
    pModel = mModel[setting];
    iModelIndex = setting;
    fModelAngle = pModel->GetAngle();
}

HRSTransport::~HRSTransport()
{
    map<int, G2PTrans*>::iterator it = mModel.begin();
    while (it!=mModel.end()) {
        delete it->second;
        it++;
    }
    mModel.clear();
    mModelIndex.clear();
}

///////////////////////////////////////////////////////////////////////////
// Transport particles through HRS using SNAKE model
// Use iModelIndex to identify which SNAKE model to be used
// 1: 484816 with shim, 5.65 deg, 3 cm raster, by JJL 
// 2: 403216 with shim, 5.65 deg, SNAKE Model not ready yet 
// 3: 400016 with shim, 5.65 deg, 3 cm raster, by Min
// Index > 10 means test
// 11: 484816 with shim, 5.76 deg, no raster, by Min
// 12: 484816 with shim, 5.65 deg, Wrong Bx, 2 cm raster, by Min
// 13: 484816 no shim, 5.65 deg, Wrong Bx, by JJL (g2p test run)
// May add more HRS packages later
///////////////////////////////////////////////////////////////////////////

void HRSTransport::RegisterModel()
{
    G2PTrans* temp;
    temp = new G2PTrans484816();
    mModelIndex["484816"] = 1;
    mModel[1] = temp;

    temp = new G2PTrans400016();
    mModelIndex["400016"] = 3;
    mModel[3] = temp;

    temp = new G2PTrans484816R00();
    mModelIndex["484816R00"] = 11;
    mModel[11] = temp;

    temp = new G2PTrans484816R20();
    mModelIndex["484816R20"] = 12;
    mModel[12] = temp;
}

bool HRSTransport::Forward(const double* V5_tg, double* V5_fp)
{
    // Definition of variables
    // V5_tg = {x_tg, theta_tg, y_tg, phi_tg, delta@tg};
    // V5_fp = {x_fg, theta_fp, y_fp, phi_fp, delta@tg};
    // delta does not change
    double V5[5];
    
    V5[0] = V5_tg[0];
    V5[1] = V5_tg[1];
    V5[2] = V5_tg[2];
    V5[3] = V5_tg[3];
    V5[4] = V5_tg[4];

#ifdef DEBUG_HRS_FORWARD
	printf("HRSTransport: %e\t%e\t%e\t%e\t%e\n", V5[0], V5[1], V5[2], V5[3], V5[4]);
#endif

    bool bGoodParticle=false;

    if (bIsLeftArm) {
        pModel->CoordsCorrection(fHRSAngle-fModelAngle, V5);
        bGoodParticle = pModel->TransLeftHRS(V5);
        pModel->FPCorrection(V5);
    }
    else {
        pModel->CoordsCorrection(fHRSAngle+fModelAngle, V5);
        bGoodParticle = pModel->TransRightHRS(V5);
        pModel->FPCorrection(V5);
    }

    V5_fp[0] = V5[0];
    V5_fp[1] = V5[1];
    V5_fp[2] = V5[2];
    V5_fp[3] = V5[3];
    V5_fp[4] = V5[4];
    
    return bGoodParticle;
}

bool HRSTransport::Backward(const double* V5_fp, double* V5_tg)
{
    // Definition of variables
    // V5_fp = {x_fp, theta_fp, y_fp, phi_fp, x_tg};
    // V5_tg = {x_tg, theta_tg, y_tg, phi_tg, delta@tg};
    // x_tg does not change
    
    double V5[5];
    
    V5[0] = V5_fp[0];
    V5[1] = V5_fp[1];
    V5[2] = V5_fp[2];
    V5[3] = V5_fp[3];
    V5[4] = V5_fp[4];

#ifdef DEBUG_HRS_BACKWARD
	printf("HRSTransport: %e\t%e\t%e\t%e\t%e\n", V5[0], V5[1], V5[2], V5[3], V5[4]);
#endif
    
    if (bIsLeftArm) {
        pModel->ReconLeftHRS(V5);
        pModel->CoordsCorrection(fModelAngle-fHRSAngle, V5);
    }
    else {
        pModel->ReconRightHRS(V5);
        pModel->CoordsCorrection(-fModelAngle-fHRSAngle, V5);
    }

    V5_tg[0] = V5[0];
    V5_tg[1] = V5[1];
    V5_tg[2] = V5[2];
    V5_tg[3] = V5[3];
    V5_tg[4] = V5[4];

    bool bGoodParticle = false;

    if (V5_tg[4]<1.0) bGoodParticle = true;
    
    return bGoodParticle;
}
