#include "Scene1.h"

#include "Entity3D\Entity3D.h"
#include "Entity3D\Mesh.h"
#include "Entity3D\Node.h"

#include "Physics\Collider.h"
#include "Physics\Physics.h"
#include "Physics\RigidBody.h"

#include <iostream>
#include <vector>
#include <math.h>

void Test2(DoMaRe::Entity3D* pk1, DoMaRe::Entity3D* pk2);

//#include "Sound\Sound.h"
using namespace MiJuego;
bool AnimEnable = true;
int Anim = 0;
float mSpeed = 0.1f;
DoMaRe::Node* Tank;
DoMaRe::Mesh* mesh;
bool Scene1::Init(DoMaRe::Import& Importer){

	pkNode = new DoMaRe::Node("Bones");

	mainLight = new DoMaRe::Light(&Importer.GetRenderer());
	mainLight->setLightType(DoMaRe::Light::DIRECTIONAL_LIGHT);
	mainLight->setPosition (0, 0, 0);
	mainLight->setDirection(0, 0, 1);
	mainLight->setAmbient (0, 0, 0, 0);
	mainLight->setSpecular(0, 0, 0, 0);
	mainLight->setLightIndex(0);
	mainLight->setRange(0.3f);
	mainLight->enable(true);

	//Importer.importScene("bones_all.x", *pkNode);
	Importer.importScene("tiny.x", *pkNode);
	pkNode->SetPos(0, 0, 0);
	//pkNode->SetScale(10, 10, 10); //Scale Bones
	pkNode->SetScale(0.1, 0.1, 0.1); //Scale Tiny

	mainCamera->SetPosition(0, 0, -20);

	Importer.GetSound().playSoundFile("sound.mp3",false);
	return true;
}

bool Scene1::Frame(DoMaRe::Renderer& renderer, DoMaRe::DirectInput& dInput, DoMaRe::Timer& timer, DoMaRe::Import& import, DoMaRe::Game& game, DoMaRe::Sound& pkSound){
	UpdateInputs(dInput,timer,pkSound,renderer);
	
	if(AnimEnable){
		switch (Anim){
		case 0:
			pkNode->PlayAnim("Idle");
			break;
		case 1:
			pkNode->PlayAnim("Move");
			break;
		case 2:
			pkNode->PlayAnim("Attack");
			break;
		case 3:
			pkNode->PlayAnim("Impact");
			break;
		case 4:
			pkNode->PlayAnim("Die");
			break;
		}
		pkNode->Update(timer.timeBetweenFrames());
	}

	mainLight->setLightIndex(0);
	mainLight->enable(true);

	return true;
}

bool Scene1::deInit(){
	delete mainLight;
	return true;
}

void Scene1::UpdateInputs(DoMaRe::DirectInput& dInput, DoMaRe::Timer& timer, DoMaRe::Sound& pkSound, DoMaRe::Renderer& renderer){
	/*if (dInput.keyDown(DoMaRe::Input::KEY_J)){
		cube->setRotation(cube->rotationX() - 1  , cube->rotationY() , cube->rotationZ() ); 
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_K)){
		cube->setRotation(cube->rotationX() + 1 , cube->rotationY() , cube->rotationZ() );
	}*/
	if (dInput.keyDown(DoMaRe::Input::KEY_1)){
		Anim = 0;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_2)){
		Anim = 1;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_3)){
		Anim = 2;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_4)){
		Anim = 3;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_5)){
		Anim = 4;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_M)){
		AnimEnable = !AnimEnable;
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_1)){
		pkSound.setMasterVolume(pkSound.getMasterVolume() - 0.1f);
	}

	if (dInput.keyDown(DoMaRe::Input::KEY_2)){
		pkSound.setMasterVolume(pkSound.getMasterVolume() + 0.1f);
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_F1)){
		renderer.setWireFrameMode(!renderer.getWireFrameMode());
	}
	if (dInput.keyDown(DoMaRe::Input::KEY_F2)){
		AnimEnable = !AnimEnable;
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_UP)){
		mainCamera->MoveForward(mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_DOWN)){
		mainCamera->MoveForward(-mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_LEFT)){
		mainCamera->MoveRight(-mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_RIGHT)){
		mainCamera->MoveRight(mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_D)){
		mainCamera->RotateRight(mSpeed / 100 * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_A)){
		mainCamera->RotateRight(-mSpeed  / 100 * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_W)){
		mainCamera->RotateDown(-mSpeed  / 100 * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_S)){
		mainCamera->RotateDown(mSpeed  / 100 * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_Q)){
		mainCamera->MoveUp(-mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_E)){
		mainCamera->MoveUp(mSpeed * timer.timeBetweenFrames());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_B)){
		mainLight->enable(!mainLight->isEnabled());
	}
	if(dInput.keyDown(DoMaRe::Input::KEY_P)){
		float newDT = timer.deltaTime() == 1.0f ? 0.0f : 1.0f;
		timer.setdeltaTime(newDT);
	}
}
