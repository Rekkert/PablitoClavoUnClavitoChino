#include "Scene2.h"
using namespace MiJuego;

bool Scene2::Init(DoMaRe::Import& Importer){
	//getEntity(&_Cubo1,"floor");
	getEntity(&_Sprite2,"player1");
	mainCamera = new DoMaRe::Camera();
	mainCamera->Init(&Importer.GetRenderer());
	return true;
}

bool Scene2::Frame(DoMaRe::Renderer& renderer, DoMaRe::DirectInput& dInput, DoMaRe::Timer& timer, DoMaRe::Import& import, DoMaRe::Game& game, DoMaRe::Sound& pkSound){
	if(dInput.keyDown(DoMaRe::Input::KEY_P)){
		game.setScene("main",import,"Archivo.xml");
		return true;
	}
	/*if(dInput.keyDown(DoMaRe::Input::KEY_DOWN)){
		_Cubo1->setPos(_Cubo1->posX(),_Cubo1->posY() - 1);
	}*/
	return true;
}

bool Scene2::deInit(){
	return true;
}