#ifndef _AbstractDynamicsEngine_h_
#define _AbstractDynamicsEngine_h_

// AbstractDynamicsEngine.h
// Authors: Frank C. Anderson, Ayman Habib, Peter Loan
/*
 * Copyright (c) 2005, Stanford University. All rights reserved. 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions
 * are met: 
 *  - Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *  - Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  - Neither the name of the Stanford University nor the names of its 
 *    contributors may be used to endorse or promote products derived 
 *    from this software without specific prior written permission. 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE. 
 */

// INCLUDES
#include <OpenSim/Simulation/rdSimulationDLL.h>
#include <iostream>
#include <string>
#include <OpenSim/Tools/Storage.h>
#include <OpenSim/Tools/Object.h>

namespace OpenSim { 

class ScaleSet;
class Coordinate;
class IKSolverInterface;
class ScalerInterface;

#ifdef SWIG
	#ifdef RDSIMULATION_API
		#undef RDSIMULATION_API
		#define RDSIMULATION_API
	#endif
#endif


//=============================================================================
//=============================================================================
/**
 * An abstract class that specifies the interface for a kinematics engine.
 * A kinematics engine is used to compute the positions, velocities, and
 * accelerations of bodies and points on bodies in an aticulated linkage.
 *
 * At a minimum, a kinematics engine must contain a description of the
 * topology of the articulated linkage.  That is, how many bodies and how
 * those bodies are connected.
 *
 * @auther Frank C. Anderson, except where noted
 * @version 1.0
 */

class RDSIMULATION_API AbstractDynamicsEngine : public Object
{

//=============================================================================
// DATA
//=============================================================================
protected:
	// PROPERTIES
	
	// REFERENCES


//=============================================================================
// METHODS
//=============================================================================
	//--------------------------------------------------------------------------
	// CONSTRUCTION AND DESTRUCTION
	//--------------------------------------------------------------------------
public:
	AbstractDynamicsEngine();
	AbstractDynamicsEngine(const std::string &aFileName);
	AbstractDynamicsEngine(DOMElement *aElement);
	virtual ~AbstractDynamicsEngine();
protected:
	AbstractDynamicsEngine(const AbstractDynamicsEngine& aKE);
#ifndef SWIG
	AbstractDynamicsEngine& operator=(const AbstractDynamicsEngine &aKE);
#endif
	void setNull();
	void setupProperties();

public:
	//--------------------------------------------------------------------------
	// NUMBERS
	//--------------------------------------------------------------------------
	virtual int getNumBodies() const = 0;
	virtual int getNumJoints() const = 0;
	virtual int getNumCoordinates() const = 0;
	virtual int getNumSpeeds() const = 0;
	virtual int getNumControls() const = 0;
	virtual int getNumContacts() const = 0;
	virtual int getNumStates() const = 0;
	virtual int getNumPseudoStates() const = 0;

	//--------------------------------------------------------------------------
	// NAMES
	//--------------------------------------------------------------------------
public:
	virtual void setBodyName(int aIndex, const std::string &aName) = 0;
	virtual std::string getBodyName(int aIndex) const = 0;
	virtual std::string getCoordinateName(int aIndex) const = 0;
	virtual std::string getSpeedName(int aIndex) const = 0;
	virtual std::string getControlName(int aIndex) const = 0;
	virtual std::string getStateName(int aIndex) const = 0;
	virtual std::string getPseudoStateName(int aIndex) const = 0;

	//--------------------------------------------------------------------------
	// INDICES FROM NAMES
	//--------------------------------------------------------------------------
	virtual int getBodyIndex(const std::string &aName) const = 0;
	virtual int getCoordinateIndex(const std::string &aName) const = 0;
	virtual int getSpeedIndex(const std::string &aName) const = 0;
	virtual int getControlIndex(const std::string &aName) const = 0;
	virtual int getStateIndex(const std::string &aName) const = 0;
	virtual int getPseudoStateIndex(const std::string &aName) const = 0;

	virtual Coordinate* getCoordinate(const std::string &aName) const = 0;
	//--------------------------------------------------------------------------
	// SET CURRENT TIME, CONTROLS, AND STATES
	//--------------------------------------------------------------------------
	virtual void set(double aT, const double aX[], const double aY[]) = 0;

