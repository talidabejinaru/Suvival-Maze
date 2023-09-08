#include "lab_m1/Tema2/Tema2.h"

using namespace std;
using namespace m1;

/*
* returns all neighbors which are all either walls or non-walls
*/
vector<struct cell> Game::findNeighs(int row, int col, char is_wall) {
	vector<struct cell> neighs;

	if (row > 1 && maze[row - 2][col] == is_wall) {
		neighs.push_back(cell(row - 2, col));
	}

	if (row < maze_size - 2 && maze[row + 2][col] == is_wall) {
		neighs.push_back(cell(row + 2, col));
	}

	if (col > 1 && maze[row][col - 2] == is_wall) {
		neighs.push_back(cell(row, col - 2));
	}

	if (col < maze_size - 2 && maze[row][col + 2] == is_wall) {
		neighs.push_back(cell(row, col + 2));
	}

	return neighs;
}


/*
* implemented Aldous Broder algorithm
* 1. pick random cell
  2. pick random neighbor and visit it
  3. repeat until all cells have been visited.
*/
void Game::generateMaze() {
	// init the maze
	maze.clear();

	for (int i = 0; i < maze_size; i++) {
		maze.push_back(string(maze_size, '*'));
	}

	// generate random starting position
	// rows and columns must be odd numbers; the graph contains only the cells located at odd rows/columns
	vector<int> nodes;
	for (int i = 1; i < maze_size; i += 2) {
		nodes.push_back(i);
	}
	int current_row = nodes[rand() % nodes.size()];
	int current_col = nodes[rand() % nodes.size()];

	maze[current_row][current_col] = ' ';
	int nr_visited = 1;

	while (nr_visited < nr_cells * nr_cells) {
		vector<struct cell> neighs = findNeighs(current_row, current_col, '*');

		if (neighs.size() == 0) {
			// pick random neighbor as current cell if all have been visited
			vector<struct cell> neighs_2 = findNeighs(current_row, current_col, ' ');
			struct cell tmp = neighs_2[rand() % neighs_2.size()];
			current_row = tmp.x;
			current_col = tmp.y;
			continue;
		}

		while (neighs.size() != 0) {
			// extract random neighbor and remove it from the list of neighbours
			int index = rand() % neighs.size();
			struct cell neighbor = neighs[index];
			neighs.erase(neighs.begin() + index);
			
			if (maze[neighbor.x][neighbor.y] == '*') {
				// open up the wall to new neighbor
				maze[(neighbor.x + current_row) / 2][(neighbor.y + current_col) / 2] = ' ';

				// the neighbor is marked as visited
				maze[neighbor.x][neighbor.y] = ' ';

				// increment number of visited nodes
				nr_visited++;

				// the neighbor becomes the current node
				current_row = neighbor.x;
				current_col = neighbor.y;

				break;
			}
		}
	}

	// choose an exit
	for (int i = 1; i < maze_size - 1; i++) {
		// make sure the exit is not blocked by an wall
		if (maze[maze_size - 2][i] == ' ') {
			maze[maze_size - 1][i] = '@';
			exit_column = i;
			break;
		}
	}

	// choose player starting position (randomly)
	int row;
	row = (1 + (rand() % (maze_size / 2))); // pick a random row from the upper half of the maze
	player_position.x = (float)row;

	// add all empty cells to a vector so we can extract a random cell
	// at least one cell will be found
	vector<int> columns;
	for (int i = 1; i < maze_size - 1; i++) {
		if (maze[row][i] == ' ') {
			columns.push_back(i);
		}
	}

	float col = rand() % columns.size();
	player_position.z = (float)columns[col];

	maze[row][col] = 'p';

	// print it
	for (auto str : maze) {
		cout << str << endl;
	}
}