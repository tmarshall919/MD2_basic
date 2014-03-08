#pragma once



#define MD2VERTEXFVF D3DFVF_XYZ | D3DFVF_TEX1
struct MD2VertexFormat
{
	float x, y, z;
	float t, u;

	MD2VertexFormat()
	{
		x = y = z = t = u = 0.0f;
	}

	MD2VertexFormat( float X, float Y, float Z, float T, float U )
	{
		x = X;
		y = Y;
		z = Z;
		t = T;
		u = U;
	}
};

struct MD2Header
{
	int nIdentifier;
	int nVersion;
	
	int nSkinWidth;
	int nSkinHeight;
	int nFrameSize;

	int nNumSkins;
	int nNumVertices;
	int nNumUV;
	int nNumTriangles;
	int nNumCmds;
	int nNumFrames;

	int nOffsetSkins;
	int nOffsetUV;
	int nOffSetTriangles;
	int nOffsetFrames;
	int nOffsetCmds;
	int nOffsetEnd;
};

struct MD2Skin
{
	char szName[64];
};

struct MD2TexCoord
{
	short t;
	short u;
};

struct MD2Triangle
{
	short nVertex[3];
	short tu[3];
};

struct MD2Vertex
{
	float fVertex[3];
	float fNormal[3];
};

struct MD2Frame
{
	char szName[16];
	MD2Vertex* pVerts;
};

struct MD2VertexCompressed
{
	unsigned char nVertex[3];
	unsigned char nLightNormalIndex;
};

struct MD2FrameCompressed
{
	float fScale[3];
	float fTranslation[3];
	char szName[16];
	MD2VertexCompressed* pVerts;
};

class MD2Model
{
public:
	MD2Model();
	~MD2Model();

public:
	bool Load( const char* pszName );

public:
	MD2Header 		m_Header;
	MD2Skin*		m_pSkins;
	MD2TexCoord*	m_pTexCoords;
	MD2Triangle*	m_pTriangles;
	MD2Frame*		m_pFrames;

	LPDIRECT3DTEXTURE9		m_pTexture;
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer;
};