	//--------------------------------------------------------------------------
	// INITIAL STATES
	//--------------------------------------------------------------------------
	virtual void setInitialStates(const double aYI[]) = 0;
	virtual void getInitialStates(double rYI[]) const = 0;
	virtual double getInitialState(int aIndex) const = 0;
	virtual double getInitialState(const std::string &aName) const = 0;

	//--------------------------------------------------------------------------
	// STATES
	//--------------------------------------------------------------------------
	virtual void setStates(const double aY[]) = 0;
	virtual void getStates(double rY[]) const = 0;
	virtual double getState(int aIndex) const = 0;
	virtual double getState(const std::string &aName) const = 0;
	virtual void applyDefaultPose() { }

	//--------------------------------------------------------------------------
	// INITIAL PSEUDO STATES
	//--------------------------------------------------------------------------
	virtual void setInitialPseudoStates(const double aYPI[]) = 0;
	virtual void getInitialPseudoStates(double rYPI[]) const = 0;
	virtual double getInitialPseudoState(int aIndex) const = 0;
	virtual double getInitialPseudoState(const std::string &aName) const = 0;

	//--------------------------------------------------------------------------
	// PSEUDO STATES
	//--------------------------------------------------------------------------
	virtual void setPseudoStates(const double aYP[]) = 0;
	virtual void getPseudoStates(double rYP[]) const = 0;
	virtual double getPseudoState(int aIndex) const = 0;
	//??virtual double getPseudoState(const std::string &aName) const = 0;

	//--------------------------------------------------------------------------
	// CONFIGURATION
	//--------------------------------------------------------------------------
	virtual void setConfiguration(const double aY[]) = 0;
	virtual void setConfiguration(const double aQ[], const double aU[]) = 0;
	virtual void getCoordinateValues(double rQ[]) const = 0;
	virtual double getCoordinateValue(int aIndex) const = 0;
	virtual double getCoordinateValue(const std::string &aName) const = 0;
	virtual void getSpeeds(double rU[]) const = 0;
	virtual double getSpeed(int aIndex) const = 0;
	virtual double getSpeed(const std::string &aName) const = 0;
	virtual void getAccelerations(double rDUDT[]) const = 0; // DYN
	virtual double getAcceleration(int aIndex) const = 0; // DYN
	virtual double getAcceleration(const std::string &aSpeedName) const = 0; // DYN
	virtual void extractConfiguration(const double aY[], double rQ[], double rU[]) const = 0;

	//--------------------------------------------------------------------------
	// ASSEMBLING THE MODEL
	//--------------------------------------------------------------------------
	virtual int assemble(double aTime, double *rState, int *aLock, double aTol, int aMaxevals, int *rFcnt, int *rErr) = 0;	//DYN

	//--------------------------------------------------------------------------
	// SCALE THE MODEL
	//--------------------------------------------------------------------------
	virtual bool scale(const ScaleSet& aScaleSet) = 0;
	virtual bool scale(const ScaleSet& aScaleSet, bool aPreserveMassDist, double aFinalMass) = 0;

	//--------------------------------------------------------------------------
	// GRAVITY
	//--------------------------------------------------------------------------
	virtual void getGravity(double rGrav[3]) const = 0; // DYN
	virtual void setGravity(double aGrav[3]) = 0; // DYN

	//--------------------------------------------------------------------------
	// BODY INFORMATION
	//--------------------------------------------------------------------------
	virtual int getGroundBodyIndex() const = 0;
	virtual void setBodyToJointBodyLocal(int abody, const double aBTJ[3]) = 0;
	virtual void getBodyToJointBodyLocal(int aBody, double rBTJ[3]) const = 0;
	virtual void setInboardToJointBodyLocal(int aBody, const double aBTJ[3]) = 0;
	virtual void getInboardToJointBodyLocal(int aBody, double rBTJ[3]) const = 0;

	//--------------------------------------------------------------------------
	// INERTIA	// DYN
	//--------------------------------------------------------------------------
	virtual double getMass() const = 0;
	virtual double getMass(int aBody) const = 0;
	virtual int getInertiaBodyLocal(int aBody, double rI[3][3]) const = 0;
	virtual int	getInertiaBodyLocal(int aBody, double *rI) const = 0;
	virtual void getSystemInertia(double *rM, double rCOM[3], double rI[3][3]) const = 0;
	virtual void getSystemInertia(double *rM, double *rCOM, double *rI) const = 0;

