#ifdef _WIN32
#include "application.h"
#endif
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc == 1) {
#ifdef _WIN32
		run_pretty_text_game(DEFAULT_SETTINGS);
#else
		printf("Command line app only available on Windows. Run with -t to run tests.\n");
#endif
	}
	return 0;
}