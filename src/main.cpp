#include "stdafx.h"
#include "mcpp/glprogram.h"
#include "mcpp/math/matrix.h"
#include "mcpp/math/frustum.h"
#include "mcpp/tessellator.h"
#include "mcpp/texture.h"
#include "mcpp/input.h"
#include "mcpp/world/block.h"
#include "mcpp/world/chunk.h"
#include "mcpp/world/entity/player.h"
#include "mcpp/world/world.h"
#include "mcpp/world/world_renderer.h"

int width = 0, height = 0;

mcpp::GLProgram* program = nullptr;
unsigned int vao = 0, vbo = 0, ebo = 0;
size_t bufferSize = 0, indicesSize = 0;
bool vaVertex = false, vaColor = false, vaTexCoord = false;

constexpr auto MOUSE_SENSITIVITY = 0.15f;
double deltaMouseX = 0.0, deltaMouseY = 0.0;
double lastMouseX = 0.0, lastMouseY = 0.0;
bool cameraMoving = false;

mcpp::Matrix4f projectionMat = mcpp::Matrix4f();
mcpp::Matrix4f viewMat = mcpp::Matrix4f();
mcpp::Matrix4f projectionViewMat = mcpp::Matrix4f();
mcpp::Matrix4f modelMat = mcpp::Matrix4f();

mcpp::World* world = nullptr;
mcpp::WorldRenderer* worldRenderer = nullptr;
mcpp::PlayerEntity* player = nullptr;

mcpp::FrustumIntersection frustum = mcpp::FrustumIntersection();

mcpp::BlockHitResult hitResult = mcpp::BlockHitResult();

unsigned int framesPerSecond = 0;

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

uniform mat4 ProjectionViewMat, ModelMat;

void main() {
    gl_Position = ProjectionViewMat * ModelMat * vec4(Position, 1.0);
    vertexColor = Color;
    texCoord0 = UV0;
}
)glsl";
constexpr auto& fragmentShader = R"glsl(
#version 150 core

in vec3 vertexColor;
in vec2 texCoord0;

out vec4 FragColor;

uniform vec4 ColorModulator;
uniform sampler2D Sampler0;

void main() {
    FragColor = vec4(vertexColor, 1.0) * ColorModulator * texture(Sampler0, texCoord0);
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
        player->turn(
            -(float)mcpp::math::toRadians(deltaMouseX * MOUSE_SENSITIVITY),
            -(float)mcpp::math::toRadians(deltaMouseY * MOUSE_SENSITIVITY)
        );
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

    program->findUniform("ColorModulator", mcpp::GLUniformType::F4)->set(1.0f, 1.0f, 1.0f, 1.0f);
    program->findUniform("Sampler0", mcpp::GLUniformType::I1)->set(0);

    return true;
}

