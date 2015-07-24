#include "stdafx.h"
#include "WiDemo.h"

Demo::Demo()
{
	MainComponent::MainComponent();
	
	demoScene = HELLOWORLD;
	interactionType = SPAWN_OBJECT;
}
Demo::~Demo()
{
	wiRenderer::CleanUpStatic();
	wiImage::CleanUp();
	wiFont::CleanUpStatic();
	wiResourceManager::CleanUp();
	wiInputManager::CleanUp();
	wiLensFlare::CleanUp();
	wiRenderer::DestroyDevice();

	MainComponent::~MainComponent();
}
void Demo::Initialize()
{
	MainComponent::Initialize();

	wiInitializer::InitializeComponents(
		wiInitializer::WICKEDENGINE_INITIALIZE_RENDERER
		| wiInitializer::WICKEDENGINE_INITIALIZE_IMAGE
		| wiInitializer::WICKEDENGINE_INITIALIZE_FONT
		| wiInitializer::WICKEDENGINE_INITIALIZE_SOUND
		| wiInitializer::WICKEDENGINE_INITIALIZE_MISC
		);

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

	wiFont::addFontStyle("basic");
	wiInputManager::addDirectInput(new DirectInput(instance, window));

	wiResourceManager::add("HelloWorld/HelloWorld.png");
	wiResourceManager::add("sound/change.wav", wiResourceManager::SOUND);
	wiSoundEffect::SetVolume(0.5f);

	wiRenderer::getCamera()->SetDefaultPosition(XMVectorSet(0, 3, -4, 0));

	RenderableComponent::screenW = screenW;
	RenderableComponent::screenH = screenH;
	demos.insert(pair<DEMOS, RenderableComponent*>(LOADINGSCREEN, new DemoLoadingScreen()));
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
	demos.insert(pair<DEMOS, RenderableComponent*>(FORWARDSCENE, new ForwardSceneDemo()));

	for (pair<DEMOS, RenderableComponent*> x : demos)
	{
		x.second->Initialize();
	}

	ChangeDemo(HELLOWORLD);
}
void Demo::Update()
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
		ChangeDemo(FORWARDSCENE);
	}

	MainComponent::Update();
}
void Demo::Compose()
{
	MainComponent::Compose();

	HudRender();
}

