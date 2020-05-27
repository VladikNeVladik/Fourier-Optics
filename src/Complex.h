// No Copyright. Vladislav Aleinik && Konstantin Nazarov 2020
//=============================================================================
// Multiple Phase Screen Dimulation Of Radiowave Propogation In The Atmosphere
//=============================================================================
// - Calculates Wave Propagation Through Some Medium
//=============================================================================
#ifndef FOURIER_OPTICS_COMPLEX_HPP_INCLUDED
#define FOURIER_OPTICS_COMPLEX_HPP_INCLUDED

struct Complex
{
	// Variables:
	double amp;
	double phase;
};

struct Complex mul(struct Complex a, struct Complex b)
{
	return {a.amp * b.amp, a.phase + b.phase};
}


#endif // FOURIER_OPTICS_COMPLEX_HPP_INCLUDED