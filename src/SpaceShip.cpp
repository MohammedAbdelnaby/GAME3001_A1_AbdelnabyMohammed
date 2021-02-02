#include "SpaceShip.h"
#include "Util.h"
#include "Game.h"


Spaceship::Spaceship():m_MaxSpeed(10.0f), m_Orientation(glm::vec2(0.0f,-1.0f)), m_RotationAngle(0.0f),m_accelerationRate(10.0f),m_turnRate(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/spaceship.png", "spaceship");
	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.f, 300.0f);
	getRigidBody()->velocity =glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SPACE_SHIP);
	

}

Spaceship::~Spaceship()
= default;
 
void Spaceship::draw()
{
	TextureManager::Instance()->draw("spaceship", getTransform()->position.x, getTransform()->position.y, m_RotationAngle, 255, true);
	/*Util::DrawLine(getTransform()->position,(getTransform()->position + getOrientation() * 70.0f));*/

	Util::DrawLine(m_leftWhisker.Start(), m_leftWhisker.End());
	Util::DrawLine(m_rightWhisker.Start(), m_rightWhisker.End());
	Util::DrawLine(m_middleWhisker.Start(), m_middleWhisker.End());
}

void Spaceship::update()
{
	m_leftWhisker.setLine(getTransform()->position, getTransform()->position + Util::getOrientation(m_RotationAngle + 30) * 100.0f);
	m_rightWhisker.setLine(getTransform()->position, getTransform()->position + Util::getOrientation(m_RotationAngle + -30) * 100.0f);
	m_middleWhisker.setLine(getTransform()->position, getTransform()->position + Util::getOrientation(m_RotationAngle) * 100.0f);
	m_Move();

}

void Spaceship::clean()
{
}
void Spaceship::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void Spaceship::setMaxSpeed(const float speed)
{
	m_MaxSpeed = speed;
}

glm::vec2 Spaceship::getOrientation() const
{
	return m_Orientation;
}

void Spaceship::setOrientation(const glm::vec2 Orientation)
{
	m_Orientation = Orientation;
}

void Spaceship::setRotation(const float Angle)
{
	m_RotationAngle = Angle;
	const auto angle_in_radians = (Angle - 90.0f) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);

	// convert the angle to a normalized vector
	setOrientation(glm::vec2(x, y));
}

float Spaceship::getRotation() const
{
	return m_RotationAngle;
}

float Spaceship::getTurnRate() const
{
	return m_turnRate;
}

void Spaceship::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float Spaceship::getAccelerationRate()const
{
	return m_accelerationRate;
}

void Spaceship::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}
void Spaceship::setTypeOfSteering(int i)
{
	m_typeOfSteering = i;
}




void Spaceship::m_Move()
{
	int Radius = 100.0f;
	float targetSpeed;
	float distance;
	auto deltalTime = TheGame::Instance()->getDeltaTime();
	m_TargetDirection = Util::normalize(m_TargetDirection);
	auto target_rotation = Util::signedAngle(getOrientation(), m_TargetDirection);
	auto turn_sensitivity = 5.0f;
	switch (m_typeOfSteering)
	{
	case 1:
		m_TargetDirection = m_destination - getTransform()->position;
		if (abs(target_rotation) > turn_sensitivity)
		{
			if (target_rotation > 0.0f)
			{
				setRotation(getRotation() + getTurnRate());
			}
			else if (target_rotation < 0.0f)
			{
				setRotation(getRotation() - getTurnRate());
			}
		}

		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
		// using the formula pf = pi + vi*t_0.5ai*t^2
		getRigidBody()->velocity += getOrientation() * (deltalTime)+0.5f * getRigidBody()->acceleration * (deltalTime);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_MaxSpeed);
		getTransform()->position += getRigidBody()->velocity;

		break;
	case 2:
		m_TargetDirection = getTransform()->position - m_destination;
		if (abs(target_rotation) > turn_sensitivity)
		{
			if (target_rotation > 0.0f)
			{
				setRotation(getRotation() + getTurnRate());
			}
			else if (target_rotation < 0.0f)
			{
				setRotation(getRotation() - getTurnRate());
			}
		}
		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
		// using the formula pf = pi + vi*t_0.5ai*t^2
		getRigidBody()->velocity += getOrientation() * (deltalTime)+0.5f * getRigidBody()->acceleration * (deltalTime);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_MaxSpeed);
		getTransform()->position += getRigidBody()->velocity;
		break;
	case 3:
		m_TargetDirection = m_destination - getTransform()->position;
		distance = Util::distance(getTransform()->position, m_destination);
		if (distance < 10)
		{
			SoundManager::Instance().load("../Assets/audio/pop.ogg", "pop", SOUND_SFX);
			SoundManager::Instance().setSoundVolume(100);
			SoundManager::Instance().playSound("pop",0,0);
			std::cout << "pop" << std::endl;
			m_typeOfSteering = 0;
			break;
		}
		if (distance > Radius)
		{
			targetSpeed = m_MaxSpeed;
		}
		else
		{
			targetSpeed = m_MaxSpeed * distance / Radius;
		}

		if (abs(target_rotation) > turn_sensitivity)
		{
			if (target_rotation > 0.0f)
			{
				setRotation(getRotation() + getTurnRate());
			}
			else if (target_rotation < 0.0f)
			{
				setRotation(getRotation() - getTurnRate());
			}
		}
		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
		// using the formula pf = pi + vi*t_0.5ai*t^2
		getRigidBody()->velocity += getOrientation()* (deltalTime)+0.5f * getRigidBody()->acceleration * (deltalTime);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, targetSpeed);
		getTransform()->position += getRigidBody()->velocity;
		std::cout << distance << " Max Speed " << targetSpeed << std::endl;
		break;
	case 4:

		if (m_middleFeel)
		{
			getRigidBody()->velocity *= 0.0f;
			setRotation(getRotation() + 30);
			m_middleFeel = false;
		}
		if (m_rightFeel)
		{
			getRigidBody()->velocity *= 0.0f;
			setRotation(getRotation() + 30);
			m_rightFeel = false;
		}
		if (m_leftFeel)
		{
			getRigidBody()->velocity *= 0.0f;
			setRotation(getRotation() - 30);
			m_leftFeel = false;
		}

		if (abs(target_rotation) > turn_sensitivity)
		{
			if (target_rotation > 0.0f)
			{
				setRotation(getRotation() + getTurnRate());
			}
			else if (target_rotation < 0.0f)
			{
				setRotation(getRotation() - getTurnRate());
			}
		}
		m_TargetDirection = m_destination - getTransform()->position;
		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
		// using the formula pf = pi + vi*t_0.5ai*t^2
		getRigidBody()->velocity += getOrientation() * (deltalTime)+0.5f * getRigidBody()->acceleration * (deltalTime);
		getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_MaxSpeed);
		getTransform()->position += getRigidBody()->velocity;
		break;
	default:
		break;
	}
}
