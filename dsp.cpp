#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include "gtuna.hpp"

int dsp_open (const char* dspdev, struct dsp_info info, int flags)  {
	int dsp, arg;

	if ((dsp=open(dspdev, flags)) < 0)  {
		return dsp;
	}

	arg = AFMT_U8;

	if (ioctl(dsp, SNDCTL_DSP_SETFMT, &arg) == -1)  {
		return -1;
	}

	if (arg != AFMT_U8)  {
		return -1;
	}

	arg = info.depth;

	if (ioctl(dsp, SOUND_PCM_WRITE_BITS, &arg) == -1)  {
		return -1;
	}

	if (arg != info.depth)  {
		return -1;
	}

	arg = info.channels;
	
	if (ioctl(dsp, SOUND_PCM_WRITE_CHANNELS, &arg) == -1)  {
		return -1;
	}

	if (arg != info.channels)  {
		return -1;
	}

	arg = info.bitrate;

	if (ioctl(dsp, SOUND_PCM_WRITE_RATE, &arg) == -1)  {
		return -1;
	}
	
	if (arg != info.bitrate)  {
		return -1;
	}

	return dsp;
}

