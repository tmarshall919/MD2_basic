#include "stdafx.h"
#include "MD2.h"

MD2Model::MD2Model()
{
}

MD2Model::~MD2Model()
{
}

bool MD2Model::Load( const char* pszName )
{
	FILE* pFile = NULL;

	fopen_s( &pFile, pszName, "rb" );
	if( !pFile )
		return false;
	
	/* Retrieve Header */
	fread( &m_Header, sizeof(MD2Header), 1, pFile );

	/* Allocate Pointers */
	m_pSkins = new MD2Skin[m_Header.nNumSkins];
	m_pTexCoords = new MD2TexCoord[m_Header.nNumUV];
	m_pTriangles = new MD2Triangle[m_Header.nNumTriangles];
	m_pFrames = new MD2Frame[m_Header.nNumFrames];

	/* Read Skins */
	fseek( pFile, m_Header.nOffsetSkins, SEEK_SET );
	fread( m_pSkins, sizeof(MD2Skin), m_Header.nNumSkins, pFile );

	/* Read Texture Coords */
	fseek( pFile, m_Header.nOffsetUV, SEEK_SET );
	fread( m_pTexCoords, sizeof(MD2TexCoord), m_Header.nNumUV, pFile );

	/* Read Faces */
	fseek( pFile, m_Header.nOffSetTriangles, SEEK_SET );
	fread( m_pTriangles, sizeof(MD2Triangle), m_Header.nNumTriangles, pFile );

	/* Read Animations */
	fseek( pFile, m_Header.nOffsetFrames, SEEK_SET );
	for( int i = 0; i < m_Header.nNumFrames; i++ )
	{
		MD2FrameCompressed* pCompressed = new MD2FrameCompressed();
		pCompressed->pVerts = new MD2VertexCompressed[m_Header.nNumVertices];
		m_pFrames[i].pVerts = new MD2Vertex[m_Header.nNumVertices];

		size_t nSize = ((sizeof(float) * 3) * 2) + 16;
		fread( pCompressed, 1, nSize, pFile );
		fread( pCompressed->pVerts, 1, (m_Header.nFrameSize - nSize), pFile );

		strcpy_s( m_pFrames[i].szName, pCompressed->szName );

		for( int j = 0; j < m_Header.nNumVertices; j++ )
		{
			m_pFrames[i].pVerts[j].fVertex[0] = pCompressed->pVerts[j].nVertex[0] *
				pCompressed->fScale[0] + pCompressed->fTranslation[0];

			m_pFrames[i].pVerts[j].fVertex[2] = -1 * (pCompressed->pVerts[j].nVertex[1] *
				pCompressed->fScale[1] + pCompressed->fTranslation[1]);

			m_pFrames[i].pVerts[j].fVertex[1] = pCompressed->pVerts[j].nVertex[2] *
				pCompressed->fScale[2] + pCompressed->fTranslation[2];
		}

		delete pCompressed;
	}

	return true;
}