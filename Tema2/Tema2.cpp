#include "lab_m1/Tema2/Tema2.h"

using namespace std;
using namespace m1;

void Game::drawMaze() {
    // render each wall
    for (int i = 0; i < maze_size; i++) {
        for (int j = 0; j < maze_size; j++) {
            if (maze[i][j] == '*') {
                int pos_x = (float)i;
                int pos_z = (float)j;

                glm::mat4 modelMatrix = glm::mat4(1);
                glm::vec3 cubePosition;
                cubePosition.x = pos_x;
                cubePosition.y = 0.5f;
                cubePosition.z = pos_z;
                walls.push_back(cubePosition);

                modelMatrix = glm::translate(modelMatrix, cubePosition);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
                RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
            }
        }
    }
    
    // render the plane
    glm::mat4 modelMatrix = glm::mat4(1);
    glm::vec3 planePosition = glm::vec3(0, 0, 0);
    modelMatrix = glm::translate(modelMatrix, planePosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1));
    RenderSimpleMesh(meshes["plane"], shaders["ObjectShader"], modelMatrix, glm::vec3(0.5, 0.5, 0.5));
}

void Game::drawPlayer() {
    if (renderCameraTarget == true) // third person
    {
        // player's head
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::rotate(modelMatrix, player_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f));
            RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        }
        
        // player's arms
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::rotate(modelMatrix, player_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.06, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.04f, 0.15f));
            RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        }

        // player's torso
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::rotate(modelMatrix, player_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.03, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.04f, 0.1f));
            RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        }

        // player's left leg
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::rotate(modelMatrix, player_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.03));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.1f, 0.04f));
            RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        }

        // player's right leg
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
            modelMatrix = glm::rotate(modelMatrix, player_angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.03));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.04f, 0.1f, 0.04f));
            RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
        }
    }
}

void Game::drawEnemies() {
    // for each enemy
    for (auto e : enemies) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, e.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(enemy_width, enemy_height, enemy_width));

        // if the enemy has been hit, we use the noise shader; otherwise we use the default shader
        if (e.has_been_hit) {
            RenderSimpleMesh(meshes["box"], shaders["ObjectShader_Noise"], modelMatrix, glm::vec3(0.06, 0.7, 0.66));
        }
        else {
            RenderSimpleMesh(meshes["box"], shaders["ObjectShader"], modelMatrix, glm::vec3(0.06, 0.7, 0.66));
        }
    }
}

void Game::drawProjectiles() {
    for (auto p : projectiles) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, p.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02));
        RenderSimpleMesh(meshes["box"], shaders["ObjectShader"], modelMatrix, glm::vec3(1, 0, 0));
    }
}

/*
* draw the bars by creating viewports  (healthbar and timebar)
* 2 viewports for each bar (one is the wireframe - white, and the other one is the actual bar - red or blue)
*/
void Game::drawHUD() {
    glm::ivec2 resolution = window->GetResolution();

    // define the lower left corners of the bars
    float bar_x = resolution.x * 0.6;
    float healthbar_y = resolution.y * 0.88;
    float timebar_y = resolution.y * 0.76;

    // define bars' sizes
    float bar_length = resolution.x * 0.3;
    float bar_width = resolution.y * 0.05;

    // draw the wireframe for the healthbar - white
    viewSpace = ViewportSpace(bar_x, healthbar_y, bar_length, bar_width);
    SetViewportArea(viewSpace, glm::vec3(1.0f, 1.0f, 1.0f), true);

    // draw the healthbar - red
    float healthbar_length = (current_health / max_health) * bar_length;
    viewSpace = ViewportSpace(bar_x, healthbar_y, healthbar_length, bar_width);
    SetViewportArea(viewSpace, glm::vec3(1.0f, 0.0f, 0.0f), true);

    // draw the wireframe for the timebar - white
    viewSpace = ViewportSpace(bar_x, timebar_y, bar_length, bar_width);
    SetViewportArea(viewSpace, glm::vec3(1.0f, 1.0f, 1.0f), true);

    // draw the timebar - blue
    float timebar_length = (time_left / initial_time) * bar_length;
    viewSpace = ViewportSpace(bar_x, timebar_y, timebar_length, bar_width);
    SetViewportArea(viewSpace, glm::vec3(0.0f, 0.0f, 1.0f), true);
}

