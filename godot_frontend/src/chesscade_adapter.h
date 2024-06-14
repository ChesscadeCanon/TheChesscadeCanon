#ifndef CHESSCADE_ADAPTER_H
#define CHESSCADE_ADAPTER_H

#include <godot_cpp/classes/node.hpp>

struct Game;

namespace godot {

	class ChesscadeAdapter : public Node {
		GDCLASS(ChesscadeAdapter, Node)

	private:
		struct Game* game = NULL;

	protected:
		static void _bind_methods();

	public:
		ChesscadeAdapter();
		~ChesscadeAdapter();

		void _ready() override;
		void _exit_tree() override;
		godot::String get_player() const;
	};

}

#endif