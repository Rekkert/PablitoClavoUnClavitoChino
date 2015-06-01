#include "Mesh.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/RenderTypes.h"
#include "../Physics/Collider.h"
#include "../Physics/Physics.h"
#include "../Renderer/Material.h"
using namespace DoMaRe;

int Mesh::debugedMeshes = 0;
Mesh::Mesh(Renderer & p_Renderer) : Entity3D(),  pk_Renderer(p_Renderer) , s_Texture(NoTexture), pk_Material(Material::Default_Material), m_pvBB(new D3DXVECTOR3[8]){
	mk_VertexBuffer3D = pk_Renderer.createVB(sizeof(DoMaRe::MeshVertex), DoMaRe::MeshVertexType);
	mk_IndexBuffer = pk_Renderer.createIB();
}

Mesh::~Mesh(){

	m_pkVertex.clear();
	m_pkIndex.clear();

	if(mk_VertexBuffer3D){
		delete mk_VertexBuffer3D;
		mk_VertexBuffer3D = NULL;
	}
	if(mk_IndexBuffer){
		delete mk_IndexBuffer;
		mk_IndexBuffer = NULL;
	}
	if(pk_Material){
		delete pk_Material;
		pk_Material = NULL;
	}

	if(m_pvBB)
		delete m_pvBB;

}

void Mesh::setData(const MeshVertex* Tex_Vertex, size_t vertexCount, DoMaRe::Primitive Prim, const unsigned short* pInt, size_t indexCount){
	pkPrimitive = Prim;
	mk_VertexBuffer3D->setVertexData((void *)Tex_Vertex, vertexCount);
	mk_IndexBuffer->setIndexData(pInt,indexCount);

	m_iVertexCount = vertexCount;
	// Guardo Vertex
	m_pkVertex.resize(vertexCount);
	memcpy( &( m_pkVertex.front() ), Tex_Vertex, vertexCount * sizeof(MeshVertex) );


	// Guardo Index
	m_pkIndex.resize(indexCount);
	memcpy( &( m_pkIndex.front() ), pInt, indexCount * sizeof(unsigned short) );

	CalculateBB();
}

void Mesh::Draw(Renderer* pRenderer){
	mk_VertexBuffer3D->bind();
	mk_IndexBuffer->bind();
	pk_Renderer.setMaterial(pk_Material);
	pk_Renderer.setCurrentTexture(s_Texture);
	pk_Renderer.Draw(pkPrimitive);

	debugedMeshes++;
}

void Mesh::setTexture(std::string pkTextureFile, DWORD theColor){
	s_Texture = pk_Renderer.loadTexture(pkTextureFile,theColor);
}

void Mesh::setTexture(Texture& theTexture){
	s_Texture = theTexture;
}

const std::vector<MeshVertex>& Mesh::vertexs() const{
	return m_pkVertex;
}

const std::vector<unsigned short> Mesh::indexs() const{
	return m_pkIndex;
}

const Material& Mesh::getMaterial() const{
	return *pk_Material;
}

void Mesh::setMaterial(Material& m_cMaterial) {
	pk_Material = &m_cMaterial;
}

void Mesh::GetTransformedBox(D3DXMATRIX * pMatrizMundo, D3DXVECTOR3* pOut){
	for(int i = 0; i < 8;i++){
		D3DXVec3TransformCoord(&pOut[i],&m_pvBB[i],pMatrizMundo);
	}

} 

void Mesh::CalculateBB(){

	Vector3 v_MaxBound;

	Vector3 v_MinBound;

	v_MaxBound.x = m_pkVertex[0].x;
	v_MaxBound.y = m_pkVertex[0].y;
	v_MaxBound.z = m_pkVertex[0].z;

	v_MinBound.x = m_pkVertex[0].x;
	v_MinBound.y = m_pkVertex[0].y;
	v_MinBound.z = m_pkVertex[0].z;

	for(int i = 1; i < m_pkVertex.size();i++){
		if(m_pkVertex[i].x > v_MaxBound.x) 
			v_MaxBound.x = m_pkVertex[i].x;

		else if(m_pkVertex[i].x < v_MinBound.x)
			v_MinBound.x = m_pkVertex[i].x;
		
		if(m_pkVertex[i].y > v_MaxBound.y) 
			v_MaxBound.y = m_pkVertex[i].y;

		else if(m_pkVertex[i].y < v_MinBound.y)
			v_MinBound.y = m_pkVertex[i].y;

		if(m_pkVertex[i].z > v_MaxBound.z) 
			v_MaxBound.z = m_pkVertex[i].z;

		else if(m_pkVertex[i].z < v_MinBound.z)
			v_MinBound.z = m_pkVertex[i].z;

	}
	m_pvBB[0].x = v_MaxBound.x;
	m_pvBB[0].y = v_MaxBound.y;
	m_pvBB[0].z = v_MaxBound.z;

	m_pvBB[1].x = v_MaxBound.x;
	m_pvBB[1].y = v_MinBound.y;
	m_pvBB[1].z = v_MaxBound.z;

	m_pvBB[2].x = v_MinBound.x;
	m_pvBB[2].y = v_MinBound.y;
	m_pvBB[2].z = v_MaxBound.z;

	m_pvBB[3].x = v_MinBound.x;
	m_pvBB[3].y = v_MaxBound.y;
	m_pvBB[3].z = v_MaxBound.z;

	m_pvBB[4].x = v_MaxBound.x;
	m_pvBB[4].y = v_MaxBound.y;
	m_pvBB[4].z = v_MinBound.z;

	m_pvBB[5].x = v_MaxBound.x;
	m_pvBB[5].y = v_MinBound.y;
	m_pvBB[5].z = v_MinBound.z;

	m_pvBB[6].x = v_MinBound.x;
	m_pvBB[6].y = v_MinBound.y;
	m_pvBB[6].z = v_MinBound.z;

	m_pvBB[7].x = v_MinBound.x;
	m_pvBB[7].y = v_MaxBound.y;
	m_pvBB[7].z = v_MinBound.z;
}