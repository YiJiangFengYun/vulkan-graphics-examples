#include "graphics/scene/camera_3.hpp"

namespace kgs
{
	const float Camera3::DEFAULT_FOVY = glm::radians(45.0f);
	const float Camera3::DEFAULT_ASPECT = 1.0f;
	const float Camera3::DEFAULT_Z_NEAR = 0.1f;
	const float Camera3::DEFAULT_Z_FAR = 10.0f;
	Camera3::Camera3()
		:Camera<SpaceType::SPACE_3>()
		, m_fovy(DEFAULT_FOVY)
		, m_aspect(DEFAULT_ASPECT)
		, m_zNear(DEFAULT_Z_NEAR)
		, m_zFar(DEFAULT_Z_FAR)
	{
		_apply();
	}

	void Camera3::updateProj(float fovy, float aspect, float zNear, float zFar)
	{
		m_fovy = fovy;
		m_aspect = aspect;
		m_zNear = zNear;
		m_zFar = zFar;
		_apply();
	}

	void Camera3::apply()
	{
		_apply();
	}

	float Camera3::getFovY() const
	{
		return m_fovy;
	}

	void Camera3::setFovY(float fovy)
	{
		m_fovy = fovy;
	}

	float Camera3::getAspect() const
	{
		return m_aspect;
	}

	void Camera3::setAspect(float aspect)
	{
		m_aspect = aspect;
	}

	float Camera3::getZNear() const
	{
		return m_zNear;
	}

	void Camera3::setZNear(float zNear)
	{
		m_zNear = zNear;
	}

	float Camera3::getZFar() const
	{
		return m_zFar;
	}

	void Camera3::setZFar(float zFar)
	{
		m_zFar = zFar;
	}

	void Camera3::_apply()
	{
		m_projMatrix = glm::perspective(m_fovy, m_aspect, m_zNear, m_zFar);
	}


} //namespace kgs