Game::Game()
{
}


Game::~Game()
{
}

void Game::Init()
{
    renderCameraTarget = true;

    srand(time(0));
    generateMaze();

    player_position.y = 0.3f;

    camera = new implemented::Camera1();
    camera->Set(player_position, glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    camera->RotateFirstPerson_OY(atan2(player_position.z, player_position.x)); // make sure the camera is aligned with the player

    player_position = camera->GetTargetPosition();

    // generate enemies
    for (int i = 0; i < 10; i++) {
        create_enemy();
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // create shaders
    {
        Shader* shader = new Shader("ObjectShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ObjectShader_Noise");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "VertexShader_Noise.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "Shaders", "FragmentShader_Noise.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, Z_NEAR, Z_FAR);
}

void Game::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Game::Update(float deltaTimeSeconds)
{   
    time_left -= deltaTimeSeconds;
    if (time_left < 0.01f) {
        game_over = true;
    }

    if (game_over) {
        cout << "Game over" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        exit(0);
    }

    // check if player reached the exit
    if (get_row(player_position.x) == maze_size - 1 && get_col(player_position.z) == exit_column) {
        cout << "Congratulations! You found the exit!" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        exit(0);
    }

    // update projectiles
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].position += deltaTimeSeconds * projectile_speed * projectiles[i].forward;

        if (projectile_wall_collision(projectiles[i])) {
            // check collision with a wall
            projectiles.erase(projectiles.begin() + i);
            break;
        }
        else if (projectiles[i].position.y < 0) {
            // collision with the floor
            projectiles.erase(projectiles.begin() + i);
            break;
        }
        
        // check collisions with enemies
        for (int j = 0; j < enemies.size(); j++) {
            if (projectile_enemy_collision(projectiles[i], enemies[j])) {
                enemies[j].has_been_hit = true;
                projectiles.erase(projectiles.begin() + i);
                i--;
                break;
            }
        }
    }

    for (int i = 0; i < enemies.size(); i++) {
        // check player - enemy collision
        if (player_enemy_collision(enemies[i])) {
            enemies[i].has_been_hit = true;
            current_health -= damage;

            if (current_health < 0.01f) {
                game_over = true;
            }
        }

        // if the enemy was hit, update it's countdown until it disappears
        if (enemies[i].has_been_hit) {
            enemies[i].time_to_disappear -= deltaTimeSeconds;

            if (enemies[i].time_to_disappear < 0.01f) {
                // delete the enemy
                enemies.erase(enemies.begin() + i);
                i--; // elements in the 'enemies' vector starting from index i shift left
            }

            continue;
        }

        // check if the enemy has arrived to his next position
        // if he did, then pick the next position
        // else update it's position (move towards the next position)
        if (check_enemy_has_arrived(enemies[i])) {
            enemies[i].position.x = enemies[i].next_point.x;
            enemies[i].position.z = enemies[i].next_point.z;

            // choose the next position (out of the 4 vertices of the square/cell)
            int choice = rand() % 2;
            if (choice) {
                enemies[i].next_point.x = (float) enemies[i].enemy_cell.x + 0.4f;
            }
            else {
                enemies[i].next_point.x = (float) enemies[i].enemy_cell.x - 0.4f;
            }

            choice = rand() % 2;
            if (choice) {
                enemies[i].next_point.z = (float) enemies[i].enemy_cell.y + 0.4f;
            }
            else {
                enemies[i].next_point.z = (float) enemies[i].enemy_cell.y - 0.4f;
            }
        }
        else {
            // update the position; move towards enemies[i].next_point
            // we actually have to compare float numbers, that is why we use the 0.001f tolerance

            // move towards next_point.x on the x axis
            if (enemies[i].position.x - enemies[i].next_point.x < -0.001f) {
                enemies[i].position.x += deltaTimeSeconds * enemy_speed;
            }
            else if (enemies[i].position.x - enemies[i].next_point.x > 0.001f) {
                enemies[i].position.x -= deltaTimeSeconds * enemy_speed;
            }

            // move towards next_point.z on the z axis
            if (enemies[i].position.z - enemies[i].next_point.z < -0.001f) {
                enemies[i].position.z += deltaTimeSeconds * enemy_speed;
            }
            else if (enemies[i].position.z - enemies[i].next_point.z > 0.001f) {
                enemies[i].position.z -= deltaTimeSeconds * enemy_speed;
            }
        }
    }

    drawMaze();
    drawPlayer();
    drawProjectiles();
    drawEnemies();
    drawHUD();
}


