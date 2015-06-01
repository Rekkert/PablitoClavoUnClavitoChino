#include "RenderTypes.h"
#include <d3dx9.h>

const DWORD DoMaRe::ColorVertexType = (D3DFVF_XYZ | D3DFVF_DIFFUSE);
const int DoMaRe::TexCoordVertexType = (D3DFVF_XYZ | D3DFVF_TEX1);
const int DoMaRe::MeshVertexType	=  (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);