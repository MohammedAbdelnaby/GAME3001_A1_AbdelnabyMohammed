#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Util.h"
#include "glm/gtx/string_cast.hpp"
#include "SoundManager.h"


// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	if (CollisionManager::lineRectCheck(m_pSpaceship->m_leftWhisker.Start(), m_pSpaceship->m_leftWhisker.End(), m_pObstacle->getTransform()->position, 200.0f, 100.0f))
	{
		/*std::cout << "HIT LEFT" << std::endl;*/
		m_pSpaceship->m_leftFeel = true;
	}
	if (CollisionManager::lineRectCheck(m_pSpaceship->m_rightWhisker.Start(), m_pSpaceship->m_rightWhisker.End(), m_pObstacle->getTransform()->position - glm::vec2(100.0f,50.0f), 200.0f, 100.0f))
	{
		/*std::cout << "HIT RIGHT" << std::endl;*/
		m_pSpaceship->m_rightFeel = true;
	}
	if (CollisionManager::lineRectCheck(m_pSpaceship->m_middleWhisker.Start(), m_pSpaceship->m_middleWhisker.End(), m_pObstacle->getTransform()->position - glm::vec2(100.0f, 50.0f), 200.0f, 100.0f))
	{
		/*std::cout << "HIT MIDDLE" << std::endl;*/
		m_pSpaceship->m_middleFeel = true;
		
		
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_GRAVE))
	{
		m_pInstructionsLabel->setEnabled(false);
	}
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}
// test
void PlayScene::start()
{
	const SDL_Color blue = { 0, 0, 255, 255 };
	// Set GUI Title
	m_guiTitle = "Play Scene";
	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(750.0f, 550.0f);
	addChild(m_pTarget);

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(400.0f, 300.0f);
	m_pObstacle->setEnabled(false);
	addChild(m_pObstacle);

	m_pSpaceship = new Spaceship();
	m_pSpaceship->getTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pSpaceship->setEnabled(false);
	m_pSpaceship->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pSpaceship);
	m_pInstructionsLabel = new Label("Press ' ` ' for Debug Menu", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	addChild(m_pInstructionsLabel);
	SoundManager::Instance().load("../Assets/audio/Elvis Presley.mp3", "Elvis Presley", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(5);
	SoundManager::Instance().playMusic("Elvis Presley", -1);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	




	ImGui::Begin("Debuger Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	static float float1 = 10.0f;
	if (ImGui::SliderFloat("MAXSPEED", &float1, 0.0f, 100.0f))
	{
		m_pSpaceship->setMaxSpeed(float1);
	}
	static float angleInRadians = 0.0f;
	if (ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceship->setRotation(angleInRadians * Util::Rad2Deg);
	}
	static float accelerationRate = 2.0f;
	if (ImGui::SliderFloat("Acceleration Rate", &accelerationRate, 0.0f, 50.0f))
	{
		m_pSpaceship->setAccelerationRate(accelerationRate);
	}
	static float turnRate = 5.0f;
	if (ImGui::SliderFloat("Turn Rate", &turnRate, 0.0f, 20.0f))
	{
		m_pSpaceship->setTurnRate(turnRate);
	}
	if(ImGui::Button("START"))
	{	
		m_pSpaceship->setEnabled(true);
		SoundManager::Instance().load("../Assets/audio/teleport.wav", "teleport", SOUND_SFX);
		SoundManager::Instance().setSoundVolume(25);
		SoundManager::Instance().playSound("teleport", 0, 0);
	}

	ImGui::SameLine();
	if (ImGui::Button("RESTART"))
	{
		m_pSpaceship->getTransform()->position = glm::vec2(100.0f, 100.0f);
		m_pSpaceship->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceship->setRotation(0.0f);
		m_pSpaceship->setEnabled(false);
		turnRate = 5.0f;
		accelerationRate = 2.0f;
		float1 = 10.0f;
		angleInRadians = m_pSpaceship->getRotation();
		m_pObstacle->setEnabled(false);
		m_pSpaceship->setTypeOfSteering(0);

		
	}
	ImGui::Separator();
	if (ImGui::Button("SEEK"))
	{
		m_pSpaceship->setTypeOfSteering(1);
	}
	ImGui::SameLine();
	if (ImGui::Button("FLEE"))
	{
		m_pSpaceship->setTypeOfSteering(2);
	}
	ImGui::SameLine();
	if (ImGui::Button("ARRIVE"))
	{
		m_pSpaceship->setTypeOfSteering(3);
	}
	ImGui::SameLine();
	if (ImGui::Button("OBSTACLE AVOIDANCE"))
	{
		m_pObstacle->setEnabled(true);
		m_pSpaceship->setTypeOfSteering(4);
	}

	ImGui::Separator();

	static float float2[2] = {m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("TARGET", float2, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(float2[0], float2[1]);
		m_pSpaceship->setDestination(m_pTarget->getTransform()->position);
	}
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