	//--------------------------------------------------------------------------
	// KINEMATICS
	//--------------------------------------------------------------------------
	virtual void getPosition(int aBody, const double aPoint[3], double rPos[3]) const = 0;
	virtual void getVelocity(int aBody, const double aPoint[3], double rVel[3]) const = 0;
	virtual void getAcceleration(int aBody, const double aPoint[3], double rAcc[3]) const = 0;
	virtual void getDirectionCosines(int aBody, double rDirCos[3][3]) const = 0;
	virtual void getDirectionCosines(int aBody, double *rDirCos) const = 0;
	virtual void getAngularVelocity(int aBody, double rAngVel[3]) const = 0;
	virtual void getAngularVelocityBodyLocal(int aBody, double rAngVel[3]) const = 0;
	virtual void getAngularAcceleration(int aBody, double rAngAcc[3]) const = 0;
	virtual void getAngularAccelerationBodyLocal(int aBody, double rAngAcc[3]) const = 0;

	//--------------------------------------------------------------------------
	// LOAD APPLICATION
	//--------------------------------------------------------------------------
	// FORCES EXPRESSED IN INERTIAL FRAME
	virtual void applyForce(int aBody, const double aPoint[3], const double aForce[3]) = 0;
	virtual void applyForces(int aN, const int aBodies[], const double aPoints[][3], const double aForces[][3]) = 0;
	virtual void applyForces(int aN, const int aBodies[], const double *aPoints, const double *aForces) = 0;

	// FORCES EXPRESSED IN BODY-LOCAL FRAME
	virtual void applyForceBodyLocal(int aBody, const double aPoint[3], const double aForce[3]) = 0;
	virtual void applyForcesBodyLocal(int aN, const int aBodies[], const double aPoints[][3], const double aForces[][3]) = 0;
	virtual void applyForcesBodyLocal(int aN, const int aBodies[], const double *aPoints, const double *aForces) = 0;

	// TORQUES EXPRESSED IN INERTIAL FRAME
	virtual void applyTorque(int aBody, const double aTorque[3]) = 0; // DYN
	virtual void applyTorques(int aN, const int aBodies[], const double aTorques[][3]) = 0; // DYN
	virtual void applyTorques(int aN, const int aBodies[], const double *aTorques) = 0; // DYN

	// TORQUES EXPRESSED IN BODY-LOCAL FRAME (sdbodyt())
	virtual void applyTorqueBodyLocal(int aBody, const double aTorque[3]) = 0; // DYN
	virtual void applyTorquesBodyLocal(int aN, const int aBodies[], const double aTorques[][3]) = 0; // DYN
	virtual void applyTorquesBodyLocal(int aN, const int aBodies[], const double *aTorques) = 0; // DYN

	// GENERALIZED FORCES
	virtual void applyGeneralizedForce(int aU, double aF) = 0;
	virtual void applyGeneralizedForces(const double aF[]) = 0;
	virtual void applyGeneralizedForces(int aN, const int aU[], const double aF[]) = 0;

	//--------------------------------------------------------------------------
	// LOAD ACCESS AND COMPUTATION
	//--------------------------------------------------------------------------
	virtual double getNetAppliedGeneralizedForce(int aU) const = 0; // DYN
	virtual void computeGeneralizedForces(double aDUDT[], double rF[]) const = 0; // DYN
	virtual void computeReactions(double rForces[][3], double rTorques[][3]) const = 0; // DYN

	//--------------------------------------------------------------------------
	// EQUATIONS OF MOTION
	//--------------------------------------------------------------------------
	virtual void formMassMatrix(double *rI) = 0; // DYN
	virtual void formEulerTransform(int aBody, double *rE) const = 0; // DYN
	virtual void formJacobianTranslation(int aBody, const double aPoint[3], double *rJ, int aRefBody=-1) const = 0; // DYN
	virtual void formJacobianOrientation(int aBody, double *rJ0, int aRefBody=-1) const = 0; // DYN
	virtual void formJacobianEuler(int aBody, double *rJE, int aRefBody=-1) const = 0; // DYN

