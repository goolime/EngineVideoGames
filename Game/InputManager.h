#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		//scn->MoveSnake(scn->pickedShape, scn->zCameraTranslate, yoffset); jump snake
		//scn->shapeTransformation(scn->zCameraTranslate,yoffset);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				case GLFW_KEY_SPACE:
					//scn->HideShape(0);
					//if(scn->IsActive())
					//	scn->Deactivate();
					//else
					//	scn->Deactivate();
					if(scn->canmove)
						scn->canmove = false;
					else 
						scn->canmove = true;
				break;
					case GLFW_KEY_RIGHT:
					//scn->shapeTransformation(scn->zGlobalRotate,-20.1f);
					scn->MoveSnake( scn->yLocalRotate, -2.0f);
					//scn->shapeTransformation(scn->zLocalRotate,5.1f);
//						cout<< "right: "<<endl;
					break;
				case GLFW_KEY_LEFT:
					//scn->shapeTransformation(scn->zGlobalRotate,20.1f);
					scn->MoveSnake(scn->yLocalRotate, 2.0f);
					//scn->shapeTransformation(scn->zLocalRotate,-5.0f);
//					cout<< "left: "<<endl;
					break;
				case GLFW_KEY_UP:
					scn->speed(1);
					//cout<< "up: "<<endl;
					//scn->MoveSnake(scn->zLocalRotate, 2.0f);
					break;
				case GLFW_KEY_DOWN:
					scn->speed(-1);
					//scn->MoveSnake(scn->zLocalRotate, -2.0f);
					//scn->MoveSnake(scn->pickedShape, scn->xGlobalRotate, -5.0f);
					//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					break;
				case GLFW_KEY_O:
					scn->Curentcamera = (scn->Curentcamera + 1) % 3;
						break;

				case GLFW_KEY_1:
					scn->hide(wall);
					break;
				case GLFW_KEY_2:
					scn->hide(rampS);
					break;
				case GLFW_KEY_3:
					scn->hide(apple);
					break;
				case GLFW_KEY_4:
					scn->hide(mine);
					break;
				case GLFW_KEY_Y:
					scn->MoveSnake(scn->zLocalRotate, 2.0f);
					break;
				case GLFW_KEY_H:
					scn->MoveSnake(scn->zLocalRotate, -2.0f);
					break;
			default:
				break;
			}
		}
	}
	

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->updatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			//scn->mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//scn->mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->resize(width,height);
		//relation = (float)width/(float)height;
	}

	void init(Display &display)
	{
		display.addKeyCallBack(key_callback);
		display.addMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.addResizeCallBack(window_size_callback);

	}
