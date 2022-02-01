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
	float aspect;					///< �x�s�ثe���������e��C
	bool ortho;						///< �O�_�ϥΥ�������C
	float zoom;
	float moveSpeed;				///< �۾������ʳt�סC

	glm::mat4 translationMatrix;	///< ����Translate�ʧ@��Matrix�C
	glm::mat4 rotationMatrix;		///< ����Rotation�ʧ@��Matrix�C
	glm::mat4 viewMatrix;			///< ����ViewMatrix�C
	glm::mat4 projMatrix;			///< ����projMatrix�C
	glm::vec3 viewVector;			///< �����۾��ݩ��J�I�ݪ��V�q�C
	glm::vec3 rotateXAxis;			///< �����۾���X�b����C
	glm::vec3 rotateYAxis;			///< �����۾���Y�b����C
	glm::vec3 eyePosition;			///< �����۾�����m�C
	glm::vec3 eyeLookPosition;		///< �����۾����Ҭݪ���m�C

	bool lmbDown;					///< �����ƹ�����O�_�Q����C
	bool midDown;					///< �����ƹ�����O�_�Q����C
	glm::vec2 lmbDownCoord;			///< �����ƹ������I���ɪ��y�СC
	glm::vec2 midDownCoord;			///< �����ƹ������I���ɪ��y�СC

	int w_width;					///< �����ù����e�C
	int w_height;					///< �����ù������C
	float wheel_val;				///< �����u�����ȡC
};

#endif // _VIEWMANAGER_H_
