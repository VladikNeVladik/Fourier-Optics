// No Copyright. Vladislav Aleinik && Konstantin Nazarov 2020
//=============================================================================
// Multiple Phase Screen Simulation Of Radiowave Propagation In The Atmosphere
//=============================================================================
// - Calculates Wave Propagation Through Some Medium
//=============================================================================
#ifndef FOURIER_OPTICS_MODEL_HPP_INCLUDED
#define FOURIER_OPTICS_MODEL_HPP_INCLUDED

#include "Experiments.hpp"

#include <cstdlib>
#include <complex>
#include <cmath>
#include <cstring>

void fast_fourier_transform(std::complex<double>* src, std::complex<double>* dst, std::complex<double>* roots_of_unity)
{
	memcpy(dst, src, SIZE_X * sizeof(std::complex<double>));

	// Teminology:
	// - L = LEVEL
	// - G = GROUP
	// - E = ENTRY
	//---------------------------------
	// 0  x        x         x         
	// 1  | x      | x       x         
	// 2  | | x    x |          x      
	// 3  | | | x    x          x      
	// 4  x | | |       x          x   
	// 5    x | |       | x        x   
	// 6      x |       x |           x
	// 7        x         x           x
	//---------------------------------
	//       L0    L1        L2
	//       G0    G0   G1   G0 G1 G2 G3
	//   E E E E   E E  E E  E  E  E  E

	// Iterate over levels:
	for (uint32_t level = 0; level < LOG_X; ++level)
	{	
		// Level characteristics:
		uint32_t group_size = SIZE_X >> level;
		uint32_t entry_size = group_size >> 1;		

		// Twiddles:
		std::complex<double> level_twiddle = roots_of_unity[level];
		std::complex<double> group_twiddle = 1.0;

		// Iterate over groups of a level:
		for (uint32_t group_x = 0; group_x < SIZE_X; group_x += group_size)
		{
			// Iterate over entries of a group:
			for (uint32_t entry_x = 0; entry_x < entry_size; entry_x += 1)
			{
				std::complex<double> not_twiddled = dst[group_x + entry_x + 0];
				std::complex<double> twiddled     = dst[group_x + entry_x + entry_size] * group_twiddle;

				dst[group_x + entry_x +          0] = not_twiddled + twiddled;
				dst[group_x + entry_x + entry_size] = not_twiddled - twiddled;
			}

			group_twiddle *= level_twiddle;
		}
	}

	// Perform reversed-bit-order swap
	for (uint32_t x = 0; x < SIZE_X; ++x)
	{
		uint32_t rev_x = 0;

		rev_x |= (x & 0x001) << 11;
		rev_x |= (x & 0x002) <<  9;
		rev_x |= (x & 0x004) <<  7;
		rev_x |= (x & 0x008) <<  5;
		rev_x |= (x & 0x010) <<  3;
		rev_x |= (x & 0x020) <<  1;
		rev_x |= (x & 0x040) >>  1;
		rev_x |= (x & 0x080) >>  3;
		rev_x |= (x & 0x100) >>  5;
		rev_x |= (x & 0x200) >>  7;
		rev_x |= (x & 0x400) >>  9;
		rev_x |= (x & 0x800) >> 11;
		
		if (x < rev_x)
		{
			std::complex<double> tmp = dst[x];
			dst[x]     = dst[rev_x];
			dst[rev_x] = tmp;
		}
	}
}

void propagate(std::complex<double>* spectrum)
{
	for (uint32_t j = 0; j < SIZE_X; ++j)
	{
		// Fix the missing 1/N in the straight-FFT
		spectrum[j] /= SIZE_X;

		std::complex<double>  k_x = WAVELENGTH / LENGTH_X * j;
		std::complex<double> sqrt = std::conj(std::sqrt(1.0 - k_x*k_x) / ((double) WAVE_SIZE_Z));

		spectrum[j] *= std::exp(std::complex<double>(0.0, 1.0) * sqrt);
	}
}

void apply_screen(std::complex<double>* amps, uint32_t z, uint32_t wave_z)
{
	double phase_lst = arg(amps[0]);
	double phase_nxt = arg(amps[0]);
	double phase_cur = 0;
	for (uint32_t x = 0; x < SIZE_X; ++x)
	{
		// Optical density:
		double n0 = optical_density_0(x, z, wave_z);
		double n2 = optical_density_2(x, z, wave_z);
		double n = n0 + n2 * norm(amps[x]);

		// Phases:
		phase_cur = phase_nxt;
		if (x + 1 == SIZE_X) phase_nxt = phase_lst;
		else                 phase_nxt = arg(amps[x + 1]);

		// Spatial spectrum component:
		double                  j = (phase_nxt - phase_cur) * SIZE_X/(2.0 * M_PI);
		std::complex<double>  k_x = WAVELENGTH / LENGTH_X * j;
		std::complex<double> sqrt = std::conj(std::sqrt(1.0 - k_x*k_x));

		// Account turbulence:
		amps[x] *= std::exp(std::complex<double>(0.0, 1.0) * (n - 1) * sqrt);

		// Account transition properties of media:
		amps[x] *= transition_function(x, z, wave_z);
	}
}

#endif // FOURIER_OPTICS_MODEL_HPP_INCLUDED