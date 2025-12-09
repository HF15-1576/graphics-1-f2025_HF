#include "Window.h"
#include "Shader.h"
#include "Mesh.h"

#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>

enum ShaderType
{
    SHADER_POSITION_COLOR,
    SHADER_TCOORD_COLOR,
    SHADER_NORMAL_COLOR,
    SHADER_TYPE_COUNT
};

enum MeshType
{
    // Platonic solids
    MESH_TETRAHEDRON,
    MESH_CUBE,
    MESH_OCTAHEDRON,
    MESH_DODECAHEDRON,
    MESH_ICOSAHEDRON,

    // Parametric surfaces
    MESH_PLANE,
    MESH_SPHERE,
    MESH_HEMISPHERE,

    // Obj files
    MESH_HEAD,

    MESH_TYPE_COUNT
};

<<<<<<< Updated upstream
=======
enum TextureType
{
    TEXTURE_GRADIENT_WARM,
    TEXTURE_GRADIENT_COOL,
    TEXTURE_TYPE_COUNT
};

enum A4DrawType
{
    A4_PAR_SHAPES_NORMAL_SHADER,
    A4_OBJ_FILE_TCOORDS_SHADER,
    A4_CT4_TEXTURE_SHADER,
    A4_MANUAL_MESH,
    A4_CUSTOM_DRAW,
    A4_TYPE_COUNT
};

void LoadTextures(Texture textures[TEXTURE_TYPE_COUNT])
{
    Image warm, cool;
    LoadImage(&warm, 512, 512);
    LoadImage(&cool, 512, 512);
    LoadImageGradient(&warm, Vector3Zeros, Vector3UnitX, Vector3UnitY, Vector3UnitX + Vector3UnitY);
    LoadImageGradient(&cool, Vector3UnitZ, Vector3UnitZ + Vector3UnitX, Vector3UnitY + Vector3UnitZ, Vector3Ones);

    // Uncomment to view gradient within the following file:
    //SaveImage("./assets/textures/cool_gradient.png", cool);

    LoadTexture(&textures[TEXTURE_GRADIENT_WARM], warm);
    LoadTexture(&textures[TEXTURE_GRADIENT_COOL], cool);
}

struct Camera
{
    float pitch = 0.0f;
    float yaw = 0.0f;
    Vector3 position = Vector3Zeros;
};

// manual mesh triangle
Mesh manualMesh;

