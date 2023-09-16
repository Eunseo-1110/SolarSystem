//Texture: http://planetpixelemporium.com/planets.html

#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#include <string>
#pragma warning( default : 4996 )

#include "Planet.h"

#define RingCount 5

LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device

Planet g_Sun;           //태양
Planet g_Mercury;     //수성
Planet g_Venus;        //금성
Planet g_Earth;         //지구
Planet g_Mars;          //화성
Planet g_Jupiter;       //목성
Planet g_Saturn;        //토성
Planet g_SaturnRing[RingCount]; //토성고리
Planet g_Uranus;        //천왕성
Planet g_Neptune;       //해왕성


HRESULT InitD3D(HWND hWnd)
{
    // Create the D3D object.
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }
    // Turn off culling
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

    return S_OK;
}

HRESULT InitGeometry()
{
    D3DMATERIAL9 tempmtrl;
    ZeroMemory(&tempmtrl, sizeof(D3DMATERIAL9));
    tempmtrl.Diffuse.r = tempmtrl.Ambient.r = 1.0f;
    tempmtrl.Diffuse.g = tempmtrl.Ambient.g = 1.0f;
    tempmtrl.Diffuse.b = tempmtrl.Ambient.b = 1.0f;
    tempmtrl.Diffuse.a = tempmtrl.Ambient.a = 1.0f;

    D3DMATERIAL9 sunmtrl;
    ZeroMemory(&sunmtrl, sizeof(D3DMATERIAL9));
    sunmtrl.Diffuse.r = sunmtrl.Ambient.r = 1.0f;
    sunmtrl.Diffuse.g = sunmtrl.Ambient.g = 1.0f;
    sunmtrl.Diffuse.b = sunmtrl.Ambient.b = 0.0f;
    sunmtrl.Diffuse.a = sunmtrl.Ambient.a = 1.0f;
    sunmtrl.Emissive.r = 1.0f;
    sunmtrl.Emissive.g = 1.0f;
    sunmtrl.Emissive.b = 1.0f;

    //태양
    if (FAILED(g_Sun.Init(g_pd3dDevice, sunmtrl, 10.0f, 100, 100, L"sunmap.jpg", D3DXVECTOR3(0, 0, 0), 2, 1, 0, 0)))
    {
        return E_FAIL;
    }

    //수성
    if (FAILED(g_Mercury.Init(g_pd3dDevice, tempmtrl, 1.0f, 100, 100, L"mercurymap.jpg", D3DXVECTOR3(20, 0, 0), 3, 47, 0, 7)))
    {
        return E_FAIL;
    }

    //금성
    if (FAILED(g_Venus.Init(g_pd3dDevice, tempmtrl, 2.0f, 100, 100, L"venusmap.jpg", D3DXVECTOR3(25, 0, 0), 18, 35, 177.3, 3.39)))
    {
        return E_FAIL;
    }

    //지구
    if (FAILED(g_Earth.Init(g_pd3dDevice, tempmtrl, 2.0f, 100, 100, L"earthmap1k.jpg", D3DXVECTOR3(30, 0, 0), 45, 30, 23.4, 0)))
    {
        return E_FAIL;
    }

    //화성
    if (FAILED(g_Mars.Init(g_pd3dDevice, tempmtrl, 1.0f, 100, 100, L"mars_1k_color.jpg", D3DXVECTOR3(35, 0, 0), 25, 24, 25.2, 1.85)))
    {
        return E_FAIL;
    }

    //목성
    if (FAILED(g_Jupiter.Init(g_pd3dDevice, tempmtrl, 4.0f, 100, 100, L"jupitermap.jpg", D3DXVECTOR3(45, 0, 0), 41, 13, 3.1, 1.31)))
    {
        return E_FAIL;
    }

    //토성
    if (FAILED(g_Saturn.Init(g_pd3dDevice, tempmtrl, 4.0f, 100, 100, L"saturnmap.jpg", D3DXVECTOR3(60, 0, 0), 44, 9, 26.7, 2.49)))
    {
        return E_FAIL;
    }

    //토성고리
    for (int i = 0; i < RingCount; i++)
    {
        if (FAILED(g_SaturnRing[i].Init(g_pd3dDevice, tempmtrl, 0.5f, 100, 100, NULL, D3DXVECTOR3(5+i, 0, 0), (i + 1) * 5, (i + 1) * 2
            , 0, 0)))
        {
            return E_FAIL;
        }
    }

    //천왕성
    if (FAILED(g_Uranus.Init(g_pd3dDevice, tempmtrl, 3.0f, 100, 100, L"uranusmap.jpg", D3DXVECTOR3(70, 0, 0), 71, 6, 97.8, 0.77)))
    {
        return E_FAIL;
    }

    //해왕성
    if (FAILED(g_Neptune.Init(g_pd3dDevice, tempmtrl, 3.0f, 100, 100, L"neptunemap.jpg", D3DXVECTOR3(80, 0, 0), 67, 5, 28.3, 1.77)))
    {
        return E_FAIL;
    }


    return S_OK;
}

VOID SetupLights()
{
    D3DLIGHT9 light;
    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Type = D3DLIGHT_POINT;
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Range = 1000.0f;
    g_pd3dDevice->SetLight(0, &light);
    g_pd3dDevice->LightEnable(0, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

VOID Cleanup()
{
    if (g_pd3dDevice != NULL)
        g_pd3dDevice->Release();

    if (g_pD3D != NULL)
        g_pD3D->Release();
}

VOID SetupCamera()
{
    D3DXVECTOR3 vEyePt(0.0f, 20.0f, -150.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 500.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupPlanet()
{
    D3DXMATRIX mattemp, matSun, matSat;
    D3DXMatrixIdentity(&mattemp);
    matSun = g_Sun.Draw(g_pd3dDevice, mattemp);
    g_Mercury.Draw(g_pd3dDevice, matSun);
    g_Venus.Draw(g_pd3dDevice, matSun);
    g_Earth.Draw(g_pd3dDevice, matSun);
    g_Mars.Draw(g_pd3dDevice, matSun);
    g_Jupiter.Draw(g_pd3dDevice, matSun);
    matSat = g_Saturn.Draw(g_pd3dDevice, matSun);
    for (int i = 0; i < RingCount; i++)
    {
        g_SaturnRing[i].Draw(g_pd3dDevice, matSat);
    }
    g_Uranus.Draw(g_pd3dDevice, matSun);
    g_Neptune.Draw(g_pd3dDevice, matSun);
}

VOID Render()
{
    // Clear the backbuffer and the zbuffer
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // Begin the scene
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        SetupLights();
        SetupPlanet();
        SetupCamera();

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        Cleanup();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    UNREFERENCED_PARAMETER(hInst);

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx(&wc);

    // Create the application's window
    HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 06: Meshes",
        WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
        NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        // Create the scene geometry
        if (SUCCEEDED(InitGeometry()))
        {
            // Show the window
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            // Enter the message loop
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            while (msg.message != WM_QUIT)
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                    Render();
            }
        }
    }

    UnregisterClass(L"D3D Tutorial", wc.hInstance);
    return 0;
}



