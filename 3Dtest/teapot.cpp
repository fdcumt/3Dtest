//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: teapot.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders a teapot in wireframe mode.  Shows how to create a teapot 
//       using the D3DXCreateTeapot function and how to render the teapot
//       using the ID3DXMesh::DrawSubset method.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9* Device = 0; 

const int Width  = 640;
const int Height = 480;
const float PI = 3.14f;

ID3DXMesh    *Teapot = 0 ; // teapot 
D3DMATERIAL9  TeapotMtrl ; // teapot material 
 
IDirect3DVertexBuffer9 *BkGndQuad = 0; // 方形背景
IDirect3DTexture9      *BkGndTex  = 0; // 材质
D3DMATERIAL9            BkGndMtrl ;    // 背景原料 



const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);


// 纹理顶点
struct TextureVertex
{
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;
	TextureVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		_x = x; _y = y; _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u; _v = v;
	}
	static const DWORD	FVF;
};

const DWORD TextureVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ;

//
// Framework Functions
//
bool Setup()
{
	TeapotMtrl = RED_MTRL; // red material
	TeapotMtrl.Diffuse.a = 0.5f;
	BkGndMtrl = WHITE_MTRL;

	D3DXCreateTeapot(Device, &Teapot, 0); // create the teapot

	// create the background quad
	Device->CreateVertexBuffer( 6*sizeof(TextureVertex), D3DUSAGE_WRITEONLY, TextureVertex::FVF, D3DPOOL_MANAGED, &BkGndQuad, 0);
	
	TextureVertex * v;
	BkGndQuad->Lock(0, 0, (void**)&v, 0);
	v[0] = TextureVertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = TextureVertex(-10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = TextureVertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = TextureVertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = TextureVertex( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = TextureVertex( 10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	BkGndQuad->Unlock();

	// set direction light 
	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type      = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse   = WHITE;      // 漫反射光颜色
	dir.Specular  = WHITE*0.2f; // 反射光颜色
	dir.Ambient   = WHITE*0.6f; // 环境光
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f); // 方向
	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true); // 规范化所有向量
	Device->SetRenderState(D3DRS_SPECULARENABLE,   true);

	D3DXCreateTextureFromFile(Device, "crate.jpg", &BkGndTex );

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // 设置放大过滤器
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); // 设置缩小过滤器
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); // 使用mipmap过滤器

	// set alpha blending states
	Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	Device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

	// set blending factors so that alpha component determines transparency
	Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 设置相机位置,即观察点以及观察视角等
	D3DXVECTOR3 Eye(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &Eye, &At, &Up);
	Device->SetTransform(D3DTS_VIEW, &V);

	// 设置目标模型矩阵,即被观察事物
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI*0.5f,
		float(Width)/float(Height),
		1.0f, 1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(BkGndQuad);
	d3d::Release<ID3DXMesh*>(Teapot);
	d3d::Release<IDirect3DTexture9*>(BkGndTex);
}

bool Display(float timeDelta)
{
	static float preTime = 0;
	float curTime = (float)timeGetTime();
	if( Device  )
	{
		if (curTime-preTime>10.0f) 
		{
			preTime = curTime;
			// increase/decrease alpha via keyboard input 
			if (::GetAsyncKeyState('A')& 0x8000f)
			{
				TeapotMtrl.Diffuse.a +=0.01f;
			}
			if (::GetAsyncKeyState('S')& 0x8000f)
			{
				TeapotMtrl.Diffuse.a -=0.01f;
			}

			// force alpha to [0,1] interval
			if (TeapotMtrl.Diffuse.a > 1.0f) 
			{
				TeapotMtrl.Diffuse.a = 1.0f;
			}

			if (TeapotMtrl.Diffuse.a < 0.0f) 
			{
				TeapotMtrl.Diffuse.a = 0.0f;
			}
		}
		
		//
		// Draw the Scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		
		// draw the background 
		D3DXMATRIX w;
		D3DXMatrixIdentity(&w);
		Device->SetTransform(D3DTS_WORLD, &w);
		Device->SetFVF(TextureVertex::FVF);
		Device->SetStreamSource(0, BkGndQuad, 0, sizeof(TextureVertex));
		Device->SetMaterial(&BkGndMtrl);
		Device->SetTexture(0, BkGndTex);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// draw the teapot 
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixScaling(&w, 1.5f, 1.5f, 1.5f);
		Device->SetTransform(D3DTS_WORLD, &w);
		Device->SetMaterial(&TeapotMtrl);
		Device->SetTexture(0, 0);
		Teapot->DrawSubset(0);
		
		Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}