//
// Created by David Burchill on 2023-09-27.
//

#include "Game.h"
#include "Components.h"
#include <fstream>
#include <iostream>
#include <filesystem>




const sf::Time Game::TIME_PER_FRAME = sf::seconds((1.f / 60.f));

void Game::run() {
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (isRunning) {

		// **********************
		// handle user input
		// **********************
		sUserInput();


		// **********************
		// update the world
		// in fixed time steps 
		// **********************
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TIME_PER_FRAME) {
			timeSinceLastUpdate -= TIME_PER_FRAME;
			sUserInput();
			sUpdate(TIME_PER_FRAME);
		}

		updateStatistics(elapsedTime);

		// **********************
		// render the next frome
		// **********************
		sRender();
	}
}

void Game::sMovement(sf::Time dt) {

	for (auto& e : entityManager.getEntities()) {
		auto& tfm = e->getComponent<CTransform>();
		tfm.pos += tfm.vel * dt.asSeconds();

		keepInBounds(*e);
	}
}

void Game::sRender() {
	window.clear(sf::Color(100, 100, 255));

	if (currentState == GameState::Title) {
		window.draw(titleSprite);
	}
	else if (currentState == GameState::Menu) {
		window.draw(menuSprite);

		sf::RectangleShape highlightRect;
		highlightRect.setSize(sf::Vector2f(200, 50));
		highlightRect.setFillColor(sf::Color(255, 255, 255, 100));

		if (currentMenuOption == 0) {
			highlightRect.setPosition(200, 200);
		}
		else if (currentMenuOption == 1) {
			highlightRect.setPosition(200, 300);
		}
		else if (currentMenuOption == 2) {
			highlightRect.setPosition(200, 400);
		}

		window.draw(highlightRect);
	}
	else if (currentState == GameState::Background) {
		scene.render(window);
	}

	window.draw(statisticsText);
	window.display();
}

void Game::sUpdate(sf::Time dt) {

	entityManager.update();

	if (isPaused)
		return;


	sMovement(dt);
	sCollision();

}

void Game::sUserInput() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			isRunning = false;
		}

		if (event.type == sf::Event::KeyPressed) {
			if (currentState == GameState::Title) {
				if (event.key.code == sf::Keyboard::Enter) {
					currentState = GameState::Menu;
				}
			}
			else if (currentState == GameState::Menu) {
				if (event.key.code == sf::Keyboard::Up) {
					currentMenuOption = (currentMenuOption - 1 + menuOptionCount) % menuOptionCount;
				}
				else if (event.key.code == sf::Keyboard::Down) {
					currentMenuOption = (currentMenuOption + 1) % menuOptionCount;
				}
				else if (event.key.code == sf::Keyboard::Enter) {
					if (currentMenuOption == 2) {
						currentState = GameState::Background;
					}
				}
			}
		}
	}
}

void Game::init(const std::string& path) {
	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	loadConfigFromFile(path);
	window.create(sf::VideoMode(windowSize.x, windowSize.y), "GEX Engine");

	statisticsText.setFont(font);
	statisticsText.setPosition(15.0f, 15.0f);
	statisticsText.setCharacterSize(15);

	if (!titleTexture.loadFromFile(assetsPath + "title.png")) {
		std::cerr << "Failed to load title.png. Check path: " << assetsPath + "title.png" << "\n";
		exit(-1);
	}
	titleSprite.setTexture(titleTexture);
	titleSprite.setScale(
		static_cast<float>(windowSize.x) / titleTexture.getSize().x,
		static_cast<float>(windowSize.y) / titleTexture.getSize().y
	);

	if (!menuTexture.loadFromFile(assetsPath + "menu.png")) {
		std::cerr << "Failed to load menu.png. Check path: " << assetsPath + "menu.png" << "\n";
		exit(-1);
	}
	menuSprite.setTexture(menuTexture);
	menuSprite.setScale(
		static_cast<float>(windowSize.x) / menuTexture.getSize().x,
		static_cast<float>(windowSize.y) / menuTexture.getSize().y
	);
}

