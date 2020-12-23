/**
 *  For image retrieval, use OpenGL's glReadPixels.
 *  Each chromesome should have one or more polygons.
 *  
 *  Pipeline :
 *      1. Read the target image, obtain its width and height and set the window's resolution accordingly
 *      2. Initialize the polygons and chromesome, then do the evolution thing
 *      3. Compute fitness and show the result of the best chromesome
 *      4. Determine the next generation of chromesome and iterate
 */

#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.hpp"
#include "image.hpp"
#include "triangle.hpp"
#include "chromesome.hpp"

namespace
{
    GLFWwindow* window;

    ycel::Image targetImage("./assets/mona-200.bmp");

    using Chromesome_t = ycel::Chromesome<30>;
    using World_t = std::array<Chromesome_t, 150>;

    World_t m_World;

    // Cache-ing reusable memory
    u_char* renderedData = new u_char[targetImage.GetWidth() * targetImage.GetHeight() * sizeof(u_char) * 3];
}

static void glInit()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // ycel::Image targetImage("./assets/mona-500.bmp");
    std::cout << targetImage.GetWidth() << ", " << targetImage.GetHeight() << "\n";

    window = glfwCreateWindow(
        targetImage.GetWidth(),
        targetImage.GetHeight(),
        "YCEL", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window!\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(!status)
    {
        std::cerr << "Failed to initialize glad!\n";
    }

    glfwSwapInterval(0);
    // glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

static int ComputeFitness(const Chromesome_t& chromesome)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    {
        auto& primitives = chromesome.GetPrimitives();
        for(uint32_t i = 0; i < primitives.size(); ++i)
        {
            hmm_vec4 color = primitives[i].GetColor();
            glColor4f(color[0], color[1], color[2], color[3]);

            auto positions = primitives[i].GetPositions();

            glVertex2f(positions[0].X, positions[0].Y);
            glVertex2f(positions[1].X, positions[1].Y);
            glVertex2f(positions[2].X, positions[2].Y);
        }
    }
    glEnd();

    glReadPixels(0, 0, targetImage.GetWidth(), targetImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, renderedData);
    glClear(GL_COLOR_BUFFER_BIT);

    int result = targetImage.PixelDifferenceMSE(renderedData);

    return result;
}

int main(int argc, char* argv[])
{
    glInit();
    // TMP
    // glEnable(GL_TEXTURE_2D);

    // uint32_t m_RendererID;

    // glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    // glTextureStorage2D(m_RendererID, 1, GL_RGB8, targetImage.GetWidth(), targetImage.GetHeight());

    // glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glTextureSubImage2D(m_RendererID, 0, 0, 0, targetImage.GetWidth(), targetImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, targetImage.GetData());

    // glBindTextureUnit(0, m_RendererID);
    // glBindTexture(GL_TEXTURE_2D, m_RendererID);
    //

    int i = 0;
    while (!glfwWindowShouldClose(window))
    {
        std::cout << i++ << "\n";
        glClear(GL_COLOR_BUFFER_BIT);

        // Update fitness
        for(uint32_t i = 0; i < m_World.size(); ++i)
        {
            m_World[i].UpdateFitness( ComputeFitness(m_World[i]) );
        }

        // Sort the chromesome and render the best
        std::sort(m_World.begin(), m_World.end());

        auto primitives = m_World[0].GetPrimitives();

        glBegin(GL_TRIANGLES);
        {
            for(uint32_t i = 0; i < primitives.size(); ++i)
            {
                hmm_vec4 color = primitives[i].GetColor();
                glColor4f(color[0], color[1], color[2], color[3]);

                auto positions = primitives[i].GetPositions();

                glVertex2f(positions[0].X, positions[0].Y);
                glVertex2f(positions[1].X, positions[1].Y);
                glVertex2f(positions[2].X, positions[2].Y);
            }
        }
        glEnd();

        // Re-production of the next-generation chromesomes
        World_t nextWorld;
        for(uint32_t i = 0; i < m_World.size(); ++i)
        {
            uint32_t p0Id = ycel::Random::RandomInt(0, m_World.size() * 0.25);
            uint32_t p1Id = ycel::Random::RandomInt(0, m_World.size() * 0.25);

            if(ycel::Random::Random01() < 0.95f)
                nextWorld[i] = Chromesome_t::Crossover(m_World[p0Id], m_World[p1Id]);
            else
                nextWorld[i] = Chromesome_t::Crossover(m_World[p0Id], m_World[p1Id], ycel::CrossoverType::FullRandom);
            
            if(ycel::Random::Random01() < 0.95f)
            {
                Chromesome_t::Mutate(nextWorld[i]);
            }
            else
            {
                nextWorld[i] = Chromesome_t();
            }
        }

        m_World = std::move(nextWorld);

        // glBegin(GL_QUADS);
        // {
		// 	//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

        //     glTexCoord2f(0.0f, 0.0f);
        //     glVertex2f(-1.0f, -1.0f);

        //     glTexCoord2f(1.0f, 0.0f);
		// 	glVertex2f(1.0f, -1.0f);

        //     glTexCoord2f(1.0f, 1.0f);
		// 	glVertex2f(1.0f, 1.0f);

        //     glTexCoord2f(0.0f, 1.0f);
		// 	glVertex2f(-1.0f, 1.0f);
        // }
        // glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}