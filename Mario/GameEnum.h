#pragma once

enum class CardType {
	EMPTY,
	MUSHROOM,
	STAR,
	FLOWER
};

enum class SceneResult {
	WIN,
	LOSE,
	TIMEOUT
};

enum class DamgeSource {
	ENEMY,
	MARIO,
	MARIO_WEAPON,
	SPACE,
	SOLID_BLOCK
};

enum class ScoreSource {
	POWERUP,
	QUESTION_BLOCK,
	DAMAGE_ENEMY,
	SERVER_KILL
};

enum class CoinState {
	COIN,
	BRICK
};

enum class Score
{
	S100 = 100,
	S200 = 200,
	S400 = 400,
	S800 = 800,
	S1000 = 1000,
	S2000 = 2000,
	S4000 = 4000,
	S8000 = 8000,
	S1UP = 1
};

enum class GameState {
	GAME_START,
	GAME_INTRO,
	GAME_MENU,
	GAME_WORLDMAP,
	GAME_OVER,
	WORLD_START,
	WORLD_RUN,
	WORLD_LOSE,
	WORLD_WIN,
	TRANSLATION
};

enum class SceneType {
	INTRO,
	MENU,
	WORLDMAP,
	PLAYSCENE,
	GAMEOVER
};