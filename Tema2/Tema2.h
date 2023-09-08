#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema1/object2D_1.h"
#include "lab_m1/Tema1/transform2D_1.h"

#include "Structures.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#include <time.h>

#include <chrono>
#include <thread>

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

using namespace std;

namespace m1
{
    class Game : public gfxc::SimpleScene
    {
    public:
        Game();
        ~Game();

        void Init() override;

        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void generateMaze();
        vector<struct cell> Game::findNeighs(int row, int col, char is_wall);

        void Game::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);

        void drawMaze();
        void drawPlayer();
        void drawProjectiles();
        void drawEnemies();
        void drawHUD();

        int get_row(float x);
        int get_col(float x);

    protected:
        glm::mat4 modelMatrix;
        int rotation;
        implemented::Camera1* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // define variables here
        int nr_cells = 10;
        int maze_size = 2 * nr_cells + 1;
        vector<string> maze;
        vector<glm::vec3> walls;

        float max_health = 100;
        float current_health = max_health;
        float damage = 20; // health loss after a collision with an enemy
        bool game_over = false;

        int exit_column; // column where we can find the exit (the exit will always be on the last row)

        float initial_time = 120;
        float time_left = initial_time;

        ViewportSpace viewSpace;

        // player
        glm::vec3 player_position;
        float player_angle = 0;

        // projectiles
        void create_projectile();

        float projectile_speed = 4.0f;

        float fire_rate = 0.5f; // minimum interval between firing two consecutive bullets
        float last_fired = 0; // point in time at which the last bullet was fired

        struct projectile
        {   
            projectile() {}
            projectile(glm::vec3 position, glm::vec3 forward, float angle)
                : position(position), forward(forward), angle(angle) {}
            glm::vec3 position;
            glm::vec3 forward;
            float angle;
        };

        vector<struct projectile> projectiles;
        bool Game::projectile_wall_collision(struct projectile p);

        // enemies
        void create_enemy();

        float enemy_height = 1.0f;
        float enemy_width = 0.2f;
        float enemy_speed = 1.5f;

        struct enemy
        {
            struct enemy() {}
            glm::vec3 position;
            glm::vec3 forward;

            glm::vec3 next_point;
            struct cell enemy_cell;
            bool has_been_hit;
            float time_to_disappear;
        };

        vector<struct enemy> enemies;

        bool projectile_enemy_collision(struct projectile p, struct enemy e);
        bool check_enemy_has_arrived(struct enemy e);
        bool player_enemy_collision(struct enemy e);
    };
}   // namespace m1
