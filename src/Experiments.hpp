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

#include <random>

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
	if (z == SIZE_X/64 && wave_z == 0)
	{
		if (7*SIZE_X/16 <= x && x < 9*SIZE_X/16)
		{
			if (x & 0x10) return 1.0;
			else          return 0.0;	
		}
		else
		{
			return 0.0;
		}
	}	
	else
	{
		return 1.0;
	}
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
	if (z == SIZE_X/64 && wave_z == 0)
	{
		if (7*SIZE_X/16 <= x && x < 9*SIZE_X/16)
		{
			return std::polar(1.0, 2.0 * M_PI * (x - 7*SIZE_X/16)/(SIZE_X/64));
		}
		else
		{
			return 0.0;
		}
	}	
	else
	{
		return 1.0;
	}
}

#endif // EXPERIMENT_PHASE_LATTICE

//-------------
// Diffraction
//-------------

#if EXPERIMENT_DIFFRACTION

std::complex<double> initial(uint32_t x)
{
	if (SIZE_X/2 - SIZE_X/16 <= x && x < SIZE_X/2 + SIZE_X/16)
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_DIFFRACTION

//--------------
// Interference
//--------------

#if EXPERIMENT_INTEREFERENCE

std::complex<double> initial(uint32_t x)
{
	int distance_l = x - 7*SIZE_X/16;
	int distance_h = x - (9*SIZE_X/16 - 1);

	if (std::abs(distance_l) < 2 || std::abs(distance_h) < 2)
	{
		return 1.0;
	}
	else
	{
		return 0.0;	
	}
}

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_INTEREFERENCE

//---------------
// Gaussian Lens
//---------------

#if EXPERIMENT_GAUSSIAN_LENS

std::complex<double> initial(uint32_t x) { return 1.0; }

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z)
{ 
	double arg = (x - (SIZE_X/2 - 0.5)) / (SIZE_X/8);

	return 1.0 + exp(-0.5 * arg*arg);
}

double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_GAUSSIAN_LENS

//--------------
// Canalisation
//--------------

#if EXPERIMENT_CANALISATION

std::complex<double> initial(uint32_t x)
{
	double arg = (x - (SIZE_X/2 - 0.5)) / (SIZE_X/32);

	return std::polar(exp(-0.5 * arg*arg));
}

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z) { return   1.0; }
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0001; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

#endif // EXPERIMENT_CANALISATION

//------------
// Turbulence
//------------

#if EXPERIMENT_TURBULENCE

std::complex<double> initial(uint32_t x)
{
	double arg = (x - (SIZE_X/2 - 0.5)) / (SIZE_X/128);

	return std::polar(exp(-0.5 * arg*arg));
}

double optical_density_0(uint32_t x, uint32_t z, uint32_t wave_z)
{
	static std::random_device rd;
	static std::mt19937 gen{rd()};
	static std::normal_distribution<double> gauss{0.0, 0.001};

	return 1.0 + gauss(gen);
}
double optical_density_2(uint32_t x, uint32_t z, uint32_t wave_z) { return 0.0; }

std::complex<double> transition_function(uint32_t x, uint32_t z, uint32_t wave_z) { return 1.0; }

void phase_error_spectrum(std::complex<double>* phase_error_spectrum)
{
	static std::random_device rd;
	static std::mt19937 gen{rd()};
	static std::normal_distribution<double>        gauss{0.0,  1.0};
	static std::uniform_real_distribution<double> linear{8.0, 10.0};

	double rand_real =  gauss(gen);
	double rand_imag =  gauss(gen);
	double randS     = linear(gen); 

	for (uint32_t j = 0; j < SIZE_X; ++j)
	{
		phase_error_spectrum[j] = std::complex<double>(rand_real, rand_imag) * std::sqrt(0.5 * randS * LENGTH_X / (2.0 * M_PI));
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