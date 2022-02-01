#pragma once
#ifndef _VIEWMANAGER_H_
#define _VIEWMANAGER_H_

#include "Common.h"

/**
 * @brief The ViewManager class
 * The ViewManager class provides viewing manipulation related functionalities.
 *
 * To use the ViewManager class, call mousePressEvent(),
 * mouseReleaseEvent(), mouseMoveEvent(), wheelEvent() in your event handlers
 * with the event objects.
 *
 * The viewing manipulation will be done for you in the class. When you are ready
 * to render something, call GetModelMatrix(), GetViewMatrix(), GetProjectionMatrix()
 * and their composite versions to get the MVP matrices which ecode current viewing
 * properties.
 */

class ViewManager
{
public:
	ViewManager();

	void mouseEvents(int button, int state, int x, int y);
	void mousePressEvent(int button, int x, int y);
	void mouseReleaseEvent(int button, int x, int y);
	void mouseMoveEvent(int x, int y);
	void keyEvents(unsigned char key);
	void wheelEvent(int direction);
	void Translate(glm::vec2 vec);

	glm::mat4 GetModelMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetProjectionMatrix(float aspect);
	glm::mat4 GetViewProjectionMatrix(float aspect);
	glm::mat4 GetModelViewProjectionMatrix(float aspect);
	glm::vec3 GetEyePosition() { return eyePosition; }
	glm::vec3 GetViewVector() { return viewVector; }

	glm::vec3 GetWorldEyePosition();
	glm::vec3 GetWorldViewVector();

	bool IsOrthoProjection() { return ortho; }

	void SetRotation(float theta, float phi);
	void SetRotation(float x, float y, float z);
	void SetWindowSize(int width, int height);

	bool ToggleOrtho() { return ortho = !ortho; }
	void Zoom(float distance);
	void Reset();

private:
	float aspect;					///< 儲存目前視窗的長寬比。
	bool ortho;						///< 是否使用正交視角。
	float zoom;
	float moveSpeed;				///< 相機的移動速度。

	glm::mat4 translationMatrix;	///< 紀錄Translate動作的Matrix。
	glm::mat4 rotationMatrix;		///< 紀錄Rotation動作的Matrix。
	glm::mat4 viewMatrix;			///< 紀錄ViewMatrix。
	glm::mat4 projMatrix;			///< 紀錄projMatrix。
	glm::vec3 viewVector;			///< 紀錄相機看往焦點看的向量。
	glm::vec3 rotateXAxis;			///< 紀錄相機的X軸旋轉。
	glm::vec3 rotateYAxis;			///< 紀錄相機的Y軸旋轉。
	glm::vec3 eyePosition;			///< 紀錄相機的位置。
	glm::vec3 eyeLookPosition;		///< 紀錄相機的所看的位置。

	bool lmbDown;					///< 紀錄滑鼠左鍵是否被按住。
	bool midDown;					///< 紀錄滑鼠中鍵是否被按住。
	glm::vec2 lmbDownCoord;			///< 紀錄滑鼠左鍵點擊時的座標。
	glm::vec2 midDownCoord;			///< 紀錄滑鼠中鍵點擊時的座標。

	int w_width;					///< 紀錄螢幕的寬。
	int w_height;					///< 紀錄螢幕的高。
	float wheel_val;				///< 紀錄滾輪的值。
};

#endif // _VIEWMANAGER_H_
