#include "Source.h"
std::vector<Image> renderList;

unsigned int texture_box;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

static void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}
void RenderAll(unsigned int _VAO, Shader &ourShader) { //wywoluje dla kazdego z osobna bazujac na polach obiektu Image
    if (renderList.empty()) { return; }
    //Sprawdz czy nie zdealokowac
    int size = renderList.size();
    for (int i = 0; i < size; i++) {
        std::vector<Image>::iterator del = renderList.begin() + i;
        if (renderList[i].updatePath()) { 
            renderList.erase(del);  size--;
            std::cout << renderList.size() << std::endl; };
    }
    //Render
    glBindVertexArray(_VAO);
    std::vector<Image>::iterator begin = renderList.begin() , finish = renderList.end();
    //get uniform locations
    unsigned int transLoc = glGetUniformLocation(ourShader.ID, "transform");
    unsigned int colorLoc = glGetUniformLocation(ourShader.ID, "ourColor"); 
    while (begin != finish) {           
        //testMat[row][col] = blabla; TAK NIE JEST!
        //testMat[col][row] = blabla; TAK CHYBA JEST
        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 temp = glm::mat4(1.0f);
        //Przesun
        temp = glm::mat4(1.0f);
        temp[3][0] = (*begin).Pos.x;
        temp[3][1] = (*begin).Pos.y;
        trans = trans * temp;
        //Obrot
        temp = glm::mat4(1.0f);
        float alfa = (*begin).angle;
        temp[0][0] = cos(alfa);
        temp[0][1] = -sin(alfa);
        temp[1][0] = sin(alfa);
        temp[1][1] = cos(alfa);
        trans = trans * temp;
        //Skaluj
        temp = glm::mat4(1.0f);
        temp[0][0] = (*begin).Scale.x;
        temp[1][1] = (*begin).Scale.y;
        trans = trans * temp;
        glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniform4f(colorLoc, (*begin).Color.x, (*begin).Color.y, (*begin).Color.z, (*begin).Color.w);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        begin++;
    }
}

 //settings
//16:9
const unsigned int SCR_WIDTH = 800;//1280
const unsigned int SCR_HEIGHT = 450;//720
float t = 0.5;
int main()
{;
    glfwSetErrorCallback(error_callback);
    ///*  glfw: initialize and configure
    //  ------------------------------*/
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    srand((unsigned int)time(NULL));
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Confetti", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    //Shader ourShader("shaderv.glsl", "shaderf.glsl"); // you can name your shader files however you like
    Shader ourShader("vs.glsl", "fs.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices_center[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    float vertices_bottom_left[] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        0.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;//zrobic tablice VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_center), vertices_center, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texture_box);
    glBindTexture(GL_TEXTURE_2D, texture_box);
    //std::cout << "tex id: " << texture << std::endl;
    //set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("box.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    /////////////////////////TEKSTURA RGBA
    //glGenTextures(1, &texture2);
    //glBindTexture(GL_TEXTURE_2D, texture2);
    //// set the texture wrapping parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //// set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    //if (data)
    //{
    //    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
    //        GL_UNSIGNED_BYTE, data);
    //    glGenerateMipmap(GL_TEXTURE_2D);
    //}
    //else
    //{
    //    std::cout << "Failed to load texture" << std::endl;
    //}
    //stbi_image_free(data);
    // //////////////////////////////////////////////
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 
    // RENDER LOOP
    // -----------
    while (!glfwWindowShouldClose(window)) 
    {
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ourShader.use();
        RenderAll(VAO, ourShader);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
#define TESTINDEX 1
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    double mx = 0, my = 0;
    glfwGetCursorPos(window,&mx, &my);
  // std::cout << "X: " << (mx/(double)SCR_WIDTH*2) - 1<< " Y: " << 1 - my/(double)SCR_HEIGHT*2 << std::endl;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) { 
        Image i(texture_box, (mx / (double)SCR_WIDTH * 2) - 1, 1 - my / (double)SCR_HEIGHT * 2);
        i.Scale = glm::vec4(PARTICLE_SCALE_X, PARTICLE_SCALE_Y, 1.0, 1.0);
        i.rand_color();
        renderList.push_back(i);       
        std::cout << renderList.size() << std::endl;
    }
}

 /*glfw: whenever the window size changed (by OS or user resize) this callback function executes
 ---------------------------------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

