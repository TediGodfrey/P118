#ifndef PROBLEM_GENERATOR_H
#define PROBLEM_GENERATOR_H

#include "enums.h"
#include "global.h"

#include "arch/traits.h"
#include "utils/numeric.h"

#include "archetypes/energy_dist.h"
#include "archetypes/particle_injector.h"
#include "archetypes/problem_generator.h"

#include "framework/domain/domain.h"
#include "framework/domain/metadomain.h"

namespace user {
  using namespace ntt;

  template <SimEngine::type S, class M>
  struct SinEDist : public arch::EnergyDistribution<S, M> {
    SinEDist(const M& metric, real_t v_max, int n1, real_t Lx)
      : arch::EnergyDistribution<S, M>{ metric }
      , v_max{ v_max }
      , kx1{ (Lx > ZERO) ? static_cast<real_t>(constant::TWO_PI) * static_cast<real_t>(n1) / Lx
                        : ZERO } {}

    // NOTE: injector expects (x_Ph, v) -- NO species arg
    Inline void operator()(const coord_t<M::Dim>& x_Ph,
                           vec_t<Dim::_3D>&       v) const {
      v[0] = v_max * math::sin(kx1 * x_Ph[0]);
      v[1] = ZERO;
      v[2] = ZERO;
    }

  private:
    const real_t v_max;
    const real_t kx1;
  };

  template <SimEngine::type S, class M>
  struct PGen : public arch::ProblemGenerator<S, M> {

    static constexpr auto engines    = traits::compatible_with<SimEngine::SRPIC>::value;
    static constexpr auto metrics    = traits::compatible_with<Metric::Minkowski>::value;
    static constexpr auto dimensions = traits::compatible_with<Dim::_1D>::value; // 1D ONLY

    using arch::ProblemGenerator<S, M>::params;

    const real_t Lx;
    const real_t vmax;
    const int    nx1;

    inline PGen(const SimulationParams& p, const Metadomain<S, M>& global_domain)
      : arch::ProblemGenerator<S, M>{ p }
      , Lx{ global_domain.mesh().extent(in::x1).second - global_domain.mesh().extent(in::x1).first }
      , vmax{ p.get<real_t>("setup.vmax", 0.01) }
      , nx1{ p.get<int>("setup.nx1", 4) } {}

    inline void InitPrtls(Domain<S, M>& local_domain) {
      const auto ed1 = SinEDist<S, M>(local_domain.mesh.metric, vmax, nx1, Lx);

      // Make species 2 “quiet” (no perturbation) to start.
      // You can change this later (e.g., opposite phase to keep net current ~0).
      const auto ed2 = arch::Cold<S, M>(local_domain.mesh.metric);

      arch::InjectUniform(params,
                          local_domain,
                          std::pair<spidx_t, spidx_t>{1, 2},
                          std::pair{ed1, ed2},
                          1.0);
    }
  };

} // namespace user

#endif