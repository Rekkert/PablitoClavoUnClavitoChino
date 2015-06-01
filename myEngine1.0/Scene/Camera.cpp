﻿#include "Camera.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/AABB.h"
#include "../Entity3D/Entity3D.h"
using namespace DoMaRe;

Camera::Camera()
	:
	m_Position(0.0f,0.0f,0.0f), //Set position to 0,0,0
	m_LookAt(0.0f,0.0f,1.0f), //Set look at to 0,0,1
	m_Right(1.0f,0.0f,0.0f), //Set right to 1,0,0
	m_Up(0.0f,1.0f,0.0f), //Set up to 0,1,0
	m_RotateAroundUp(0.0f),
	m_RotateAroundRight(0.0f),
	m_RotateAroundLookAt(0.0f)
{
	/*for(unsigned int i=0; i < 6; i++)
		m_FrustumPlane[i] = new D3DXPLANE();
		*/
	D3DXMatrixIdentity(&m_MatView);
	//BuildFrustum();
}
//----------------------------------------------------------------
bool Camera::Init(Renderer* pkRenderer){
	m_pkRenderer = pkRenderer;
	BuildFrustum();
	return true;
}
Camera::~Camera()
{
	// Delete Frustum Planes...
	/*for(unsigned int i=0; i < 6; i++){
		delete m_FrustumPlane[i];
		m_FrustumPlane[i] = NULL;
	}*/
}
//----------------------------------------------------------------
void Camera::SetPosition(float fX, float fY, float fZ)
{
	m_Position = D3DXVECTOR3(fX, fY, fZ);
	m_bChanged= true;
}
//----------------------------------------------------------------
void Camera::MoveForward(float Dist)
{
	m_Position += Dist*m_LookAt;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveRight(float Dist)
{
	m_Position += Dist*m_Right;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveUp(float Dist)
{
	m_Position += Dist*m_Up;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::MoveInDirection(float Dist, float fDirectionX, float fDirectionY, float fDirectionZ)
{
	m_Position += Dist*D3DXVECTOR3(fDirectionX, fDirectionY, fDirectionZ);
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::RotateDown(float Angle)
{
	m_RotateAroundRight = Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::RotateRight(float Angle)
{
	m_RotateAroundUp = Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Roll(float Angle)
{
	m_RotateAroundLookAt += Angle;
	m_bChanged = true;
}
//----------------------------------------------------------------
void Camera::Update()
{
	if(m_bChanged)
	{
		//Matrices to store the transformations about our axes
		D3DXMATRIX MatTotal;
		D3DXMATRIX MatRotateAroundRight;
		D3DXMATRIX MatRotateAroundUp;
		D3DXMATRIX MatRotateAroundLookAt;
		//Get the matrix for each rotation
		D3DXMatrixRotationAxis(&MatRotateAroundRight, &m_Right, m_RotateAroundRight);
		D3DXMatrixRotationAxis(&MatRotateAroundUp, &m_Up, m_RotateAroundUp);
		D3DXMatrixRotationAxis(&MatRotateAroundLookAt, &m_LookAt, m_RotateAroundLookAt);
		//Combine the transformations into one matrix
		D3DXMatrixMultiply(&MatTotal, &MatRotateAroundUp, &MatRotateAroundRight);
		D3DXMatrixMultiply(&MatTotal, & MatRotateAroundLookAt, & MatTotal);
		//Transforms two vectors by our matrix and computes the third by
		//cross product
		D3DXVec3TransformCoord(&m_Right, &m_Right, &MatTotal);
		D3DXVec3TransformCoord(&m_Up, &m_Up, &MatTotal);
		D3DXVec3Cross(&m_LookAt, &m_Right, &m_Up);
		//Check to ensure vectors are perpendicular
		if (fabs(D3DXVec3Dot(&m_Up, &m_Right)) > 0.01)
		{
			//If theyre not
			D3DXVec3Cross(&m_Up, &m_LookAt, &m_Right);
		}
		//Normalize our vectors
		D3DXVec3Normalize(&m_Right, &m_Right);
		D3DXVec3Normalize(&m_Up, &m_Up);
		D3DXVec3Normalize(&m_LookAt, &m_LookAt);
		//Compute the bottom row of the view matrix
		float fView41,fView42,fView43;
		fView41 = -D3DXVec3Dot(&m_Right, &m_Position);
		fView42 = -D3DXVec3Dot(&m_Up, &m_Position);
		fView43 = -D3DXVec3Dot(&m_LookAt, &m_Position);
		//Fill in the view matrix
		m_MatView = D3DXMATRIX(m_Right.x, m_Up.x, m_LookAt.x, 0.0f,
			m_Right.y, m_Up.y, m_LookAt.y, 0.0f,
			m_Right.z, m_Up.z, m_LookAt.z, 0.0f,
			fView41, fView42, fView43, 1.0f);
		//BuildFrustum();
	}
	//Set view transform
	m_pkRenderer->loadIdentity();
	m_pkRenderer->setTransformMatrix(&m_MatView);
	//Reset update members
	m_RotateAroundRight = m_RotateAroundUp = m_RotateAroundLookAt = 0.0f;
	m_bChanged = false;
}

void Camera::BuildFrustum(){
	/*
	D3DXMATRIX FrustumProjectionMatrix;
	D3DXMatrixMultiply( &FrustumProjectionMatrix, &m_MatView, m_pkRenderer->projectionMatrix() );

	// left plane
	m_FrustumPlane[0]->a = FrustumProjectionMatrix._14 + FrustumProjectionMatrix._11;
	m_FrustumPlane[0]->b = FrustumProjectionMatrix._24 + FrustumProjectionMatrix._21;
	m_FrustumPlane[0]->c = FrustumProjectionMatrix._34 + FrustumProjectionMatrix._31;
	m_FrustumPlane[0]->d = FrustumProjectionMatrix._44 + FrustumProjectionMatrix._41;

	// right plane
	m_FrustumPlane[1]->a = FrustumProjectionMatrix._14 - FrustumProjectionMatrix._11;
	m_FrustumPlane[1]->b = FrustumProjectionMatrix._24 - FrustumProjectionMatrix._21;
	m_FrustumPlane[1]->c = FrustumProjectionMatrix._34 - FrustumProjectionMatrix._31;
	m_FrustumPlane[1]->d = FrustumProjectionMatrix._44 - FrustumProjectionMatrix._41;

	// top plane
	m_FrustumPlane[2]->a = FrustumProjectionMatrix._14 - FrustumProjectionMatrix._12;
	m_FrustumPlane[2]->b = FrustumProjectionMatrix._24 - FrustumProjectionMatrix._22;
	m_FrustumPlane[2]->c = FrustumProjectionMatrix._34 - FrustumProjectionMatrix._32;
	m_FrustumPlane[2]->d = FrustumProjectionMatrix._44 - FrustumProjectionMatrix._42;

	// bottom plane
	m_FrustumPlane[3]->a = FrustumProjectionMatrix._14 + FrustumProjectionMatrix._12;
	m_FrustumPlane[3]->b = FrustumProjectionMatrix._24 + FrustumProjectionMatrix._22;
	m_FrustumPlane[3]->c = FrustumProjectionMatrix._34 + FrustumProjectionMatrix._32;
	m_FrustumPlane[3]->d = FrustumProjectionMatrix._44 + FrustumProjectionMatrix._42;

	// near plane
	m_FrustumPlane[4]->a = FrustumProjectionMatrix._13;
	m_FrustumPlane[4]->b = FrustumProjectionMatrix._23;
	m_FrustumPlane[4]->c = FrustumProjectionMatrix._33;
	m_FrustumPlane[4]->d = FrustumProjectionMatrix._43;

	// far plane    
	m_FrustumPlane[5]->a = FrustumProjectionMatrix._14 - FrustumProjectionMatrix._13;
	m_FrustumPlane[5]->b = FrustumProjectionMatrix._24 - FrustumProjectionMatrix._23;
	m_FrustumPlane[5]->c = FrustumProjectionMatrix._34 - FrustumProjectionMatrix._33;
	m_FrustumPlane[5]->d = FrustumProjectionMatrix._44 - FrustumProjectionMatrix._43;

	// normalize planes
	for (unsigned int i=0; i<6; i++){
		D3DXPlaneNormalize( m_FrustumPlane[i], m_FrustumPlane[i] );
	}
	*/
}
/*int Camera::AABBinFrustum(Entity3D& pkNode){
	AABB& b = pkNode.aabb();
	D3DXVECTOR3 aabbSize = D3DXVECTOR3(b.width() * 0.5f,b.height() * 0.5f,b.depth() * 0.5f);
	D3DXVECTOR3 aabbCenter = D3DXVECTOR3(b.offset()->x + pkNode.transformationMatrix()->_41 ,b.offset()->y + pkNode.transformationMatrix()->_42 ,b.offset()->z + pkNode.transformationMatrix()->_43);

	int result = INSIDE;
	for(int i=0; i < 6; i++){
	Plane frustumPlane = m_FrustumPlane[i];
	float d = aabbCenter.x * frustumPlane->a +
	aabbCenter.y * frustumPlane->b +
	aabbCenter.z * frustumPlane->c;

	float r = aabbSize.x * std::abs(frustumPlane->a) +
	aabbSize.y * std::abs(frustumPlane->b) +
	aabbSize.z * std::abs(frustumPlane->c);

	float d_p_r = d + r;
	float d_m_r = d - r;

	if(d_p_r < -frustumPlane->d){
	result = OUTSIDE;
	break;
	}else if(d_m_r < -frustumPlane->d)
	result = INTERSECT;
	}

	return result;
} */