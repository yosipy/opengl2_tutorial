#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

// glmの使う機能をインクルード
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//using namespace glm;でもいいけどここでは一部のみ「glm::」を省力できるようにする
using glm::vec3;
using glm::vec4;
using glm::mat4;


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

GLint readShaderSource(GLuint shaderObj, std::string fileName)
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

    GLuint matrixID = glGetUniformLocation(shader, "MVP");

    // フレームループ
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        glUseProgram(shader);

        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);
        glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 宣言時には単位行列が入っている
        mat4 modelMat, viewMat, projectionMat;

        // View行列を計算
        viewMat = glm::lookAt(
            vec3(1.0, 2.0, 6.0), // ワールド空間でのカメラの座標
            vec3(0.0, 0.0, 0.0), // 見ている位置の座標
            vec3(0.0, 0.0, 1.0)  // 上方向を示す。(0,1.0,0)に設定するとy軸が上になります
        );

        // Projection行列を計算
        projectionMat = glm::perspective(
            glm::radians(45.0f), // ズームの度合い(通常90～30)
            (GLfloat)width / (GLfloat)height,		// アスペクト比
            0.1f,		// 近くのクリッピング平面
            100.0f		// 遠くのクリッピング平面
        );

        // ModelViewProjection行列を計算
        mat4 mvpMat = projectionMat * viewMat* modelMat;

        // 現在バインドしているシェーダのuniform変数"MVP"に変換行列を送る
        // 4つ目の引数は行列の最初のアドレスを渡しています。
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvpMat[0][0]);

        // 4枚のポリゴンから成る三角錐のデータを転送
        vec3 position[4][3] = { 
            {vec3( 0, 0, 1),vec3(-1,-1, 0),vec3( 1, 0, 0)},
            {vec3( 0, 0, 1),vec3( 1, 0, 0),vec3( 0, 1, 0)},
            {vec3( 0, 0, 1),vec3( 0, 1, 0),vec3(-1,-1, 0)},
            {vec3(-1,-1, 0),vec3( 0, 1, 0),vec3( 1, 0, 0)} 
        };
        vec4 color[4] = { vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(1,1,0,1)};
        for (int i = 0; i < 4; ++i)
        {
            glColor4f(color[i].r, color[i].g, color[i].b, color[i].a);
            glBegin(GL_TRIANGLES);
            glVertex3f(position[i][0].x, position[i][0].y, position[i][0].z);
            glVertex3f(position[i][1].x, position[i][1].y, position[i][1].z);
            glVertex3f(position[i][2].x, position[i][2].y, position[i][2].z);
            glEnd();
        }

        // ダブルバッファのスワップ
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GLFWの終了処理
    glfwTerminate();

    return 0;
}