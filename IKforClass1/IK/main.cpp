#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#pragma comment(lib, "winmm.lib")

void init()
{
	glfwSetKeyCallback(display.m_window,key_callback);
	glfwSetWindowSizeCallback(display.m_window,window_size_callback);
}


int main(int argc, char** argv)
{
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};



	unsigned int indices[] = {0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };


	scn.init(vertices,indices,sizeof(vertices)/sizeof(vertices[0]), sizeof(indices)/sizeof(indices[0]));
	scn.addShader("./res/shaders/basicShader");
	scn.addShader("./res/shaders/pickingShader");

	//PlaySound(TEXT("C:\\Users\\tbarami\\Documents\\GitHub\\3D-Snake\\IKforClass1\\IK\\res\\sounds\\theme.wav"), NULL, SND_FILENAME | SND_ASYNC);
	std::cout << std::setprecision(2) << std::fixed;
	//scn.addShape("./res/monkey3.obj","./res/grass.bmp");
	//Shader shader("./res/basicShader");
	//inputManager input = inputManager();
	init();	
	//glfwSetInputMode(display.m_window,GLFW_STICKY_MOUSE_BUTTONS,1);
	
	while(!glfwWindowShouldClose(display.m_window))
	{
		if(!scn.paused)
		{
			Sleep(30);
			if(scn.gameOver == 1)
			{
				std::cout << "You won! Your score is: " << scn.score << std::endl;
			}
			else if (scn.gameOver == 2) {
				std::cout << "You lost! Your score is: " << scn.score << std::endl;
			}
			else
			{
				scn.move_enemies();
				scn.make_change();
				scn.update_movement();
			}
		}
		
		

		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		if(display.IsFullscreen())
		{
			GLint viewport[4];
			glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3] );
			window_size_callback(display.m_window, viewport[2],viewport[3]);
		}
		scn.draw(0,0,false); //change false to true for axis in every joint

		display.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}
