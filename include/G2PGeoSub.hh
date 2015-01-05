// -*- C++ -*-

/* class G2PGeoSub
 * Defines the subtraction of geometries.
 * It derives from G2PGeoBase.
 */

// History:
//   Jan 2015, C. Gu, Add this class to g2p geometries.
//

#ifndef G2P_GEOSUB_H
#define G2P_GEOSUB_H

#include "G2PGeoBase.hh"

class G2PAppList;

class G2PGeoSub : public G2PGeoBase
{
public:
    G2PGeoSub(G2PGeoBase *geo);
    virtual ~G2PGeoSub();

    virtual bool TouchBoundary(double x, double y, double z);

    // Gets

    // Sets

protected:
    G2PGeoSub(); // Only for ROOT I/O

    G2PGeoBase *fMinuend;

    G2PAppList *fSubGeos;

private:
    ClassDef(G2PGeoSub, 1)
};

#endif