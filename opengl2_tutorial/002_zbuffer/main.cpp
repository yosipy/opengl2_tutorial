#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* initGLFW(int width, int height)
{
    // GLFW初期化
    if (glfwInit() == GL_FALSE)
    {
        return nullptr;
    }

    // ウィンドウ生成
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Sample", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    // バージョン2.1指定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // GLEW初期化
    if (glewInit() != GLEW_OK)
    {
        return nullptr;
    }

    return window;
}

int readShaderSource(GLuint shaderObj, std::string fileName)
{
    //ファイルの読み込み
    std::ifstream ifs(fileName);
    if (!ifs)
    {
        std::cout << "error" << std::endl;
        return -1;
    }

    std::string source;
    std::string line;
    while (getline(ifs, line))
    {
        source += line + "\n";
    }

    // シェーダのソースプログラムをシェーダオブジェクトへ読み込む
    const GLchar *sourcePtr = (const GLchar *)source.c_str();
    GLint length = source.length();
    glShaderSource(shaderObj, 1, &sourcePtr, &length);

    return 0;
}

GLint makeShader(std::string vertexFileName, std::string fragmentFileName)
{
    // シェーダーオブジェクト作成
    GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shader;

    // シェーダーコンパイルとリンクの結果用変数
    GLint compiled, linked;

    /* シェーダーのソースプログラムの読み込み */
    if (readShaderSource(vertShaderObj, vertexFileName)) return -1;
    if (readShaderSource(fragShaderObj, fragmentFileName)) return -1;

    /* バーテックスシェーダーのソースプログラムのコンパイル */
    glCompileShader(vertShaderObj);
    glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        fprintf(stderr, "Compile error in vertex shader.\n");
        return -1;
    }

    /* フラグメントシェーダーのソースプログラムのコンパイル */
    glCompileShader(fragShaderObj);
    glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        fprintf(stderr, "Compile error in fragment shader.\n");
        return -1;
    }

    /* プログラムオブジェクトの作成 */
    shader = glCreateProgram();

    /* シェーダーオブジェクトのシェーダープログラムへの登録 */
    glAttachShader(shader, vertShaderObj);
    glAttachShader(shader, fragShaderObj);

    /* シェーダーオブジェクトの削除 */
    glDeleteShader(vertShaderObj);
    glDeleteShader(fragShaderObj);

    /* シェーダープログラムのリンク */
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        fprintf(stderr, "Link error.\n");
        return -1;
    }

    return shader;
}


int main()
{
    GLint width = 640, height = 480;
    GLFWwindow* window = initGLFW(width, height);

    GLint shader = makeShader("shader.vert", "shader.frag");

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        glUseProgram(shader);

        // デプステストを有効にする
        glEnable(GL_DEPTH_TEST);
        // 前のものよりもカメラに近ければ、フラグメントを受け入れる
        glDepthFunc(GL_LESS);

        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        // スクリーンをクリアする
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//glClear(GL_COLOR_BUFFER_BIT);

        //赤いポリゴン
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.5, -1.0);
        glVertex3f(-0.5, -0.5, -1.0);
        glVertex3f(0.5, -0.5, -1.0);
        glEnd();

        //黄色いポリゴン
        glColor4f(1.0, 1.0, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(-1.0, -0.5, 0.0);
        glVertex3f(0.5, -0.8, 0.0);
        glEnd();

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GLFWの終了処理
    glfwTerminate();

    return 0;
}