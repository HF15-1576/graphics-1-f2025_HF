#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>

// 4 lines in 1 square -- each line has 2 vertices, therefore 8 array elements because 4 lines * 2 vertices per line = 8
// Hint: if 1 square is 8 vertices, and Assignment 2 requires you to render 8 squares, then 8 squares * 8 vertices per square = 64 vertices;
// (Consider reserving 64 vertices worth of space if you'd like to fit all your positions in a single vertex array)

static const float square_colors[8][3] = {
    {1.0f, 0.0f, 0.0f}, // Red
    {2.0f, 0.0f, 1.0f}, // Pinkish
    {0.0f, 0.0f, 1.0f}, // Blue
    {1.0f, 1.0f, 0.0f}, // Yellow
    {1.0f, 0.0f, 2.0f}, // Magenta
    {0.0f, 1.0f, 1.0f}, // Cyan
    {1.0f, 0.5f, 0.0f}, // Orange
    {0.5f, 0.0f, 2.0f}  // Purple
};

static const int line_vertex_count = 8;
static const Vector2 line_vertex_positions[line_vertex_count]
{
    { -1.0f,  -1.0f },   // bottom-left
    {  1.0f,  -1.0f },   // bottom-right

    {  1.0f, -1.0f },   // bottom-right
    {  1.0f,  1.0f },   // top-right

    {   1.0f,  1.0f },   // top-right
    {  -1.0f,  1.0f },   // top-left

    { -1.0f,   1.0f },   // top-left
    { -1.0f,  -1.0f }    // bottom-left
};

// Generates vertices for 'count' shrinking squares
std::vector<Vector2> generateSquares(int count) {
    std::vector<Vector2> vertices;

    // start with the outermost square
    std::vector<Vector2> current = {
        { -1.0f, -1.0f },
        {  1.0f, -1.0f },
        {  1.0f,  1.0f },
        { -1.0f,  1.0f }
    };

    for (int i = 0; i < count; i++) {
        // add current square's lines to vertices
        vertices.push_back(current[0]); vertices.push_back(current[1]);
        vertices.push_back(current[1]); vertices.push_back(current[2]);
        vertices.push_back(current[2]); vertices.push_back(current[3]);
        vertices.push_back(current[3]); vertices.push_back(current[0]);

        // compute next square by finding midpoints of current square's edges
        std::vector<Vector2> next;
        for (int x = 0; x < 4; x++) {
            Vector2 a = current[x];
            Vector2 b = current[(x + 1) % 4];
            next.push_back(Vector2Lerp(a, b, 0.5f));
        }
        current = next;
    }

    return vertices;
}

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    // Hint: The a1_triangle shaders handle vertex position AND vertex colour.
    // Vertex colour is needed in order to receive full marks on this assignment!
    GLuint a2_lines_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a2_lines.vert");
    GLuint a2_lines_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a2_lines.frag");
    GLuint a2_lines_shader = CreateProgram(a2_lines_vert, a2_lines_frag);

    // Generate 8 shrinking squares
    const int square_count = 24;
    std::vector<Vector2> allVertices = generateSquares(square_count);

    GLuint vbo_line_positions;
    glGenBuffers(1, &vbo_line_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vector2),
        allVertices.data(), GL_STATIC_DRAW);
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertex_positions), line_vertex_positions, GL_STATIC_DRAW);*/
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertex_positions2), line_vertex_positions2, GL_STATIC_DRAW);*/
	// this method above is a bit inefficient since we're creating multiple squares manually so went with an automated approach
    // Comment/uncomment to see 1st square vs 2nd square.
    // Your job is to automate the generation of squares so all 8 squares are rendered at once!

    GLuint vao_line;
    glGenVertexArrays(1, &vao_line);
    glBindVertexArray(vao_line);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_line_positions);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

    glBindVertexArray(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLint u_color = glGetUniformLocation(a2_lines_shader, "u_color");
    GLint u_mvp = glGetUniformLocation(a2_lines_shader, "u_mvp");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Note: change MatrixOrtho's left/right/bottom/top values to modify the extents of your screen!
        Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(a2_lines_shader);
        glBindVertexArray(vao_line);
        glLineWidth(3.0f);

        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

        // draw each square with different colour
        for (int i = 0; i < square_count; i++) {
            int color_idx = i % 8;
            glUniform3f(u_color, square_colors[color_idx][0], square_colors[color_idx][1], square_colors[color_idx][2]);
            glDrawArrays(GL_LINES, i * line_vertex_count, line_vertex_count);
        }

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }

    glDeleteVertexArrays(1, &vao_line);
    glDeleteBuffers(1, &vbo_line_positions);
    glDeleteProgram(a2_lines_shader);
    glDeleteShader(a2_lines_frag);
    glDeleteShader(a2_lines_vert);

    DestroyWindow();
    return 0;
}