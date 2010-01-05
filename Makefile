all:
	g++ -o gtuna dsp.cpp fft.cpp main.cpp -lm -lfftw3 -lncurses -g

clean:
	rm gtuna