void Game::FrameEnd()
{   
   // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Game::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    mesh->Render();
}

void Game::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int location = glGetUniformLocation(shader->GetProgramID(), "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    location = glGetUniformLocation(shader->GetProgramID(), "View");

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    // TODO(student): Set shader uniform "View" to viewMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->GetProgramID(), "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // set the color
    GLint object_color = glGetUniformLocation(shader->program, "color");
    glUniform3fv(object_color, 1, glm::value_ptr(color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Game::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        // Translate the camera forward
        if (window->KeyHold(GLFW_KEY_W))
        {   
            float factor = 1.1f; // use this to approximate the collision better
            glm::vec3 next_pos = player_position + camera->forward * deltaTime * cameraSpeed * factor;
            if (maze[get_row(next_pos.x)][get_col(next_pos.z)] != '*') {
                player_position = next_pos;
                camera->MoveForward(deltaTime * cameraSpeed);
            }
        }

        // Translate the camera to the left
        if (window->KeyHold(GLFW_KEY_A))
        {   
            glm::vec3 next_pos = player_position - camera->right * deltaTime * cameraSpeed;
            next_pos -= camera->right * glm::vec3(0.07f); // use this to approximate the collision better
            if (maze[get_row(next_pos.x)][get_col(next_pos.z)] != '*') {
                player_position = next_pos;
                camera->TranslateRight(-deltaTime * cameraSpeed);
            }
        }

        if (window->KeyHold(GLFW_KEY_S))
        {
            // Translate the camera backwards
            float factor = 1.1f; // use this to approximate the collision better
            glm::vec3 next_pos = player_position - camera->forward * deltaTime * cameraSpeed * factor;
            if (maze[get_row(next_pos.x)][get_col(next_pos.z)] != '*') {
                player_position = next_pos;
                camera->MoveForward(-deltaTime * cameraSpeed);
            }
        }

        if (window->KeyHold(GLFW_KEY_D))
        {
            // Translate the camera to the right
            float factor = 1.07f;
            glm::vec3 next_pos = player_position + camera->right * deltaTime * cameraSpeed * factor;
            next_pos += camera->right * glm::vec3(0.07f); // use this to approximate the collision better
            if (maze[get_row(next_pos.x)][get_col(next_pos.z)] != '*') {
                player_position = next_pos;
                camera->TranslateRight(deltaTime * cameraSpeed);
            }
        }

        /*if (window->KeyHold(GLFW_KEY_F))
        {
            // Translate the camera to the right
            float x = camera->position.x;
            float z = camera->position.z;
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_C))
        {
            // Translate the camera to the right
            float x = camera->position.x;
            float z = camera->position.z;
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }*/

        if (renderCameraTarget) {
            player_position = camera->GetTargetPosition();
        }
        else {
            player_position = camera->position;
        }
        
    }
}


void Game::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T)
    {   
        renderCameraTarget = !renderCameraTarget;
        if (!renderCameraTarget) {
            // first person
            camera->position = camera->GetTargetPosition();
        }
        else {
            // third person
            camera->position -= camera->forward * camera->distanceToTarget;
            camera->position.y += 0.2f; // adjust the height
        }
    }
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (!renderCameraTarget)// first-person mode
        {
            camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
        else {
            camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
        
        player_angle -= deltaX * sensivityOX;
    }
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !renderCameraTarget) {
        create_projectile();
    }
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}