void LoadManualMesh()
{
    manualMesh.positions =
    {
        { -1.0f, -1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        {  0.0f,  1.0f, 0.0f }
    };

    manualMesh.normals =
    {
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }
    };

    manualMesh.indices = { 0, 1, 2 };
    manualMesh.vertex_count = 3;

    glGenVertexArrays(1, &manualMesh.vao);
    glBindVertexArray(manualMesh.vao);

  
    glGenBuffers(1, &manualMesh.pbo);
    glBindBuffer(GL_ARRAY_BUFFER, manualMesh.pbo);
    glBufferData(GL_ARRAY_BUFFER,
        manualMesh.positions.size() * sizeof(Vector3),
        manualMesh.positions.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    
    std::vector<Vector2> fakeTcoords =
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.5f, 1.0f}
    };
	// Texture coordinate buffer
    glGenBuffers(1, &manualMesh.tbo);
    glBindBuffer(GL_ARRAY_BUFFER, manualMesh.tbo);
    glBufferData(GL_ARRAY_BUFFER,
        fakeTcoords.size() * sizeof(Vector2),
        fakeTcoords.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Normal buffer
    glGenBuffers(1, &manualMesh.nbo);
    glBindBuffer(GL_ARRAY_BUFFER, manualMesh.nbo);
    glBufferData(GL_ARRAY_BUFFER,
        manualMesh.normals.size() * sizeof(Vector3),
        manualMesh.normals.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Index buffer
    glGenBuffers(1, &manualMesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, manualMesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        manualMesh.indices.size() * sizeof(uint16_t),
        manualMesh.indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);
}

>>>>>>> Stashed changes
int main()
{
    CreateWindow(800, 800, "Graphics 1");

    Mesh meshes[MESH_TYPE_COUNT];

    LoadMeshTetrahedron(&meshes[MESH_TETRAHEDRON]);
    LoadMeshCube(&meshes[MESH_CUBE]);
    LoadMeshOctahedron(&meshes[MESH_OCTAHEDRON]);
    LoadMeshDodecahedron(&meshes[MESH_DODECAHEDRON]);
    LoadMeshIcosahedron(&meshes[MESH_ICOSAHEDRON]);

    LoadMeshPlane(&meshes[MESH_PLANE]);
    LoadMeshSphere(&meshes[MESH_SPHERE]);
    LoadMeshHemisphere(&meshes[MESH_HEMISPHERE]);

<<<<<<< Updated upstream
    //LoadMeshObj(&meshes[MESH_HEAD], "./assets/meshes/head.obj");
    LoadMeshObj(&meshes[MESH_HEAD], "./assets/meshes/plane.obj");
    
=======
    LoadMeshObj(&meshes[MESH_HEAD], "./assets/meshes/head.obj");
	LoadManualMesh();


>>>>>>> Stashed changes
    GLuint position_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/position_color.vert");
    GLuint tcoord_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/tcoord_color.vert");
    GLuint normal_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/normal_color.vert");
    GLuint vertex_color_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/vertex_color.frag");

    GLuint shaders[SHADER_TYPE_COUNT];
    shaders[SHADER_POSITION_COLOR] = CreateProgram(position_color_vert, vertex_color_frag);
    shaders[SHADER_TCOORD_COLOR] = CreateProgram(tcoord_color_vert, vertex_color_frag);
    shaders[SHADER_NORMAL_COLOR] = CreateProgram(normal_color_vert, vertex_color_frag);

<<<<<<< Updated upstream
    int shader_index = 0;
    int mesh_index = 0;
=======
    Texture textures[TEXTURE_TYPE_COUNT];
    LoadTextures(textures);

    Camera camera;
    camera.position = { 0.0f, 0.0f, 5.0f };

    int shader_index = SHADER_SAMPLE_TEXTURE;
    int mesh_index = MESH_PLANE;
    int texture_index = TEXTURE_GRADIENT_COOL;
    int draw_index = A4_PAR_SHAPES_NORMAL_SHADER;

>>>>>>> Stashed changes
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        if (IsKeyPressed(KEY_GRAVE_ACCENT))
            ++shader_index %= SHADER_TYPE_COUNT;

        if (IsKeyPressed(KEY_TAB))
            ++mesh_index %= MESH_TYPE_COUNT;

        float tt = Time();

<<<<<<< Updated upstream
        //Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
=======
        // Below is test-rotation code. For full marks, you must rotate the camera with the mouse delta that should be implemented as follows:
        // Extend Window.h & Window.cpp based on glfw documentation to track the change in mouse-position between frames, then make a function to return the mouse delta as a Vector2.
       
        //if (IsKeyDown(KEY_1))
            //camera.yaw -= 100.0f * dt * DEG2RAD;

        //if (IsKeyDown(KEY_2))
            //camera.yaw += 100.0f * dt * DEG2RAD;

        //if (IsKeyDown(KEY_3))
            //camera.pitch -= 100.0f * dt * DEG2RAD;

        //if (IsKeyDown(KEY_4))
            //camera.pitch += 100.0f * dt * DEG2RAD;
       

        // mouse movement/looking code:
        Vector2 mouseDelta = GetMouseDelta();
        float sensitivity = 0.0025f;

        camera.yaw -= mouseDelta.x * sensitivity;
        camera.pitch -= mouseDelta.y * sensitivity;

        // Limit pitch to avoid flipping
        float pitch_limit = 89.0f * DEG2RAD;
        if (camera.pitch > pitch_limit) camera.pitch = pitch_limit;
        if (camera.pitch < -pitch_limit) camera.pitch = -pitch_limit;

        Matrix camera_rotation = MatrixRotateY(camera.yaw) * MatrixRotateX(camera.pitch);
        Vector3 camera_direction_z = { camera_rotation.m8, camera_rotation.m9, camera_rotation.m10 };
        Vector3 camera_direction_x = { camera_rotation.m0, camera_rotation.m1, camera_rotation.m2 };
        Vector3 camera_direction_y = { camera_rotation.m4, camera_rotation.m5, camera_rotation.m6 };

        if (IsKeyDown(KEY_W))
            camera.position -= camera_direction_z * 10.0f * dt;

        if (IsKeyDown(KEY_S))
            camera.position += camera_direction_z * 10.0f * dt;

        if (IsKeyDown(KEY_D))
            camera.position += camera_direction_x * 10.0f * dt;

        if (IsKeyDown(KEY_A))
            camera.position -= camera_direction_x * 10.0f * dt;

        if (IsKeyDown(KEY_SPACE))
            camera.position += camera_direction_y * 10.0f * dt;

        if (IsKeyDown(KEY_LEFT_SHIFT))
            camera.position -= camera_direction_y * 10.0f * dt;

        // view-matrix is the inverse of the camera matrix
        // camera-matrix is the translation & rotation about y & x of the camera
>>>>>>> Stashed changes
        Matrix proj = MatrixPerspective(75.0f * DEG2RAD, WindowWidth() / (float)WindowHeight(), 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        //Matrix world = MatrixRotateY(tt * 100.0f * DEG2RAD);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

<<<<<<< Updated upstream
        BeginShader(shaders[shader_index]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(meshes[MESH_PLANE]);
            //DrawMesh(meshes[mesh_index]);
        EndShader();
=======
        // Example "mix-and-match" draw calls to understand Smiley's code
        //BeginShader(shaders[shader_index]);
        //BeginTexture(textures[texture_index]);
        //    SendMat4(mvp, "u_mvp");
        //    DrawMesh(meshes[mesh_index]);
        //EndTexture();
        //EndShader();
        // (Replace with A4 draw types within the switch-case below):

        switch (draw_index)
        {
        case A4_PAR_SHAPES_NORMAL_SHADER:
            BeginShader(shaders[SHADER_NORMAL_COLOR]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(meshes[MESH_SPHERE]);
            EndShader();
            break;

        case A4_OBJ_FILE_TCOORDS_SHADER:
        {
            Mesh& m = meshes[MESH_HEAD];
            if (m.vao == 0) 
            {
                printf("OBJ is invaild\n");
                break;
            }

            BeginShader(shaders[SHADER_POSITION_COLOR]); // not TCOORD shader
            SendMat4(mvp, "u_mvp");
            DrawMesh(m);
            EndShader();
            break;
        }

        case A4_CT4_TEXTURE_SHADER:
            BeginShader(shaders[SHADER_SAMPLE_TEXTURE]);
            BeginTexture(textures[TEXTURE_GRADIENT_COOL]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(meshes[MESH_PLANE]);
            EndTexture();
            EndShader();
            break;

        case A4_MANUAL_MESH:
            BeginShader(shaders[SHADER_POSITION_COLOR]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(manualMesh);
            EndShader();
            break;

        case A4_CUSTOM_DRAW:
            BeginShader(shaders[SHADER_NORMAL_COLOR]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(meshes[MESH_HEMISPHERE]);
            EndShader();
            break;
        }
>>>>>>> Stashed changes

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }

    DestroyShader(&position_color_vert);
    DestroyShader(&tcoord_color_vert);
    DestroyShader(&normal_color_vert);
    DestroyShader(&vertex_color_frag);

    for (int i = 0; i < SHADER_TYPE_COUNT; i++)
        DestroyProgram(&shaders[i]);

    for (int i = 0; i < MESH_TYPE_COUNT; i++)
        UnloadMesh(&meshes[i]);

    // cleanup manual mesh
    UnloadMesh(&manualMesh);

    DestroyWindow();
    return 0;
}