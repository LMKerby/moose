/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "RecomputeGeneralReturn.h"

template<>
InputParameters validParams<RecomputeGeneralReturn>()
{
  InputParameters params = validParams<Material>();
  params.addParam<unsigned int>("max_iterations", 30, "Maximum number of newton iterations in the radial return material");
  params.addParam<std::string>("base_name", "Optional parameter that allows the user to define multiple mechanics material systems on the same block, i.e. for multiple phases");
  params.set<bool>("compute") = false; //The return stress increment classes are intended to be iterative materials
  return params;
}


RecomputeGeneralReturn::RecomputeGeneralReturn(const InputParameters & parameters) :
    Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : "" ),
    _inelastic_strain_increment(declareProperty<RankTwoTensor>("inelastic_strain_increment")),
    _max_its(parameters.get<unsigned int>("max_iterations")),
    _elasticity_tensor(getMaterialPropertyByName<RankFourTensor>(_base_name + "elasticity_tensor")),
    _strain_increment(getMaterialProperty<RankTwoTensor>(_base_name + "strain_increment")),
    _elastic_strain_old(getMaterialPropertyOldByName<RankTwoTensor>(_base_name + "elastic_strain"))
{
}

void
RecomputeGeneralReturn::resetQpProperties()
{
  // Values here to be set to a constant, ideally zero, as in the initQpProperties method of non-discrete materials
  _inelastic_strain_increment[_qp].zero();
}

void
RecomputeGeneralReturn::computeQpProperties()
{
  // Nothing to update during the first time step, return immediately
  if (_t_step == 0)
    return;

  // Call the specific iterative material model to compute the inelastic strain increment
  computeInelasticStrainIncrement();
}
