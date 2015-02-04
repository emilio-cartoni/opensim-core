/* -------------------------------------------------------------------------- *
 *                           OpenSim:  Actuator.cpp                           *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2012 Stanford University and the Authors                *
 * Author(s): Ajay Seth                                                       *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

//=============================================================================
// INCLUDES
//=============================================================================
#include <OpenSim/Simulation/Model/Model.h>
#include "Actuator.h"


using namespace std;
using namespace OpenSim;
using namespace SimTK;

//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/**
 * Construct an actuator of controls.
 * This is the base class which generates a vector of actuator controls.
 *
 */
Actuator::Actuator()
{
    setNull();
}


//=============================================================================
// CONSTRUCTION METHODS
//=============================================================================
//_____________________________________________________________________________
/**
 * Set the data members of this Actuator to their null values.
 */
void Actuator::setNull()
{
    setAuthors("Ajay Seth");
    _controlIndex = -1;
}

// Create the underlying computational system component(s) that support the
// Actuator model component
void Actuator::extendAddToSystem(SimTK::MultibodySystem& system) const
{
    Super::extendAddToSystem(system);
    // Beyond the const Component get the index so we can access the SimTK::Force later
    Actuator* mutableThis = const_cast<Actuator *>(this);

    // Model is in charge of creating the shared cache for all all actuator controls
    // but it does so based on the size and order in its _defaultControls
    // Actuator has the opportunity here to add slots for its control and record
    // the index into the shared cache Vector.
    mutableThis->_controlIndex = _model->updDefaultControls().size();
    _model->updDefaultControls().resizeKeep(_controlIndex + numControls());
    _model->updDefaultControls()(_controlIndex, numControls()) = Vector(numControls(), 0.0);
}


//_____________________________________________________________________________
/**
 * Update the geometric representation of the Actuator if any.
 * The resulting geometry is maintained at the VisibleObject layer
 * 
 */
void Actuator::updateGeometry()
{
}

// CONTROLS
//_____________________________________________________________________________
/**
 * Get the actuator's control values
 *
 * @param the current state
 * @return The value of the controls.
 */
const VectorView_<Real> Actuator::getControls( const SimTK::State& s ) const
{
    const Vector &controlsCache = _model->getControls(s);
    return  controlsCache(_controlIndex, numControls());
}

void Actuator::getControls(const Vector& modelControls, Vector& actuatorControls) const
{
    SimTK_ASSERT(modelControls.size() == _model->getNumControls(), 
        "Actuator::getControls, input modelControls size does not match model.getNumControls().\n");
    SimTK_ASSERT(actuatorControls.size() == numControls(), 
        "Actuator::getControls, output actuatorControls incompatible with actuator's numControls().\n");
    actuatorControls = modelControls(_controlIndex, numControls());
}

void Actuator::setControls(const Vector& actuatorControls, Vector& modelControls) const
{
    SimTK_ASSERT(actuatorControls.size() == numControls(), 
        "Actuator::setControls, input actuatorControls incompatible with actuator's numControls().\n");

    SimTK_ASSERT(modelControls.size() == _model->getNumControls(), 
    "Actuator::setControls, output modelControls size does not match model.getNumControls()\n");

    modelControls(_controlIndex, numControls()) = actuatorControls;
}

void Actuator::addInControls(const Vector& actuatorControls, Vector& modelControls) const
{
    SimTK_ASSERT(actuatorControls.size() == numControls(), 
        "Actuator::addInControls, input actuatorControls incompatible with actuator's numControls()\n");

    SimTK_ASSERT(modelControls.size() == _model->getNumControls(), 
    "Actuator::addInControls, output modelControls size does not match model.getNumControls().\n");

    modelControls(_controlIndex, numControls()) += actuatorControls;
}




//=============================================================================
// Scalar Actuator Implementation
//=============================================================================
//_____________________________________________________________________________

/** Default constructor */
ScalarActuator::ScalarActuator()
{
    constructInfrastructure();
}

/**
 * Set up the serializable member variables. This involves constructing and
 * initializing properties.
 */
void ScalarActuator::constructProperties()
{
    constructProperty_min_control(-Infinity);
    constructProperty_max_control( Infinity);
}

void ScalarActuator::constructOutputs() 
{
    constructOutput<double>("actuation", &ScalarActuator::getActuation, SimTK::Stage::Velocity);
    constructOutput<double>("speed", &ScalarActuator::getSpeed, SimTK::Stage::Velocity);
}

// Create the underlying computational system component(s) that support the
// ScalarActuator model component
void ScalarActuator::extendAddToSystem(SimTK::MultibodySystem& system) const
{
    Super::extendAddToSystem(system);
    // Add modeling flag to compute actuation with dynamic or by-pass with 
    // override actuation provided
    addModelingOption("override_actuation", 1);

    // Cache the computed actuation and speed of the scalar valued actuator
    addCacheVariable<double>("actuation", 0.0, Stage::Velocity);
    addCacheVariable<double>("speed", 0.0, Stage::Velocity);

    // Discrete state variable is the override actuation value if in override mode
    addDiscreteVariable("override_actuation", Stage::Time);
}

double ScalarActuator::getControl(const SimTK::State& s) const
{
    return getControls(s)[0];
}

double ScalarActuator::getStress(const SimTK::State& s) const
{
    OPENSIM_ERROR_IF_NOT_OVERRIDDEN();
}

double ScalarActuator::getOptimalForce() const
{
    OPENSIM_ERROR_IF_NOT_OVERRIDDEN();
}

double ScalarActuator::getActuation(const State &s) const
{
    if (isDisabled(s)) return 0.0;
    return getCacheVariableValue<double>(s, "actuation");
}

void ScalarActuator::setActuation(const State& s, double aActuation) const
{
    setCacheVariableValue<double>(s, "actuation", aActuation);
}

double ScalarActuator::getSpeed(const State& s) const
{
    return getCacheVariableValue<double>(s, "speed");
}

void ScalarActuator::setSpeed(const State &s, double speed) const
{
    setCacheVariableValue<double>(s, "speed", speed);
}

void ScalarActuator::overrideActuation(SimTK::State& s, bool flag) const
{
    setModelingOption(s, "override_actuation", int(flag));
}

bool ScalarActuator::isActuationOverriden(const SimTK::State& s) const
{
    return (getModelingOption(s, "override_actuation") > 0);
}
       
void ScalarActuator::setOverrideActuation(SimTK::State& s, double actuation) const
{
    setDiscreteVariableValue(s, "override_actuation", actuation);;
}

double ScalarActuator::getOverrideActuation(const SimTK::State& s) const
{
    return getDiscreteVariableValue(s, "override_actuation");
}
double ScalarActuator::computeOverrideActuation(const SimTK::State& s) const
{
    double appliedActuation = getOverrideActuation(s);
    setActuation(s, appliedActuation);
    return appliedActuation;
}