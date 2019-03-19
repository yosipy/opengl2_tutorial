#include "stdafx.h"
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>


int main()
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE) 
    {
        return -1;
    }

    // ウィンドウ生成
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Simple", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // バージョン2.1指定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // コンテキストの作成
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // GLEW初期化
    if (glewInit() != GLEW_OK) 
    {
        return -1;
    }

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE) 
    {
        // バッファのクリア
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GLFWの終了処理
    glfwTerminate();

    return 0;
}