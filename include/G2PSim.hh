// This file defines a class G2PSim.
// This class is the main class for this sim package.
//
// History:
//   Jan 2013, C. Gu, First public version.
//

#ifndef G2P_SIM_H
#define G2P_SIM_H

#include "TObject.h"

class TList;
class G2PRunBase;

class G2PSim : public TObject
{
public:
    G2PSim();
    ~G2PSim();
   
    void SetNEvent(int n) { nEvent = n; }
    void SetSeed(int n);
    void SetDebug(int n) { fDebug = n; }
    void SetOutFile(const char* name) { pOutFile = name; }

    int Init();
    int Begin();
    int End();

    void Run();
    void Run(int n) { nEvent = n; Run(); }

    static G2PSim* GetInstance() { return pG2PSim; }

protected:
    int nEvent;
    int nCounter;

    int fDebug;

    // TFile* pFile;
    const char* pOutFile;
    // TTree* pTree;

    TList* fApps;
    G2PRunBase* pRun;

private:
    static G2PSim* pG2PSim;

    ClassDef(G2PSim, 1)
};

#endif
