#include <iostream>
#include <cmath>
#include <fcntl.h>
#include <ncurses.h>
#include "gtuna.hpp"

using std::cout;
using std::cerr;
using std::endl;

const char notes[][3] = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };

void gtuna_banner()  {
	attrset(A_BOLD);
	color_set (RED, NULL);

	printw ("===============================================================\n");
	printw ("gTuna - A chromatic cool DSP tuner for your cool Linux terminal\n");
	printw ("===============================================================\n\n");

	attrset(A_STANDOUT);
	color_set (WHITE, NULL);

}

void do_colors()  {
	init_pair (WHITE, COLOR_WHITE, COLOR_BLACK);
	init_pair (RED, COLOR_RED, COLOR_BLACK);
	init_pair (GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair (YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair (CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair (BLUE, COLOR_BLUE, COLOR_BLACK);
}

int main()  {
	int dsp, lastnote = -1;
	struct dsp_info dspinfo;

	dspinfo.bitrate = 88200;
	dspinfo.depth = 8;
	dspinfo.channels = 1;

	const size_t bufsize = 8192;

	if ((dsp = dsp_open("/dev/dsp", dspinfo, O_RDONLY)) < 0)  {
		cerr << "dsp" << endl;
		return 1;
	}

	initscr();
	bool colors = (start_color() != ERR) ? true : false;

	if (colors)  {
		do_colors();
	} else {
		cerr << "Can't set ncurses colors on your terminal - exiting\n";
		return 1;
	}

	while (1)  {
		bool locked = false;
		clear();
		gtuna_banner();

		while (!locked)  {
			unsigned char *data = new unsigned char[bufsize];
		
			if (read (dsp, data, bufsize) < 0)
				return 1;
		
			vector<double> comp(bufsize);

			for (int i=0; i < bufsize * dspinfo.channels; i += dspinfo.channels)  {
				comp[i] = static_cast<double>(data[i]);
			}

			delete [] data;
			comp = FFT(comp);

			double freq = (dspinfo.bitrate * getMaxFreq(comp)) / bufsize;
			int note = (int) rint(12 * (log(freq / BASE_FREQ) / log(2) ));

			if (note > 36 || note < -48)
				continue;
			else
				locked = true;

			double freqPrevN = BASE_FREQ * pow(2.0, (note-1)/12.0);
			double effectiveFreq = BASE_FREQ * pow(2.0, note/12.0);
			double freqNextN = BASE_FREQ * pow(2.0, (note+1)/12.0);
			for ( note = (note + ((note < 0) ? 12 : 0)) % 12; note < 0; note += 12);

			if (note != lastnote)  {
				attrset(A_BOLD);
				color_set (GREEN, NULL);

				printw ("%s ", notes[note]);
				
				attrset(A_NORMAL);
				color_set (WHITE, NULL);

				double freqMin = exp( (log(effectiveFreq) + log(freqPrevN)) / 2.0 );
				double freqMax = exp( (log(freqNextN) + log(effectiveFreq)) / 2.0 );
				double freqInterval = (freqMax - freqMin) / 5.0;
				int freqState = -2;

				for (double freqFlag = freqMin; freqFlag < freqMax; freqFlag += freqInterval, freqState++)  {
					if (freq >= freqFlag && freq <= freqFlag + freqInterval)
						break;
				}

				printw ("(%f, [%f, %f, %f]) [", freq, freqMin, effectiveFreq, freqMax);
				attrset(A_UNDERLINE | A_BOLD);

				switch (freqState)  {
					case -2:
						color_set (RED, NULL);
						printw ("--");
						break;

					case -1:
						color_set (YELLOW, NULL);
						printw ("-");
						break;

					case  0:
						color_set (GREEN, NULL);
						printw ("O");
						break;

					case  1:
						color_set (CYAN, NULL);
						printw ("+");
						break;

					case  2:
						color_set (BLUE, NULL);
						printw ("++");
						break;
				}
				
				attrset(A_NORMAL);
				color_set (WHITE, NULL);
				printw ("]\n");

				refresh();
				lastnote = note;
			}
		}
	}

	endwin();
	close(dsp);
	return 0;
}

