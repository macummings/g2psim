// -*- C++ -*-

/* class G2PGeoSub
 * Defines the subtraction of geometries.
 * It derives from G2PGeoBase.
 */

// History:
//   Jan 2015, C. Gu, Add this class to g2p geometries.
//

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "TROOT.h"
#include "TError.h"
#include "TObject.h"

#include "G2PAppList.hh"
#include "G2PGeoBase.hh"

#include "G2PGeoSub.hh"

G2PGeoSub::G2PGeoSub()
{
    // Nothing to do
}

G2PGeoSub::G2PGeoSub(G2PGeoBase *geo) : fMinuend(geo)
{
    fSubGeos = new G2PAppList();
}

G2PGeoSub::~G2PGeoSub()
{
    TIter next(fSubGeos);

    while (G2PAppBase *aobj = static_cast<G2PAppBase *>(next())) {
        fSubGeos->Remove(aobj);
        aobj->Delete();
    }

    delete fMinuend;
    delete fSubGeos;
}

int G2PGeoSub::Begin()
{
    if (G2PAppBase::Begin() != 0)
        return (fStatus = kBEGINERROR);

    fMinuend->Begin();

    TIter geo_iter(fSubGeos);

    while (G2PGeoBase *geo = static_cast<G2PGeoBase *>(geo_iter())) {
        if (!geo->IsInit()) {
            geo->SetDebugLevel(fDebug);

            if (geo->Begin() != 0)
                return (fStatus = kBEGINERROR);
        }
    }

    return (fStatus = kOK);
}

bool G2PGeoSub::IsInside(const double *V3)
{
    bool result = false;

    if (fMinuend->IsInside(V3))
        result = true;

    TIter next(fSubGeos);

    while (G2PGeoBase *geo = static_cast<G2PGeoBase *>(next())) {
        if (geo->IsInside(V3))
            result = false;
    }

    return result;
}

void G2PGeoSub::Subtract(G2PGeoBase *geo)
{
    fSubGeos->Add(geo);
}

void G2PGeoSub::SetOrigin(double x, double y, double z)
{
    fMinuend->SetOrigin(x, y, z);
}

void G2PGeoSub::SetEulerAngle(double alpha, double beta, double gamma)
{
    fMinuend->SetEulerAngle(alpha, beta, gamma);
}

bool G2PGeoSub::IsInside(double x, double y, double z)
{
    // Nothing to do

    return true;
}

ClassImp(G2PGeoSub)
