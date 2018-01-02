#include "Windows.h"
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cube.h>
#include <SceneData.h>
#include <stdlib.h>     /* srand, rand */
#include <fstream>
#include <string> 


using namespace glm;
using namespace std;

static const int DISPLAY_WIDTH = 800,DISPLAY_HEIGHT = 800,mySize =3;//general variable for cube size

float  yAngle = 0.0, zAngle = 0.0,angle = 90.0f;
int current_axis = 0, clockwise = 1,mixNumber = 1;
int cube_index[mySize][mySize][mySize];
bool notYet = true;
mat4 rotations[mySize*mySize*mySize];//cube rotations
cube *myCube;
SceneData* myScene;
vec3 partial_rotations[mySize*mySize*mySize];//for rotation less than 45 deg




void rotate_face(int axis, int col, int direction) {
	//Xaxis=0,Yaxis=1,Zaxis=2
	if (angle == 180 && notYet) {
		angle = 90.0f;
		notYet = false;
		rotate_face( axis,  col,  direction);
		rotate_face(axis, col, direction);
		notYet = true;
		angle = 180.0f;
		return;
	}

	current_axis = axis;
	int i = 0, j = 0;
	int face[mySize][mySize];// temporary face array for rotating

	for ( i = 0; i < mySize; i++) {
		for ( j = 0; j < mySize; j++) {
			if(axis == 0)
				face[i][j] = cube_index[i][col][j];
			if (axis == 1)
				face[i][j] = cube_index[col][i][j];
			if (axis == 2)
				face[i][j] = cube_index[i][j][col];
		}
	}

	for (i = 0; i < mySize; i++) {
		for (j = 0; j < mySize; j++) {
			if (axis == 0) {
				float prev = partial_rotations[cube_index[i][col][j]][axis];		
				partial_rotations[cube_index[i][col][j]][axis] += direction*angle;

				if ((int)(partial_rotations[cube_index[i][col][j]][axis]) % 90 == 0 && prev == 0) {//for rotation less than 45 and without a previous angle is 0;
					partial_rotations[cube_index[i][col][j]][axis] = 0.0f;
					if (direction == 1)
						cube_index[i][col][j] = face[j][mySize - i -1];
					else
						cube_index[i][col][j] = face[mySize - j - 1][i];
					rotations[cube_index[i][col][j]] = rotate(direction*angle, vec3(1, 0, 0))*rotations[cube_index[i][col][j]];
				}
				else if ((int)(partial_rotations[cube_index[i][col][j]][axis]) % 90 == 0 && prev != 0) {
					rotations[cube_index[i][col][j]] = rotate(partial_rotations[cube_index[i][col][j]][axis], vec3(1, 0, 0))*rotations[cube_index[i][col][j]];
					partial_rotations[cube_index[i][col][j]][axis] = 0.0f;
					if (direction == 1)
						cube_index[i][col][j] = face[j][mySize - i - 1];
					else
						cube_index[i][col][j] = face[mySize - j - 1][i];
				}
			}
			else if (axis == 1) {
				float prev = partial_rotations[cube_index[col][i][j]][axis];

				partial_rotations[cube_index[col][i][j]][axis] += direction*angle;

				if ((int)(partial_rotations[cube_index[col][i][j]][axis]) % 90 == 0 && prev == 0) {
					partial_rotations[cube_index[col][i][j]][axis] = 0.0f;
					if (direction == 1)
						cube_index[col][i][j] = face[mySize - j - 1][i];
					else
						cube_index[col][i][j] = face[j][mySize - i - 1];
					rotations[cube_index[col][i][j]] = rotate(direction*angle, vec3(0, 1, 0))*rotations[cube_index[col][i][j]];
				}
				else if ((int)(partial_rotations[cube_index[col][i][j]][axis]) % 90 == 0 && prev != 0) {
					rotations[cube_index[col][i][j]] = rotate(partial_rotations[cube_index[col][i][j]][axis], vec3(0, 1, 0))*rotations[cube_index[col][i][j]];
					partial_rotations[cube_index[col][i][j]][axis] = 0.0f;
					if (direction == 1)
						cube_index[col][i][j] = face[mySize - j - 1][i];
					else
						cube_index[col][i][j] = face[j][mySize - i - 1];
				}
			}
			else if (axis == 2) {
				float prev = partial_rotations[cube_index[i][j][col]][axis];

				partial_rotations[cube_index[i][j][col]][axis] += direction*angle;

				if ((int)(partial_rotations[cube_index[i][j][col]][axis]) % 90 == 0 && prev == 0) {
					partial_rotations[cube_index[i][j][col]][axis] = 0.0f;
					if (direction == 1)
						cube_index[i][j][col] = face[mySize - j - 1][i];
					else
						cube_index[i][j][col] = face[j][mySize - i - 1];
					rotations[cube_index[i][j][col]] = rotate(direction*angle, vec3(0, 0, 1))*rotations[cube_index[i][j][col]];
				}
				else if ((int)(partial_rotations[cube_index[i][j][col]][axis]) % 90 == 0 && prev != 0) {
					rotations[cube_index[i][j][col]] = rotate(partial_rotations[cube_index[i][j][col]][axis], vec3(0, 0, 1))*rotations[cube_index[i][j][col]];
					partial_rotations[cube_index[i][j][col]][axis] = 0.0f;
					if (direction == 1)
						cube_index[i][j][col] = face[mySize - j - 1][i];
					else
						cube_index[i][j][col] = face[j][mySize - i - 1];
				}
			}
		}
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ofstream  myfile;
	int length = rand() % 25 + 25;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
		/*----------------------x axis-------------------*/
	case GLFW_KEY_L:
		if (action == GLFW_PRESS) {
			if(clockwise ==1)
				rotate_face(0, mySize-1, 1);
			else
				rotate_face(0, mySize - 1, -1);
		}
		break;

	case GLFW_KEY_R:
		if (action == GLFW_PRESS) {
			if (clockwise == 1)
				rotate_face(0, 0, 1);
			else
				rotate_face(0, 0, -1);
		}
		break;

		/*----------------------y axis-------------------*/
	case GLFW_KEY_U:
		if (action == GLFW_PRESS) {
			if (clockwise == 1)
				rotate_face(1, mySize - 1, 1);
			else
				rotate_face(1, mySize - 1, -1);
		}
		break;

	case GLFW_KEY_D:
		if (action == GLFW_PRESS) {
			if (clockwise == 1)
				rotate_face(1, 0, 1);
			else
				rotate_face(1, 0, -1);
		}
		break;
		/*----------------------z axis-------------------*/
	case GLFW_KEY_B:
		if (action == GLFW_PRESS) {
			if (clockwise == 1)
				rotate_face(2, mySize - 1, 1);
			else
				rotate_face(2, mySize - 1, -1);
		}
		break;

	case GLFW_KEY_F:
		if (action == GLFW_PRESS) {
			if (clockwise == 1)
				rotate_face(2, 0, 1);
			else
				rotate_face(2, 0, -1);
		}
		break;
		/*----------------------flip clockwise-------------------*/

	case GLFW_KEY_APOSTROPHE:
		if (action == GLFW_PRESS) {
			clockwise = -1 * clockwise;
		}
		break;
		/*----------------------rotation angle-------------------*/
	case GLFW_KEY_A:
		if (action == GLFW_PRESS) {
			angle = angle * 2;
			if (angle > 180.0f)
				angle = 180.0f;
		}
		break;
	case GLFW_KEY_Z:
			if (action == GLFW_PRESS) {
				angle = angle/2;
			}
			break;

		/*----------------------Arrows-------------------*/
	case GLFW_KEY_UP:
		yAngle = 5;
		//myScene->setMainMat(rotate(mat4(myScene->getMainMat()), yAngle, vec3(0, 0, 1)));
		myScene->setProjection(rotate(myScene->getProjection(), yAngle, vec3(1, 0, 0)));
		break;

	case GLFW_KEY_DOWN:
		yAngle = -5;
		myScene->setProjection(rotate(myScene->getProjection(), yAngle, vec3(1, 0, 0)));
		break;

	case GLFW_KEY_RIGHT:
		zAngle = 5;
		myScene->setProjection(rotate(myScene->getProjection(), zAngle, vec3(0, 1, 0)));
		break;

	case GLFW_KEY_LEFT:
		zAngle = -5;
		myScene->setProjection(rotate(myScene->getProjection(), zAngle, vec3(0, 1, 0)));
		break;
		//4X4 CUBE ROTATION
	case GLFW_KEY_Q:
		if (action == GLFW_PRESS) {//up_middle_x
			if (mySize == 4 || mySize == 5) {
				if (clockwise == 1)
					rotate_face(1, 1, 1);
				else
					rotate_face(1, 1, -1);
			}
		}
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS) {//down_middle_x
			if (mySize == 4 || mySize == 5) {

				if (clockwise == 1)
					rotate_face(1, 2, 1);
				else
					rotate_face(1, 2, -1);
			}
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_PRESS) {//right_middle_y
			if (mySize == 4 || mySize == 5) {

				if (clockwise == 1)
					rotate_face(0, 1, 1);
				else
					rotate_face(0, 1, -1);
			}
		}
		break;
	case GLFW_KEY_T:
		if (action == GLFW_PRESS) {//left_middl_y
			if (mySize == 4 || mySize == 5) {

				if (clockwise == 1)
					rotate_face(0, 2, 1);
				else
					rotate_face(0, 2, -1);
			}
		}
		break;
	case GLFW_KEY_Y:
		if (action == GLFW_PRESS) {//down_middle_z
			if (mySize == 4 || mySize == 5) {

				if (clockwise == 1)
					rotate_face(2, 1, 1);
				else
					rotate_face(2, 1, -1);
			}
		}
		break;
	case GLFW_KEY_I:
		if (action == GLFW_PRESS) {//up_middle_z
			if (mySize == 4 || mySize == 5) {

				if (clockwise == 1)
					rotate_face(2, 2, 1);
				else
					rotate_face(2, 2, -1);
			}
		}
		break;
		// operate just on  5x5 
	case GLFW_KEY_O:
		if (action == GLFW_PRESS) {//up_middle_x
			if (mySize == 5) {
				if (clockwise == 1)
					rotate_face(1, 3, 1);
				else
					rotate_face(1, 3, -1);
			}
		}
		break;
	case GLFW_KEY_P:
		if (action == GLFW_PRESS) {//right_middle_y
			if (mySize == 5) {

				if (clockwise == 1)
					rotate_face(0, 3, 1);
				else
					rotate_face(0, 3, -1);
			}
		}
		break;
	case GLFW_KEY_G:
		if (action == GLFW_PRESS) {//down_middle_z
			if (mySize == 5) {

				if (clockwise == 1)
					rotate_face(2, 3, 1);
				else
					rotate_face(2, 3, -1);
			}
		}
		break;

	case GLFW_KEY_M:
		//MIXER- mixing the cube and writing the steps in a target file- mixer.txt
		myfile.open("mixer.txt", ios::app);
		myfile << "==========MIXING "+ to_string(mySize)+"x"+ to_string(mySize)+" for the "+to_string(mixNumber)+" time ===== \n";
		for (int i = 0; i < length; i++){
			int a = rand() % 3; int b = rand() % mySize; int c = rand() & 1;
			rotate_face(a, b, c);
			myfile << to_string(i + 1) + ") Rotating ";
			switch (a) {
			case 0:
				myfile << "on x axis ";
				break;
			case 1:
				myfile << "on y axis ";
				break;

			case 2:
				myfile << "on z axis ";
				break;

			}
			
			switch (b) {
			case mySize-1:
				myfile << "the " + to_string(mySize - 1) + " face ";
				break;
			case mySize-2:
				myfile << "the " + to_string(mySize - 2) + " face ";
				break;
			case mySize-3:
				myfile << "the " + to_string(mySize - 3) + " face ";
				break;
			case mySize-4:
				myfile << "the " + to_string(mySize - 4) + " face ";
				break;
			case mySize-5:
				myfile << "the " + to_string(mySize - 5) + " face ";
				break;
			}

			switch (c) {
			case 0:
				myfile << " clockwise .\n";
				break;
			case 1:
				myfile << "counterclockwise .\n";
				break;

			}
			Sleep(1);
		}
		myfile << "=========Finished=======\n\n";

		//myfile.close();
		mixNumber++;
		break;


	default:
		break;

	}
}

