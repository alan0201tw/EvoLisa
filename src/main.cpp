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
#include "polygon.hpp"
#include "chromesome.hpp"

#include "stb_image_write.h"

namespace
{
    GLFWwindow* window;

    ycel::Image targetImage("./assets/mona-200.bmp");

	// every polygon have V vertices, we have T polygons
    using Chromesome_t = ycel::Chromesome<4, 50>;
    using World_t = std::array<Chromesome_t, 150>;

    World_t m_World;
	float m_CrossoverRate = 0.95f;

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
	if (!status)
	{
		std::cerr << "Failed to initialize glad!\n";
	}

	glfwSwapInterval(0);
	// glfwSwapInterval(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

static uint32_t ComputeFitness(const Chromesome_t& chromesome)
{
	glClear(GL_COLOR_BUFFER_BIT);

	auto& primitives = chromesome.GetPrimitives();
	for (uint32_t i = 0; i < primitives.size(); ++i)
	{
		glBegin(GL_POLYGON);
		{
			hmm_vec4 color = primitives[i].GetColor();
			glColor4f(color[0], color[1], color[2], color[3]);

			auto positions = primitives[i].GetPositions();

			for (uint32_t vid = 0; vid < primitives[i].GetVerticesCount(); ++vid)
			{
				glVertex2f(positions[vid].X, positions[vid].Y);
			}
		}
		glEnd();
	}

	glReadPixels(0, 0, targetImage.GetWidth(), targetImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, renderedData);
	glClear(GL_COLOR_BUFFER_BIT);

	uint32_t result = targetImage.PixelDifferenceMSE(renderedData);

	return result;
}

static void OutputImage(const std::string& fileName)
{
	stbi_flip_vertically_on_write(true);

	glReadPixels(0, 0, targetImage.GetWidth(), targetImage.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, renderedData);

	if (stbi_write_png(fileName.c_str(), targetImage.GetWidth(), targetImage.GetHeight(), 3, renderedData, 0) == 0)
	{
		std::cerr << "Error when saving image\n";
	}
}

int main(int argc, char* argv[])
{
	glInit();

	uint32_t iteration = 0;
	while (!glfwWindowShouldClose(window))
	{
		std::cout << iteration++ << "\n";
		glClear(GL_COLOR_BUFFER_BIT);

		// Update fitness
		for (uint32_t i = 0; i < m_World.size(); ++i)
		{
			m_World[i].UpdateFitness(ComputeFitness(m_World[i]));
		}

		// Sort the chromesome and render the best
		std::sort(m_World.begin(), m_World.end());

		auto& primitives = m_World[0].GetPrimitives();
		for (uint32_t i = 0; i < primitives.size(); ++i)
		{
			glBegin(GL_POLYGON);
			{
				hmm_vec4 color = primitives[i].GetColor();
				glColor4f(color[0], color[1], color[2], color[3]);

				auto positions = primitives[i].GetPositions();

				for (uint32_t vid = 0; vid < primitives[i].GetVerticesCount(); ++vid)
				{
					glVertex2f(positions[vid].X, positions[vid].Y);
				}
			}
			glEnd();
		}

		if (iteration % 500 == 1)
			OutputImage("./images/result_" + std::to_string(iteration-1) + ".png");

		// Re-production of the next-generation chromesomes
		World_t nextWorld;
		uint32_t survivorCount = static_cast<uint32_t>(m_World.size() * 0.25);
		for (uint32_t i = 0; i < m_World.size(); ++i)
		{
			if (i <= survivorCount)
			{
				nextWorld[i] = m_World[i];
				continue;
			}

			uint32_t p0Id = ycel::Random::RandomInt(0, static_cast<int>(m_World.size() - 1));
			uint32_t p1Id = ycel::Random::RandomInt(0, static_cast<int>(m_World.size() - 1));

			if (ycel::Random::Random01() < m_CrossoverRate)
			{
				if (ycel::Random::Random01() < 0.5f)
					nextWorld[i] = Chromesome_t::Crossover(m_World[p0Id], m_World[p1Id], ycel::CrossoverType::Devision);
				else
					nextWorld[i] = Chromesome_t::Crossover(m_World[p0Id], m_World[p1Id], ycel::CrossoverType::FullRandom);
			}
			else
			{
				nextWorld[i] = m_World[i];
				if (ycel::Random::Random01() < 0.95f)
				{
					Chromesome_t::Mutate(nextWorld[i]);
				}
				else
				{
					Chromesome_t::MutateRand(nextWorld[i]);
				}
			}
		}

		m_World = std::move(nextWorld);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
