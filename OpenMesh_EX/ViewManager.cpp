#include "ViewManager.h"

using namespace glm;

/**
* 建立相機，並進行初始化。
*/
ViewManager::ViewManager()
{
	ortho = false;
	zoom = 1.0f;
	moveSpeed = 5.0f;
	lmbDown = false;
	midDown = false;
	eyePosition = vec3(0.0f, 0.0f, 1.0f);
	eyeLookPosition = vec3(0.05f, 0.0f, 0.0f);
	vec3 up = vec3(0, 1, 0);
	viewMatrix = lookAt(eyePosition, eyeLookPosition, up);
	rotationMatrix= mat4(1.0f);
	translationMatrix = mat4(1.0f);
	viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);
}

/**
* 取得Model Matrix。
*/
mat4 ViewManager::GetModelMatrix() {
	return translationMatrix * rotationMatrix;
	//return rotationMatrix;
}

/**
* 取得View Matrix。
*/
mat4 ViewManager::GetViewMatrix()
{	
	return viewMatrix;
}

/**
* 設定並取得Projection Matrix。
* @param aspect 畫面的長寬比。
*/
mat4 ViewManager::GetProjectionMatrix(float aspect)
{
	float nearVal;
	float farVal;
	nearVal = 0.1f;
	farVal = 1000.0f;
	if (ortho) {
		float size = 1.5f * zoom;
		projMatrix = glm::ortho(-aspect * size, aspect * size, -size, size, nearVal, farVal);
	}
	else {
		projMatrix = perspective(radians(30.0f * zoom), aspect, nearVal, farVal);
	}
	return projMatrix;
}

/**
* 取得Projection Matrix。
*/
mat4 ViewManager::GetProjectionMatrix()
{
	return GetProjectionMatrix(aspect);
}


/**
* 取得 V * P 的矩陣。
* @param aspect 畫面的長寬比。
*/
/*mat4 ViewManager::GetViewProjectionMatrix(float aspect)
{
	return GetProjectionMatrix(aspect) * viewMatrix;
}*/

/**
* 取得 M * V * P 的矩陣。
* @param aspect 畫面的長寬比。
*/
/*mat4 ViewManager::GetModelViewProjectionMatrix(float aspect)
{
	return GetViewProjectionMatrix(aspect) * GetModelMatrix();
}*/

/**
* 取得目前相機在世界座標的位置。
*/
/*vec3 ViewManager::GetWorldEyePosition() {
	vec4 wordEyePosition = vec4(eyePosition, 0) * GetModelMatrix();
	return wordEyePosition.xyz();
}*/

/**
* 取得目前相機在世界座標的所看的點位置。
*/
/*vec3 ViewManager::GetWorldViewVector() {
	vec4 wordLookVector = vec4(-viewVector, 0) * GetModelMatrix();
	return wordLookVector.xyz();
}*/

/**
* 處理當有按鍵輸入時，相機的動作。
* @param key 輸入的按鍵。
*/
void ViewManager::keyEvents(unsigned char key) {
	
	switch (key)
	{
		//向上移動。
	case 'w':
	case 'W':
		Translate(vec2(0, moveSpeed));
		break;

		//向左移動。
	case 'a':
	case 'A':
		Translate(vec2(moveSpeed, 0));
		std::cout << "a" << std::endl;
		break;

		//向下移動。
	case 's':
	case 'S':
		Translate(vec2(0, -moveSpeed));
		break;

		//向右移動。
	case 'd':
	case 'D':
		Translate(vec2(-moveSpeed, 0));
		break;

		//放大。
	case '+':
		wheelEvent(-moveSpeed);
		break;

		//縮小。
	case '-':
		wheelEvent(moveSpeed);
		break;
	default:
		break;
	}
}

/**
* 處理當有滑鼠事件時，相機的動作。
* @param button 輸入的按鍵。
* @param state 按鍵的狀態,點下,彈起。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
void ViewManager::mouseEvents(int button, int state, int x, int y) {
	//std::cout <<"button= "<<button << "state= " << state <<" x=" << x << "  y=" << y << std::endl;
	//if (state == GLUT_UP)
	//if (state == 0)
	//{
		//mouseReleaseEvent(button, x, y);
		
	//}
	//else if (state == GLUT_DOWN)
	if (state == 1)
	{
		mousePressEvent(button, x, y);
	}

	//處理滑鼠中鍵向上滾動時
	if (button == 4)
	{
		wheelEvent(1);
	}
	//處理滑鼠中鍵向下滾動時
	else if (button == 3)
	{
		wheelEvent(-1);
	}
}

/**
* 處理當滑鼠按鍵點下時的動作。
* @param button 滑鼠的按鍵。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
void ViewManager::mousePressEvent(int button, int x, int y)
{
	//std::cout <<"mousePressEvent:"<< "button= " << button  << " x=" << x << "  y=" << y << std::endl;
	//if (button == GLUT_LEFT_BUTTON)
	if (button == 0)
	{
		//紀錄現在左鍵被按住
		lmbDown = true;
		lmbDownCoord = vec2(x, y);
		mat4 invrtRot = inverse(rotationMatrix);
		//std::cout << "rotationMatrix" << glm::to_string(rotationMatrix).c_str() << std::endl;
		rotateYAxis = (invrtRot * vec4(0, 1, 0, 0)).xyz();
		//std::cout <<"rotateYAxis"<< glm::to_string(rotateYAxis).c_str() << std::endl;
		rotateXAxis = (invrtRot * vec4(1, 0, 0, 0)).xyz();
		//std::cout << "rotateYAxis" << glm::to_string(rotateXAxis).c_str() << std::endl;
		//std::cout << "LEFT_BUTTON" << std::endl;
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		//紀錄現在中鍵被按住
		midDown = true;
		midDownCoord = vec2(x, y);
	}
}

/**
* 處理當滑鼠按鍵彈起時的動作。
* @param button 滑鼠的按鍵。
* @param x 輸入的時, 滑鼠在畫面的x座標值。
* @param y 輸入的時, 滑鼠在畫面的y座標值。
*/
/*void ViewManager::mouseReleaseEvent(int button, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		lmbDown = false;
	}
	else if (button == GLUT_MIDDLE_BUTTON || button == 3 || button == 4) {
		midDown = false;
	}
}*/

