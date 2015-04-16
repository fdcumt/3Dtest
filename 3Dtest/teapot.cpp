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

// Mesh interface that will store the teapot data and contains
// ma method to render the teapot data.
ID3DXMesh* Teapot = 0;

// vertex buffer to store our triangle data.
IDirect3DVertexBuffer9* Triangle = 0;


struct Vertex 
{
	Vertex() {}
	Vertex( float x, float y, float z) 
	{
		_x=x;
		_y=y;
		_z=z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;

//
// Framework Functions
//
bool Setup()
{
	// create the vertex buffer 
	Device->CreateVertexBuffer(
		3*sizeof(Vertex),
		D3DUSAGE_WRITEONLY, 
		Vertex::FVF, 
		D3DPOOL_MANAGED, 
		&Triangle,
		0);

	// fill the buffers with the triangle data
	Vertex* vertices;
	Triangle->Lock(0, 0, (void **)&vertices, 0);
	
	vertices[0] = Vertex(-1.0f, 0.0f, 2.0f);
	vertices[1] = Vertex( 0.0f, 1.0f, 2.0f);
	vertices[2] = Vertex( 1.0f, 0.0f, 2.0f);

	Triangle->Unlock();


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


	//
	// Set wireframe mode render state.
	
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //设置渲染状态
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		//
		// Spin the teapot:
		//

		D3DXMATRIX Ry;
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);

		y += timeDelta;
		if(y >= PI*2)
			y = 0.0f;

		Device->SetTransform(D3DTS_WORLD, &Ry);

		//
		// Draw the Scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		// draw one triangle
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
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