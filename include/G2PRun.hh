// -*- C++ -*-

/* class G2PRun
 * Run manager for g2p simulation.
 * Parse the configuration file and store all run parameters.
 * It use libconfig, a 3rd party package, to do the parsing.
 */

// History:
//   Jan 2013, C. Gu, First public version.
//   May 2013, C. Gu, Add G2PProcBase classes, G2PRunBase is more general.
//   Sep 2013, C. Gu, Combine G2PRunBase and G2PRun to be the new run manager.
//   Nov 2014, J. Liu, Add target type and field type.
//

#ifndef G2P_RUN_H
#define G2P_RUN_H

#include <cstring>
#include <map>
#include <set>

#include "TObject.h"

#include "libconfig.h"

#include "G2PVarDef.hh"

using namespace std;

class TTree;
class G2PRand;

class G2PRun : public TObject
{
public:
    G2PRun();
    virtual ~G2PRun();

    virtual int Begin();
    virtual int End();
    virtual void Clear(Option_t *opt = "");

    virtual void Save() const;
    virtual void Print(Option_t *opt = "") const;

    int GetConfig(const ConfDef *item, const char *prefix);
    int SetConfig(const ConfDef *item, const char *prefix);

    //Sets
    void SetConfigFile(const char *file);
    void SetDebugLevel(int n);
    void SetSeed(unsigned n);
    void SetBeamEnergy(double value);
    void SetParticle(int id);
    void SetTargetType(const char *type);
    void SetTarget(int Z, int A);
    void SetTargetMass(double M);
    void SetTargetPF(double pf);
    void SetHRSAngle(double angle);
    void SetHRSMomentum(double P0);
    void SetFieldType(const char *type);
    void SetFieldRatio(double ratio);

protected:
    int ParseConfigFile();
    int ParseSetting(const char *prefix, const config_setting_t *setting);
    double GetValue(const config_setting_t *setting);

    const char *fConfigFile;

    map<string, double> fConfig;
    set<string> fConfigIsSet;

    TTree *fConfigTree;

private:
    // Random number generator
    static G2PRand *pRand;
    static void StaticSetSeed(unsigned n);

    static G2PRun *pG2PRun;

    ClassDef(G2PRun, 1)
};

#endif