void Demo::CameraControl(){
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
void Demo::CameraReset(){
	wiRenderer::getCamera()->Reset();
}
void Demo::StartLoadingChangeDemo(DEMOS newDemo){
	CameraReset();
	wiRenderer::CleanUpStaticTemp();
	demos[newDemo]->Load();
}
void Demo::FinishLoadingChangeDemo(DEMOS newDemo)
{
	Sleep(150);
	demoScene = newDemo;
	activateComponent(demos[demoScene]);
}
void Demo::ChangeDemo(DEMOS newDemo){
	if (demoScene == LOADINGSCREEN)
	{
		return;
	}

	static_cast<wiSoundEffect*>(wiResourceManager::get("sound/change.wav")->data)->Play();

	if (demos.find(newDemo) != demos.end()){
		LoadingScreenComponent* loading = dynamic_cast<LoadingScreenComponent*>(demos[LOADINGSCREEN]);
		if (loading != nullptr)
		{
			loading->Unload();
			for (int i = 0; i < 10; ++i)
			{
				//Added some fake loading simulation tasks
				//just to see it visually, because these demos load in one task
				loading->addLoadingFunction(bind(Sleep, i * 20));
			}
			loading->addLoadingFunction(bind(&Demo::StartLoadingChangeDemo, this, newDemo));
			loading->onFinished(bind(&Demo::FinishLoadingChangeDemo, this, newDemo));
			demoScene = LOADINGSCREEN;
			activateComponent(demos[demoScene]);
		}
		//StartLoadingChangeDemo(newDemo);
		//FinishLoadingChangeDemo(newDemo);
	}

}
void Demo::HudRender(){
	if (demoScene == LOADINGSCREEN)
		return;

	stringstream ss("");
	ss << "Wicked Engine v" << WICKED_ENGINE_VERSION;
#ifdef _DEBUG
	ss << " [DEBUG]";
#endif
	ss << "\nResolution: " << screenW << " x " << screenH;
	ss << "\nDeferred context support: " << (wiRenderer::getMultithreadingSupport() ? "yes" : "no");
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
	ss << "\n[F1] : ForwardScene";
	ss << "\n\nControls:\n-----------------\nMove with WASD\nLook with RMB\nChange interaction type with MOUSEWHEEL";
	ss << "\nInteraction type: ";
	switch (interactionType)
	{
	case DECAL:
		ss << "DECAL";
		break;
	case WATER:
		ss << "WATER";
		break;
	case SPAWN_OBJECT:
		ss << "SPAWN OBJECT";
		break;
	case SPAWN_LIGHT:
		ss << "SPAWN LIGHT";
		break;
	default:
		break;
	}
	wiFont::Draw(ss.str(), "basic", XMFLOAT4(0, 0, -5, -4), "left", "top");
	ss.str("");
	ss.precision(1);
	ss << fixed << wiFrameRate::FPS() << " FPS";
	ss << "\nCPU: " << wiCpuInfo::GetCpuPercentage() << "%";
	wiFont::Draw(ss.str(), "basic", XMFLOAT4((float)screenW - 15, 0, -5, -4), "right", "top");
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
	case SSRTEST:
		ss << "SSRTEST";
		break;
	default:
		break;
	}
	ss << " DEMO";
	wiFont::Draw(ss.str(), "basic", XMFLOAT4((float)screenW / 2, -(float)screenH, -5, -4), "center", "bottom");
}


void DemoLoadingScreen::Compose()
{
	stringstream ss("");
	ss << "Loading: " << getPercentageComplete() << "%";
	wiFont::Draw(ss.str(),XMFLOAT4(screenW/2.f,-screenH/2.f,10.f,0),"center","center");
}


HelloWorldDemo::HelloWorldDemo(){
	image = wiSprite("HelloWorldDemo/HelloWorld.png");
	image.effects.siz = XMFLOAT2(400, 200);
	image.effects.pos = XMFLOAT3(screenW / 2 - image.effects.siz.x / 2, -screenH / 2 + image.effects.siz.y / 2, 0);
	image.anim.rot = 0.01f;
}
HelloWorldDemo::~HelloWorldDemo(){
	image.CleanUp();
}
void HelloWorldDemo::Update(){
	image.Update(1);
}
void HelloWorldDemo::Compose(){
	wiImage::BatchBegin();
	image.Draw();
}

void BasicModelDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setBloomEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);
	setVolumeLightsEnabled(false);
	setEmitterParticlesEnabled(false);
	setHairParticlesEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void BasicModelDemo::Load()
{
	wiRenderer::LoadModel("BasicModelDemo/barrel/", "barrel");
	wiRenderer::FinishLoading();
}
void BasicModelDemo::Start(){
	ForwardRenderableComponent::Start();
	wiRenderer::objects.front()->translation_rest.y = 2.2f;
}
void BasicModelDemo::Update(){
	ForwardRenderableComponent::Update();

	wiRenderer::objects.back()->transform(XMMatrixRotationRollPitchYaw(0, 0.001f*XM_2PI, 0));
	wiRenderer::UpdateRenderInfo(wiRenderer::getImmediateContext());
}

void SkinnedModelDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setBloomEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);
	setVolumeLightsEnabled(false);
	setEmitterParticlesEnabled(false);
	setHairParticlesEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void SkinnedModelDemo::Load()
{
	wiRenderer::LoadModel("SkinnedModelDemo/", "girl");
	wiRenderer::FinishLoading();
}
void SkinnedModelDemo::Start(){
	ForwardRenderableComponent::Start();
	wiRenderer::objects.front()->translation_rest.y = 2.2f;

	for (unsigned int i = 0; i < wiRenderer::armatures.back()->actions.size(); ++i){
		if (wiRenderer::armatures.back()->actions[i].name.find("Idle") != string::npos){
			wiRenderer::armatures.back()->activeAction = i;
			break;
		}
	}
	wiRenderer::SetToDrawDebugLines(true);
}

void EmittedParticleDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setHairParticlesEnabled(false);
	setVolumeLightsEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void EmittedParticleDemo::Load()
{
	wiRenderer::LoadModel("EmitterParticleDemo/", "emitter");
	wiRenderer::FinishLoading();
}
void EmittedParticleDemo::Start(){
	ForwardRenderableComponent::Start();
	for (Object* o : wiRenderer::objects)
		o->translation_rest.y = 2.2f;
}

void HairParticleDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setBloomEnabled(false);
	setEmitterParticlesEnabled(false);
	setVolumeLightsEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void HairParticleDemo::Load()
{
	wiRenderer::LoadModel("HairParticleDemo/", "hair");
	wiRenderer::FinishLoading();
}
void HairParticleDemo::Start(){
	ForwardRenderableComponent::Start();
	wiHairParticle::Settings(8, 14, 28);
}

void RigidBodyDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setBloomEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);
	setVolumeLightsEnabled(false);
	setEmitterParticlesEnabled(false);
	setHairParticlesEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void RigidBodyDemo::Load()
{
	wiRenderer::LoadModel("RigidBodyDemo/", "rigidScene");
	wiRenderer::FinishLoading();
}
void RigidBodyDemo::Start(){
	ForwardRenderableComponent::Start();
}

void SoftBodyDemo::Initialize()
{
	setFXAAEnabled(false);
	setColorGradingEnabled(false);
	setBloomEnabled(false);
	setLightShaftsEnabled(false);
	setLensFlareEnabled(false);
	setVolumeLightsEnabled(false);
	setEmitterParticlesEnabled(false);
	setHairParticlesEnabled(false);

	ForwardRenderableComponent::Initialize();
}
void SoftBodyDemo::Load()
{
	wiRenderer::LoadModel("SoftBodyDemo/", "flag");
	wiRenderer::FinishLoading();
}
void SoftBodyDemo::Start(){
	ForwardRenderableComponent::Start();
}

void DeferredLightDemo::Initialize()
{
	setSSAOEnabled(true);
	setSSREnabled(false);

	DeferredRenderableComponent::Initialize();
}
void DeferredLightDemo::Load()
{
	wiRenderer::LoadModel("DeferredSceneDemo/lightBenchmark/", "lightBenchmark");
	wiRenderer::FinishLoading();
}
void DeferredLightDemo::Start(){
	DeferredRenderableComponent::Start();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void DeferredSceneDemo::Initialize()
{
	setSSAOEnabled(true);
	setSSREnabled(false);
	setReflectionsEnabled(true);

	DeferredRenderableComponent::Initialize();
}
void DeferredSceneDemo::Load()
{
	wiRenderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	wiRenderer::FinishLoading();
}
void DeferredSceneDemo::Start(){
	DeferredRenderableComponent::Start();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void ForwardSceneDemo::Initialize()
{
	setReflectionsEnabled(true);

	ForwardRenderableComponent::Initialize();
}
void ForwardSceneDemo::Load()
{
	wiRenderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	wiRenderer::FinishLoading();
}
void ForwardSceneDemo::Start(){
	ForwardRenderableComponent::Start();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void SSRTestDemo::Initialize()
{
	setSSAOEnabled(true);
	setSSREnabled(true);

	DeferredRenderableComponent::Initialize();
}
void SSRTestDemo::Load()
{
	wiRenderer::LoadModel("DeferredSceneDemo/ssrtest/", "ssrtest");
	wiRenderer::FinishLoading();
}
void SSRTestDemo::Start(){
	DeferredRenderableComponent::Start();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

