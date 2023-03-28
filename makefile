TARGET = final_project

SOURCES = src/audioMixer/audioMixer_template.c \
	src/audioMixer/BeatController.c \
	src/currentTime.c \
	src/ledControl.c \
	src/ledMap.c \
	src/main.c \
	src/potentiometer.c \
	src/udpComms.c \
	src/Utility.c

PUBDIR = $(HOME)/cmpt433/public/myApps
LIBDIR = $(HOME)/cmpt433/public
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB
FFTW3_LFLAGS = -L$(HOME)/cmpt433/public/fftw3

all: wav fftw3 node react
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) $(FFTW3_LFLAGS) -lpthread -lasound -lfftw3 -lm

fftw3:
	sudo apt-get install libfftw3-dev
	mkdir -p $(LIBDIR)/fftw3/
	chmod a+rwx $(LIBDIR)/fftw3/
	cp -R src/lib/* $(LIBDIR)/fftw3/
	mv $(LIBDIR)/fftw3/libfftw3.so.* $(LIBDIR)/fftw3/libfftw3.so

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
	
testLED:
	cd src/testLED && make
	