int main(int argc, char** argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	Shader shader("./res/shaders/basicShader");
	// initialize Scene Data
	myScene = new SceneData(vec3(0, 0, -15), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f));//camera propreties
	myScene->setPerspectiveProjection(60.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);//projection propreties
	myScene->setProjection(mat4((myScene->getProjection()))*lookAt(vec3(myScene->getPos()), vec3(myScene->getPos() + myScene->getForward()), vec3(myScene->getUp())));
	myScene->muliplyMVP();

	glfwSetKeyCallback(display.m_window, key_callback);

	myCube = new cube(0,0,0);

	/* init */
	for (int i = 0; i < mySize; ++i)
		for (int j = 0; j < mySize; ++j)
			for (int k = 0; k < mySize; ++k) {
				cube_index[i][j][k] = i * mySize*mySize + j * mySize + k;
				rotations[cube_index[i][j][k]] = mat4(1.0);
				partial_rotations[cube_index[i][j][k]] = vec3(0.0f, 0.0f, 0.0f);
			}

	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(3);

		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		for (int i = 0; i < mySize; i++) {
			for (int j = 0; j < mySize; j++) {
				for (int k = 0; k < mySize; k++) {
					mat4 t = mat4(1.0);

					// Translate current cube to the center of face
					if (current_axis == 0)
						t = translate(t, vec3((i - 1)*1.0f, 0.0f, (k - 1)*1.0f));
					else if (current_axis == 1)
						t = translate(t, vec3(0.0f, (j - 1)*1.0f, (k - 1)*1.0f));
					else
						t = translate(t, vec3((i - 1)*1.0f, (j - 1)*1.0f, 0.0f));
					
					mat4 cube_rotation = rotations[cube_index[j][i][k]];
					myScene->setMainMat(t*cube_rotation);

					mat4 x = rotate(partial_rotations[cube_index[j][i][k]].x, vec3(1, 0, 0));
					mat4 y = rotate(partial_rotations[cube_index[j][i][k]].y, vec3(0, 1, 0));
					mat4 z = rotate(partial_rotations[cube_index[j][i][k]].z, vec3(0, 0, 1));

					// Translate to the right position in face
				
					if (current_axis == 0)
						t = translate(t, vec3(0.0f, (j - 1)*2.0f, 0.0f));
					else if (current_axis == 1)
						t = translate(t, vec3((i - 1)*2.0f, 0.0f, 0.0f));
					else
						t = translate(t, vec3(0.0f, 0.0f, (k - 1)*2.0f));

					myScene->setMainMat(z*y*x*t*myScene->getMainMat());

					myScene->muliplyMVP();

					shader.Bind();
					shader.Update(mat4(myScene->getMVP()), mat4(myScene->getMainMat()));

					myCube->Draw();
				}
			}
		}
		display.SwapBuffers();

		glfwPollEvents();
	}
	return 0;
}