#ifdef _WIN32
#include "application.h"
#endif
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_help() {

#ifdef _WIN32
	printf("Run with no arguments to play command line app.\n");
#else
	printf("Command line app only available on Windows. Only tests are available.\n");
#endif
	printf("Run with -t to run tests.\n");
	printf("Run with -t [test number] to run a specific test.\n");
	printf("Run with -h to see this message.\n");
}

Bool argmatch(int argc, const char** argv, int index, const char* arg) {

	return argc > index && strncmp(arg, argv[index], 2);
}

int main(int argc, const char** argv) {
	if (argc == 1) {
#ifdef _WIN32
		run_pretty_text_game(DEFAULT_SETTINGS);
#else
		print_help();
#endif
	}
	else if (argmatch(argc, argv, 1, "-h")) {

		print_help();
	}
	else if (argmatch(argc, argv, 1, "-t")) {

		int select = -1;

		if (argc >= 3) {

			select = atoi(argv[2]);
		}

		run_tests(select);
	}
	return 0;
}