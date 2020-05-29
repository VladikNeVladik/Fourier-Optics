// No Copyright. Vladislav Aleinik && Konstantin Nazarov 2020

#include "vendor/cnpy/cnpy.h"

#include "Model.hpp"

#include <cstdlib>
#include <cstdio>

#include <fenv.h> 

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: model <output-file>");
		exit(EXIT_FAILURE);
	}

	// Initialize data points:
	double* data_points = (double*) calloc(SIZE_Z * 2, sizeof(*data_points));
	if (data_points == nullptr)
	{
		printf("Unable to allocate memory for data points");
		exit(EXIT_FAILURE);
	}

	// Initialise roots of unity
	std::complex<double>* roots_of_unity_str = (std::complex<double>*) calloc(LOG_X, sizeof(*roots_of_unity_str));
	if (roots_of_unity_str == nullptr)
	{
		printf("Unable to allocate memory for straight roots of unity");
		exit(EXIT_FAILURE);
	}

	std::complex<double>* roots_of_unity_rev = (std::complex<double>*) calloc(LOG_X, sizeof(*roots_of_unity_rev));
	if (roots_of_unity_rev == nullptr)
	{
		printf("Unable to allocate memory for reversed roots of unity");
		exit(EXIT_FAILURE);
	}

	for (uint32_t level = 0; level < LOG_X; ++level)
	{
		roots_of_unity_str[level] = std::polar(1.0, +(2.0 * M_PI)/(2 << level));
		roots_of_unity_rev[level] = std::polar(1.0, -(2.0 * M_PI)/(2 << level));
	}

	// Initialise complex amplitudes:
	std::complex<double>* amps = (std::complex<double>*) calloc(SIZE_X, sizeof(*amps));
	if (amps == nullptr)
	{
		printf("Unable to allocate memory for complex amplitudes");
		exit(EXIT_FAILURE);
	}

	for (uint32_t x = 0; x < SIZE_X; ++x)
	{
		amps[x] = initial(x);
	}

	// Initialise spectrum:
	std::complex<double>* spectrum = (std::complex<double>*) calloc(SIZE_X, sizeof(*spectrum));
	if (spectrum == nullptr)
	{
		printf("Unable to allocate memory for spectrum");
		exit(EXIT_FAILURE);
	}

	// Initialise phase errors and phase error spectrum:
	std::complex<double>* errors = (std::complex<double>*) calloc(SIZE_X, sizeof(*spectrum));
	if (spectrum == nullptr)
	{
		printf("Unable to allocate memory for errors");
		exit(EXIT_FAILURE);
	}

	std::complex<double>* error_spectrum = (std::complex<double>*) calloc(SIZE_X, sizeof(*spectrum));
	if (spectrum == nullptr)
	{
		printf("Unable to allocate memory for error spectrum");
		exit(EXIT_FAILURE);
	}

	// Delete previous saved data:
	cnpy::npy_save(argv[1], data_points, {0, SIZE_X, 2}, "w");

	// Set rounding mode:
	if (fesetround(FE_TOWARDZERO) != 0)
	{
		printf("Unable to update rounding mode");
		exit(EXIT_FAILURE);
	}

	// Perform computation:
	for (uint32_t z = 0; z < SIZE_Z; ++z)
	{
		printf("Computed %7.2fm out of %7.2fm\r", z * WAVELENGTH, SIZE_Z * WAVELENGTH);
		fflush(stdout);

		// Perform computations on sub-wave scale:
		for (uint32_t wave_z = 0; wave_z < WAVE_SIZE_Z; ++wave_z)
		{
			// Obtain spatial spectrum
			fast_fourier_transform(amps, spectrum, roots_of_unity_rev);

			// Perform propagation in the medium:
			propagate(spectrum);

			// Return back to the filed values:
			fast_fourier_transform(spectrum, amps, roots_of_unity_str);

			// Apply amplitude and phase screen: 
			apply_screen(amps, z, wave_z);

			// Add errors:
			// phase_error_spectrum(error_spectrum);
			// fast_fourier_transform(error_spectrum, errors, roots_of_unity_str);

			// for (uint32_t x = 0; x < SIZE_X; ++x)
			// {
			// 	amps[x] *= std::polar(1.0, std::real(errors[x]));
			// }
		}

		// Aggregate results:
		if (z % AGGREGATE_OVER == 0)
		{
			for (uint32_t x = 0; x < SIZE_X; ++x)
			{
				data_points[2 * x + 0] = std::norm(amps[x]);
				data_points[2 * x + 1] = std::arg (amps[x]);	
			}

			// Save aggregated data:
			cnpy::npy_save(argv[1], data_points, {1, SIZE_X, 2}, "a");
		}
	}

	// Free all memory:
	free(data_points);

	free(roots_of_unity_str);
	free(roots_of_unity_rev);

	free(amps);
	free(spectrum);

	free(errors);
	free(error_spectrum);

	return EXIT_SUCCESS;
}