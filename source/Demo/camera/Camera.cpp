#include "StdAfx.h"
#include "camera.h"
#include "MathTool.h"
#include <glm\gtx\transform.hpp>

Camera::Camera()
{
}

Camera::Camera(const glm::vec3& eye , const glm::vec3& right, const glm::vec3& front, const glm::vec2& size, const Sphere& modelSphere, const glm::vec4& viewPort)
	: m_Eye(eye)
	, m_Right(glm::normalize(right))
	, m_Front(glm::normalize(front))
	, m_ModelSphere(modelSphere)
	, m_ViewPort(viewPort)
{
	if (size.x / size.y > m_ViewPort.z / m_ViewPort.w)
	{
		m_Size.x = size.x;
		m_Size.y = m_Size.x* m_ViewPort.w / m_ViewPort.z;
	}
	else
	{
		m_Size.y = size.y;
		m_Size.x = m_Size.y * m_ViewPort.z / m_ViewPort.w;
	}
}

Camera::~Camera()
{
}

const glm::vec3 & Camera::GetEye() const
{
	return m_Eye;
}

void Camera::SetEye(const glm::vec3 & val)
{
	m_Eye = val;
}

const glm::vec2 & Camera::GetSize() const
{
	return m_Size;
}

const glm::vec3 & Camera::GetFront() const
{
	return  m_Front;
}

void Camera::SetFront(const glm::vec3 & val)
{
	m_Front = glm::normalize(val);
}

const glm::vec3 & Camera::GetRight() const
{
	return m_Right;
}

void Camera::SetRight(const glm::vec3 & val)
{
	m_Right = glm::normalize(val);
}

const glm::vec4 & Camera::GetViewPort() const
{
	return  m_ViewPort;
}

glm::vec3 Camera::GetRotateCenter() const
{
	return m_RotateCenter;
}

void Camera::SetRotateCenter(const glm::vec3 & val)
{
	m_RotateCenter = val;
}

void Camera::SetViewPort(const glm::vec4 & viewPort)
{
	m_ViewPort = viewPort;
}

const glm::vec3 & Camera::GetUp() const
{
	return m_Up;
}

const glm::mat4 & Camera::GetViewMatrix() const
{
	return  m_ViewMatrix;
}

const glm::mat4 & Camera::GetProjectionMatrix() const
{
	return  m_ProjectionMatrix;
}

glm::vec4 Camera::GetViewPlane() const
{
	return glm::vec4(m_Front, -glm::dot(m_Front, m_Eye));
}

float Camera::GetDistanceToViewPlane(glm::vec3 point) const
{
	glm::vec4 plane = GetViewPlane();
	//球心到视平面的距离
	return glm::dot(glm::vec4(point, 1.0f), plane) / glm::length(glm::vec3(plane.xyz));
}

glm::vec3 Camera::WorldToScreen(const glm::vec3 & world) const
{
	return glm::project(world, m_ViewMatrix, m_ProjectionMatrix, glm::vec4(m_ViewPort.x, m_ViewPort.y, m_ViewPort.z, m_ViewPort.w));
}

glm::vec3 Camera::ScreenToWorld(const glm::vec3 & screen) const
{
	return glm::unProject(screen, m_ViewMatrix, m_ProjectionMatrix, glm::vec4(m_ViewPort.x, m_ViewPort.y, m_ViewPort.z, m_ViewPort.w));
}

glm::vec3 Camera::ScreenToWorld(const glm::vec2 & screen)const
{
	glm::vec4 plane = GetViewPlane();
	glm::mat4 pMat = MathTool::ProjectionMatrixToPlane(plane);
	glm::vec3 nearPoint = ScreenToWorld(glm::vec3(screen, 0.0f));
	return (pMat*glm::vec4(nearPoint, 1.0f)).xyz;
}

void Camera::MoveView(glm::vec3 offset)
{
	m_Eye += offset;
}

void Camera::RotateViewX(float angle)
{
	UpdateData();
	glm::mat4	mat = glm::rotate(angle, m_Right);
	m_Front = (mat * glm::vec4(m_Front, 1.0)).xyz;
	m_Right = (mat * glm::vec4(m_Right, 1.0)).xyz;
	UpdateData();
}

void Camera::RotateViewY(float angle)
{
	UpdateData();
	glm::mat4	mat = glm::rotate(angle, m_Up);
	m_Front = (mat * glm::vec4(m_Front, 1.0)).xyz;
	m_Right = (mat * glm::vec4(m_Right, 1.0)).xyz;
	UpdateData();
}

void Camera::RotateViewZ(float angle)
{
	UpdateData();
	glm::mat4	mat = glm::rotate(angle, m_Front);
	m_Front = (mat * glm::vec4(m_Front, 1.0)).xyz;
	m_Right = (mat * glm::vec4(m_Right, 1.0)).xyz;
	UpdateData();
}

void Camera::RotateViewByCenter(float angle, glm::vec3 pos, glm::vec3 axis)
{
	//如果给定旋转点与摄像机位置重合，特殊处理一下
	if (glm::distance(pos, m_Eye) < 1e-6)
	{
		return;
	}
	UpdateData();
	glm::mat4 mat = glm::rotate(angle, axis);
	//计算眼睛到鼠标点的方向
	glm::vec3  vFront = pos - m_Eye;
	//计算旋转点和眼睛之间的距离
	float   len = glm::length(vFront);
	//旋转点和眼睛直接的方向
	vFront = glm::normalize(vFront);
	vFront = (mat * glm::vec4(vFront, 1.0)).xyz;
	//推导出眼睛的位置
	m_Eye = pos - vFront * len;
	m_Front = (mat * glm::vec4(m_Front, 1.0)).xyz;
	m_Right = (mat * glm::vec4(m_Right, 1.0)).xyz;
	UpdateData();
}

void Camera::RotateViewXByCenter(float angle, glm::vec3 pos)
{
	RotateViewByCenter(angle, pos, m_Right);
}

void Camera::RotateViewYByCenter(float angle, glm::vec3 pos)
{
	RotateViewByCenter(angle, pos, glm::vec3(0, 1, 0));
}

void Camera::RotateViewZByCenter(float angle, glm::vec3 pos)
{
	RotateViewByCenter(angle, pos, glm::vec3(0, 0, 1));
}

void Camera::ScaleCameraByPos(const glm::vec3 & pos, float present)
{
	m_Size *= present;
	glm::vec3 offset = (pos - m_Front*glm::dot(pos, m_Front) - m_Eye)*(1 - present);
	m_Eye += offset;
	UpdateData();
}

void Camera::UpdateData()
{
	//保证裁剪范围与屏幕同比例
	m_Size.y = m_Size.x / (m_ViewPort.z / m_ViewPort.w);
	//_size.x = _size.y * (_viewPort.z / _viewPort.w);

	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	m_ViewMatrix = glm::lookAt(m_Eye, m_Eye + m_Front, m_Up);

	//球心到视平面的距离
	float midDistance = abs(GetDistanceToViewPlane(m_ModelSphere.center));

	//_matProjection = glm::ortho(-_size.x / 2, _size.x / 2, -_size.y / 2, _size.y / 2);
	//近、远裁剪面分别为球心到视平面的距离±球半径
	m_ProjectionMatrix = glm::ortho(-m_Size.x / 2, m_Size.x / 2, -m_Size.y / 2, m_Size.y / 2, -midDistance - m_ModelSphere.radius, midDistance + m_ModelSphere.radius);
}
