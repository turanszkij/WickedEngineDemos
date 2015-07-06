#pragma once
#include "WickedEngine.h"

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
private:
	wiRenderTarget renderTarget;
public:
	BasicModelDemo();
	~BasicModelDemo();

	void Start();
};

class SkinnedModelDemo : public ForwardRenderableComponent
{
private:
	wiRenderTarget renderTarget;
public:
	SkinnedModelDemo();
	~SkinnedModelDemo();

	void Start();
};

class EmittedParticleDemo : public ForwardRenderableComponent
{
private:
	wiRenderTarget renderTarget, renderTargetParticle, renderTargetAdditiveParticle, renderTargetLinearDepth;
public:
	EmittedParticleDemo();
	~EmittedParticleDemo();

	void Start();
};

class HairParticleDemo : public ForwardRenderableComponent
{
private:
	wiRenderTarget renderTarget;
public:
	HairParticleDemo();
	~HairParticleDemo();

	void Start();
};

class RigidBodyDemo : public ForwardRenderableComponent
{
private:
	wiRenderTarget renderTarget;
public:
	RigidBodyDemo();
	~RigidBodyDemo();

	void Start();
};

class SoftBodyDemo : public ForwardRenderableComponent
{
private:
	wiRenderTarget renderTarget;
public:
	SoftBodyDemo();
	~SoftBodyDemo();

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

class SoftBodyDeferredDemo :public DeferredRenderableComponent
{
public:
	void Start();
};