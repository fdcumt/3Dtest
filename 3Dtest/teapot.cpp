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

IDirect3DVertexBuffer9 *Pyramid = 0;

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

struct NormalVertex 
{
	NormalVertex( float x, float y, float z, float nx, float ny, float nz) 
	{
		_x=x;
		_y=y;
		_z=z;
		_nx=nx;
		_ny=ny;
		_nz=nz;

	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	static const DWORD FVF;
};
const DWORD NormalVertex::FVF = D3DFVF_XYZ|D3DFVF_NORMAL;



const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );


// 设定材质
D3DMATERIAL9 iniMaterial(const D3DXCOLOR& a, const D3DXCOLOR& d, const D3DXCOLOR& s, const D3DXCOLOR &e, float p ) 
{
	D3DMATERIAL9 material;
	material.Ambient  = a; // 指定此表面反射的漫射光的数量
	material.Diffuse  = d; // 指定次表面发射的环境光的数量
	material.Specular = s; // 指定此表面反射的漫反射光的数量
	material.Emissive = e; // 给表面添加颜色,就像自己发光一样
	material.Power    = p; // 指定锐利的镜面高光,它的值是高光的锐利值。
	return material;
}

const D3DMATERIAL9 white_mtrl  = iniMaterial( WHITE, WHITE, WHITE, BLACK, 8.0f );
const D3DMATERIAL9   red_mtrl  = iniMaterial(   RED,   RED,   RED, BLACK, 8.0f );
const D3DMATERIAL9 green_mtrl  = iniMaterial( GREEN, GREEN, GREEN, BLACK, 8.0f );
const D3DMATERIAL9  blue_mtrl  = iniMaterial(  BLUE,   BLUE,  BLUE, BLACK, 8.0f );
const D3DMATERIAL9 yellow_mtrl = iniMaterial( YELLOW, YELLOW, YELLOW, BLACK, 8.0f );

// 通过三角形的定点计算三角形的面法线,三个定点是顺时针方向指定的
void computeNormal(D3DXVECTOR3 *p0, D3DXVECTOR3 *p1, D3DXVECTOR3 *p2, D3DXVECTOR3 *out ) 
{
	D3DXVECTOR3 u = *p1 - *p0;
	D3DXVECTOR3 v = *p2 - *p0;
	D3DXVec3Cross(out, &u, &v);
	D3DXVec3Normalize(out, out);
}

// 初始化方向光源
D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR *color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color*0.4f;
	light.Diffuse = *color*0.6f;
	light.Direction = *direction;
	return light;
}

// 初始化点光源
D3DLIGHT9 InitPointLight(D3DXVECTOR3* direction, D3DXCOLOR *color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color*0.4f;
	light.Diffuse = *color*0.6f;
	light.Direction = *direction;
	return light;
}

// 初始化聚光灯光源
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* direction, D3DXCOLOR *color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color*0.4f;
	light.Diffuse = *color*0.6f;
	light.Direction = *direction;
	return light;
}


//
// Framework Functions
//
bool Setup()
{
	Device->SetRenderState(D3DRS_LIGHTING, true); // 允许使用灯光,(默认是允许的)
	Device->CreateVertexBuffer(
		12*sizeof(NormalVertex),
		D3DUSAGE_WRITEONLY,
		NormalVertex::FVF,
		D3DPOOL_MANAGED,
		&Pyramid, 0
		);
	NormalVertex *v;
	Pyramid->Lock(0,0,(void**)&v, 0);

	// front face
	v[0] = NormalVertex(-1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	v[1] = NormalVertex( 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, -0.707f);
	v[2] = NormalVertex( 1.0f, 0.0f, -1.0f, 0.0f, 0.707f, -0.707f);
	// lef t face
	v[3] = NormalVertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
	v[4] = NormalVertex( 0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
	v[5] = NormalVertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);
	// right face
	v[6] = NormalVertex( 1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
	v[7] = NormalVertex( 0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
	v[8] = NormalVertex( 1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);
	// back face
	v[9]  = NormalVertex( 1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
	v[10] = NormalVertex( 0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
	v[11] = NormalVertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

	Pyramid->Unlock();

	// 设定材质
	D3DMATERIAL9 mtrl = iniMaterial(WHITE, WHITE, WHITE, BLACK, 5.0f );
	Device->SetMaterial(&mtrl);

	// 创建方向光源
	D3DLIGHT9 dir;
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = WHITE;
	dir.Specular = WHITE*0.3f;
	dir.Ambient = WHITE*0.6f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true); //设置法线从新规格化
	Device->SetRenderState(D3DRS_SPECULARENABLE, true); // 使用镜面光必须设置D3DRS_SPECULARENABLE开启,默认是关闭的


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
	
	 ///Device->SetRenderState(D3DRS_FILLMODE, fillMode);     // 填充模式
	// Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);  // 着色模式
	// Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);      //不适用背面消隐
	 

	// Device->SetRenderState(D3DRS_LIGHTING, false);

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
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, Pyramid, 0, sizeof(NormalVertex));
		//Device->SetIndices(indexBuffer);
		Device->SetFVF(NormalVertex::FVF);

		// draw one triangle
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4 );
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