bool init() {
    int fbWidth = 0, fbHeight = 0;
    mcpp::window->getFramebufferSize(&fbWidth, &fbHeight);
    onResize(mcpp::window->handle(), fbWidth, fbHeight);

    if (glfwRawMouseMotionSupported())
    {
        mcpp::window->setInputMode(GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
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
                unsigned int terrainAtlas = 0;
                glGenTextures(1, &terrainAtlas);
                mcpp::texture::putTexture2D(mcpp::texture::TERRAIN_ATLAS, terrainAtlas);
                mcpp::texture::bind2D(0, terrainAtlas);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
                int w = pw, h = ph;
                for (int i = 0; i < 5; i++)
                {
                    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                    w = max(w / 2, 1);
                    h = max(h / 2, 1);
                }
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, pw, ph, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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
    player = new mcpp::PlayerEntity(world);

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
        .translate(-lerpPos.x, -lerpPos.y - player->eyeHeight, -lerpPos.z);
}

void setupCamera(double partialTick) {
    projectionMat.setPerspective(mcpp::math::RAD90_f, (float)width / (float)height, 0.05f, 1000.0f);
    viewMat.identity();
    moveCameraToPlayer(partialTick);
}

void renderHit() {
    if (hitResult.missing) return;

    program->findUniform("ColorModulator", mcpp::GLUniformType::F4)->set(1.0f, 1.0f, 1.0f, (float)(sin(glfwGetTime()) + 1.0f) * 0.5f);
    program->uploadUniforms();

    mcpp::AABBox box = mcpp::AABBox();
    mcpp::Block* block = world->getBlock(hitResult.x, hitResult.y, hitResult.z);
    block->getOutlineShape(box).move((float)hitResult.x, (float)hitResult.y, (float)hitResult.z);

    auto& t = mcpp::Tessellator::getInstance();
    t.begin();
    t.color(1,1,1);
    t.index({
            // -x
            0, 1, 0, 2, 1, 3, 2, 3,
            // +x
            4, 5, 4, 6, 5, 7, 6, 7,
            // -y
            0, 4, 1, 5,
            // +y
            2, 6, 3, 7
        });
    t.vertex(box.minX, box.minY, box.minZ).emit();
    t.vertex(box.minX, box.minY, box.maxZ).emit();
    t.vertex(box.minX, box.maxY, box.minZ).emit();
    t.vertex(box.minX, box.maxY, box.maxZ).emit();
    t.vertex(box.maxX, box.minY, box.minZ).emit();
    t.vertex(box.maxX, box.minY, box.maxZ).emit();
    t.vertex(box.maxX, box.maxY, box.minZ).emit();
    t.vertex(box.maxX, box.maxY, box.maxZ).emit();
    t.end(vao, vbo, ebo, &vaVertex, &vaColor, &vaTexCoord, &bufferSize, &indicesSize);
    glLineWidth(2.0f);
    glBindVertexArray(vao);
    glDrawElements(GL_LINES, (GLsizei)indicesSize, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glLineWidth(1.0f);

    program->findUniform("ColorModulator", mcpp::GLUniformType::F4)->set(1.0f, 1.0f, 1.0f, 1.0f);
    program->uploadUniforms();
}

void render(double partialTick) {
    setupCamera(partialTick);
    frustum.set(projectionMat.mul(viewMat, projectionViewMat));
    worldRenderer->pick(frustum, projectionViewMat, player, hitResult);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldRenderer->updateDirtyChunks(frustum);

    program->use();
    program->findUniform("ProjectionViewMat", mcpp::GLUniformType::M4F)->set(projectionViewMat);
    program->findUniform("ModelMat", mcpp::GLUniformType::M4F)->set(modelMat);
    program->uploadUniforms();
    worldRenderer->render(frustum);
    renderHit();
    glUseProgram(0);

    mcpp::window->swapBuffers();
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
    mcpp::texture::deleteTextures2D();
}

#ifdef _DEBUG
int main()
#elif defined(_WIN32) // _DEBUG
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
    mcpp::window = new mcpp::Window(854, 480, "Minecraft++  Press ~ to toggle camera");
    if (mcpp::window->handle() == NULL)
    {
        std::cerr << "Failed to create the GLFW window\n";
        return 0;
    }
    mcpp::window->setKeyCallback(onKey);
    mcpp::window->setFramebufferSizeCallback(onResize);
    mcpp::window->setCursorPosCallback(onCursorPos);
    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (vidmode != NULL)
    {
        int w = 0, h = 0;
        mcpp::window->getSize(&w, &h);
        mcpp::window->setPos(
            (vidmode->width - w) / 2,
            (vidmode->height - h) / 2
        );
    }
    mcpp::window->makeContextCurrent();
    gladLoadGL(glfwGetProcAddress);
    if (!init())
    {
        return 0;
    }
    mcpp::window->show();
    unsigned int frames = 0;
    double lastTime = glfwGetTime();
    while (!mcpp::window->shouldClose())
    {
        update();
        render(gameTimer.partialTick);
        ++frames;
        while (glfwGetTime() >= lastTime + 1.0)
        {
#ifdef _DEBUG
            std::cout << "FPS: " << frames << "\n";;
#endif // _DEBUG
            framesPerSecond = frames;
            lastTime += 1.0;
            frames = 0;
        }
    }
    dispose();
    delete mcpp::window;
    mcpp::window = nullptr;
    glfwTerminate();
    return 0;
}
