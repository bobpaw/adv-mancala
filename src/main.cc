#ifdef HAVE_CMAKE_CONFIG_H
#include "cmakeconfig.h"
#endif

#if defined(HAVE_FILESYSTEM)
#include <filesystem>
#elif defined(HAVE_EXPERIMENTAL_FILESYSTEM)
#include <experimental/filesystem>
#else
#include <boost/filesystem.hpp>
#endif

#include <iostream>
#include <stdexcept>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "mancala.h"

namespace {

template<typename T>
sf::Vector2<T> center(sf::Rect<T> r) {
	return {r.left + r.width / 2, r.top + r.height / 2};
}

class TileMap: public sf::Drawable, public sf::Transformable {
public:
	explicit TileMap(const sf::Texture& texture, sf::Vector2u tilesize):
			m_vertices(sf::Quads), m_tileset(&texture), tileSize(tilesize),
			size(0, 0) {
		m_tileset = &texture;
	}
	bool load(const int* tiles, unsigned int width, unsigned int height) {
		if (!m_tileset) return false;

		// resize the vertex array to fit the level size
		m_vertices.resize(width * height * 4);

		size.x = width;
		size.y = height;

		// populate the vertex array, with one quad per tile
		for (unsigned int i = 0; i < width; ++i)
			for (unsigned int j = 0; j < height; ++j) {
				// get the current tile number
				int tileNumber = tiles[i + j * width];
				if (tileNumber < 0) continue;

				set_tile(i, j, tileNumber);
			}

		return true;
	}

	void set_tile(unsigned int x, unsigned int y, int tileNumber) {
		sf::Vertex* quad = &m_vertices[(x + y * size.x) * 4];

		if (tileNumber == -1) {
			quad[0] = quad[1] = quad[2] = quad[3] = sf::Vertex();
			return;
		}

		// Set position if not set.
		quad[0].position =
				sf::Vector2f(sf::Vector2u(x * tileSize.x, y * tileSize.y));
		quad[1].position =
				sf::Vector2f(sf::Vector2u((x + 1) * tileSize.x, y * tileSize.y));
		quad[2].position =
				sf::Vector2f(sf::Vector2u((x + 1) * tileSize.x, (y + 1) * tileSize.y));
		quad[3].position =
				sf::Vector2f(sf::Vector2u(x * tileSize.x, (y + 1) * tileSize.y));

		unsigned int tu = tileNumber % (m_tileset->getSize().x / tileSize.x);
		unsigned int tv = tileNumber / (m_tileset->getSize().x / tileSize.x);

		quad[0].texCoords =
				sf::Vector2f(sf::Vector2u(tu * tileSize.x, tv * tileSize.y));
		quad[1].texCoords =
				sf::Vector2f(sf::Vector2u((tu + 1) * tileSize.x, tv * tileSize.y));
		quad[2].texCoords = sf::Vector2f(
				sf::Vector2u((tu + 1) * tileSize.x, (tv + 1) * tileSize.y));
		quad[3].texCoords =
				sf::Vector2f(sf::Vector2u(tu * tileSize.x, (tv + 1) * tileSize.y));
	}

