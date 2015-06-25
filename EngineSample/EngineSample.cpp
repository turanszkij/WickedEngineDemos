// EngineSample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EngineSample.h"
#include "WiDemo.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND g_hWnd;
int screenW=800, screenH=600;
bool mousebuttondown = false;

enum DEMOS{
	HELLOWORLD,
	BASICMODEL,
	SKINNEDMODEL,
	EMITTERPARTICLE,
	HAIRPARTICLE,
	RIGIDBODY,
	SOFTBODY,
	DEFERREDSCENE,
	DEFERREDLIGHTS,
};
DEMOS demoScene = HELLOWORLD;
map<DEMOS, WiDemo*> demos;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void BasicModelInit();
void EmitterParticleInit();

void LoadProgram(){
	Renderer::SetUpStaticComponents();
	Renderer::VSYNC = false;
	Renderer::EMITTERSENABLED = true;
	Renderer::HAIRPARTICLEENABLED = true;
	Renderer::setRenderResolution(screenW, screenH);
	Renderer::SHADOWMAPRES = 1024;
	Renderer::POINTLIGHTSHADOW = 6;
	Renderer::POINTLIGHTSHADOWRES = 512;
	Renderer::SOFTSHADOW = 2;
	Renderer::DX11 = false;
	Renderer::physicsEngine = new BULLET();

	Image::Load();
	Image::SetScreenResolution(screenW, screenH);
	ResourceManager::SetUp();
	BackLog::Initialize(screenW, screenH);
	Font::SetUpStaticComponents();
	Font::SetScreenResolution(screenW, screenH);
	SoundEffect::Initialize();
	Music::Initialize(); 
	FrameRate::Initialize();
	CpuInfo::Initialize();
	FrameRate::Initialize();
	LensFlare::Initialize(screenW,screenH);

	Font::addFontStyle("basic");
	InputManager::addDirectInput(new DirectInput(hInst, g_hWnd));

	ResourceManager::add("HelloWorld/HelloWorld.png");
	ResourceManager::add("sound/change.wav",ResourceManager::SOUND);
	SoundEffect::SetVolume(0.5f);

	Renderer::cam->SetDefaultPosition(XMVectorSet(0, 3, -4, 0));

	WiDemo::screenW = screenW;
	WiDemo::screenH = screenH;
	demos.insert(pair<DEMOS, WiDemo*>(HELLOWORLD, new HelloWorldDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(BASICMODEL, new BasicModelDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(SKINNEDMODEL, new SkinnedModelDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(EMITTERPARTICLE, new EmittedParticleDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(HAIRPARTICLE, new HairParticleDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(RIGIDBODY, new RigidBodyDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(SOFTBODY, new SoftBodyDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(DEFERREDSCENE, new DeferredSceneDemo()));
	demos.insert(pair<DEMOS, WiDemo*>(DEFERREDLIGHTS, new DeferredLightDemo()));
}
void CleanUpProgram(){

	Renderer::CleanUpStatic();
	Image::CleanUp();
	Font::CleanUpStatic();
	ResourceManager::CleanUp();
	InputManager::CleanUp();
	LensFlare::CleanUp();
	Renderer::DestroyDevice();
}
void CameraControl(){
	DirectInput* dinput = InputManager::dinput;
	Camera* cam = Renderer::cam;
	float speed = (dinput->IsKeyDown(DIK_LSHIFT) ? 10.0f : 1.0f);
	if (dinput->IsKeyDown(DIK_A)) cam->AddtoCameraPosition(XMVectorSet(-speed, 0, 0, 0));
	if (dinput->IsKeyDown(DIK_D)) cam->AddtoCameraPosition(XMVectorSet(speed, 0, 0, 0));
	if (dinput->IsKeyDown(DIK_W)) cam->AddtoCameraPosition(XMVectorSet(0, 0, speed, 0));
	if (dinput->IsKeyDown(DIK_S)) cam->AddtoCameraPosition(XMVectorSet(0, 0, -speed, 0));
	if (dinput->IsKeyDown(DIK_SPACE)) cam->AddtoCameraPosition(XMVectorSet(0, speed, 0, 0));
	if (dinput->IsKeyDown(DIK_LCONTROL)) cam->AddtoCameraPosition(XMVectorSet(0, -speed, 0, 0));
	cam->ProcessInput(1.0f / 60.0f, mousebuttondown);
}
void CameraReset(){
	Renderer::cam->Reset();
}


void ChangeDemo(DEMOS newDemo){

	CameraReset();

	static_cast<SoundEffect*>(ResourceManager::get("sound/change.wav")->data)->Play();

	if (demos.find(newDemo) != demos.end()){
		Renderer::CleanUpStaticTemp();
		demos[newDemo]->Start();
		demoScene = newDemo;
	}

}


void HudRender(){
	stringstream ss("");
	ss << "Wicked Engine v" << WICKED_ENGINE_VERSION;
#ifdef _DEBUG
	ss << " [DEBUG]";
#endif
	ss << "\n\nDemo Select:\n------------------";
	ss << "\n[1] :  HelloWorld";
	ss << "\n[2] :  BasicModel";
	ss << "\n[3] :  SkinnedModel";
	ss << "\n[4] :  EmitterParticle";
	ss << "\n[5] :  HairParticle";
	ss << "\n[6] :  RigidBody";
	ss << "\n[7] :  SoftBody";
	ss << "\n[8] :  DeferredLights";
	ss << "\n[9] :  DeferredScene";
	ss << "\n\nControls:\n-----------------\nMove with WASD\nLook with RMB";
	Font::Draw(ss.str(), "basic", XMFLOAT4(0, 0, -5, -4), "left", "top");
	ss.str("");
	ss.precision(1);
	ss <<fixed<<FrameRate::FPS()<< " FPS";
	ss << "\nCPU: " << CpuInfo::GetCpuPercentage()<< "%";
	Font::Draw(ss.str(), "basic", XMFLOAT4(screenW-15, 0, -5, -4), "right", "top");
	ss.str("");
	switch (demoScene)
	{
	case HELLOWORLD:
		ss << "HELLOWORLD";
		break;
	case BASICMODEL:
		ss << "BASICMODEL";
		break;
	case SKINNEDMODEL:
		ss << "SKINNEDMODEL";
		break;
	case EMITTERPARTICLE:
		ss << "EMITTERPARTICLE";
		break;
	case HAIRPARTICLE:
		ss << "HAIRPARTICLE";
		break;
	case RIGIDBODY:
		ss << "RIGIDBODY";
		break;
	case SOFTBODY:
		ss << "SOFTBODY";
		break;
	case DEFERREDSCENE:
		ss << "DEFERREDSCENE";
		break;
	case DEFERREDLIGHTS:
		ss << "DEFERREDLIGHTS";
		break;
	default:
		break;
	}
	ss << " DEMO";
	Font::Draw(ss.str(), "basic", XMFLOAT4(screenW/2, -screenH, -5, -4), "center", "bottom");
}



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ENGINESAMPLE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENGINESAMPLE));

	RECT rect = RECT();
	GetClientRect(g_hWnd, &rect);
	screenW = rect.right - rect.left;
	screenH = rect.bottom - rect.top;

	short requestMultiThreading = 0;
	if (FAILED(Renderer::InitDevice(g_hWnd, screenW, screenH, 1, requestMultiThreading)))
	{
		MessageBox(NULL, L"Could not initialize the D3D device", L"Error", MB_OK);
		return 0;
	}

	LoadProgram();
	ChangeDemo(HELLOWORLD);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{

			//Update demo
			static bool frameskip = true;
			static Timer timer = Timer();
			static const double dt = 1.0 / 60.0;
			static double accumulator = 0.0;

			accumulator += timer.elapsed() / 1000.0;
			if (accumulator>10) //application probably lost control
				accumulator = 0;
			timer.record();


			while (accumulator >= dt)
			{

				InputManager::Update();
				CameraControl();
				CpuInfo::Frame();

				if (InputManager::press(DIK_1)){
					ChangeDemo(HELLOWORLD);
				}
				else if (InputManager::press(DIK_2)){
					ChangeDemo(BASICMODEL);
				}
				else if (InputManager::press(DIK_3)){
					ChangeDemo(SKINNEDMODEL);
				}
				else if (InputManager::press(DIK_4)){
					ChangeDemo(EMITTERPARTICLE);
				}
				else if (InputManager::press(DIK_5)){
					ChangeDemo(HAIRPARTICLE);
				}
				else if (InputManager::press(DIK_6)){
					ChangeDemo(RIGIDBODY);
				}
				else if (InputManager::press(DIK_7)){
					ChangeDemo(SOFTBODY);
				}
				else if (InputManager::press(DIK_8)){
					ChangeDemo(DEFERREDLIGHTS);
				}
				else if (InputManager::press(DIK_9)){
					ChangeDemo(DEFERREDSCENE);
				}

				demos[demoScene]->Update();

				accumulator -= dt;

				if (!frameskip)
					break;

			}

			//Render demo
			demos[demoScene]->Render();
			Renderer::Present(bind(&WiDemo::Compose,demos[demoScene]), bind(HudRender));
		}
	}


	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENGINESAMPLE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ENGINESAMPLE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		mousebuttondown = true;
		ShowCursor(false);
		break;
	case WM_RBUTTONUP:
		mousebuttondown = false;
		ShowCursor(true);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		CleanUpProgram();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
