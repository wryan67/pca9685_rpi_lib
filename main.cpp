#include <wiringPi.h>
#include <pca9685.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

static int PCA9865_CAP      = 50;
static int PCA9865_RES      = 4046;
static int PCA9865_ADDRESS  = 0x40;
static int pwmPin           = 0;
static int pwmFreq          = -1;
static bool pwmEven = false;
static bool debug = false;

static int pwmMinFreq = 103;
static int pwmMaxFreq = 516;

static int pca9685fd = -1;

#define PCA9685_PIN_BASE 100

void setFrequency(int pin, int speed) {
	if (speed < 1) {
		speed = 0;
	} else if (speed >= 4096) {
		speed = 4096;
	}
	//int xSpeed = PCA9865_RES * speed / 100.0;
	printf("pwm -p %d -f %d\n", pin, speed); fflush(stdout);
	pwmWrite(PCA9685_PIN_BASE + pin, speed);
	if (debug) {
		printf("finished\n", pin, speed); fflush(stdout);
	}
}



bool setup() {

	if (int ret = wiringPiSetup()) {
		fprintf(stderr, "Wiring Pi setup failed, ret=%d\n", ret);
		return false;
	}

	int seed;
	FILE *fp;
	fp = fopen("/dev/urandom", "r");
	fread(&seed, sizeof(seed), 1, fp);
	fclose(fp);
	srand(seed);

	//  the following statements setup the proper input or output for their respective 
	//  inputs or outputs

	if (debug) {
		printf("init pca9685 address=%02x, cap=%d\n", PCA9865_ADDRESS, PCA9865_CAP); fflush(stdout);
	}

	if ((pca9685fd=pca9685Setup(PCA9685_PIN_BASE, PCA9865_ADDRESS, PCA9865_CAP)) <= 0) {
		printf("pca9685 setup failed!\n");
		return false;
	}


	return true;
}                                             // end of setup function


bool usage() {
	fprintf(stderr, "usage: pwm [-a 00-FF] [-c cap] [-p 0-15] [-f freq] [-m freq] [-x freq] [-e]\n");
	fprintf(stderr, "a = hexadecimal i2c address of the PCA9685 ($ gpio i2cd); default 0x40\n");
	fprintf(stderr, "p = PCA9685 pin number (0-15)\n");
	fprintf(stderr, "e = use even split pulse, default is trigger pulse which is for servos\n");
	fprintf(stderr, "c = frequency cap (40-1526).  The default is 50, which is for servos\n");
	fprintf(stderr, "m = min freq\n");
	fprintf(stderr, "x = max freq\n");
	fprintf(stderr, "f = frequency 0-100\n");
	fprintf(stderr, "Mode:   f     - set frequency and exit. (override m & x)");
	fprintf(stderr, "        m & x - run test over range. ");

	return false;
}

bool commandLineOptions(int argc, char **argv) {
	int c, index;

	if (argc < 2) {
		return usage();
	}

	while ((c = getopt(argc, argv, "a:c:dep:f:hm:x:")) != -1)
		switch (c) {
		case 'e':
			pwmEven = true;
			break;
		case 'f':
			sscanf(optarg, "%d", &pwmFreq);
			break;
		case 'c':
			sscanf(optarg, "%d", &PCA9865_CAP);
			break;
		case 'd':
			debug=true;
			break;
		case 'm':
			sscanf(optarg, "%d", &pwmMinFreq);
			break;
		case 'x':
			sscanf(optarg, "%d", &pwmMaxFreq);
			break;
		case 'a':
			sscanf(optarg, "%x", &PCA9865_ADDRESS);
			break;
		case 'p':
			sscanf(optarg, "%d", &pwmPin);
			break;
		case '?':
			if (optopt == 'f' || optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character \\x%x.\n", optopt);

			return usage();

		default:
			abort();
		}

/*
	if (index >= argc) {
		return usage();
	}

	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);
*/

	
	if (pwmPin < 0 || pwmPin>15) {
		return usage();
	}
	
	return true;
}


int main(int argc, char **argv) {

	if (!commandLineOptions(argc, argv)) {
		return 1;
	}


	if (!setup()) {
		return 1;
	}

	if (pwmFreq > -1) {
		setFrequency(pwmPin, pwmFreq);
		return 0;
	}

	if (debug) {
		printf("set min freq\n" ); fflush(stdout);
	}
	setFrequency(pwmPin, pwmMinFreq);
	delay(500);

	for (int i = 0; i < 2; ++i) {
		setFrequency(pwmPin, pwmMaxFreq);
		delay(350);

		setFrequency(pwmPin, pwmMinFreq);
		delay(500);
	}
	delay(2000);

	for (int f = pwmMinFreq; f <= pwmMaxFreq; ++f) {

		if (pwmEven) {
			pca9685PWMWrite(pca9685fd, pwmPin, f / 2, f / 2);
			printf("pwm -p %d -f %d\n", pwmPin, f);
			delay(20);
		} else {
			setFrequency(pwmPin, f);
			delay(20);
		}
	}


//	delay(4294967295U);   // 3.27 years

	return 0;
}