	void set_color(int x, int y, sf::Color color) {
		sf::Vertex* quad = &m_vertices[(x + y * size.x) * 4];

		quad[0].color = color;
		quad[1].color = color;
		quad[2].color = color;
		quad[3].color = color;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	const sf::Texture* m_tileset;
	sf::Vector2u tileSize;
	sf::Vector2i size;
};

}  // namespace

#if defined(HAVE_FILESYSTEM)
namespace fs = std::filesystem;
#elif defined(HAVE_EXPERIMENTAL_FILESYSTEM)
namespace fs = std::experimental::filesystem;
#else
namespace fs = boost::filesystem;
#endif

int main(int argc, char* argv[]) {
	std::string resource_stem;

	if (argc > 0) {
#if defined(HAVE_FILESYSTEM) || defined(HAVE_EXPERIMENTAL_FILESYSTEM)
		resource_stem = (fs::path(argv[0]).parent_path() / "").string();
#else
		resource_stem = (fs::path(argv[0]).parent_path() / "").native();
#endif
	} else {
		// Die I guess?
		return -1;
	}

	sf::RenderWindow graphics_window(sf::VideoMode(64 * 4, 56 * 8), "Adv Mancala",
																	 sf::Style::Default,
																	 sf::ContextSettings(0, 0, 0));
	graphics_window.setVerticalSyncEnabled(true);

	sf::Font freesans;
	if (!freesans.loadFromFile(resource_stem + "FreeSans.ttf")) {
		std::cerr << "Failed to load font FreeSans.ttf." << std::endl;
		return -1;
	}

	sf::Texture spritemap;
	if (!spritemap.loadFromFile(resource_stem + "spritemap.png")) {
		std::cerr << "Failed to load spritemap.png." << std::endl;
		return -1;
	}

	// clang-format off
	int mancala_board[] = {
		4, 5, 5, 7,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		4, 5, 5, 7
	};
	// clang-format on
	TileMap map(spritemap, sf::Vector2u(64, 56));
	map.load(mancala_board, 4, 8);

	// clang-format off
	int ui_elements[] = {
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1,
		-1,  2,  3, -1,
		-1,  8,  9, -1,
		-1, 10, -1, -1,
		-1, -1, -1, -1,
		-1, -1, -1, -1
	};
	// clang-format on
	TileMap ui(spritemap, sf::Vector2u(64, 56));
	ui.load(ui_elements, 4, 8);
	sf::FloatRect preview_button(64 * 2, 56 * 3, 64, 56);

	sf::Event event;

	sf::Text rule_choice("Capture or Avalanche?", freesans);

	rule_choice.setOrigin(center(rule_choice.getLocalBounds()));
	rule_choice.setPosition(sf::Vector2f(graphics_window.getSize()) / 2.f);

	const sf::FloatRect pocket_dimensions[] = {
			{0, 0, 64 * 4, 56},       {0, 56 * 1, 64, 56},
			{0, 56 * 2, 64, 56},      {0, 56 * 3, 64, 56},
			{0, 56 * 4, 64, 56},      {0, 56 * 5, 64, 56},
			{0, 56 * 6, 64, 56},      {0, 56 * 7, 64 * 4, 56},
			{64 * 3, 56 * 6, 64, 56}, {64 * 3, 56 * 5, 64, 56},
			{64 * 3, 56 * 4, 64, 56}, {64 * 3, 56 * 3, 64, 56},
			{64 * 3, 56 * 2, 64, 56}, {64 * 3, 56 * 1, 64, 56},
	};

	sf::Text pocket_values("0", freesans);
	pocket_values.setOrigin(center(pocket_values.getLocalBounds()));

	mancala::Board fun_board(mancala::Board::Ruleset::Capture, {0, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6});
	mancala::Board preview_board;

	sf::Vector2f mouse_pos;
	bool preview_mode = false;

	while (graphics_window.isOpen()) {
		while (graphics_window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: graphics_window.close(); break;
			case sf::Event::MouseButtonReleased:
				mouse_pos = graphics_window.mapPixelToCoords(
						sf::Mouse::getPosition(graphics_window));
				// Process
				for (int i = 0; i < 14; ++i)
					if (pocket_dimensions[i].contains(mouse_pos) &&
							fun_board.on_my_side(i)) {
						if (preview_mode)
							preview_board = fun_board.preview(i);
						else
							fun_board.move(i);
					}
				if (preview_button.contains(mouse_pos)) {
					if (preview_mode) {
						map.set_color(2, 3, sf::Color::White);
						preview_mode = false;
					} else {
						map.set_color(2, 3, sf::Color::Blue);
						preview_mode = true;
						preview_board = fun_board;
					}
				}
				break;
			default:
				// Do nothing
				break;
			}
		}

		for (int i = 0; i < static_cast<int>(fun_board.size()); ++i) {
			int tile = 0;
			int val = preview_mode ? preview_board[i] : fun_board[i];
			if (val >= 5)
				tile = 15;
			else if (val == 0)
				tile = -1;
			else
				tile = 10 + val;

			int tx = unsigned(pocket_dimensions[i].left) / 64,
					ty = unsigned(pocket_dimensions[i].top) / 56;
			ui.set_tile(tx, ty, tile);
			if (fun_board.on_my_side(i)) {
				map.set_tile(tx, ty, 16);
				/* if (preview_mode)
					map.set_color(tx, ty, sf::Color::Blue);
				else
					map.set_color(tx, ty, sf::Color::White); */
			} else if (i != fun_board.my_mancala() &&
								 i != fun_board.their_mancala()) {
				map.set_tile(tx, ty, 1);
			}
		}
		// Yada yada nobody cares
		graphics_window.clear();
		graphics_window.draw(map);
		graphics_window.draw(ui);

		for (int i = 0; i < static_cast<int>(fun_board.size()); ++i) {
			pocket_values.setString(std::to_string(preview_mode ? preview_board[i] : fun_board[i]));
			pocket_values.setPosition(center(pocket_dimensions[i]));
			graphics_window.draw(pocket_values);
		}
		graphics_window.display();
	}

	// let the player pick the ruleset
	int ruleset = -1;
	while (!std::cin.fail() && !(ruleset == 0 || ruleset == 1)) {
		std::cout << "Which ruleset would you like to use? \n  0: Capture\n  1: "
								 "Avalanche\n> "
							<< std::flush;
		std::cin >> ruleset;
	}

	// Discard newline left in input by std::cin.operator>> call.
	std::cin.ignore();

	std::cout << "Starting Game..." << std::endl;

	mancala::Board game;
	game.rules = mancala::Board::Ruleset(ruleset);

	for (unsigned i = 1; i < 7; ++i) {
		game[i] = 6;
		game[7 + i] = 6;
	}

	// game loop
	bool endGame = false;
	std::string line;

	std::cout << game << "\nPlayer " << game.player + 1;
	std::cout << ", please make your move (1-6).";
	std::cout << " Prepend p to preview.\n > " << std::flush;

	while (!endGame && std::getline(std::cin, line)) {
		int moveNum = 0;
		if (line[0] == 'p') {
			try {
				moveNum = std::stoi(line.substr(1));
			} catch (const std::invalid_argument&) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1;
			} catch (const std::out_of_range&) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1;
			}

			if (moveNum >= 1 && moveNum <= 7) {
				auto preview = game.preview(game.player * 7 + moveNum);

				std::cout << "Preview:\n" << preview << std::endl;
			}
		} else {
			try {
				moveNum = std::stoi(line);
			} catch (const std::invalid_argument&) {
				std::cerr << "Invalid argument." << std::endl;
				moveNum = -1;
			} catch (const std::out_of_range&) {
				std::cerr << "Input out of range." << std::endl;
				moveNum = -1;
			}

			if (moveNum >= 1 && moveNum <= 7) {
				game.move(game.player * 7 + moveNum);
				std::cout << game << std::endl;

				std::cout << "Player " << game.player + 1;
				std::cout << ", please make your move (1-6).";
				std::cout << " Prepend p to preview.\n";
			}
		}

		std::cout << "> " << std::flush;
	}

	return 0;
}
