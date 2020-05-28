// No Copyright. Vladislav Aleinik && Konstantin Nazarov 2020
//=============================================================
// Values
//=============================================================
// - Known constants and functions to switch dimension systems
//=============================================================
#ifndef FOURIER_OPTICS_VALUES_HPP_INCLUDED
#define FOURIER_OPTICS_VALUES_HPP_INCLUDED

#include <complex>
#include <cmath>
#include <cstdint>

//-----------
// Constants
//-----------

const double LENGTH_X   = 1.0; // meters (L)
const double WAVELENGTH = 0.1; // meters (lambda)   

const uint32_t LOG_X  = 12;         // !!! Remember the reverse-bit-order precedure !!!
const uint32_t SIZE_X = 1 << LOG_X; // <no dim> (N)

const uint32_t WAVE_LOG_Z  = 0;
const uint32_t WAVE_SIZE_Z = 1 << WAVE_LOG_Z; // <no dim> (m)

const uint32_t LOG_Z  = 12;
const uint32_t SIZE_Z = 1 << LOG_Z; // <no dim> (M)

const double FIELD = 1.0;

#endif // FOURIER_OPTICS_VALUES_HPP_INCLUDED
