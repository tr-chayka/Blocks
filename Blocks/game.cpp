#include "game.h"
#include <cstdlib>
#include <ctime>

namespace Game
{
	bool Game::validPoint(const Point &position) const
	{
		return
			position.x >= 0 && position.x < GAME_FIELD_WIDTH &&
			position.y >= 0 && position.y < GAME_FIELD_HEIGHT &&
			m_field[position.x][position.y] == 0;
	}

	bool Game::valid(const Figure &figure, const Point &position) const
	{
		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
			if (!validPoint(position + figure.points[i]))
				return false;

		return true;
	}

	void Game::updateTempField()
	{
		for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
			for (UINT j = 0; j < GAME_FIELD_HEIGHT; j++)
				m_tempField[i][j] = m_field[i][j];

		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
		{
			Point p = m_currentPosition + m_currentFigure.points[i];
			m_tempField[p.x][p.y] = m_currentFigureIndex;
		}
	}

	void Game::removeLine(UINT line)
	{
		for (UINT j = line; j > 0; j--)
			for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
				m_field[i][j] = m_field[i][j - 1];

		for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
			m_field[i][0] = 0;
	}

	bool Game::stop() const
	{
		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
		{
			Point p = m_currentPosition + m_currentFigure.points[i];

			if (p.y >= GAME_FIELD_HEIGHT - 1 || m_field[p.x][p.y + 1] > 0)
				return true;
		}

		return false;
	}

	void Game::nextFigure()
	{
		m_currentFigureIndex = m_nextFigureIndex;
		m_nextFigureIndex = random();

		m_currentPosition = START_POSITION[m_currentFigureIndex - 1];
		m_currentFigure = getFigure(m_currentFigureIndex - 1);
	}

	void Game::setFigure()
	{
		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
		{
			Point p = m_currentPosition + m_currentFigure.points[i];
			m_field[p.x][p.y] = m_currentFigureIndex;
		}
	}

	void Game::calc(UINT lines)
	{
		if (lines == 0)
			return;

		m_score += m_scorePerLine * SCORE_RATIO_X[lines - 1];
		m_lines += lines;

		if (m_lines >= m_linesForLevel)
		{
			m_level++;
			m_scorePerLine *= SCORE_PER_LEVEL_RATIO;
			m_linesForLevel *= LINES_FOR_LEVEL_RATIO;

			if (m_delay > MIN_DELAY)
				m_delay -= DELAY_DELTA;
		}
	}

	bool Game::gameOver() const
	{
		for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
			if (m_field[i][0] != 0)
				return true;

		return false;
	}
	
	void Game::newGame()
	{
		srand(time(nullptr));

		m_level = 1;
		m_lines = 0;
		m_score = 0;

		m_delay = MAX_DELAY;

		m_isStarted = true;
		m_isPause = false; 
		m_isGameOver = false;
		m_isFullSpeed = false;

		m_scorePerLine = MIN_SCORE_PER_LINE;
		m_linesForLevel = MIN_LINES_FOR_LEVEL;

		for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
			for (UINT j = 0; j < GAME_FIELD_HEIGHT; j++)
				m_tempField[i][j] = m_field[i][j] = 0;

		m_nextFigureIndex = random();
		nextFigure();
		updateTempField();
	}

	void Game::pause()
	{
		if (m_isStarted && !m_isGameOver)
			m_isPause = true;
	}

	void Game::resume()
	{
		if (m_isStarted && !m_isGameOver)
			m_isPause = false;
	}

	void Game::rotate()
	{
		if (!m_isStarted || m_isGameOver || m_isPause || m_currentFigureIndex == 1)
			return;

		if (valid(rotateFigure(m_currentFigure), m_currentPosition))
		{
			m_currentFigure = rotateFigure(m_currentFigure);
			updateTempField();
		}
	}

	void Game::moveRight()
	{
		move({ 1, 0 });
	}

	void Game::moveLeft()
	{
		move({ -1, 0 });
	}

	void Game::move(const Point &dir)
	{
		if (!m_isStarted || m_isGameOver || m_isPause)
			return;

		if (valid(m_currentFigure, m_currentPosition + dir))
		{
			m_currentPosition = m_currentPosition + dir;
			updateTempField();
		}
	}

	bool Game::step()
	{
		if (!m_isStarted || m_isGameOver || m_isPause)
			return false;

		if (stop())
		{
			setFigure();

			if (gameOver())
			{
				m_isStarted = false;
				m_isGameOver = true;
				return true;
			}

			UINT lines = 0;

			for (UINT j = 0; j < GAME_FIELD_HEIGHT; j++)
			{
				bool isFull = true;
				for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
					if (m_field[i][j] == 0)
						isFull = false;
			
				if (isFull)
				{
					lines++;
					removeLine(j);
				}
			}

			calc(lines);
			nextFigure();
			updateTempField();
		
			return true;
		}
		
		m_currentPosition.y++;
		updateTempField();
		return false;
	}

	bool Game::isPause() const
	{
		return m_isPause;
	}

	bool Game::isGameOver() const
	{
		return m_isGameOver;
	}

	bool Game::isStarted() const
	{
		return m_isStarted;
	}

	UINT Game::lines() const
	{
		return m_lines;
	}

	UINT Game::level() const
	{
		return m_level;
	}

	UINT Game::score() const
	{
		return m_score;
	}

	UINT Game::delay() const
	{
		return m_delay;
	}

	UINT Game::getNextFigureIndex() const
	{
		return m_nextFigureIndex;
	}

	UINT Game::get(UINT x, UINT y) const
	{
		return m_tempField[x][y];
	}

	void Game::setFullSpeed()
	{
		if (!m_isStarted || m_isGameOver || m_isPause)
			return;

		m_isFullSpeed = !m_isFullSpeed;
	}

	bool Game::isFullSpeed() const
	{
		return m_isFullSpeed;
	}
}