	//--------------------------------------------------------------------------
	// DERIVATIVES
	//--------------------------------------------------------------------------
	virtual int computeAccelerations(double *dqdt, double *dudt) = 0; // DYN
	virtual void computeAuxiliaryDerivatives(double *dydt) = 0;  // DYN

	//--------------------------------------------------------------------------
	// UTILITY
	//--------------------------------------------------------------------------
	virtual void transform(int aBody1, const double aVec1[3], int aBody2, double rVec2[3]) const = 0;
	virtual void transformPosition(int aBody, const double aPos[3], double rPos[3]) const = 0;

	virtual void convertQuaternionsToAngles(double *aQ, double *rQAng) const = 0;
	virtual void convertQuaternionsToAngles(Storage *rQStore) const = 0;
	virtual void convertAnglesToQuaternions(double *aQAng, double *rQ) const = 0;
	virtual void convertAnglesToQuaternions(Storage *rQStore) const = 0;

	virtual void convertRadiansToDegrees(double *aQRad, double *rQDeg) const = 0;
	virtual void convertRadiansToDegrees(Storage *rQStore) const = 0;
	virtual void convertDegreesToRadians(double *aQDeg, double *rQRad) const = 0;
	virtual void convertDegreesToRadians(Storage *rQStore) const = 0;

	virtual void convertAnglesToDirectionCosines(double aE1, double aE2, double aE3, double rDirCos[3][3]) const = 0;
	virtual void convertAnglesToDirectionCosines(double aE1, double aE2, double aE3, double *rDirCos) const = 0;

	virtual void convertDirectionCosinesToAngles(double aDirCos[3][3], double *rE1, double *rE2, double *rE3) const = 0;
	virtual void convertDirectionCosinesToAngles(double *aDirCos, double *rE1, double *rE2, double *rE3) const = 0;

	virtual void convertDirectionCosinesToQuaternions(double aDirCos[3][3],	double *rQ1, double *rQ2, double *rQ3, double *rQ4) const = 0;
	virtual void convertDirectionCosinesToQuaternions(double *aDirCos, double *rQ1, double *rQ2, double *rQ3, double *rQ4) const = 0;

	virtual void convertQuaternionsToDirectionCosines(double aQ1, double aQ2, double aQ3, double aQ4, double rDirCos[3][3]) const = 0;
	virtual void convertQuaternionsToDirectionCosines(double aQ1, double aQ2, double aQ3, double aQ4, double *rDirCos) const = 0;

	//--------------------------------------------------------------------------
	// CONTACT
	//--------------------------------------------------------------------------
	virtual void computeContact() = 0;
	virtual void applyContactForce(int aID) = 0; // DYN
	virtual void applyContactForces() = 0; // DYN
	virtual int getContactBodyA(int aID) const = 0;
	virtual int getContactBodyB(int aID) const = 0;
	virtual void setContactPointA(int aID, const double aPoint[3]) = 0;
	virtual void getContactPointA(int aID, double rPoint[3]) const = 0;
	virtual void setContactPointB(int aID, const double aPoint[3]) = 0;
	virtual void getContactPointB(int aID, double rPoint[3]) const = 0;
	virtual void getContactForce(int aID, double rF[3]) const = 0;
	virtual void getContactNormalForce(int aID, double rFP[3], double rFV[3], double rF[3]) const = 0;
	virtual void getContactTangentForce(int aID, double rFP[3], double rFV[3], double rF[3]) const = 0;
	virtual void getContactStiffness(int aID, const double aDX[3], double rDF[3]) const = 0;
	virtual void getContactViscosity(int aID, const double aDV[3], double rDF[3]) const = 0;
	virtual void getContactFrictionCorrection(int aID, double aDFFric[3]) const = 0;
	virtual double getContactForce(int aID) const = 0;
	virtual double getContactSpeed(int aID) const = 0;
	virtual double getContactPower(int aID) const = 0;

//=============================================================================
};	// END of class AbstractDynamicsEngine

}; //namespace
//=============================================================================
//=============================================================================

#endif // __AbstractDynamicsEngine_h__