sf::FloatRect Game::getViewBounds() {
	auto view = window.getView();
	return sf::FloatRect(
		(view.getCenter().x - view.getSize().x / 2.f), (view.getCenter().y - view.getSize().y / 2.f),
		view.getSize().x, view.getSize().y);
}


void Game::sCollision() {


}


void Game::keepInBounds(Entity& e)
{
	if (e.hasComponent<CCollision>()) {
		auto  cr = e.getComponent<CCollision>().radius;
		auto& tfm = e.getComponent<CTransform>();
		auto wbounds = window.getSize();

		if (tfm.pos.x < cr || tfm.pos.x >(wbounds.x - cr)) {
			tfm.vel.x *= -1;
			tfm.pos.x = (tfm.pos.x < cr) ? cr : wbounds.x - cr;
		}

		if (tfm.pos.y < cr || tfm.pos.y >(wbounds.y - cr)) {
			tfm.vel.y *= -1;
			tfm.pos.y = (tfm.pos.y < cr) ? cr : wbounds.y - cr;
		}
	}
}


void Game::adjustPlayerPosition() {
	auto vb = getViewBounds();

	auto& player_pos = player->getComponent<CTransform>().pos;
	auto player_cr = player->getComponent<CCollision>().radius;

	player_pos.x = std::max(player_pos.x, vb.left + player_cr + 5);
	player_pos.x = std::min(player_pos.x, (vb.left + vb.width) - player_cr - 5);
	player_pos.y = std::max(player_pos.y, vb.top + player_cr + 5);
	player_pos.y = std::min(player_pos.y, (vb.top + vb.height) - player_cr - 5);
}

void Game::loadConfigFromFile(const std::string& path) {
	std::ifstream config(path);
	if (config.fail()) {
		std::cerr << "Open file " << path << " failed\n";
		config.close();
		exit(1);
	}

	std::string token{ "" };
	config >> token;
	while (!config.eof()) {
		if (token == "Window") {
			config >> windowSize.x >> windowSize.y;

		}
		else if (token == "Font") {
			std::string path;
			config >> path;
			if (!font.loadFromFile(path)) {
				std::cerr << "Failed to load font " << path << "\n";
				exit(-1);
			}
		}
		else if (token == "Circle") {
			auto& pcf = playerConfig;

			int fr{ 255 }, fg{ 255 }, fb{ 0 };
			int olr{ 255 }, olg{ 255 }, olb{ 255 };
			config >> pcf.radius >> pcf.speed
				>> fr >> fg >> fb
				>> olr >> olg >> olb
				>> pcf.outlineThickness
				>> pcf.nVerticies;

			pcf.fillColor = sf::Color(fr, fg, fb);
			pcf.outlineColor = sf::Color(olr, olg, olb);
		}
		else if (token == "Circle")
		{
			std::string tName;
			sf::Vector2f pos, vel;
			int r, g, b;
			int radius;

			config >> tName >> pos.x >> pos.y >> vel.x >> vel.y >> r >> g >> b >> radius;
			auto e = entityManager.addEntity("Circle");
			e->addComponent<CTransform>(pos, vel);
			e->addComponent<CShape>(
				std::shared_ptr<sf::Shape>(new sf::CircleShape(radius)),
				sf::Color(r, g, b));
			e->addComponent<CCollision>(radius);


		}
		else if (token[0] == '#') {
			std::string tmp;
			std::getline(config, tmp);
			std::cout << tmp << "\n";
		}

		config >> token;
	}

	config.close();
}

Game::Game(const std::string& path) : scene("background.png") {
	init(path);
}

void Game::updateStatistics(sf::Time dt) {
	statisticsUpdateTime += dt;
	statisticsNumFrames += 1;
	if (statisticsUpdateTime >= sf::seconds(1.0f)) {
		statisticsText.setString("FPS: " + std::to_string(statisticsNumFrames));
		statisticsUpdateTime -= sf::seconds(1.0f);
		statisticsNumFrames = 0;
	}

}