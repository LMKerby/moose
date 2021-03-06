/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "PeriodicDistanceAux.h"
#include "GeneratedMesh.h"

template<>
InputParameters validParams<PeriodicDistanceAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredParam<Point>("point", "Some point in the domain");
  return params;
}

PeriodicDistanceAux::PeriodicDistanceAux(const InputParameters & parameters) :
    AuxKernel(parameters),
    _point(getParam<Point>("point"))
{
  // Make sure the point is in the domain
  for (unsigned int i=0; i<LIBMESH_DIM; ++i)
    if (_point(i) < _mesh.getMinInDimension(i) || _point(i) > _mesh.getMaxInDimension(i))
    {
      _console << _mesh.getMinInDimension(i) << "\t" << _mesh.getMaxInDimension(i) << "\n";
      mooseError("\"point\" is outside of the domain.");
    }
}

PeriodicDistanceAux::~PeriodicDistanceAux()
{
}

Real
PeriodicDistanceAux::computeValue()
{
  // Compute the periodic distance from a given feature
  // Note: For this test kernel we are just going to use the first nonlinear variable (index: 0)
  return _mesh.minPeriodicDistance(0, *_current_node, _point);
}
