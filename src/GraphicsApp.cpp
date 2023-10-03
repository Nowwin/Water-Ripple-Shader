#include "GraphicsApp.hpp"
#include "ShaderLoader.hpp"
#include "Object.hpp"

int GNumberOfVertices = 0;
int mouseX, mouseY;
float timeLoop = 0.0f;
const int desiredFrameRate = 90;
const int frameDuration = 1000 / desiredFrameRate;


void GraphicsApp::GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void GraphicsApp::IntializeProgram() {

    std::cout << std::filesystem::current_path().string() << std::endl; 

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL did not initiliaze" << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsApplicationWindow = SDL_CreateWindow(
        "SDL2 Window",               // window title
        0,      // x position, centered
        0,      // y position, centered
        m_screenWidth,                         // width, in pixels
        m_screenHeight,                         // height, in pixels
        SDL_WINDOW_OPENGL             // flags
    );

    if (gGraphicsApplicationWindow == nullptr)
    {
        std::cout << "Window did not setup" << std::endl;
        exit(1);
    }

    gOpenGlContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

    if (gOpenGlContext == nullptr)
    {
        std::cout << "Graphics did not load" << std::endl;
        exit(1);
    }
    
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        std::cout << "glad was not initialized" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
    
}

void GraphicsApp::VertexSpecification() {
    //Not using a .obj file
    //Creating a simple rectanggle
    const std::vector<GLfloat> vertexData {
        // positions          // texture coords
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    const std::vector<GLuint> indexBufferData {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &gVertexArrayObject);
    //Selecting
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


    GNumberOfVertices = indexBufferData.size();
                                                
    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*)(sizeof(GL_FLOAT)*3));

    glBindVertexArray(0);    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
}

void GraphicsApp::LoadShaders() {
    std::string vertexShaderSource = ShaderLoader::LoadShaderAsString(std::filesystem::current_path().string() + "/shaders/vert.glsl");
    std::string fragmentShaderSource = ShaderLoader::LoadShaderAsString(std::filesystem::current_path().string() + "/shaders/frag.glsl");
    gGraphicsPipelineShaderProgram = ShaderLoader::CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}


void GraphicsApp::Input(){
    SDL_Event event;
    timeLoop += 0.01f;

    // (1) Handle Input
    // Start our event loop
    while(SDL_PollEvent(&event)){
        // Handle each specific event
        if(event.type == SDL_QUIT){
            gQuit= true;
        } else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                
                SDL_GetMouseState(&mouseX, &mouseY);
            }
        }


        
    }
}

void GraphicsApp::PreDraw() {
    //Setting opengl state
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);

    glm::vec2 interactionPoint = glm::vec2(mouseX / (float)m_screenWidth, 1.0f - mouseY / (float)m_screenHeight);
    glUniform2fv(glGetUniformLocation(gGraphicsPipelineShaderProgram, "interactionPoint"), 1, glm::value_ptr(interactionPoint));
    glUniform1f(glGetUniformLocation(gGraphicsPipelineShaderProgram, "time"), timeLoop);
    
    
}

void GraphicsApp::Draw() {

    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    

}

void GraphicsApp::Initialize() {
    IntializeProgram();
    
    VertexSpecification();
    
    LoadShaders();
}

void GraphicsApp::MainLoop() {
    while (!gQuit) {
        Uint32 frameStart = SDL_GetTicks();

        Input();
        
        PreDraw();

        Draw();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);

        Uint32 frameEnd = SDL_GetTicks();
        Uint32 frameTime = frameEnd - frameStart;
        int delayTime = frameDuration - frameTime;

        if (delayTime > 0) {
            SDL_Delay(delayTime);
        }
    }
}

void GraphicsApp::CleanUp() {
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

