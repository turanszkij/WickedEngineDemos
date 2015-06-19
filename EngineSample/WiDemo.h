#pragma once
#include "WickedEngine.h"

class WiDemo
{
private:
public:
	static int screenW, screenH;

	virtual void Start() = 0;
	//Update Demo
	virtual void Update() = 0;
	//Render the composition by layers
	virtual void Render() = 0;
	//Compose the rendered layers
	virtual void Compose() = 0;
};

class HelloWorldDemo : public WiDemo
{
private:
	oImage image;
public:
	HelloWorldDemo();
	~HelloWorldDemo();

	void Start();
	void Update();
	void Render();
	void Compose();
};

class BasicModelDemo : public WiDemo
{
private:
	RenderTarget renderTarget;
public:
	BasicModelDemo();
	~BasicModelDemo();

	void Start();
	void Update();
	void Render();
	void Compose();
};

class EmittedParticleDemo : public WiDemo
{
private:
	RenderTarget renderTarget, renderTargetParticle, renderTargetAdditiveParticle, renderTargetLinearDepth;
public:
	EmittedParticleDemo();
	~EmittedParticleDemo();

	void Start();
	void Update();
	void Render();
	void Compose();
};

class HairParticleDemo : public WiDemo
{
private:
	RenderTarget renderTarget;
public:
	HairParticleDemo();
	~HairParticleDemo();

	void Start();
	void Update();
	void Render();
	void Compose();
};

class SoftBodyDemo : public WiDemo
{
private:
	RenderTarget renderTarget;
public:
	SoftBodyDemo();
	~SoftBodyDemo();

	void Start();
	void Update();
	void Render();
	void Compose();
};
