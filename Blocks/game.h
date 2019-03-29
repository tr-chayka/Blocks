#pragma once
#include "utils.h"

namespace Game
{
	class Game
	{
	public:
		void newGame();
		void rotate();
		void moveLeft();
		void moveRight();
		void pause();
		void resume();

		bool isPause() const;
		bool isGameOver() const;
		bool isStarted() const;

		UINT lines() const;
		UINT level() const;
		UINT score() const;
		UINT delay() const;

		bool step();

		void setFullSpeed();
		bool isFullSpeed() const;

		UINT getNextFigureIndex() const;
		UINT get(UINT x, UINT y) const;

	private:
		void nextFigure();
		void updateTempField();
		
		void move(const Point &);
		void removeLine(UINT);
		void setFigure();
		void calc(UINT);

		bool gameOver() const;

		bool valid(const Figure &, const Point &) const;
		bool validPoint(const Point &p) const;
		
		bool stop() const;

	private:
		bool m_isStarted = false;
		bool m_isPause = false;
		bool m_isGameOver = false;
		bool m_isFullSpeed = false;

		UINT m_level = 0;
		UINT m_lines = 0;
		UINT m_score = 0;
		UINT m_delay = 0;

		UINT m_currentFigureIndex = 0;
		UINT m_nextFigureIndex = 0;

		UINT m_scorePerLine = 0;
		UINT m_linesForLevel = 0;

		Point m_currentPosition;
		Figure m_currentFigure;
		
		int m_field[GAME_FIELD_WIDTH][GAME_FIELD_HEIGHT];
		int m_tempField[GAME_FIELD_WIDTH][GAME_FIELD_HEIGHT];
	};
};