//
//  main.cpp
//  triangle
//
//  Created by Nazirul Hasan on 26/8/23.
//  modified by Badiuzzaman
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 1200;

float rotateAngle = 0.0;
float translate_X = -0.5;
float translate_Y = -0.3;
float scale_X = 1.0;
float scale_Y = 1.0;

float rotateAngle2 = 0.0;
float translate_X2 = -0.5;
float translate_Y2 = -0.3;
float scale_X2 = 1.0;
float scale_Y2 = 1.0;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 triangleColor;\n"  // Color uniform for each triangle fan
"void main()\n"
"{\n"
"   FragColor = triangleColor;\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Assignment 1", NULL, NULL);
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
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //triangle
    float vertices[] = {
        0.284810126582278,0.996889580093312,0,
        0.316455696202531,0.996889580093312,0,
        0.316455696202531,0.996889580093312,0,
        0.341772151898734,0.995334370139969,0,
        0.367088607594936,0.990668740279937,0,
        0.367088607594936,0.990668740279937,0,
        0.392405063291139,0.986003110419906,0,
        0.411392405063291,0.981337480559875,0,
        0.430379746835443,0.976671850699844,0,
        0.446202531645569,0.970451010886469,0,
        0.465189873417721,0.962674961119751,0,
        0.446202531645569,0.95800933125972,0,
        0.484177215189873,0.967340590979782,0,

        0.465189873417721,0.978227060653188,0,
        0.493670886075949,0.979782270606531,0,
        0.515822784810126,0.978227060653188,0,
        0.537974683544303,0.972006220839813,0,
        0.55379746835443,0.964230171073094,0,
        0.563291139240506,0.953343701399688,0,
        0.566455696202531,0.940902021772939,0,
        0.563291139240506,0.925349922239502,0,
        0.550632911392405,0.914463452566096,0,
        0.531645569620253,0.90357698289269,0,
        0.503164556962025,0.898911353032659,0,
        0.471518987341772,0.898911353032659,0,
        0.443037974683544,0.900466562986003,0,
        0.417721518987341,0.909797822706065,0,
        0.395569620253164,0.926905132192846,0,
        0.401898734177215,0.942457231726283,0,
        0.414556962025316,0.954898911353032,0,
        0.427215189873417,0.965785381026438,0,
        0.43,0.97,0,
        0.45,0.97,0,
        0.493670886075949,0.979782270606531,0,

        0.408227848101265,0.911353032659409,0,
        0.430379746835443,0.90357698289269,0,
        0.455696202531645,0.895800933125972,0,
        0.481012658227848,0.895800933125972,0,
        0.503164556962025,0.900466562986003,0,
        0.528481012658227,0.905132192846034,0,
        0.550632911392405,0.912908242612752,0,
        0.566455696202531,0.923794712286158,0,
        0.585443037974683,0.930015552099533,0,
        0.613924050632911,0.928460342146189,0,
        0.642405063291139,0.922239502332814,0,
        0.677215189873417,0.914463452566096,0,
        0.70253164556962,0.908242612752721,0,
        0.727848101265822,0.900466562986003,0,
        0.75632911392405,0.892690513219284,0,
        0.781645569620253,0.881804043545878,0,
        0.794303797468354,0.869362363919129,0,
        0.810126582278481,0.856920684292379,0,
        0.822784810126582,0.844479004665629,0,
        0.838607594936708,0.830482115085536,0,
        0.844936708860759,0.810264385692068,0,
        0.848101265822784,0.779160186625194,0,
        0.848101265822784,0.766718506998444,0,
        0.841772151898734,0.752721617418351,0,
        0.835443037974683,0.740279937791601,0,
        0.825949367088607,0.729393468118195,0,
        0.816455696202531,0.716951788491446,0,
        0.794303797468354,0.704510108864696,0,
        0.75632911392405,0.698289269051321,0,
        0.718354430379746,0.696734059097978,0,
        0.686708860759493,0.695178849144634,0,
        0.658227848101265,0.696734059097978,0,
        0.620253164556962,0.698289269051321,0,
        0.582278481012658,0.699844479004665,0,
        0.544303797468354,0.702954898911353,0,
        0.518987341772151,0.707620528771384,0,
        0.484177215189873,0.713841368584758,0,
        0.449367088607594,0.71850699844479,0,
        0.414556962025316,0.724727838258164,0,
        0.379746835443037,0.734059097978227,0,
        0.354430379746835,0.741835147744945,0,
        0.313291139240506,0.749611197511664,0,
        0.294303797468354,0.763608087091757,0,
        0.275316455696202,0.776049766718507,0,
        0.272151898734177,0.793157076205287,0,
        0.275316455696202,0.811819595645412,0,
        0.284810126582278,0.828926905132192,0,
        0.297468354430379,0.846034214618973,0,
        0.313291139240506,0.860031104199066,0,
        0.332278481012658,0.875583203732503,0,
        0.38,0.90,0,
        0.40,0.91,0,
        0.408227848101265,0.911353032659409,0,


        0.278481012658227,0.730948678071539,0,
        0.310126582278481,0.723172628304821,0,
        0.341772151898734,0.712286158631415,0,
        0.382911392405063,0.710730948678071,0,
        0.414556962025316,0.701399688958009,0,
        0.45253164556962,0.69362363919129,0,
        0.487341772151898,0.685847589424572,0,
        0.518987341772151,0.678071539657853,0,
        0.563291139240506,0.681181959564541,0,
        0.60126582278481,0.674961119751166,0,
        0.642405063291139,0.678071539657853,0,
        0.677215189873417,0.673405909797822,0,
        0.712025316455696,0.671850699844479,0,
        0.712025316455696,0.671850699844479,0,
        0.746835443037974,0.673405909797822,0,
        0.775316455696202,0.678071539657853,0,
        0.806962025316455,0.690513219284603,0,
        0.791139240506329,0.701399688958009,0,
        0.746835443037974,0.698289269051321,0,
        0.715189873417721,0.698289269051321,0,
        0.674050632911392,0.698289269051321,0,
        0.626582278481012,0.701399688958009,0,
        0.588607594936708,0.701399688958009,0,
        0.544303797468354,0.70606531881804,0,
        0.493670886075949,0.709175738724727,0,
        0.455696202531645,0.71850699844479,0,
        0.411392405063291,0.726283048211508,0,
        0.367088607594936,0.734059097978227,0,
        0.332278481012658,0.744945567651633,0,
        0.30379746835443,0.757387247278382,0,
        0.275316455696202,0.763608087091757,0,
        0.25632911392405,0.755832037325038,0,
        0.259493670886075,0.746500777604976,0,
        0.278481012658227, 0.730948678071539, 0,


        0.275316455696202, 0.729393468118195, 0,
        0.25, 0.716951788491446, 0,
        0.227848101265822, 0.70606531881804, 0,
        0.215189873417721, 0.690513219284603, 0,
        0.193037974683544, 0.673405909797822, 0,
        0.174050632911392, 0.653188180404354, 0,
        0.170886075949367, 0.636080870917573, 0,
        0.170886075949367, 0.618973561430793, 0,
        0.174050632911392, 0.601866251944012, 0,
        0.177215189873417, 0.5800933125972, 0,
        0.189873417721519, 0.564541213063763, 0,
        0.215189873417721, 0.552099533437014, 0,
        0.234177215189873, 0.544323483670295, 0,
        0.268987341772151, 0.539657853810264, 0,
        0.30379746835443, 0.53810264385692, 0,
        0.341772151898734, 0.536547433903577, 0,
        0.382911392405063, 0.534992223950233, 0,
        0.420886075949367, 0.534992223950233, 0,
        0.449367088607594, 0.536547433903577, 0,
        0.481012658227848, 0.539657853810264, 0,
        0.512658227848101, 0.542768273716951, 0,
        0.544303797468354, 0.544323483670295, 0,
        0.579113924050633, 0.548989113530326, 0,
        0.60126582278481, 0.555209953343701, 0,
        0.636075949367088, 0.559875583203732, 0,
        0.670886075949367, 0.569206842923794, 0,
        0.705696202531645, 0.578538102643856, 0,
        0.727848101265822, 0.587869362363919, 0,
        0.746835443037974, 0.598755832037325, 0,
        0.753164556962025, 0.617418351477449, 0,
        0.746835443037974, 0.636080870917573, 0,
        0.743670886075949, 0.654743390357698, 0,
        0.734177215189873, 0.670295489891135, 0,
        0.693037974683544, 0.671850699844479, 0,
        0.65506329113924, 0.673405909797822, 0,
        0.617088607594936, 0.67651632970451, 0,
        0.579113924050633, 0.67651632970451, 0,
        0.534810126582278, 0.679626749611197, 0,
        0.509493670886076, 0.684292379471228, 0,
        0.477848101265822, 0.688958009331259, 0,
        0.433544303797468, 0.69362363919129, 0,
        0.398734177215189, 0.701399688958009, 0,
        0.35759493670886, 0.710730948678071, 0,
        0.325949367088607, 0.715396578538102, 0,
        0.300632911392405, 0.726283048211508, 0,
        0.275316455696202, 0.729393468118195, 0,


        0.234177215189873, 0.544323483670295, 0,
        0.268987341772151, 0.539657853810264, 0,
        0.30379746835443, 0.53810264385692, 0,
        0.341772151898734, 0.536547433903577, 0,
        0.382911392405063, 0.534992223950233, 0,
        0.420886075949367, 0.534992223950233, 0,
        0.449367088607594, 0.536547433903577, 0,
        0.481012658227848, 0.539657853810264, 0,
        0.512658227848101, 0.542768273716951, 0,
        0.544303797468354, 0.544323483670295, 0,
        0.579113924050633, 0.548989113530326, 0,
        0.60126582278481, 0.555209953343701, 0,
        0.636075949367088, 0.559875583203732, 0,
        0.670886075949367, 0.569206842923794, 0,
        0.705696202531645, 0.578538102643856, 0,
        0.727848101265822, 0.587869362363919, 0,
        0.75632911392405, 0.584758942457231, 0,
        0.759493670886076, 0.569206842923794, 0,
        0.734177215189873, 0.55054432348367, 0,
        0.680379746835443, 0.542768273716951, 0,
        0.636075949367088, 0.534992223950233, 0,
        0.591772151898734, 0.524105754276827, 0,
        0.544303797468354, 0.52099533437014, 0,
        0.493670886075949, 0.513219284603421, 0,
        0.45253164556962, 0.514774494556765, 0,
        0.40506329113924, 0.505443234836703, 0,
        0.354430379746835, 0.514774494556765, 0,
        0.316455696202531, 0.505443234836703, 0,
        0.291139240506329, 0.514774494556765, 0,
        0.243670886075949, 0.517884914463452, 0,
        0.20253164556962, 0.525660964230171, 0,
        0.20886075949367, 0.542768273716951, 0,
        0.234177215189873, 0.544323483670295, 0,


        0.734177215189873, 0.55054432348367, 0,
        0.680379746835443, 0.542768273716951, 0,
        0.636075949367088, 0.534992223950233, 0,
        0.591772151898734, 0.524105754276827, 0,
        0.544303797468354, 0.52099533437014, 0,
        0.493670886075949, 0.513219284603421, 0,
        0.45253164556962, 0.514774494556765, 0,
        0.40506329113924, 0.505443234836703, 0,
        0.354430379746835, 0.514774494556765, 0,
        0.316455696202531, 0.505443234836703, 0,
        0.291139240506329, 0.514774494556765, 0,
        0.243670886075949, 0.517884914463452, 0,
        0.234177215189873, 0.505443234836703, 0,
        0.227848101265822, 0.493001555209953, 0,
        0.224683544303797, 0.47900466562986, 0,
        0.224683544303797, 0.463452566096423, 0,
        0.221518987341772, 0.447900466562986, 0,
        0.227848101265822, 0.435458786936236, 0,
        0.240506329113924, 0.419906687402799, 0,
        0.268987341772151, 0.410575427682737, 0,
        0.297468354430379, 0.405909797822706, 0,
        0.329113924050632, 0.398133748055987, 0,
        0.360759493670886, 0.393468118195956, 0,
        0.392405063291139, 0.390357698289269, 0,
        0.430379746835443, 0.388802488335925, 0,
        0.465189873417721, 0.388802488335925, 0,
        0.503164556962025, 0.385692068429237, 0,
        0.55379746835443, 0.387247278382581, 0,
        0.588607594936708, 0.387247278382581, 0,
        0.626582278481012, 0.390357698289269, 0,
        0.658227848101265, 0.393468118195956, 0,
        0.693037974683544, 0.399688958009331, 0,
        0.731012658227848, 0.404354587869362, 0,
        0.759493670886076, 0.41213063763608, 0,
        0.791139240506329, 0.423017107309486, 0,
        0.800632911392405, 0.443234836702954, 0,
        0.800632911392405, 0.465007776049766, 0,
        0.800632911392405, 0.486780715396578, 0,
        0.791139240506329, 0.510108864696734, 0,
        0.778481012658227, 0.527216174183514, 0,
        0.759493670886076, 0.544323483670295, 0,
        0.734177215189873, 0.55054432348367, 0,


        0.25, 0.413685847589424, 0,
        0.284810126582278, 0.407465007776049, 0,
        0.313291139240506, 0.401244167962674, 0,
        0.344936708860759, 0.396578538102643, 0,
        0.344936708860759, 0.396578538102643, 0,
        0.382911392405063, 0.391912908242612, 0,
        0.420886075949367, 0.390357698289269, 0,
        0.455696202531645, 0.390357698289269, 0,
        0.493670886075949, 0.387247278382581, 0,
        0.541139240506329, 0.385692068429237, 0,
        0.582278481012658, 0.388802488335925, 0,
        0.626582278481012, 0.391912908242612, 0,
        0.664556962025316, 0.3950233281493, 0,
        0.693037974683544, 0.401244167962674, 0,
        0.731012658227848, 0.405909797822706, 0,
        0.768987341772151, 0.41213063763608, 0,
        0.800632911392405, 0.404354587869362, 0,
        0.784810126582278, 0.390357698289269, 0,
        0.75632911392405, 0.38258164852255, 0,
        0.718354430379746, 0.377916018662519, 0,
        0.693037974683544, 0.371695178849144, 0,
        0.651898734177215, 0.371695178849144, 0,
        0.617088607594936, 0.368584758942457, 0,
        0.588607594936708, 0.363919129082426, 0,
        0.547468354430379, 0.363919129082426, 0,
        0.518987341772151, 0.360808709175738, 0,
        0.481012658227848, 0.360808709175738, 0,
        0.439873417721518, 0.363919129082426, 0,
        0.395569620253164, 0.368584758942457, 0,
        0.35759493670886, 0.371695178849144, 0,
        0.319620253164556, 0.373250388802488, 0,
        0.278481012658227, 0.381026438569206, 0,
        0.243670886075949, 0.391912908242612, 0,
        0.221518987341772, 0.404354587869362, 0,
        0.24, 0.41, 0,
        0.25, 0.413685847589424, 0,

    };

    float vertices2[] = {
        0.302237917625585,0.373690580112689,0,
        0.515841996666931,-0.00158717562098242,0,
        0.735784858344576,0.373898896912943,0,
        0.710390048408856,0.367708911991112,0,
        0.678725894770256,0.366240774541703,0,
        0.643917149432584,0.366349892865645,0,
        0.618542179192127,0.361727243869534,0,
        0.583693754463931,0.358701690342036,0,
        0.552049440520593,0.358800888818348,0,
        0.520385286881993,0.357332751368939,0,
        0.485576541544322,0.357441869692881,0,
        0.457176017776366,0.363800492024442,0,
        0.4286762955321,0.362322434727402,0,
        0.400255932068883,0.367113721133243,0,
        0.368611618125545,0.367212919609555,0,
        0.330658281088802,0.368899293706848,0,
        0.302237917625585,0.373690580112689,0,
    };

    unsigned int VBO1, VBO2, VAO1, VAO2;

    // Generate two VAOs and VBOs
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);

    // First Shape (e.g., Triangle)
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO for the first shape
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Second Shape (e.g., Square)
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO for the second shape
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Define the updated colors
        glm::vec4 color1 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);  // Red
        glm::vec4 color2 = glm::vec4(0.564f, 0.933f, 0.564f, 1.0f);  // Light Green
        glm::vec4 color3 = glm::vec4(0.45f, 0.75f, 0.45f, 1.0f);  // Variation of Light Green
        glm::vec4 color4 = glm::vec4(1.0f, 0.6f, 0.7f, 1.0f);  // Light Pink
        glm::vec4 color5 = glm::vec4(1.0f, 0.4f, 0.5f, 1.0f);  // Variation of Light Pink
        glm::vec4 color6 = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f);  // Light Yellow
        glm::vec4 color7 = glm::vec4(1.0f, 1.0f, 0.3f, 1.0f);  // Variation of Light Yellow
        glm::vec4 color8 = glm::vec4(1.0f, 0.65f, 0.0f, 1.0f);  // Light Orange
        glm::vec4 color9 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);  // Black

        // Identity matrix, used as the base for transformations
        glm::mat4 identityMatrix = glm::mat4(1.0f);

        // Transformation matrices for vertices1
        glm::mat4 translationMatrix1, rotationMatrix1, scaleMatrix1, modelMatrix1;
        translationMatrix1 = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, 0.0f));
        rotationMatrix1 = glm::rotate(identityMatrix, glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, 1.0f));
        modelMatrix1 = translationMatrix1 * rotationMatrix1 * scaleMatrix1;

        // Transformation matrices for vertices2
        glm::mat4 translationMatrix2, rotationMatrix2, scaleMatrix2, modelMatrix2;
        translationMatrix2 = glm::translate(identityMatrix, glm::vec3(translate_X2, translate_Y2, 0.0f));
        rotationMatrix2 = glm::rotate(identityMatrix, glm::radians(rotateAngle2), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(scale_X2, scale_Y2, 1.0f));
        modelMatrix2 = translationMatrix2 * rotationMatrix2 * scaleMatrix2;

        // get matrix's uniform location and set matrix
        glUseProgram(shaderProgram);
        unsigned int transformLoc1 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(modelMatrix1));

        // draw our first triangle
        glBindVertexArray(VAO1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_LINE_STRIP, 0, 13);
        glDrawArrays(GL_LINE_STRIP, 14, 20);
        glDrawArrays(GL_LINE_STRIP, 34, 53);
        glDrawArrays(GL_LINE_STRIP, 87, 34);
        glDrawArrays(GL_LINE_STRIP, 121, 46);
        glDrawArrays(GL_LINE_STRIP, 167, 33);
        glDrawArrays(GL_LINE_STRIP, 200, 42);
        glDrawArrays(GL_LINE_STRIP, 242, 36);
        

        // Get the location of the color uniform
        unsigned int colorLocation = glGetUniformLocation(shaderProgram, "triangleColor");

        // Set the color and draw the first triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color1));
        glDrawArrays(GL_TRIANGLE_FAN, 14, 20);

        // Set the color and draw the second triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color2));
        glDrawArrays(GL_TRIANGLE_FAN, 34, 53);

        // Set the color and draw the third triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color3));
        glDrawArrays(GL_TRIANGLE_FAN, 87, 34);

        // Set the color and draw the fourth triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color4));
        glDrawArrays(GL_TRIANGLE_FAN, 121, 46);

        // Set the color and draw the fifth triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color5));
        glDrawArrays(GL_TRIANGLE_FAN, 167, 33);

        // Set the color and draw the sixth triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color6));
        glDrawArrays(GL_TRIANGLE_FAN, 200, 42);

        // Set the color and draw the seventh triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color7));
        glDrawArrays(GL_TRIANGLE_FAN, 242, 36);

      
        //glDrawArrays(GL_LINES, 0, 6);
        //glDrawArrays(GL_LINE_LOOP, 0, 6);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time


        glUseProgram(shaderProgram);
        unsigned int transformLoc2 = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(modelMatrix2));

        // draw our first triangle
        glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glDrawArrays(GL_LINE_STRIP, 0, 17);

        // Set the color and draw the first triangle fan
        glUniform4fv(colorLocation, 1, glm::value_ptr(color8));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 17);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);

    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);

    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// Process input in processInput function
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // For vertices1 transformations (using one set of keys for movement)
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rotateAngle += 0.05f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translate_Y += 0.001f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translate_Y -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translate_X -= 0.001f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translate_X += 0.001f;

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        scale_X += 0.001f;
        scale_Y += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        scale_X -= 0.001f;
        scale_Y -= 0.001f;
    }

    // For vertices2 transformations (using a different set of keys for movement)
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rotateAngle2 += 0.05f; // Rotate with T

    // Translation controls for numpad (move up, down, left, right)
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)   // Numpad 8
        translate_Y2 += 0.001f; // Move up
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)   // Numpad 2
        translate_Y2 -= 0.001f; // Move down
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)   // Numpad 4
        translate_X2 -= 0.001f; // Move left
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)   // Numpad 6
        translate_X2 += 0.001f; // Move right

    // Scaling controls for numpad (+ and -)
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)  // Numpad +
    {
        scale_X2 += 0.001f;
        scale_Y2 += 0.001f;  // Increase scaling
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)  // Numpad -
    {
        scale_X2 -= 0.001f;
        scale_Y2 -= 0.001f;  // Decrease scaling
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}