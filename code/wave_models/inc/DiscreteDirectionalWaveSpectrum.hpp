/*
 * DiscreteDirectionalWaveSpectrum.hpp
 *
 *  Created on: Jul 31, 2014
 *      Author: cady
 */

#ifndef DISCRETEDIRECTIONALWAVESPECTRUM_HPP_
#define DISCRETEDIRECTIONALWAVESPECTRUM_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include <vector>

class WaveSpectralDensity;
class WaveDirectionalSpreading;

/** \author cec
 *  \date Jul 31, 2014, 1:08:15 PM
 *  \brief Used by the wave models (eg. Airy, Stokes, etc.)
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/DirectionalWaveSpectrumTest.cpp DirectionalWaveSpectrumTest expected output
 */
struct DiscreteDirectionalWaveSpectrum
{
    DiscreteDirectionalWaveSpectrum();

    std::vector<double> Si;
    std::vector<double> Dj;
    std::vector<double> omega;
    std::vector<double> psi;
    std::vector<double> k;
    double domega;
    double dpsi;
};

#endif /* DISCRETEDIRECTIONALWAVESPECTRUM_HPP_ */
