// No Copyright. Vladislav Aleinik && Konstantin Nazarov 2020
//=============================================================================
// MPS Experiments
//=============================================================================
// - Vacuum Propagation
// - Diffraction
// - Interferention
// - Gaussian Lens
// - Self-Canalisation
// - Propagation In Turbulent Medium
//=============================================================================
#ifndef FOURIER_OPTICS_EXPERIMENTS_HPP_INCLUDED
#define FOURIER_OPTICS_EXPERIMENTS_HPP_INCLUDED

#include "Values.hpp"

//--------
// Vacuum
//--------

#if EXPERIMENT_VACUUM

std::complex<double> initial(uint32_t x) { return 0.0; }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_VACUUM

//-------------
// Amp Lattice
//-------------

#if EXPERIMENT_AMP_LATTICE

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z)
{
	if (z == SIZE_Z/8) return std::polar(1 + 0.5 * std::sin(8.0 * M_PI * ((double) x) / SIZE_X), 0.0);
	else               return 1.0;
}

#endif // EXPERIMENT_AMP_LATTICE

//---------------
// Phase Lattice
//---------------

#if EXPERIMENT_PHASE_LATTICE

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z)
{ 
	if (z == SIZE_Z/8) return std::polar(1.0, 16.0 * M_PI * ((double) x) / SIZE_X);
	else               return 1.0;
}

#endif // EXPERIMENT_PHASE_LATTICE

//-------------
// Diffraction
//-------------

#if EXPERIMENT_DIFFRACTION

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z)
{ 
	if (z == SIZE_Z/8 && !(7*SIZE_X/16 < x && x < 9*SIZE_X/16)) return 0.0;
	else                                                        return 1.0;
}

#endif // EXPERIMENT_DIFFRACTION

//--------------
// Interference
//--------------

#if EXPERIMENT_INTEREFERENCE

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z)
{ 
	if (z == SIZE_Z/8 && 7*SIZE_X/16 != x && x != 9*SIZE_X/16) return 0.0;
	else                                                       return 1.0;
}

#endif // EXPERIMENT_INTEREFERENCE

//---------------
// Gaussian Lens
//---------------

#if EXPERIMENT_INTEREFERENCE

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z)
{ 
	double arg = 1.0 * (x - SIZE_X/2) / (SIZE_X/8)

	return 1.0 + exp(-0.5 * arg*arg);
}

double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_INTEREFERENCE

//--------------
// Canalisation
//--------------

#if EXPERIMENT_CANALISATION

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.00000001; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_CANALISATION

//------------
// Turbulence
//------------

#if EXPERIMENT_TURBULENCE

std::complex<double> initial(uint32_t x) { return std::polar(1.0, 0.0); }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

void phase_error_spectrum(std::complex<double>* phase_error_spectrum)
{
	static std::random_device rd;
	static std::mt19937 gen{rd()};
	static std::normal_ditribution<double>         gauss(0.0,  1.0);
	static std::uniform_real_distribution<double> linear(8.0, 10.0);

	double rand_real =  gauss(gen);
	double rand_imag =  gauss(gen);
	double randS     = linear(gen); 

	for (uint32_t j = 0; j < SIZE_X; ++j)
	{
		phase_error_spectrum[j] = std::complex(rand_real, rand_imag) * std::sqrt(0.5 * randS * LENGTH_X / (2.0 * M_PI));
	}
}

#else

void phase_error_spectrum(std::complex<double>* phase_error_spectrum)
{
	for (uint32_t j = 0; j < SIZE_X; ++j)
	{
		phase_error_spectrum[j] = 0.0;
	}
}

#endif // EXPERIMENT_TURBULENCE

#endif // FOURIER_OPTICS_EXPERIMENTS_HPP_INCLUDED