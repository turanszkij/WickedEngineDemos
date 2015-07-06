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
	SSRTEST,
	SOFTBODY_DEFERRED,
	FORWARDSCENE,
	DEMO_COUNT,
};
DEMOS demoScene = HELLOWORLD;
map<DEMOS, RenderableComponent*> demos;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void BasicModelInit();
void EmitterParticleInit();

void LoadProgram(){
	wiRenderer::SetUpStaticComponents();
	wiRenderer::VSYNC = false;
	wiRenderer::EMITTERSENABLED = true;
	wiRenderer::HAIRPARTICLEENABLED = true;
	wiRenderer::setRenderResolution(screenW, screenH);
	wiRenderer::SHADOWMAPRES = 1024;
	wiRenderer::POINTLIGHTSHADOW = 6;
	wiRenderer::POINTLIGHTSHADOWRES = 512;
	wiRenderer::SOFTSHADOW = 2;
	wiRenderer::DX11 = false;
	wiRenderer::physicsEngine = new BULLET();

	wiImage::Load();
	wiResourceManager::SetUp();
	wiBackLog::Initialize();
	wiFont::SetUpStaticComponents();
	wiSoundEffect::Initialize();
	wiMusic::Initialize();
	wiFrameRate::Initialize();
	wiCpuInfo::Initialize();
	wiFrameRate::Initialize();
	wiLensFlare::Initialize();

	wiFont::addFontStyle("basic");
	wiInputManager::addDirectInput(new DirectInput(hInst, g_hWnd));

	wiResourceManager::add("HelloWorld/HelloWorld.png");
	wiResourceManager::add("sound/change.wav",wiResourceManager::SOUND);
	wiSoundEffect::SetVolume(0.5f);

	wiRenderer::getCamera()->SetDefaultPosition(XMVectorSet(0, 3, -4, 0));

	RenderableComponent::screenW = screenW;
	RenderableComponent::screenH = screenH;
	demos.insert(pair<DEMOS, RenderableComponent*>(HELLOWORLD, new HelloWorldDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(BASICMODEL, new BasicModelDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(SKINNEDMODEL, new SkinnedModelDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(EMITTERPARTICLE, new EmittedParticleDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(HAIRPARTICLE, new HairParticleDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(RIGIDBODY, new RigidBodyDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(SOFTBODY, new SoftBodyDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(DEFERREDSCENE, new DeferredSceneDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(DEFERREDLIGHTS, new DeferredLightDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(SSRTEST, new SSRTestDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(SOFTBODY_DEFERRED, new SoftBodyDeferredDemo()));
	demos.insert(pair<DEMOS, RenderableComponent*>(FORWARDSCENE, new ForwardSceneDemo()));
}
void CleanUpProgram(){

	wiRenderer::CleanUpStatic();
	wiImage::CleanUp();
	wiFont::CleanUpStatic();
	wiResourceManager::CleanUp();
	wiInputManager::CleanUp();
	wiLensFlare::CleanUp();
	wiRenderer::DestroyDevice();
}
void CameraControl(){
	DirectInput* dinput = wiInputManager::dinput;
	Camera* cam = wiRenderer::getCamera();
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
	wiRenderer::getCamera()->Reset();
}


void ChangeDemo(DEMOS newDemo){

	CameraReset();

	static_cast<wiSoundEffect*>(wiResourceManager::get("sound/change.wav")->data)->Play();

	if (demos.find(newDemo) != demos.end()){
		wiRenderer::CleanUpStaticTemp();
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
	ss << "\n[0] :  SSRTest";
	ss << "\n[F1] : SoftBody Deferred";
	ss << "\n[F2] : ForwardScene";
	ss << "\n\nControls:\n-----------------\nMove with WASD\nLook with RMB";
	wiFont::Draw(ss.str(), "basic", XMFLOAT4(0, 0, -5, -4), "left", "top");
	ss.str("");
	ss.precision(1);
	ss <<fixed<<wiFrameRate::FPS()<< " FPS";
	ss << "\nCPU: " << wiCpuInfo::GetCpuPercentage()<< "%";
	wiFont::Draw(ss.str(), "basic", XMFLOAT4(screenW-15, 0, -5, -4), "right", "top");
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
	case FORWARDSCENE:
		ss << "FORWARDSCENE";
		break;
	case SOFTBODY_DEFERRED:
		ss << "SOFTBODY DEFERRED";
		break;
	case SSRTEST:
		ss << "SSRTEST";
		break;
	default:
		break;
	}
	ss << " DEMO";
	wiFont::Draw(ss.str(), "basic", XMFLOAT4(screenW/2, -screenH, -5, -4), "center", "bottom");
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
	if (FAILED(wiRenderer::InitDevice(g_hWnd, screenW, screenH, 1, requestMultiThreading)))
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

				wiInputManager::Update();
				CameraControl();
				wiCpuInfo::Frame();

				if (wiInputManager::press(DIK_1)){
					ChangeDemo(HELLOWORLD);
				}
				else if (wiInputManager::press(DIK_2)){
					ChangeDemo(BASICMODEL);
				}
				else if (wiInputManager::press(DIK_3)){
					ChangeDemo(SKINNEDMODEL);
				}
				else if (wiInputManager::press(DIK_4)){
					ChangeDemo(EMITTERPARTICLE);
				}
				else if (wiInputManager::press(DIK_5)){
					ChangeDemo(HAIRPARTICLE);
				}
				else if (wiInputManager::press(DIK_6)){
					ChangeDemo(RIGIDBODY);
				}
				else if (wiInputManager::press(DIK_7)){
					ChangeDemo(SOFTBODY);
				}
				else if (wiInputManager::press(DIK_8)){
					ChangeDemo(DEFERREDLIGHTS);
				}
				else if (wiInputManager::press(DIK_9)){
					ChangeDemo(DEFERREDSCENE);
				}
				else if (wiInputManager::press(DIK_0)){
					ChangeDemo(SSRTEST);
				}
				else if (wiInputManager::press(DIK_F1)){
					ChangeDemo(SOFTBODY_DEFERRED);
				}
				else if (wiInputManager::press(DIK_F2)){
					ChangeDemo(FORWARDSCENE);
				}

				demos[demoScene]->Update();

				accumulator -= dt;

				if (!frameskip)
					break;

			}

			//Render demo
			demos[demoScene]->Render();
			wiRenderer::Present(bind(&RenderableComponent::Compose,demos[demoScene]), bind(HudRender));
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
