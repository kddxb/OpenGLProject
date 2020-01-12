#pragma once
#include <GLM/glm.hpp>
#include "Sphere.h"

class Camera
{
public:
	Camera();

	Camera(
		const glm::vec3&	eye,
		const glm::vec3&	right,
		const glm::vec3&	front,
		const glm::vec2&	size,
		const Sphere&		modelSphere,
		const glm::vec4&	viewPort
	);
	virtual ~Camera();

#pragma region getter & setter
	const glm::vec3& GetEye() const;
	void SetEye(const glm::vec3& val);
	const glm::vec2& GetSize() const;
	const glm::vec3& GetFront() const;
	void SetFront(const glm::vec3& val);
	const glm::vec3& GetRight() const;
	void SetRight(const glm::vec3& val);
	const glm::vec4& GetViewPort()const;
	glm::vec3 GetRotateCenter()const;
	void SetRotateCenter(const glm::vec3& val);
	void SetViewPort(const glm::vec4& viewPort);
	const glm::vec3& GetUp() const;
	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const;

	//获取视平面
	glm::vec4 GetViewPlane()const;
	//获取某点到视平面的距离
	float GetDistanceToViewPlane(glm::vec3 point)const;
#pragma endregion

	//世界坐标转化为窗口坐标
	glm::vec3 WorldToScreen(const glm::vec3& world)const;
	//窗口坐标转化为世界坐标
	glm::vec3 ScreenToWorld(const glm::vec3& screen)const;
	//默认深度为视平面
	glm::vec3 ScreenToWorld(const glm::vec2& screen)const;
	//移动相机
	void MoveView(glm::vec3 offset);
	//绕X轴旋转相机
	void RotateViewX(float angle);
	//绕Y轴旋转相机
	void RotateViewY(float angle);
	//绕Z轴旋转相机
	void RotateViewZ(float angle);
	//绕某点某轴旋转相机
	void RotateViewByCenter(float angle, glm::vec3  pos, glm::vec3 axis);
	//绕某点X轴旋转相机
	void RotateViewXByCenter(float angle, glm::vec3  pos);
	//绕某点Y轴旋转相机
	void RotateViewYByCenter(float angle, glm::vec3  pos);
	//绕某点Z轴旋转相机
	void RotateViewZByCenter(float angle, glm::vec3  pos);
	//定点缩放
	void ScaleCameraByPos(const glm::vec3& pos, float present);
	//更新间接变量
	void UpdateData();

	//直接变量
private:
	glm::vec3		m_Eye;				//摄像机位置
	glm::vec3       m_Right;			//右方向
	glm::vec3       m_Front;			//前方向
	glm::vec2		m_Size;				//裁剪大小
	glm::vec4       m_ViewPort;			//显示大小
	glm::vec3		m_RotateCenter;		//相机旋转中心
public:
	Sphere			m_ModelSphere;		//模型包围球

	//间接变量
private:
	glm::vec3       m_Up;				//上方向
	glm::mat4		m_ModelMatrix;		//模型矩阵
	glm::mat4		m_ViewMatrix;		//视图矩阵
	glm::mat4		m_ProjectionMatrix;	//投影矩阵

};

