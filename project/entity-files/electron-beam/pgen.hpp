#ifndef PROBLEM_GENERATOR_H
#define PROBLEM_GENERATOR_H

#include "enums.h"
#include "global.h"

#include "arch/traits.h"
#include "utils/error.h"

#include "archetypes/energy_dist.h"
#include "archetypes/particle_injector.h"
#include "archetypes/problem_generator.h"

#include "framework/domain/domain.h"
#include "framework/domain/metadomain.h"

namespace user {
  using namespace ntt;

  template <SimEngine::type S, class M>
  struct PGen : public arch::ProblemGenerator<S, M> {
    static constexpr auto engines =
      traits::compatible_with<SimEngine::SRPIC>::value;
    static constexpr auto metrics =
      traits::compatible_with<Metric::Minkowski>::value;
    static constexpr auto dimensions =
      traits::compatible_with<Dim::_1D>::value;

    using arch::ProblemGenerator<S, M>::params;

    const real_t global_xmin;
    const real_t global_xmax;

    // background component
    const real_t temperature;
    const real_t drift_ux;
    const real_t number_density;

    // bump component
    const real_t bump_temperature;
    const real_t bump_drift_ux;
    const real_t bump_density;

    const real_t filling_fraction;

    inline PGen(const SimulationParams& p,
                const Metadomain<S, M>& global_domain)
      : arch::ProblemGenerator<S, M>{ p }
      , global_xmin{ global_domain.mesh().extent(in::x1).first }
      , global_xmax{ global_domain.mesh().extent(in::x1).second }
      , temperature{ p.template get<real_t>("setup.temperature", 0.01) }
      , drift_ux{ p.template get<real_t>("setup.drift_ux", 0.0) }
      , number_density{ p.template get<real_t>("setup.number_density", 0.85) }
      , bump_temperature{ p.template get<real_t>("setup.bump_temperature", 0.002) }
      , bump_drift_ux{ p.template get<real_t>("setup.bump_drift_ux", 0.6) }
      , bump_density{ p.template get<real_t>("setup.bump_density", 0.15) }
      , filling_fraction{ p.template get<real_t>("setup.filling_fraction", 1.0) } {

      raise::ErrorIf(filling_fraction <= ZERO || filling_fraction > ONE,
                     "setup.filling_fraction must be in (0, 1]",
                     HERE);

      raise::ErrorIf(temperature < ZERO,
                     "setup.temperature must be non-negative",
                     HERE);

      raise::ErrorIf(bump_temperature < ZERO,
                     "setup.bump_temperature must be non-negative",
                     HERE);

      raise::ErrorIf(number_density < ZERO,
                     "setup.number_density must be non-negative",
                     HERE);

      raise::ErrorIf(bump_density < ZERO,
                     "setup.bump_density must be non-negative",
                     HERE);
    }

    inline void InitPrtls(Domain<S, M>& local_domain) {
      const std::vector<real_t> bg_drift   = { drift_ux, ZERO, ZERO };
      const std::vector<real_t> bump_drift = { bump_drift_ux, ZERO, ZERO };

      const auto bg1 = arch::Maxwellian<S, M>(
        local_domain.mesh.metric,
        local_domain.random_pool(),
        temperature,
        bg_drift);

      const auto bg2 = arch::Maxwellian<S, M>(
        local_domain.mesh.metric,
        local_domain.random_pool(),
        temperature,
        bg_drift);

      const auto bump1 = arch::Maxwellian<S, M>(
        local_domain.mesh.metric,
        local_domain.random_pool(),
        bump_temperature,
        bump_drift);

      const auto bump2 = arch::Maxwellian<S, M>(
        local_domain.mesh.metric,
        local_domain.random_pool(),
        bump_temperature,
        bump_drift);

      const real_t xg_min = global_xmin;
      const real_t xg_max =
        global_xmin + filling_fraction * (global_xmax - global_xmin);

      boundaries_t<real_t> box;
      box.push_back({ xg_min, xg_max });

      // broad background
      arch::InjectUniform<S, M, decltype(bg1), decltype(bg2)>(
        params,
        local_domain,
        { 1, 2 },
        { bg1, bg2 },
        number_density,
        false,
        box);

      // drifting narrow bump
      arch::InjectUniform<S, M, decltype(bump1), decltype(bump2)>(
        params,
        local_domain,
        { 1, 2 },
        { bump1, bump2 },
        bump_density,
        false,
        box);
    }
  };

} // namespace user

#endif