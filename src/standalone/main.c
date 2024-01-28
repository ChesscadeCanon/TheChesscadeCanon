#include "application.h"
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc == 1) {
#ifdef _WIN32
		run_pretty_text_game(DEFAULT_SETTINGS);
#else
		printf("Command line app only available on Windows. Run with -t to run tests.")
#endif
	}
	return 0;
}