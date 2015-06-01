#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include "../VertexBuffer/VertexBuffer.h"

#include "../3DBuffers/pg2_indexbuffer.h"
#include "../3DBuffers/pg2_vertexbuffer.h"

#include "../myEngine_API.h"
#include "RenderTypes.h"
#include "Frustrum.h"
#include "EMath.h"
#include <d3d9.h>
#include <d3dx9.h>
namespace DoMaRe{
	class Material;
	class Frustrum;
	class MYENGINE_API Renderer{
	public:
		Renderer();
		~Renderer();
		bool Init(HWND _HwnD);
		void BeginFrame();
		void EndFrame();
		void SetCamera(D3DXVECTOR3 kViewerPos, D3DXVECTOR3 kLookPos, D3DXVECTOR3 kViewerUp);
		void SetCamera(D3DXMATRIX * matrix);
		void setCurrentVertexBuffer(VertexBuffer3D * p);
		void setCurrentIndexBuffer(IndexBuffer * p);
		void setMaterial(Material* pkMaterial = NULL);
		void loadIdentity();
		void setTransformMatrix(D3DXMATRIX * matrix);
		void setMatrix(MatrixType, const Matrix&);
		void setWireFrameMode(bool wF);
		bool getWireFrameMode(){ return wireFrameMode; }
		void Draw(ColorVertex*, DoMaRe::Primitive, size_t);
		void Draw(TexCoordVertex*, DoMaRe::Primitive, size_t);
		void Draw(DoMaRe::Primitive);
		void Clear();
		const Texture loadTexture (const std::string& Fname, int KeyCode);
		const Texture loadTexture (const std::string& Fname);
		void setCurrentTexture (const Texture& r_Texture);
		VertexBuffer3D* createVB(size_t vSize, unsigned int FVF);
		IndexBuffer* createIB();
		const Matrix&	projectionMatrix	() const;

		void setLight(D3DLIGHT9*, unsigned long);
		void enableLight(bool, unsigned long);

		void CalculateFrustrum(); 
		bool CheckFrustumCulling(D3DXVECTOR3* Box); 
	private:
		bool wireFrameMode;
		IDirect3D9  * d3d;
		IDirect3DDevice9  * d3d_dev;
		DoMaRe::VertexBuffer * p_vb;
		DoMaRe::VertexBuffer * p_vbT;

		DoMaRe::VertexBuffer3D * p_vb3D;
		DoMaRe::IndexBuffer * p_ib;

		Matrix	m_pkProjectionMatrix;
		Matrix  m_pkViewMatrix;

		std::vector<Texture> r_vTextures;

		Frustrum* m_Frustrum;
	};
}