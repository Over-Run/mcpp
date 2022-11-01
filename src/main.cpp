#include "stdafx.h"
#include "mcpp/glprogram.h"
#include "mcpp/matrix.h"
#include "mcpp/tessellator.h"
#include "mcpp/texture.h"
#include "mcpp/world/block.h"
#include "mcpp/world/chunk.h"
#include "mcpp/world/entity/player.h"
#include "mcpp/world/world.h"
#include "mcpp/world/world_renderer.h"

GLFWwindow* window = NULL;
int width = 0, height = 0;
mcpp::GLProgram* program = nullptr;
unsigned int vao = 0, vbo = 0, ebo = 0;
size_t bufferSize = 0, indicesSize = 0;
bool vaVertex = false, vaColor = false, vaTexCoord = false;
unsigned int terrainAtlas = 0;

constexpr auto MOUSE_SENSITIVITY = 0.15f;
double deltaMouseX = 0.0, deltaMouseY = 0.0;
double lastMouseX = 0.0, lastMouseY = 0.0;
bool cameraMoving = false;

mcpp::Matrix4f projectionMat = mcpp::Matrix4f();
mcpp::Matrix4f viewMat = mcpp::Matrix4f();
mcpp::Matrix4f modelMat = mcpp::Matrix4f();

mcpp::World* world = nullptr;
mcpp::WorldRenderer* worldRenderer = nullptr;
mcpp::Player* player = nullptr;

unsigned int frames = 0;

class Timer {
public:
    int ticks;
    int maxTicks;
private:
    double lastTime, passedTime;
public:
    double deltaFrameTime;
    double partialTick;
    double tps;
    double timescale;

    Timer(double _tps, int _maxTicks = 100) :
        ticks(0), maxTicks(_maxTicks),
        lastTime(0.0), passedTime(0.0),
        deltaFrameTime(0.0), partialTick(0.0),
        tps(_tps), timescale(1.0) {}

    void update() {
        double currentTime = glfwGetTime();
        double pt = currentTime - lastTime;
        deltaFrameTime = pt;
        lastTime = currentTime;
        if (pt < 0.0) pt = 0.0;
        if (pt > 1.0) pt = 1.0;
        passedTime += pt * timescale * tps;
        ticks = (int)passedTime;
        if (ticks < 0) ticks = 0;
        if (ticks > maxTicks) ticks = maxTicks;
        passedTime -= ticks;
        partialTick = passedTime;
    }
};

Timer gameTimer = Timer(20.0);

constexpr auto& vertexShader = R"glsl(
#version 150 core

in vec3 Position;
in vec3 Color;
in vec2 UV0;

out vec3 vertexColor;
out vec2 texCoord0;

uniform mat4 ProjectionMat, ViewMat, ModelMat;

void main() {
    gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(Position, 1.0);
    vertexColor = Color;
    texCoord0 = UV0;
}
)glsl";
constexpr auto& fragmentShader = R"glsl(
#version 150 core

in vec3 vertexColor;
in vec2 texCoord0;

out vec4 FragColor;

uniform sampler2D Sampler0;

void main() {
    FragColor = vec4(vertexColor, 1.0) * texture(Sampler0, texCoord0);
}
)glsl";

void onError(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void onKey(GLFWwindow* handle, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(handle, GLFW_TRUE);
        }
        if (key == GLFW_KEY_GRAVE_ACCENT)
        {
            cameraMoving = !cameraMoving;
            glfwSetInputMode(handle, GLFW_CURSOR, cameraMoving ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        }
    }
}

void onResize(GLFWwindow*, int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    glViewport(0, 0, width, height);
}

void onCursorPos(GLFWwindow*, double xpos, double ypos) {
    deltaMouseX = xpos - lastMouseX;
    deltaMouseY = ypos - lastMouseY;
    if (cameraMoving)
    {
        player->yaw -= (float)mcpp::math::toRadians(deltaMouseX * MOUSE_SENSITIVITY);
        if (player->yaw > mcpp::math::RAD180_f)
        {
            player->yaw -= mcpp::math::RAD360_f;
        }
        if (player->yaw < -mcpp::math::RAD180_f)
        {
            player->yaw += mcpp::math::RAD360_f;
        }
        player->pitch -= (float)mcpp::math::toRadians(deltaMouseY * MOUSE_SENSITIVITY);
        if (player->pitch > mcpp::math::RAD90_f)
        {
            player->pitch = mcpp::math::RAD90_f;
        }
        else if (player->pitch < -mcpp::math::RAD90_f)
        {
            player->pitch = -mcpp::math::RAD90_f;
        }
    }
    lastMouseX = xpos;
    lastMouseY = ypos;
}

unsigned int compileShader(int type, const char* src) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    int status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        int logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, log);
        std::cerr << "Failed to compile the shader! Type: " << type << ", log: " << log;
        delete[] log;
        return 0;
    }
    return shader;
}

