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

	//��ȡ��ƽ��
	glm::vec4 GetViewPlane()const;
	//��ȡĳ�㵽��ƽ��ľ���
	float GetDistanceToViewPlane(glm::vec3 point)const;
#pragma endregion

	//��������ת��Ϊ��������
	glm::vec3 WorldToScreen(const glm::vec3& world)const;
	//��������ת��Ϊ��������
	glm::vec3 ScreenToWorld(const glm::vec3& screen)const;
	//Ĭ�����Ϊ��ƽ��
	glm::vec3 ScreenToWorld(const glm::vec2& screen)const;
	//�ƶ����
	void MoveView(glm::vec3 offset);
	//��X����ת���
	void RotateViewX(float angle);
	//��Y����ת���
	void RotateViewY(float angle);
	//��Z����ת���
	void RotateViewZ(float angle);
	//��ĳ��ĳ����ת���
	void RotateViewByCenter(float angle, glm::vec3  pos, glm::vec3 axis);
	//��ĳ��X����ת���
	void RotateViewXByCenter(float angle, glm::vec3  pos);
	//��ĳ��Y����ת���
	void RotateViewYByCenter(float angle, glm::vec3  pos);
	//��ĳ��Z����ת���
	void RotateViewZByCenter(float angle, glm::vec3  pos);
	//��������
	void ScaleCameraByPos(const glm::vec3& pos, float present);
	//���¼�ӱ���
	void UpdateData();

	//ֱ�ӱ���
private:
	glm::vec3		m_Eye;				//�����λ��
	glm::vec3       m_Right;			//�ҷ���
	glm::vec3       m_Front;			//ǰ����
	glm::vec2		m_Size;				//�ü���С
	glm::vec4       m_ViewPort;			//��ʾ��С
	glm::vec3		m_RotateCenter;		//�����ת����
public:
	Sphere			m_ModelSphere;		//ģ�Ͱ�Χ��

	//��ӱ���
private:
	glm::vec3       m_Up;				//�Ϸ���
	glm::mat4		m_ModelMatrix;		//ģ�;���
	glm::mat4		m_ViewMatrix;		//��ͼ����
	glm::mat4		m_ProjectionMatrix;	//ͶӰ����

};

