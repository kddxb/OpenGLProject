#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "Vertex.h"
#include "Mesh.h"
#include <GLM/gtx/transform.hpp>
#include "MeshUtils.h"
#include <map>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>

//[a,b]之间随机数
int RandomBetween(int a, int b)
{
    std::uniform_int_distribution<int> u(a, b);
    std::default_random_engine e(int(glfwGetTime() * 1000));
    return u(e);
}

//窗体宽高
const int winWidth = 600, winHeight = 800;
//方块尺寸
const int cellSize = 30;
//方块行列数
const int rows = 15, columns = 10;
//顶端空余行数
const int spaceRows = 5;
//是否有方块填充
bool hasCube[rows][columns] = {};

//索引与位置的关系
glm::vec3 GetModelMatrixByRowAndColumn(int row, int column)
{
    return glm::vec3(cellSize * (column + 0.5), cellSize * (row + 0.5), 0);
};

//根据行和列更新位置
void UpdateCube(Mesh* pMesh, int row, int column)
{
    pMesh->ResetModelMatrix();
    pMesh->Translate(GetModelMatrixByRowAndColumn(row, column));
};

bool IsColumnFull(int column)
{
    return hasCube[rows - 1][column];
}

bool IsFull()
{
    for (int column = 0; column < columns; ++column)
    {
        if (!IsColumnFull(column))
        {
            return false;
        }
    }
    return true;
}

struct Cube
{
    int row = 0;
    int column = 0;
    std::unique_ptr<Mesh> upMesh = nullptr;

    void Draw()
    {
        if (!upMesh || !upMesh->IsVisible())
        {
            return;
        }
        UpdateCube(upMesh.get(), row, column);
        upMesh->Draw();
    }
};


int main16()
{
#pragma region 初始化环境
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, u8"OpenGL学习", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        glViewport(0, 0, winWidth, winHeight);
        glfwSetWindowSize(window, winWidth, winHeight);
    });

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
#pragma endregion

    //背景方块
    std::vector<std::unique_ptr<Mesh>> backgroundCubes;
    //移动的方块
    std::vector<std::unique_ptr<Cube>> movingCubes;

    std::mutex lock4MovingCubes;

    //背景
    for (int y = 0; y < rows + spaceRows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            std::unique_ptr<Mesh> upCuboid = MeshUtils::CreateAsCuboid(glm::vec3(1, 1, 1) * (cellSize * 0.9f), y < rows ? glm::vec4(0, 1, 0, 1) : glm::vec4(0, 0, 0.3, 0));
            upCuboid->Translate(GetModelMatrixByRowAndColumn(y, x) + glm::vec3(0, 0, -cellSize));
            upCuboid->Setup();
            backgroundCubes.push_back(std::move(upCuboid));
        }
    }

    std::thread th([&movingCubes,&lock4MovingCubes]() {
        int count = 0;
        while (true)
        {
            if (IsFull())
            {
                break;
            }
            lock4MovingCubes.lock();
            std::unique_ptr<Cube> upMovingCube = std::make_unique<Cube>();
            Cube* pMovingCube = upMovingCube.get();
            movingCubes.push_back(std::move(upMovingCube));
            std::cout << ++count << std::endl;
            pMovingCube->column = RandomBetween(0, columns - 1);
            while (IsColumnFull(pMovingCube->column))
            {
                pMovingCube->column = RandomBetween(0, columns - 1);
            }
            pMovingCube->row = 19;
            std::unique_ptr<Mesh> upCuboid = MeshUtils::CreateAsCuboid(glm::vec3(1, 1, 1) * (cellSize * 0.9f), glm::vec4(1, 1, 0, 1));
            pMovingCube->upMesh = std::move(upCuboid);
            lock4MovingCubes.unlock();
            while (true)
            {
                if (hasCube[pMovingCube->row - 1][pMovingCube->column])
                {
                    hasCube[pMovingCube->row][pMovingCube->column] = true;
                    break;
                }
                if (--pMovingCube->row <= 0)
                {
                    hasCube[0][pMovingCube->column] = true;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    });
    th.detach();

    Program* pProgram = ProgramFactory::Get(ProgramType::Mesh);
    glm::vec3 eye = glm::vec3(0.5f * winWidth, 0.5f * winHeight, 50);
    glm::vec3 front = glm::vec3(0, 0, -1);
    pProgram->SetUniform("view", glm::lookAt(eye, eye + front, glm::vec3(0, 1, 0)));

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pProgram->SetUniform("projection", glm::ortho(-0.5f * winWidth, 0.5f * winWidth, -0.5f * winHeight, 0.5f * winHeight, 0.1f, 100.0f));

        //绘制背景
        for (auto& cube : backgroundCubes)
        {
            if (cube->IsVisible())
            {
                cube->Draw();
            }
        }

        lock4MovingCubes.lock();
        //绘制移动的方块
        for (auto& movingCube : movingCubes)
        {
            if (!movingCube->upMesh->IsSetup())
            {
                movingCube->upMesh->Setup();
            }
            movingCube->Draw();
        }
        lock4MovingCubes.unlock();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}