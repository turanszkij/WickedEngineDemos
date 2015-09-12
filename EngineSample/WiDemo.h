#pragma once
#include "WickedEngine.h"

class Demo : public MainComponent
{
private:
	enum DEMOS{
		LOADINGSCREEN,
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
		FORWARDSCENE,
		DEMO_COUNT,
	};
	DEMOS demoScene;
	map<DEMOS, RenderableComponent*> demos;
	void CameraControl();
	void CameraReset(); 
	void StartLoadingChangeDemo(DEMOS newDemo);
	void FinishLoadingChangeDemo(DEMOS newDemo);
	void ChangeDemo(DEMOS newDemo);
	void HudRender();
public:
	Demo();
	~Demo();

	void Initialize();
	void Update();
	void Render();

	enum INTERACT{
		DECAL,
		WATER,
		SPAWN_OBJECT,
		SPAWN_LIGHT,
	};
	INTERACT interactionType;
	bool mousebuttondown;
};

class DemoLoadingScreen : public LoadingScreenComponent
{
public:
	void Load();
	void Update();
	void Compose();
};

class HelloWorldDemo : public Renderable2DComponent
{
public:
	HelloWorldDemo();
	~HelloWorldDemo();

	void Update();
	void Compose();
};

class BasicModelDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
	void Update();
};

class SkinnedModelDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
	void Stop();
};

class EmittedParticleDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class HairParticleDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class RigidBodyDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class SoftBodyDemo : public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class DeferredLightDemo :public DeferredRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class DeferredSceneDemo :public DeferredRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class ForwardSceneDemo :public ForwardRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};

class SSRTestDemo :public DeferredRenderableComponent
{
public:
	void Initialize();
	void Load();
	void Start();
};
