#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#include <string>
#pragma warning( default : 4996 )

struct _VERTEX
{
    D3DXVECTOR3 pos;     // vertex position
    D3DXVECTOR3 norm;    // vertex normal
    float tu;            // texture coordinates
    float tv;
};

#define FVF_VERTEX    D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
#define Time 10000.0f


//http://www.mvps.org/directx/articles/spheremap.htm
//D3DXCreateSphere�Լ��� ���� �޽��� uv��ǥ �����
LPD3DXMESH CreateMappedSphere(LPDIRECT3DDEVICE9 pDev, float fRad, UINT slices, UINT stacks)
{
    // create the sphere
    LPD3DXMESH mesh;
    if (FAILED(D3DXCreateSphere(pDev, fRad, slices, stacks, &mesh, NULL)))
        return NULL;

    // create a copy of the mesh with texture coordinates,
    // since the D3DX function doesn't include them
    LPD3DXMESH texMesh;
    if (FAILED(mesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_VERTEX, pDev, &texMesh)))
        // failed, return un-textured mesh
        return mesh;

    // finished with the original mesh, release it
    mesh->Release();

    // lock the vertex buffer
    _VERTEX* pVerts;
    if (SUCCEEDED(texMesh->LockVertexBuffer(0, (void**)&pVerts))) {

        // get vertex count
        int numVerts = texMesh->GetNumVertices();

        // loop through the vertices
        for (int i = 0; i < numVerts; i++) {

            // calculate texture coordinates
            pVerts->tu = asinf(pVerts->norm.x) / D3DX_PI + 0.5f;
            pVerts->tv = asinf(pVerts->norm.y) / D3DX_PI + 0.5f;

            // go to next vertex
            pVerts++;
        }

        // unlock the vertex buffer
        texMesh->UnlockVertexBuffer();
    }

    // return pointer to caller
    return texMesh;
}

class Planet
{
public:
    D3DXVECTOR3 Pos;//��ġ
    FLOAT RotSpeed; //�����ӵ�
    FLOAT RevSpeed; //�����ӵ�
    FLOAT RotAngle; //������
    FLOAT RevAngle; //������

    Planet();
    ~Planet();

    /// <summary>
    /// �ʱ�ȭ
    /// </summary>
    /// <param name="pDev"></param>
    /// <param name="mtrl"></param>
    /// <param name="radian">���� ������</param>
    /// <param name="slices"></param>
    /// <param name="stacks"></param>
    /// <param name="texturename">�ؽ����̸�</param>
    /// <param name="pos">��ġ</param>
    /// <param name="rotspeed">�����ӵ�</param>
    /// <param name="revspeed">�����ӵ�</param>
    /// <param name="rotangle">������</param>
    /// <param name="revangle">������</param>
    /// <returns></returns>
    HRESULT Init(LPDIRECT3DDEVICE9 pDev, D3DMATERIAL9 mtrl, FLOAT radian, UINT slices, UINT stacks, LPCWSTR texturename,
        D3DXVECTOR3 pos, FLOAT rotspeed, FLOAT revspeed, FLOAT rotangle, FLOAT revangle);
    
    /// <summary>
    /// �׸���
    /// </summary>
    /// <param name="pDev"></param>
    /// <param name="matPar">�θ��༺ ��Ʈ����</param>
    /// <returns></returns>
    D3DXMATRIX Draw(LPDIRECT3DDEVICE9 pDev, D3DXMATRIX matPar);

private:
    LPD3DXMESH m_pMesh;
    LPDIRECT3DTEXTURE9 m_pTexture;
    D3DMATERIAL9 m_Mtrl;
};

Planet::Planet()
{

}

Planet::~Planet()
{
    if (m_pMesh != NULL)
        m_pMesh->Release();

    if (m_pTexture != NULL)
        m_pTexture->Release();
}

inline HRESULT Planet::Init(LPDIRECT3DDEVICE9 pDev, D3DMATERIAL9 mtrl, FLOAT radian, UINT slices, UINT stacks, LPCWSTR texturename, D3DXVECTOR3 pos, FLOAT rotspeed, FLOAT revspeed, FLOAT rotangle, FLOAT revangle)
{
    //�޽�
	m_pMesh = CreateMappedSphere(pDev, radian, slices, stacks);

    if (texturename != NULL)
    {
        //�ؽ���
        if (FAILED(D3DXCreateTextureFromFile(pDev, texturename, &m_pTexture)))
        {
            // If texture is not in current folder, try parent folder
            if (FAILED(D3DXCreateTextureFromFile(pDev, texturename, &m_pTexture)))
            {
                MessageBox(NULL, L"Could not find texture", texturename, MB_OK);
                return E_FAIL;
            }
        }
    }

    m_Mtrl = mtrl; //���׸���
	Pos = pos;//��ġ
	RotSpeed = rotspeed; //�����ӵ�
	RevSpeed = revspeed; //�����ӵ�
	RotAngle = rotangle; //������
	RevAngle = revangle; //������
	return S_OK;;
}

inline D3DXMATRIX Planet::Draw(LPDIRECT3DDEVICE9 pDev, D3DXMATRIX matPar)
{
	D3DXMATRIX matPos;  //��ġ��Ʈ����
	D3DXMATRIX matRot;  //������Ʈ����
	D3DXMATRIX matRev;  //������Ʈ����
	D3DXMATRIX matRotAxis;  //������ ��Ʈ����
	D3DXMATRIX matRevAxis;  //������ ��Ʈ����
	D3DXMATRIX result;      //��� ��Ʈ����

	D3DXMatrixTranslation(&matPos, Pos.x, Pos.y, Pos.z); //��ġ �̵�
	D3DXMatrixRotationY(&matRot, (timeGetTime() / Time) * RotSpeed); //����
	D3DXMatrixRotationY(&matRev, (timeGetTime() / Time) * RevSpeed); //����
	D3DXMatrixRotationZ(&matRotAxis, D3DXToRadian(RotAngle)); //������
	D3DXMatrixRotationZ(&matRevAxis, D3DXToRadian(RevAngle)); //������

    result = matRotAxis * matRot * matPos *matRev* matRevAxis* matPar;

	pDev->SetTransform(D3DTS_WORLD, &result);

	pDev->SetTexture(0, m_pTexture);
	pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    pDev->SetMaterial(&m_Mtrl);
	m_pMesh->DrawSubset(0);
	return result;

}
