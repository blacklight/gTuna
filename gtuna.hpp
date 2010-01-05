#include <vector>

using std::vector;

#define 	BASE_FREQ 	440
#define 	WHITE 		1
#define 	RED 			2
#define 	GREEN 		3
#define 	YELLOW 		4
#define 	CYAN 		5
#define 	BLUE 		6

struct dsp_info  {
	unsigned int bitrate;
	unsigned int depth;
	unsigned int channels;
};

vector<double> FFT (vector<double> f);
size_t getMaxFreq (vector<double> v);
int dsp_open (const char* dspdev, struct dsp_info info, int flags);

