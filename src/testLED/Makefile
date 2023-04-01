OUTFILE = testLed
OUTDIR = $(HOME)/cmpt433/public/myApps
CROSS_COMPILE = arm-linux-gnueabihf-
CC_C = $(CROSS_COMPILE)gcc
CFLAGS = -pthread -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -lpthread

all:
		$(CC_C) $(CFLAGS) test_ledMatrix.c ../ledControl.c ../currentTime.c ../ledMap.c ../Utility.c -o $(OUTDIR)/$(OUTFILE)

clean:
		rm $(OUTDIR)/$(OUTFILE)