bool initProgram() {
    program = new mcpp::GLProgram();
    unsigned int vsh = compileShader(GL_VERTEX_SHADER, vertexShader);
    if (vsh == 0)
    {
        return false;
    }
    unsigned int fsh = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (fsh == 0)
    {
        return false;
    }
    program->attachShader(vsh);
    program->attachShader(fsh);
    program->bindAttribLocation(0, "Position");
    program->bindAttribLocation(1, "Color");
    program->bindAttribLocation(2, "UV0");
    program->link();
    int status = 0;
    program->getiv(GL_LINK_STATUS, &status);
    if (!status)
    {
        int logLength = 0;
        program->getiv(GL_INFO_LOG_LENGTH, &logLength);
        char* log = new char[logLength];
        program->getInfoLog(logLength, NULL, log);
        std::cerr << "Failed to link the program! Id: " << program->getId() << ", log: " << log;
        delete program;
        delete[] log;
        return false;
    }
    program->detachShader(vsh);
    program->detachShader(fsh);
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    program->use();
    program->findUniform("Sampler0", mcpp::GLUniformType::I1)->set(0);
    glUseProgram(0);

    return true;
}

bool init() {
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    onResize(window, fbWidth, fbHeight);

    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

    if (!initProgram())
    {
        return false;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    HRSRC hResource = FindResource(GetModuleHandle(NULL), L"TERRAIN_ATLAS", TEXT("PNG"));
    if (hResource)
    {
        HGLOBAL hg = LoadResource(GetModuleHandle(NULL), hResource);
        if (hg)
        {
            LPVOID pData = LockResource(hg);
            if (pData)
            {
                DWORD size = SizeofResource(GetModuleHandle(NULL), hResource);
                int pw = 0, ph = 0, pc = 0;
                stbi_uc* pixels = stbi_load_from_memory((stbi_uc*)pData, size, &pw, &ph, &pc, STBI_rgb_alpha);
                UnlockResource(hg);
                FreeResource(hg);
                if (pixels == NULL)
                {
                    std::cerr << "Failed to load the terrain atlas. Reason: " << stbi_failure_reason() << std::endl;
                    return false;
                }
                glGenTextures(1, &terrainAtlas);
                mcpp::texture::bind2D(0, terrainAtlas);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pw, ph, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
                stbi_image_free(pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                mcpp::texture::bind2D(0, 0);
            }
        }
    }

    world = new mcpp::World(256, 64, 256);
    worldRenderer = new mcpp::WorldRenderer(world);
    for (int x = 0; x < world->getWidth(); x++)
    {
        for (int z = 0; z < world->getDepth(); z++)
        {
            for (int y = 0; y < world->getHeight(); y++)
            {
                mcpp::Block* p_block = mcpp::Blocks::AIR;
                if (y == 8)
                {
                    p_block = mcpp::Blocks::GRASS_BLOCK;
                }
                else if (y > 4 && y < 8)
                {
                    p_block = mcpp::Blocks::DIRT;
                }
                else if (y <= 4)
                {
                    p_block = mcpp::Blocks::COBBLESTONE;
                }
                world->setBlock(p_block, x, y, z);
            }
        }
    }
    player = new mcpp::Player(world);

    return true;
}

void tick() {
    player->tick();
}

void update() {
    glfwPollEvents();
    gameTimer.update();
    for (int i = 0; i < gameTimer.ticks; i++)
    {
        tick();
    }
}

void moveCameraToPlayer(double partialTick) {
    mcpp::Vector3f lerpPos = mcpp::Vector3f();
    player->prevPosition.lerp(player->position, partialTick, lerpPos);
    viewMat.translate(0.0f, 0.0f, -0.3f)
        .rotateX(-player->pitch)
        .rotateY(-player->yaw)
        .translate(-lerpPos.x, -lerpPos.y, -lerpPos.z);
}

void setupCamera(double partialTick) {
    projectionMat.setPerspective(mcpp::math::RAD90_f, (float)width / (float)height, 0.05f, 1000.0f);
    viewMat.identity();
    moveCameraToPlayer(partialTick);
}

void render(double partialTick) {
    setupCamera(partialTick);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldRenderer->updateDirtyChunks();

    program->use();
    program->findUniform("ProjectionMat", mcpp::GLUniformType::M4F)->set(projectionMat);
    program->findUniform("ViewMat", mcpp::GLUniformType::M4F)->set(viewMat);
    program->findUniform("ModelMat", mcpp::GLUniformType::M4F)->set(modelMat);
    program->uploadUniforms();
    mcpp::texture::bind2D(0, terrainAtlas);
    worldRenderer->render();
    mcpp::texture::bind2D(0, 0);
    glUseProgram(0);

    glfwSwapBuffers(window);
}

void dispose() {
    delete player;
    player = nullptr; 
    delete world;
    world = nullptr;
    delete worldRenderer;
    worldRenderer = nullptr;

    delete program;
    program = nullptr;
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &terrainAtlas);
}

#ifdef _DEBUG
int main()
#else // _DEBUG
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
#endif // _DEBUG
{
    glfwSetErrorCallback(onError);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 0;
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(854, 480, "Minecraft++  Press ~ to toggle camera", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create the GLFW window\n";
        return 0;
    }
    glfwSetKeyCallback(window, onKey);
    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetCursorPosCallback(window, onCursorPos);
    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (vidmode != NULL)
    {
        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glfwSetWindowPos(window,
            (vidmode->width - w) / 2,
            (vidmode->height - h) / 2);
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    if (!init())
    {
        return 0;
    }
    glfwShowWindow(window);
    unsigned int frames = 0;
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        update();
        render(gameTimer.partialTick);
        ++frames;
        while (glfwGetTime() >= lastTime + 1.0)
        {
            std::cout << "FPS: " << frames << "\n";;
            ::frames = frames;
            lastTime += 1.0;
            frames = 0;
        }
    }
    dispose();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
