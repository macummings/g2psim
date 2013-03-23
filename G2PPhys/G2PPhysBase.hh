#ifndef G2P_PHYSBASE_H
#define G2P_PHYSBASE_H

#include <vector>

using namespace std;

class G2PPhysBase
{
public:
    G2PPhysBase();
    virtual ~G2PPhysBase();

    void SetTarget(int Z, int A);
    void SetTargetMass(double value) { fTargetMass = value; }
    void SetParticle(int pid) { iPID = pid; }

    virtual void SetPars(double* array, int n);

    virtual double GetXS(double Ei, double Ef, double theta) = 0;
    
protected:
    void SetTargetMass();

    int iZ, iA; // Define Target
    double fTargetMass;

    int iPID; // Define particle

    vector<double> fPars;
};

#endif

/*
This function is provided to calculate quasi-elastic and resonance cross section for different kinds of particles
 Use EPC model for proton, neutron, pions
 Use QFS model for electron
 Do not support positron any more
 Use a subroutine to calculate photons
  The photon cross section is calculated from the pi0 decay so it could not be used with pi0 simultaneously
 
Usage: There are several forms of this getQElXS function
 getQElXS(PID,Z,N,Eb,theta,pf);
 getQElXS(PID,Z,N,Eb,theta,pf,EPS,EPSD,FP);
 getQElXS(PID,Z,N,Eb,theta,pf,Tb,Ta);
 getQElXS(PID,Z,N,Eb,theta,pf,EPS,EPSD,FP,Tb,Ta);
 
Meaning of parameters:
 PID: praticle ID, following the PDG definition
     =	2212	for p		;	2112	for n	;	211	for pi+	;
		-211	for pi-		;	111		for pi0	;	11	for e-	;
		22		for photon	;
 Z,N: proton and neutron number of the nucleus.
 Eb: incoming electron energy in GeV;
 theta: scattering angle for outgoing particle in radian;
 pf: outgoing particle momentum in GeV/c;
 
 EPS,EPSD,FP: nucleus parameters, which used in the QFS code. EPS - seperation energy in MeV, 
 EPSD - delta seperation energy in MeV, FP - Fermi momentum in MeV/c; their values can be changed 
 to fit experimetal results, they will be set with the recommended value if not provided (EPS=10, EPSD=-10, FP=220);
 Tb,Ta: target parameters, Tb - total radiative length before scattering, Ta - total radiative 
 length after scattering, both in the unit of Radiation Length, if they are not provided, they will 
 be set with 0, which means not taking target radiative length into account;
*/
