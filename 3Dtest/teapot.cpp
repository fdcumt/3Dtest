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
IDirect3DVertexBuffer9* vertexBuffer = 0;
IDirect3DIndexBuffer9*  indexBuffer  = 0;

// 设置着色模式 
/*
typedef enum _D3DFILLMODE {
	D3DFILL_POINT               = 1,
	D3DFILL_WIREFRAME           = 2,
	D3DFILL_SOLID               = 3,
	D3DFILL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
//} D3DFILLMODE; 



D3DFILLMODE fillMode = D3DFILL_WIREFRAME ;

// 设置颜色,把alpha设置为0xff(255)
#define D3DCOLOR_0XFFRGB(r,g,b) D3DCOLOR_ARGB(0,r,g,b)

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

struct ColorVertex 
{
	ColorVertex( float x, float y, float z, D3DCOLOR color) 
	{
		_x=x;
		_y=y;
		_z=z;
		_color=color;

	}
	D3DCOLOR _color;
	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD ColorVertex::FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE;

const D3DCOLOR WHITE = D3DCOLOR_0XFFRGB(255, 255, 255);
const D3DCOLOR BLACK = D3DCOLOR_0XFFRGB( 255, 98, 255);
const D3DCOLOR RED   = D3DCOLOR_0XFFRGB(255, 0, 0);
const D3DCOLOR GREEN = D3DCOLOR_0XFFRGB( 0, 255, 0);
const D3DCOLOR BLUE  = D3DCOLOR_0XFFRGB( 0, 0, 255);
const D3DCOLOR YELLOW = D3DCOLOR_0XFFRGB(255, 255, 0);
const D3DCOLOR CYAN   = D3DCOLOR_0XFFRGB( 0, 255, 255);
const D3DCOLOR MAGENTA= D3DCOLOR_0XFFRGB(255, 0, 255);

//
// Framework Functions
//
bool Setup()
{
	// 创建定点索引缓存
	// Device->CreateVertexBuffer( 8*sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vertexBuffer, 0);
	Device->CreateVertexBuffer( 8*sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &vertexBuffer, 0);

	Device->CreateIndexBuffer(36*sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuffer, 0 );

	// 向定点缓存写入数据
	ColorVertex * pColorVetex;
	vertexBuffer->Lock(0, 0, (void**)&pColorVetex, 0);
	// pColorVetex[0] = ColorVertex(-1.0f, -1.0f, -1.0f, BLACK); 
	// pColorVetex[1] = ColorVertex(-1.0f,  1.0f, -1.0f, BLACK); 
	// pColorVetex[2] = ColorVertex( 1.0f,  1.0f, -1.0f, RED); 
	// pColorVetex[3] = ColorVertex( 1.0f, -1.0f, -1.0f, GREEN); 
	// pColorVetex[4] = ColorVertex(-1.0f, -1.0f,  1.0f, BLUE); 
	// pColorVetex[5] = ColorVertex(-1.0f,  1.0f,  1.0f, YELLOW); 
	// pColorVetex[6] = ColorVertex( 1.0f,  1.0f,  1.0f, CYAN); 
	// pColorVetex[7] = ColorVertex( 1.0f, -1.0f,  1.0f, MAGENTA); 

	pColorVetex[0] = ColorVertex(-1.0f, -1.0f, -1.0f, BLACK); 
	pColorVetex[1] = ColorVertex(-1.0f,  1.0f, -1.0f, BLACK); 
	pColorVetex[2] = ColorVertex( 1.0f,  1.0f, -1.0f, BLACK); 
	pColorVetex[3] = ColorVertex( 1.0f, -1.0f, -1.0f, BLACK); 
	pColorVetex[4] = ColorVertex(-1.0f, -1.0f,  1.0f, BLACK); 
	pColorVetex[5] = ColorVertex(-1.0f,  1.0f,  1.0f, BLACK); 
	pColorVetex[6] = ColorVertex( 1.0f,  1.0f,  1.0f, BLACK); 
	pColorVetex[7] = ColorVertex( 1.0f, -1.0f,  1.0f, BLACK); 
	vertexBuffer->Unlock();

	// 向索引写入数据, 全部为顺时针写入
	WORD* index = 0;
	indexBuffer->Lock(0, 0, (void **)&index, 0);
	// 前面
	index[0]  = 0;  index[1]  = 1;  index[2]  = 2;
	index[3]  = 0;  index[4]  = 2;  index[5]  = 3;

	// 后面
	index[6]  = 4;  index[7]  = 5;  index[8]  = 6;
	index[9]  = 4;  index[10] = 6;  index[11] = 7;

	// 右面
	index[12] = 3;  index[13] = 2;  index[14] = 6;
	index[15] = 3;  index[16] = 6;  index[17] = 2; 

	// 左面
	index[18] = 4;  index[19] = 1;  index[20] = 0;
	index[21] = 4;  index[22] = 5;  index[23] = 1;

	// 上面
	index[24] = 5;  index[25] = 6;  index[26] = 2;
	index[27] = 5;  index[28] = 2;  index[29] = 1;


	// 下面
	index[30] = 0;  index[31] = 4;  index[32] = 7;
	index[33] = 0;  index[34] = 7;  index[35] = 3;

	indexBuffer->Unlock();


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
	
	 Device->SetRenderState(D3DRS_FILLMODE, fillMode);     // 填充模式
	 Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);  // 着色模式
	 Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);      //不适用背面消隐
	 

	 Device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DIndexBuffer9*>(indexBuffer);
	d3d::Release<IDirect3DVertexBuffer9*>(vertexBuffer);
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
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(5, 5, 5), 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, vertexBuffer, 0, sizeof(ColorVertex));
		Device->SetIndices(indexBuffer);
		Device->SetFVF(ColorVertex::FVF);

		// draw one triangle
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
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