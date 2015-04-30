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

IDirect3DVertexBuffer9 *Quad = 0;
IDirect3DTexture9      *Tex  = 0;


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
	Device->CreateVertexBuffer(
		6*sizeof(TextureVertex),
		D3DUSAGE_WRITEONLY,
		TextureVertex::FVF,
		D3DPOOL_MANAGED,
		&Quad, 0
		);
	TextureVertex *v;
	Quad->Lock(0,0,(void**)&v, 0);

	v[0] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = TextureVertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = TextureVertex( 1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[3] = TextureVertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = TextureVertex( 1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = TextureVertex( 1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	Quad->Unlock();

	// create texture from file
	D3DXCreateTextureFromFile( Device, "dx5_logo.bmp", &Tex );

	// 清空以前的纹理
	Device->SetTexture(0, Tex);
	
	// 设置纹理采样属性 
	// 设置纹理采样属性  
	// 	HRESULT  SetSamplerState(  
	//	DWORD    Sampler,     //指定纹理采样属性的纹理层ID（0~7）  
	//	D3DSAMPLERSTATETYPE    Type,       //纹理过滤类型  
	//	DWORD    Value     //设置纹理采样属性值  
	//	);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //处理放大过滤 
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //处理缩小过滤 
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); //多纹理过滤 

	Device->SetRenderState(D3DRS_LIGHTING, false); // 允许使用灯光,(默认是允许的)

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
	d3d::Release<IDirect3DVertexBuffer9*>(Quad);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		// 沿着y轴旋转
		D3DXMATRIX R, Rx, Ry, Rz;
		static float x = 0.0f, y = 0.0f, z = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		D3DXMatrixRotationX(&Rx, x);
		D3DXMatrixRotationZ(&Rz, z);
		y += timeDelta;
		if(y >= PI*2)
			y = 0.0f;
		x += timeDelta;
		if(x >= PI*2)
			x = 0.0f;
		z += timeDelta;
		if(z >= PI*2)
			z = 0.0f;
	
		R = Rx*Ry*Rz;

		Device->SetTransform(D3DTS_WORLD, &R);

		//
		// Draw the Scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Quad, 0, sizeof(TextureVertex));
		//Device->SetIndices(indexBuffer);
		Device->SetFVF(TextureVertex::FVF);

		// draw one triangle
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2 );
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