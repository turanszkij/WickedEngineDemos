#pragma once
#include "WickedEngine.h"

class DemoLoadingScreen : public LoadingScreenComponent
{
public:
	void Compose();
};

class HelloWorldDemo : public RenderableComponent
{
private:
	wiSprite image;
public:
	HelloWorldDemo();
	~HelloWorldDemo();

	void Update();
	void Compose();
};

class BasicModelDemo : public ForwardRenderableComponent
{
public:
	void Start();
	void Update();
};

class SkinnedModelDemo : public ForwardRenderableComponent
{
public:
	void Start();
};

class EmittedParticleDemo : public ForwardRenderableComponent
{
public:
	void Start();
};

class HairParticleDemo : public ForwardRenderableComponent
{
public:
	void Start();
};

class RigidBodyDemo : public ForwardRenderableComponent
{
public:
	void Start();
};

class SoftBodyDemo : public ForwardRenderableComponent
{
public:
	void Start();
};

class DeferredLightDemo :public DeferredRenderableComponent
{
public:
	void Start();
};

class DeferredSceneDemo :public DeferredRenderableComponent
{
public:
	void Start();
};

class ForwardSceneDemo :public ForwardRenderableComponent
{
public:
	void Start();
};

class SSRTestDemo :public DeferredRenderableComponent
{
public:
	void Start();
};
