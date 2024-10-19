#ifndef CHESSCADE_ADAPTER_H
#define CHESSCADE_ADAPTER_H

#include <scene/main/node.h>
#include <core/string/string_name.h>
#include <core/string/ustring.h>
#include <core/math/vector2i.h>
#include <stdint.h>

struct Game;

class ChesscadeAdapter : public Node {
	GDCLASS(ChesscadeAdapter, Node)

	typedef StringName GodotPiece;
	typedef int64_t GodotInt;

private:
	struct Game* game = NULL;

protected:
	static void _bind_methods();

public:
	ChesscadeAdapter();
	~ChesscadeAdapter();
		
	/*
	* gets the current state of the board as a string
	*/
	String get_board_state() const;

	/*
	* begins the game by spawning the first piece
	*/
	void begin_game();

	/*
	* destroys the current game
	* call in _exit_tree()
	*/
	void clear();

	/*
	* destroys the current game and creates a new one
	* call in _ready()
	*/
	void reset();

	/**
	* pauses the game if it's unpaused and vice versa
	*/
	void input_toggle_pause();

	/**
 	* returns a character denoting the type of the current falling piece
	*/
	GodotPiece get_player() const;

	/**
	* returns a bigflag denoting the attack pattern of the current falling piece
	*/
	GodotInt attack_pattern() const;

	/**
	* return a bitset denoting what pieces would be captured by the current piece if it dropped
	*/
	GodotInt forecast_captures() const;

	/**
	* returns the number of consecutive captures
	*/
	GodotInt get_combo() const;

	/**
	* returns the size of the deck
	*/
	Vector2i get_deck_size() const;

	/**
	* returns -1 if the cursor is going left or 1 if it's going right
	*/
	GodotInt get_cursor_direction() const;

	/**
	* returns the location of the cursor on the deck
	*/
	Vector2i get_cursor() const;

	/**
	* gets the piece from the spawn deck at the given grade and increment
	*/
	GodotPiece get_deck_piece(Vector2i) const;

	/**
	* returns the number of seconds it currently takes for the piece to fall
	*/
	double get_ease() const;

	/**
	* returns the time in seconds when a finished game ended, or -1 if the game is still going
	*/
	double get_end_time() const;

	/**
	* returns the time in seconds for which the game has been going
	*/
	double get_time() const;

	/**
	* returns a bitset denoting what events took place in the last frame
	*/
	GodotInt get_events() const;

	/**
	* returns the rank that the current piece would fall to if it fell right now
	*/
	GodotInt get_forecast_rank() const;

	/**
	* returns the minimum amount of time between player moves
	*/
	GodotInt get_move_rate() const;

	/**
	* returns the next piece that will spawn if the current piece lands right now
	*/
	GodotPiece get_next_piece() const;

	/**
	* returns an integer vector denoting the rank and file of the board where the
	* falling piece currently is
	*/
	Vector2i get_player_square() const;

	/**
	* returns the height of the board
	*/
	Vector2i get_board_size() const;

	/**
	* returns the full text of the rules of Chesscade
	*/
	GodotPiece get_rules() const;

	/**
	* returns the current score
	*/
	GodotInt get_score() const;

	/**
	* gets the rank where the next piece will spawn
	*/
	GodotInt get_spawn_rank() const;

	/**
	* returns the number of points scored on the last landing
	*/
	GodotInt get_scored() const;

	/**
	* gets the contents of the specified square
	*/
	GodotPiece get_square(const Vector2i) const;

	/**
	* gets the bit that represents the square at the given coordinates for purposes of finding it in a bitflag
	*/
	GodotInt get_square_bit(const Vector2i) const;

	/**
	* On any given frame, call input_analog_move() with floating point values
	* constituting a vector denoting how far the mouse or touch has moved on the x and y
	* axes in terms of what fraction of a square width it has moved. For instance, if
	* the squares were 64 pixels across in your interface, and on some frame the player
	* dragged the mouse 96 pixels to the left and 16 pixels down, you would call:
	* input_analog_move(game, -1.5, 0.25);
	*/
	void input_analog_move(const Vector2);

	/**
	* On any given frame, call input_digital_move() with boolean values denoting whether
	* to try and move the piece left, right and/or down, as indicated by key inputs. For
	* instance, if the right and down keys were being held, you would call:
	* input_digital_move(game, false, True, True);
	*/
	void input_digital_move(const bool, const bool, const bool);

	/**
	* drops the current piece
	*/
	void input_drop();

	/**
	* returns whether the game has ended or not
	*/
	bool is_game_over() const;

	/**
	* returns whether dropping the current piece now would result in a game over
	*/
	bool is_on_brink() const;

	/**
	* returns whether not the game is paused
	*/
	bool is_paused() const;

	/**
	* returns whether not the game is paused
	*/
	bool is_repeat() const;

	/**
	* progresses the game by the given number of milliseconds
	*/
	void pump_game(double);

	/*
	* gets the specified chunk of the binary representation of the specified piece's image
	*/
	GodotInt get_figure(GodotPiece, GodotInt, GodotInt);

	//void _ready() override;
	//void _exit_tree() override;
};

#endif
