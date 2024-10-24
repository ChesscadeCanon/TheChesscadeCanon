#ifdef _WIN32
#include "application.h"
#endif
#include "def.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <simulator.h>
#include <time.h>

#define ARGMAX 6

void print_help() {

#ifdef _WIN32
	printf("Run with no arguments to play command line app.\n");
	printf("Run with -h to see this message.\n");
	printf("Run with -a to run an automated game.\n");
	printf("Follow -a with -s and a number to specify the random seed.\n");
	printf("When an automated game exits, it will show the seed it used.\n");
	printf("Follow -a with -a and a number to specify a custom filter sequence.\n");
	printf("The expert system will find the shortest path to each square the current piece can reach.\n");
	printf("Then it will apply the filters in the order specified, from left to right.\n");
	printf("The currently implemented filters are as follows:\n");
#else
	printf("Command line app only available on Windows.\n");
#endif
}

Bool argmatch(int argc, const char** argv, int index, const char* arg) {

	return argc > index && strncmp(arg, argv[index], 2) == 0;
}

Bool argfind(int argc, const char** argv, const char* arg) {

	for (int ret = 1; ret < ARGMAX; ++ret) {

		if (argmatch(argc, argv, ret, arg)) {

			return ret;
		}
	}

	return 0;
}

int argget(const int argc, const char** argv, const int index) {

	if (index + 1 >= argc) {
	
		printf("Could not find data for argument %s; exiting\n", argv[index]);
		exit(1);
	}
	
	return strtol(argv[index + 1], NULL, 16);
}

void _simulate(const int argc, const char** argv) {

	unsigned int seed = 0;
	unsigned int seed_index = argfind(argc, argv, "-s");
	
	if (seed_index) {

		seed = argget(argc, argv, seed_index);
	}

	if (!seed) {

		seed = (unsigned int)time(NULL);
	}

	init_simulator(seed);
	run_pretty_auto_text_game(STANDARD_SETTINGS);
	printf("seed: %Xl", seed);
}

int main(const int argc, const char** argv) {

	if (argc == 1) {
#ifdef _WIN32
		run_pretty_text_game(STANDARD_SETTINGS);
#else
		print_help();
#endif
	}
	else if (argfind(argc, argv, "-h")) {

		print_help();
	}
	else if (argfind(argc, argv, "-a")) {
#ifdef _WIN32
		_simulate(argc, argv);
#else
		print_help();
#endif
	}

	return 0;
}