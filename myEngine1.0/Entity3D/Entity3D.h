#pragma once
#include <iostream>
#include <string>
#include "../Renderer/EMath.h"
#include "../Renderer/RenderTypes.h"
#include "Vector3.h"

//#include "../Physics/RigidBody.h"
#include "../../ext/irrKlang/include/irrKlang.h"

namespace DoMaRe{
	class Renderer;
	class Timer;
	class Node;
	class Vector3;
	class MYENGINE_API Entity3D{

		friend class Node;

	public:
	public:
		Entity3D(float posX, float posY, std::string name);
		Entity3D();
		void SetPos(const Vector3& vPosition);//para nodo
		void SetPos(const float &x,const float &y, const float &z);//para nodo
		void SetPos(const float &x,const float &y);
		void setName(std::string s) { m_Name = s; }
		void SetScale(const Vector3& vScaling);
		void SetScale(const float &x,const float &y);
		void SetScale(const float &x,const float &y, const float&z);
		void SetScaleX(const float &x);
		void SetScaleY(const float &y);

		void Rotate(const Vector3& vRotation);
		void Rotate(const float &degree);

		Vector3 GetScale() { return m_vScale;}
		Vector3 GetPos(){return m_vPos;}
		Vector3 GetRot(){return m_vRot;}

		float GetRotationX();
		float GetRotationY();
		float GetRotationZ();   

		std::string GetName();

		friend class Scene;

		//------------------------------------

	protected:
		virtual void Draw(Renderer * pRenderer) = 0;
		virtual void Update(const double& dDeltaTime);
		virtual void OnUpdate(const double& dDeltaTime);

		std::string m_Name;
		bool m_bVisible;
		bool m_bCollisionEnabled;

		Vector3 m_vPos;
		Vector3 m_vScale;
		Vector3 m_vRot; 

	};
}