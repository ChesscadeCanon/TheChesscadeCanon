#ifdef _WIN32
#include "application.h"
#endif
#include "tests.h"
#include "def.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <simulator.h>
#include <time.h>

void print_help() {

#ifdef _WIN32
	printf("Run with no arguments to play command line app.\n");
#else
	printf("Command line app only available on Windows. Only tests are available.\n");
#endif
	printf("Run with -t to run tests.\n");
	printf("Run with -v to run tests in verbose mode.\n");
	printf("Run with -t [test number] or -v [test number] to run a specific test.\n");
	printf("Run with -h to see this message.\n");
}

Bool argmatch(int argc, const char** argv, int index, const char* arg) {

	return argc > index && strncmp(arg, argv[index], 2) == 0;
}

void _tests(int argc, const char** argv, Bool verbose) {

	int select = -1;

	if (argc >= 3) {

		select = atoi(argv[2]);
	}

	run_tests(select, verbose);
}

void _simulate(int argc, const char** argv) {

	unsigned int seed = 0;

	if (argc >= 3) {

		seed = atol(argv[2]);
	}

	if (!seed) {

		seed = (unsigned int)time(NULL);
	}

	init_simulator(seed);
	run_pretty_auto_text_game(STANDARD_SETTINGS);
	printf("seed: %ul", seed);
}

int main(int argc, const char** argv) {

	if (argc == 1) {
#ifdef _WIN32
		run_pretty_text_game(STANDARD_SETTINGS);
#else
		print_help();
#endif
	}
	else if (argmatch(argc, argv, 1, "-h")) {

		print_help();
	}
	else if (argmatch(argc, argv, 1, "-t")) {

		_tests(argc, argv, False);
	}
	else if (argmatch(argc, argv, 1, "-v")) {

		_tests(argc, argv, True);
	}
	else if (argmatch(argc, argv, 1, "-s")) {

		_simulate(argc, argv);
	}

	return 0;
}