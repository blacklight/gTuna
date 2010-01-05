#include <cmath>
#include <fftw3.h>
#include "gtuna.hpp"
#define 	SIZE 	4096

#ifndef M_PI
#define M_PI 3.14159265358979323
#endif

size_t getMaxFreq (vector<double> data)  {
	int i, maxIndex = 1;
	double maxFreq = data[maxIndex];
	
	for (i=2; i < data.size(); i++)  {
		if (data[i] > maxFreq)  {
			maxIndex = i;
			maxFreq = data[i];
		}
	}

	return maxIndex;
}

vector<double> FFT (vector<double> data)  {
	fftw_plan p;
	fftw_complex *dataIn  = (fftw_complex*) fftw_malloc(data.size() * sizeof(fftw_complex));
	fftw_complex *dataOut = (fftw_complex*) fftw_malloc(data.size() * sizeof(fftw_complex));

	for (int i=0; i < data.size(); i++)  {
		dataIn[i][0] = data[i];
		dataIn[i][1] = 0.0;
	}

	p = fftw_plan_dft_1d(data.size(), dataIn, dataOut, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);

	for (int i=0; i < data.size(); i++)  {
		data[i] = sqrt( dataOut[i][0]*dataOut[i][0] + dataOut[i][1]*dataOut[i][1] );
	}

	fftw_destroy_plan(p);
	fftw_free(dataIn);
	fftw_free(dataOut);
	return data;
}

