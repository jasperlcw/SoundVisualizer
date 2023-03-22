TARGET = final_project

SOURCES = src/Utility.c src/ledControl.c src/audioMixer/audioMixer_template.c src/potentiometer.c src/audioMixer/BeatController.c src/udpComms.c src/main.c

PUBDIR = $(HOME)/cmpt433/public/myApps
LIBDIR = $(HOME)/cmpt433/public
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB
FFTW3_LFLAGS = -L$(HOME)/cmpt433/public/fftw3

all: wav node react
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) $(FFTW3_LFLAGS) -lpthread -lasound -lfftw3 -lm

#fftw3:
#	sudo apt-get install fftw3
#	mkdir -p $(LIBDIR)/fftw3/
#	cp /usr/lib/arm-linux-gnueabihf/libfftw3.so.3.5.8 $(LIBDIR)/fftw3/

# Copy wave files to the shared folder
wav:
	mkdir -p $(PUBDIR)/final-wav-files/
	cp src/wave-files/* $(PUBDIR)/final-wav-files/

node:
	mkdir -p $(PUBDIR)/final-server-copy/
	cp -R src/webServer/* $(PUBDIR)/final-server-copy/
	cd $(PUBDIR)/final-server-copy/rgb_front && sudo npm install
#	cd $(PUBDIR)/final-server-copy/ && sudo npm install

react:
	cd $(PUBDIR)/final-server-copy/rgb_front && sudo npm run build