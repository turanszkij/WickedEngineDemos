#pragma once
#include "WickedEngine.h"

class Demo : public MainComponent
{
public:
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
		SKINTEST,
		DEMO_COUNT,
	};
	DEMOS demoScene;
	std::map<DEMOS, RenderableComponent*> demos;
	void CameraControl(float dt);
	void CameraReset(); 
	void StartLoadingChangeDemo(DEMOS newDemo);
	void FinishLoadingChangeDemo(DEMOS newDemo);
	void ChangeDemo(DEMOS newDemo);
	void HudRender();
public:
	Demo();
	~Demo();


	virtual void Initialize() override;
	virtual void Update(float dt) override;
	virtual void Render() override;

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
	virtual void Load() override;
	virtual void Update(float dt) override;
	virtual void Compose() override;
};

class HelloWorldDemo : public Renderable2DComponent
{
private:
	wiButton button;
	wiSlider slider;
	wiLabel label;
public:
	HelloWorldDemo();
	~HelloWorldDemo();

	virtual void Update(float dt) override;
	virtual void Compose() override;
};

class BasicModelDemo : public DeferredRenderableComponent
{
private:
	Model* model;
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
	virtual void Update(float dt) override;
};

class SkinnedModelDemo : public ForwardRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
	virtual void Stop() override;
};

class EmittedParticleDemo : public ForwardRenderableComponent
{
private:
	Model* model;
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class HairParticleDemo : public ForwardRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class RigidBodyDemo : public ForwardRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class SoftBodyDemo : public ForwardRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class DeferredLightDemo :public DeferredRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class DeferredSceneDemo :public DeferredRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class ForwardSceneDemo :public TiledForwardRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class SSRTestDemo :public DeferredRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};

class SkinTestDemo :public DeferredRenderableComponent
{
public:
	virtual void Initialize() override;
	virtual void Load() override;
	virtual void Start() override;
};
