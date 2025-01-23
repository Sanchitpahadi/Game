#include"main.h"





glm::vec3 trianglePosition(0.0f, -0.6f, 0.0f);
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Bullet> enemyBullets;

float enemySpawnCooldown = 0.0001f;
float enemySpeed = 0.01f;
int enemyKillCount = 0; // Counter for killed enemies

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void movement(GLFWwindow* window);



int main() {
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    GLFWwindow*window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string vspath = readShaderFromFile("shader.vs");
    const char* vss = vspath.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vss, NULL);
    glCompileShader(vertexShader);

    std::string fspath = readShaderFromFile("shader.fs");
    const char* fss = fspath.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fss, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float startscreen[] = {
        // Position         // Texture Coordinates
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top-left
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f   // Bottom-left
    };

    float wholescreen[] = {
        // Position         // Texture Coordinates
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top-left
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top-right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom-right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f   // Bottom-left
    };
    // Vertex data for objects


    float vertices[] = {
        // Positions           // Texture coordinates
        -0.12f ,  0.12f, 0.0f,   0.0f, 1.0f,   // Top left
         0.12f,  0.12f, 0.0f,   1.0f, 1.0f,   // Top right
         0.12f, -0.12f, 0.0f,   1.0f, 0.0f,   // Bottom right
        -0.12f, -0.12f, 0.0f,   0.0f, 0.0f    // Bottom left
    };

    float bulletVertices[] = {
        // Position       // Texture Coordinates
        -0.02f,  0.05f, 0.0f,  0.0f, 1.0f,  // Top-left
         0.02f,  0.05f, 0.0f,  1.0f, 1.0f,  // Top-right
         0.02f, -0.05f, 0.0f,  1.0f, 0.0f,  // Bottom-right
        -0.02f, -0.05f, 0.0f,  0.0f, 0.0f   // Bottom-left
    };

    unsigned int indices[] = { 0, 1, 3, 1, 2, 3 };

    unsigned int ebo;
    unsigned int wsebo;
    unsigned int startebo;
    // for whole square
    VertexBuffer wsVB(sizeof(wholescreen), wholescreen);
    VertexBufferLayout wslayout;
    glGenBuffers(1, &wsebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wsebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    wslayout.push<float>(3);
    wslayout.push<float>(2);
    VertexArray wsA;
    wsA.AddBuffer(wsVB, wslayout);
    //for starting
    VertexBuffer startVB(sizeof(startscreen), startscreen);
    VertexBufferLayout startlayout;
    glGenBuffers(1, &startebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, startebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    startlayout.push<float>(3);
    startlayout.push<float>(2);
    VertexArray startA;
    startA.AddBuffer(startVB, wslayout);

    VertexBuffer vbSquare(sizeof(vertices), vertices);
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    VertexArray va;
    va.AddBuffer(vbSquare, layout);

    VertexBuffer bulletBuffer(sizeof(bulletVertices), bulletVertices);
    VertexBufferLayout bulletLayout;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    bulletLayout.push<float>(3);
    bulletLayout.push<float>(2);

    VertexArray bA;
    bA.AddBuffer(bulletBuffer, bulletLayout);

    Texture tex("container.png");
    Texture tex2("bullet.png");
    Texture tex3("Gray2.png");
    Texture texforstart("starting.png");
    Texture texwhileplay1("background.png");
    Texture texwhileplay2("background2.png");
    Texture texwhileplay3("background4.png");
    Texture texforgover("ending.png");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        wsA.Bind();
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix (no translation)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        texforstart.Bind(1);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            break;

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Main game loop
    do {
        movement(window);
        glUseProgram(shaderProgram);
        glClear(GL_COLOR_BUFFER_BIT);

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        

        glm::mat4 model = glm::mat4(1.0f); // Identity matrix (no translation)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // First Rectangle
        texwhileplay1.Bind(1);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
        wsA.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  
        model = glm::translate(model, trianglePosition);
       
         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
         tex.Bind(0);
         glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
         va.Bind();
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
            static bool wasPressed = false;
            bool isPressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

            if (isPressed && !wasPressed) {
                bullets.emplace_back(trianglePosition.x, trianglePosition.y + 0.18f);
            }
            wasPressed = isPressed;

            for (auto& bullet : bullets) {
                bullet.PlayerBulletUpdate();

                // Check for collision and mark for deletion if collided
                for (auto& enemy : enemies) {
                    enemyCollisionLogic(enemy, bullets);
                }

                // Remove bullets marked for deletion
                bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                    [](Bullet& bullet) { return bullet.markForDeletion; }), bullets.end());

                // Render bullet
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, bullet.getPosition());

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                tex2.Bind(1);
                glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
                bA.Bind();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            }

            // Remove bullets that moved off-screen
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return b.getPosition().y > 1.0f || b.markForDeletion; }), bullets.end());

            // Handle enemy spawn
            enemySpawnCooldown += 0.009f;
            if (enemySpawnCooldown >= 2.0f && enemies.size() < 10) {
                float x = static_cast<float>(((rand() % 180) - 90) / 100.0f); // Ensures enemy is spawned in the screen area
                enemies.push_back({ glm::vec3(x, 1.0f, 0.0f), 0.0f });
                enemySpawnCooldown = 0.0f;
            }

            // Update and render enemies
            for (auto& enemy : enemies) {
                enemy.update();

                if (enemy.markForDeletion) {
                    ++enemyKillCount;
                }

                // Render enemy
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, enemy.getPosition());
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                tex3.Bind(0);
                glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                va.Bind();
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            }

            // Remove enemies that went off-screen or marked for deletion
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& e) { return e.markForDeletion; }), enemies.end());

            if (std::any_of(enemies.begin(), enemies.end(), [](const Enemy& e) { return e.getPosition().y < -1.0f; })) {
                std::cout << "Game Over! An enemy has reached the bottom of the screen." << std::endl;

                // Display Game Over Screen
                while (!glfwWindowShouldClose(window)) {
                    glUseProgram(shaderProgram);
                    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    // Render Game Over Screen
                    int modelLoc = glGetUniformLocation(shaderProgram, "model");
                    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                    texforgover.Bind(1); // Bind Game Over texture
                    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
                    wsA.Bind(); // Bind VAO for the Game Over screen
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    // Wait for Enter Key to Restart
                    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
                        break; // Exit Game Over loop
                    }

                    glfwSwapBuffers(window);
                    glfwPollEvents();
                }

                // Reset the game state here
                enemies.clear(); // Remove all enemies
                // Add code to reinitialize enemies, player position, or other game state as needed.
                continue; // Return to the main game loop
            }

            // Print score (number of enemies killed)
            std::cout << "Enemies Killed: " << enemyKillCount << std::endl;

            glfwSwapBuffers(window);
            glfwPollEvents();
        
    } while (!glfwWindowShouldClose(window));

        glfwTerminate();
        return 0;
 }


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void movement(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        trianglePosition.x -= 0.007f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        trianglePosition.x += 0.007f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        trianglePosition.y += 0.007f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        trianglePosition.y -= 0.007f;
    }
    if (trianglePosition.x < -0.9f) trianglePosition.x = 0.9f;
    if (trianglePosition.x > 0.9f) trianglePosition.x = -0.9f;
    if (trianglePosition.y < -0.9f) trianglePosition.y = 0.9f;
    if (trianglePosition.y > 0.9f) trianglePosition.y = -0.9f;
}
