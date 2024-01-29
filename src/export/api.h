#pragma once
#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

extern "C" {
	struct Game;

	/**
	* returns a bigflag denoting the attack pattern of the current falling piece
	*/
	EXPORT unsigned long long attack_pattern(struct Game*);

	/**
	* begins the game by spawning the first piece
	*/
	EXPORT void begin_game(struct Game*);

	/**
	* frees all memory allocated on behalf of the given game object
	*/
	EXPORT void delete_game(struct Game*);

	/**
	* returns the number of consecutive captures
	*/
	EXPORT unsigned long long get_combo(const struct Game*);

	/**
	* returns -1 if the cursor is going left or 1 if it's going right
	*/
	EXPORT short get_cursor_direction(const struct Game*);

	/**
	* returns an unsigned short denoting the rank of the board where the falling piece currently is
	*/
	EXPORT unsigned short get_cursor_grade(const struct Game*);

	/**
	* returns an unsigned short denoting the file of the board where the falling piece currently is
	*/
	EXPORT unsigned short get_cursor_increment(const struct Game*);

	/**
	* gets the piece from the spawn deck at the given grade and increment
	*/
	EXPORT char get_deck_piece(const unsigned short, const unsigned short);

	/**
	* returns the number of milliseconds it currently takes for the piece to fall
	*/
	EXPORT long long get_ease(const struct Game*);

	/**
	* returns the time when a finished game ended, or -1 if the game is still going
	*/
	EXPORT long long get_end_time(const struct Game*);

	/**
	* returns a bitset denoting what events took place in the last frame
	*/
	EXPORT int get_events(const struct Game*);

	/**
	* returns the width of the board
	*/
	EXPORT unsigned short get_files();

	/**
	* return a bitset denoting what pieces would be captured by the current piece if it dropped
	*/
	EXPORT unsigned long long forecast_captures(struct Game*);

	/**
	* returns the piece that the current falling piece would promote to if it fell right now
	*/
	EXPORT char get_forecast_piece(const struct Game*);

	/**
	* returns the rank that the current piece would fall to if it fell right now
	*/
	EXPORT unsigned short get_forecast_rank(const struct Game*);

	/**
	* returns the number of milliseconds of gameplay
	*/
	EXPORT long long get_milliseconds(const struct Game*);

	/**
	* returns the minimum amount of time between player moves
	*/
	EXPORT long long get_move_rate(const struct Game*);

	/**
	* returns the next piece that will spawn if the current piece lands right now
	*/
	EXPORT char get_next_piece(const struct Game*);

	/**
	* returns a character denoting the type of the current falling piece
	*/
	EXPORT char get_player(const struct Game*);

	/**
	* returns an unsigned short denoting the file of the board where the falling piece currently is
	*/
	EXPORT unsigned short get_player_file(const struct Game*);

	/**
	* returns an unsigned short denoting the rank of the board where the falling piece currently is
	*/
	EXPORT unsigned short get_player_rank(const struct Game*);

	/**
	* returns the height of the board
	*/
	EXPORT unsigned short get_ranks();

	/**
	* returns the full text of the rules of Chesscade
	*/
	EXPORT const char* get_rules();

	/**
	* returns the full text of the rules of Chesscade
	*/
	EXPORT unsigned long long get_rules_length();

	/**
	* returns the current score
	*/
	EXPORT unsigned long long get_score(const struct Game*);

	/**
	* returns the number of points scored on the last landing
	*/
	EXPORT unsigned long long get_scored(const struct Game*);

	/**
	* gets the rank where the next piece will spawn
	*/
	EXPORT unsigned short get_spawn_rank(const struct Game*);

	/**
	* seturns the current state of the board as a string
	*/
	EXPORT char get_square(const struct Game* game, const unsigned short, const unsigned short);

	/**
	* gets the bit that represents the square at the given coordinates for purposes of finding it in a bitflag
	*/
	EXPORT unsigned long long get_square_bit(const unsigned short, const unsigned short);

	/**
	* On any given frame, call input_analog_move() with floating point values
	* constituting a vector denoting how far the mouse or touch has moved on the x and y
	* axes in terms of what fraction of a square width it has moved. For instance, if
	* the squares were 64 pixels across in your interface, and on some frame the player
	* dragged the mouse 96 pixels to the left and 16 pixels down, you would call:
	* input_analog_move(game, -1.5, 0.25);
	*/
	EXPORT void input_analog_move(struct Game*, const long double, const long double);

	/**
	* On any given frame, call input_digital_move() with boolean values denoting whether
	* to try and move the piece left, right and/or down, as indicated by key inputs. For
	* instance, if the right and down keys were being held, you would call:
	* input_digital_move(game, false, True, True);
	*/
	EXPORT void input_digital_move(struct Game*, const bool, const bool, const bool);

	/**
	* drops the current piece
	*/
	EXPORT void input_drop(struct Game*);

	/**
	* pauses the game if it's unpaused and vice versa
	*/
	EXPORT void input_toggle_pause(struct Game*);

	/**
	* returns whether the game has ended or not
	*/
	EXPORT bool is_game_over(const struct Game*);

	/**
	* returns whether dropping the current piece now would result in a game over
	*/
	EXPORT bool is_on_brink(struct Game* game);

	/**
	* returns whether not the game is paused
	*/
	EXPORT bool is_paused(const struct Game* game);

	/**
	* returns whether not the game is paused
	*/
	EXPORT bool is_repeat(const struct Game* game);

	/**
	* returns a pointer to a newly allocated and initialized Game struct
	*/
	EXPORT struct Game* malloc_init_custom_game(const int);

	/**
	* returns a pointer to a newly allocated and initialized Game struct
	*/
	EXPORT struct Game* malloc_init_default_game();

	/**
	* prints the current state of the board in ascii form to stdout
	*/
	EXPORT void print_board(const struct Game*);

	/**
	* progresses the game by the given number of milliseconds
	*/
	EXPORT void pump_game(struct Game*, const long long);
}
