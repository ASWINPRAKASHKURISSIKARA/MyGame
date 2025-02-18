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
		highlightRect.setSize(sf::Vector2f(400, 70));
		highlightRect.setFillColor(sf::Color(255, 255, 255, 100));

		if (currentMenuOption == 0) {
			highlightRect.setPosition(200, 400);
		}
		else if (currentMenuOption == 1) {
			highlightRect.setPosition(200, 500);
		}
		else if (currentMenuOption == 2) {
			highlightRect.setPosition(200, 600);
		}

		window.draw(highlightRect);
	}
	else if (currentState == GameState::Background) {
		scene.render(window);

		for (const auto& car : cars) {
			window.draw(car);
		}

		window.draw(dogSprite);

		if (isGameOver) {
			sf::Text gameOverText;
			gameOverText.setFont(font);
			gameOverText.setString("GAME OVER! \n Press R to Restart");
			gameOverText.setCharacterSize(100);
			gameOverText.setFillColor(sf::Color::Red);
			gameOverText.setPosition(300, 250);
			window.draw(gameOverText);
		}


	}

	window.draw(statisticsText);
	window.display();
}

void Game::sUpdate(sf::Time dt) {
	entityManager.update();

	if (isPaused || currentState != GameState::Background)
		return;

	sMovement(dt);
	sCollision();

	if (animationClock.getElapsedTime().asSeconds() > 0.1f) {
		dogAnimationFrame = (dogAnimationFrame + 1) % 3;
		sf::IntRect textureRect = dogSprite.getTextureRect();
		textureRect.left = dogAnimationFrame * 32;
		dogSprite.setTextureRect(textureRect);

		animationClock.restart();
	}

	dogSprite.setPosition(dogPosition);

	float spawnInterval = 1.5f;

	if (carSpawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
		sf::Sprite car;
		car.setTexture(carSheetTexture);

		int carIndex = rand() % 3;
		car.setTextureRect(carFrames[carIndex]);

		int laneX[] = { 450, 640, 830 };
		car.setPosition(laneX[rand() % 3], -220);

		car.setScale(0.5f, 0.5f);

		cars.push_back(car);
		carSpawnClock.restart();
	}

	for (auto& car : cars) {
		car.move(0, carSpeed * dt.asSeconds());
	}

	cars.erase(std::remove_if(cars.begin(), cars.end(),
		[&](const sf::Sprite& c) { return c.getPosition().y > windowSize.y; }),
		cars.end());

	for (const auto& car : cars) {
		float carX = car.getPosition().x;
		float carY = car.getPosition().y;

		if (carY < 0) continue;

		float dogX = dogSprite.getPosition().x;
		float dogY = dogSprite.getPosition().y;

		if (std::abs(carX - dogX) > 50)
			continue;

		sf::FloatRect dogBounds = dogSprite.getGlobalBounds();
		dogBounds.left += 10;
		dogBounds.width -= 20;
		dogBounds.top += 5;
		dogBounds.height -= 10;

		if (dogBounds.intersects(car.getGlobalBounds())) {
			isGameOver = true;

			backgroundMusic.stop();

			gameOverMusic.setVolume(100);
			gameOverMusic.play();

			break;
		}
	}

	for (const auto& car : cars) {
		sf::FloatRect dogBounds = dogSprite.getGlobalBounds();
		sf::FloatRect carBounds = car.getGlobalBounds();

		dogBounds.left += 10;
		dogBounds.width -= 20;
		dogBounds.top += 5;
		dogBounds.height -= 10;

		float carX = car.getPosition().x;
		float dogX = dogSprite.getPosition().x;

		if (abs(carX - dogX) > 50)
			continue;

		if (dogBounds.intersects(carBounds)) {
			isGameOver = true;
			break;
		}
	}

}

void Game::sUserInput() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			isRunning = false;
		}

		if (event.type == sf::Event::KeyPressed) {
			if (isGameOver && event.key.code == sf::Keyboard::R) {
				isGameOver = false;
				cars.clear();
				dogPosition = sf::Vector2f(640.f, 600.f);

				backgroundMusic.play();
			}

			if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}

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

	if (currentState == GameState::Background && !isGameOver) {
		sf::Vector2f direction(0.f, 0.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			direction.y -= 1;
			dogSprite.setTextureRect(sf::IntRect(0, 96, 32, 32));  // Set upward-facing frame
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			direction.y += 1;
			dogSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));  // Set downward-facing frame
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			direction.x -= 1;
			dogSprite.setTextureRect(sf::IntRect(0, 32, 32, 32));  // Set left-facing frame
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			direction.x += 1;
			dogSprite.setTextureRect(sf::IntRect(0, 64, 32, 32));  // Set right-facing frame
		}

		if (direction.x != 0 || direction.y != 0) {
			direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);
			dogPosition += direction * dogSpeed * TIME_PER_FRAME.asSeconds();
		}

		sf::Vector2u windowSize = window.getSize();
		dogPosition.x = std::max(0.f, std::min(dogPosition.x, windowSize.x - 32.f));  // Keep within X
		dogPosition.y = std::max(0.f, std::min(dogPosition.y, windowSize.y - 32.f));  // Keep within Y
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

	if (!dogTexture.loadFromFile(assetsPath + "dog.png")) {
		std::cerr << "Failed to load dog.png. Check path: " << assetsPath + "dog.png" << "\n";
		exit(-1);
	}
	dogTexture.setSmooth(true);
	dogSprite.setTexture(dogTexture);
	dogSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	dogSprite.setPosition(dogPosition);
	dogSprite.setScale(2.0f, 2.0f);

	if (!carSheetTexture.loadFromFile(assetsPath + "cars.png")) {
		std::cerr << "Failed to load cars.png\n";
		exit(-1);
	}

	int carWidth = 120;
	int carHeight = 220;
	int numCars = 3;

	for (int i = 0; i < numCars; i++) {
		int x = i * carWidth;
		int y = 0;
		carFrames.push_back(sf::IntRect(x, y, carWidth, carHeight));
	}

	if (!backgroundMusic.openFromFile(assetsPath + "backmusic.mp3")) {
		std::cerr << "Failed to load background music!" << std::endl;
	}
	else {
		backgroundMusic.setLoop(true);
		backgroundMusic.setVolume(100);
		backgroundMusic.play();
	}

	if (!gameOverMusic.openFromFile(assetsPath + "gameover.mp3")) {
		std::cerr << "Failed to load game over music!" << std::endl;
	}

	std::cout << "Car textures initialized successfully.\n";
}

sf::FloatRect Game::getViewBounds() {
	sf::View view = window.getView();
	return sf::FloatRect(
		view.getCenter().x - (view.getSize().x / 2.f),
		view.getCenter().y - (view.getSize().y / 2.f),
		view.getSize().x,
		view.getSize().y
	);
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