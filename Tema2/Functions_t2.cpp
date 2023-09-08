#include "lab_m1/Tema2/Tema2.h"

using namespace std;
using namespace m1;

void Game::create_projectile() {
	struct projectile p = struct projectile();

	p.position = player_position;
	p.position.y -= 0.1f;
	p.forward = camera->forward;

	projectiles.push_back(p);
}

void Game::create_enemy() {
	struct enemy e;
	e.forward = glm::vec3(0);
	e.has_been_hit = false;
	e.time_to_disappear = 5.0f; // enemy will disappear after 5 seconds after being hit

	// choose a random and valid starting cell
	while (1) {
		int i = 1 + (rand() % (maze_size - 1)); //  between 1 and (maze_size - 2);
		int j = 1 + (rand() % (maze_size - 1));

		if (maze[i][j] == ' ') {
			e.position.x = float(i);
			e.position.z = float(j);
			e.position.y = 0.3f;

			e.enemy_cell.x = i;
			e.enemy_cell.y = j;

			maze[i][j] = 'e'; // the enemy will be in this cell
			break;
		}
 	}

	// pick the next point the enemy will travel to
	// the enemy can travel between the 4 vertices of the square he's in
	// e.position is now in the center of the square
	int choice = rand() % 2;
	if (choice) {
		e.next_point.x = e.position.x + 0.4f;
	}
	else {
		e.next_point.x = e.position.x - 0.4f;
	}

	choice = rand() % 2;
	if (choice) {
		e.next_point.z = e.position.z + 0.4f;
	}
	else {
		e.next_point.z = e.position.z - 0.4f;
	}

	enemies.push_back(e);
}

bool Game::check_enemy_has_arrived(struct enemy e) {
	// checks if the enemy has arrived to the next point
	if (abs(e.position.x - e.next_point.x) < 0.01f && abs(e.position.z - e.next_point.z) < 0.01f) {
		return true;
	}

	return false;
}

bool Game::projectile_wall_collision(struct projectile p) {
	if (maze[get_row(p.position.x)][get_col(p.position.z)] == '*' ||
		maze[get_row(p.position.x)][get_col(p.position.z)] == '@') {
		return true;
	}

	return false;
}

bool Game::projectile_enemy_collision(struct projectile p, struct enemy e) {
	bool collision = 
		p.position.x > e.position.x - enemy_width / 2 && p.position.x < e.position.x + enemy_width / 2 &&
		p.position.z > e.position.z - enemy_width / 2 && p.position.z < e.position.z + enemy_width / 2 &&
		p.position.y > e.position.y - enemy_height / 2 && p.position.y < e.position.y + enemy_height / 2;

	return collision;
}

bool Game::player_enemy_collision(struct enemy e) {
	// if the enemy is already dead, then collision won't be checked
	if (e.has_been_hit) {
		return false;
	}

	bool collision =
		player_position.x > e.position.x - enemy_width / 2 - 0.02f && player_position.x < e.position.x + enemy_width / 2 + 0.02f &&
		player_position.z > e.position.z - enemy_width / 2 - 0.02f && player_position.z < e.position.z + enemy_width / 2 + 0.02f;

	return collision;
}

/* returns the row in the maze corresponding to the x coordinate
* x between -0.5 and 0.5 => row 0
* x between 0.5 and 1.5 => row 1
* and so on
*/
int Game::get_row(float x) {
	if (x - floor(x) > 0.5) {
		return floor(x) + 1;
	}
	else {
		return floor(x);
	}
}

int Game::get_col(float z) {
	// same as get_row
	return get_row(z);
}