/**
* 處理當滑鼠移動時的動作。
* @param x 滑鼠在畫面的x座標值。
* @param y 滑鼠在畫面的y座標值。
*/
void ViewManager::mouseMoveEvent(int x, int y)
{
	if (lmbDown)
	{
		//std::cout << "mouseMoveEvent:"  << " x=" << x << "  y=" << y << std::endl;
		/*
		* 當滑鼠左鍵按住,進行拖曳時的時候
		* 計算移動的向量,進行相機的旋轉
		*/
		vec2 coord = vec2(x, y);
		vec2 diff = coord - lmbDownCoord;
		float factor = 0.002f;
		rotationMatrix = rotate(rotationMatrix, diff.x * factor, rotateYAxis);
		rotationMatrix = rotate(rotationMatrix, diff.y * factor, rotateXAxis);
		//std::cout << "rotationMatrix"<< glm::to_string(rotationMatrix).c_str() << std::endl;
		lmbDownCoord = coord;
	}
	else if (midDown)
	{
		vec2 coord = vec2(x, y);
		vec2 diff = coord - midDownCoord;

		vec4 up = vec4(0, 1, 0, 0);
		vec4 right = vec4(1, 0, 0, 0);

		vec3 diffUp = up.xyz() * diff.y / (float)w_height;
		vec3 diffRight = right.xyz() * diff.x / (float)w_width;

		translationMatrix = translate(translationMatrix, (-diffUp + diffRight) * zoom * 3.0f);
		midDownCoord = coord;
	}
}

/**
* 根據中鍵的滾動方向處理事件。
* @param direction 前滾,後滾。
*/
void ViewManager::wheelEvent(int direction)
{
	wheel_val = direction * 15.0f*10.0f;
	Zoom(wheel_val / 120.0f);
}

/**
* 根據輸入的值,調整目前的縮放值。
* @param distance 增加的值。
*/
void ViewManager::Zoom(float distance)
{
	zoom *= (1.0f + 0.05f * distance*10.0f);
	zoom = clamp(0.0001f, zoom, 3.0f);
}

/**
* 告訴相機現在的螢幕大小。
* @param width 螢幕的寬。
* @param height 螢幕的高。
*/
/*void ViewManager::SetWindowSize(int width, int height) {
	w_width = width;
	w_height = height;
	projMatrix = GetProjectionMatrix();
	aspect = height / width;
}*/


/**
* 以尤拉角設定相機的旋轉。
* @param theta 尤拉角的theta旋轉。
* @param phi 尤拉角的phi旋轉。
*/
/*void ViewManager::SetRotation(float theta, float phi)
{
	rotationMatrix = mat4(1.0);
	rotationMatrix = rotate(rotationMatrix, theta, vec3(0, 1, 0));
	rotationMatrix = rotate(rotationMatrix, phi, vec3(1, 0, 0));
}*/

/**
* 以尤拉角設定相機的旋轉。
* @param x 尤拉角的x值。
* @param y 尤拉角的y值。
* @param z 尤拉角的z值。
*/
/*void ViewManager::SetRotation(float x, float y, float z)
{
	vec3 v(x, y, z);
	v = normalize(v);
	vec3 o(0, 0, 1);
	double angle = acos(dot(v, o));
	rotationMatrix = mat4(1.0);
	rotationMatrix = rotate(rotationMatrix, (float)angle, cross(o, v));
}*/

/**
* 重設相機的設定。
*/
/*void ViewManager::Reset()
{
	wheel_val = 0.0f;
	zoom = 1.0f;
	translationMatrix = mat4(1.0);
	rotationMatrix = mat4(1.0);
}*/

/**
* 使相機移動。
* @param vec 使相機移動vec單位。
*/
void ViewManager::Translate(vec2 vec) {
	vec2 diff = vec;
	float w_height = 600;
	float w_width = 800;
	vec4 up = vec4(0, 1, 0, 0);
	vec4 right = vec4(1, 0, 0, 0);

	vec3 diffUp = up.xyz() * diff.y / (float)w_height;
	vec3 diffRight = right.xyz() * diff.x / (float)w_width;

	translationMatrix = translate(translationMatrix, (-diffUp + diffRight) * zoom * 3.0f);
}