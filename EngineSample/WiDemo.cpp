#include "stdafx.h"
#include "WiDemo.h"

int WiDemo::screenW = 0, WiDemo::screenH = 0;

HelloWorldDemo::HelloWorldDemo(){
	image = oImage("HelloWorldDemo/HelloWorld.png");
	image.effects.siz = XMFLOAT2(400, 200);
	image.effects.pos = XMFLOAT3(screenW / 2 - image.effects.siz.x / 2, -screenH / 2 + image.effects.siz.y / 2, 0);
	image.anim.rot = 0.01f;
}
HelloWorldDemo::~HelloWorldDemo(){
	image.CleanUp();
}
void HelloWorldDemo::Start(){}
void HelloWorldDemo::Update(){
	image.Update(1);
}
void HelloWorldDemo::Render(){}
void HelloWorldDemo::Compose(){
	Image::BatchBegin();
	image.Draw();
}


BasicModelDemo::BasicModelDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
BasicModelDemo::~BasicModelDemo(){}
void BasicModelDemo::Start(){
	Renderer::LoadModel("BasicModelDemo/barrel/", "barrel");
	Renderer::FinishLoading();
	Renderer::objects.front()->translation_rest.y = 2.2f;
}
void BasicModelDemo::Update(){
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void BasicModelDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, false
			, nullptr, true);
	}
}
void BasicModelDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}


EmittedParticleDemo::EmittedParticleDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
	static const float particleDownSample = 1.0f;
	renderTargetParticle.Initialize(screenW*particleDownSample, screenH*particleDownSample, 1, false);
	renderTargetAdditiveParticle.Initialize(screenW*particleDownSample, screenH*particleDownSample, 1, false);
	renderTargetLinearDepth.Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R32_FLOAT
		);
}
EmittedParticleDemo::~EmittedParticleDemo(){}
void EmittedParticleDemo::Start(){
	Renderer::LoadModel("EmitterParticleDemo/", "emitter");
	Renderer::FinishLoading();
	for (Object* o:Renderer::objects)
		o->translation_rest.y = 2.2f;
}
void EmittedParticleDemo::Update(){
	Renderer::Update();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void EmittedParticleDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerFrameCB(Renderer::immediateContext);
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, false
			, nullptr, true);
	}

	renderTargetLinearDepth.Activate(Renderer::immediateContext); {
		ImageEffects fx;
		fx.blendFlag = BLENDMODE_OPAQUE;
		fx.sampleFlag = SAMPLEMODE_CLAMP;
		fx.quality = QUALITY_NEAREST;
		fx.process.setLinDepth(true);
		Image::BatchBegin();
		Image::Draw(renderTarget.depth->shaderResource, fx);
	}

	renderTargetParticle.Activate(Renderer::immediateContext, 0, 0, 0, 0);
	Renderer::DrawSoftParticles(Renderer::cam->Eye, Renderer::cam->View, Renderer::immediateContext
		, renderTargetLinearDepth.shaderResource.back());

	renderTargetAdditiveParticle.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	Renderer::DrawSoftPremulParticles(Renderer::cam->Eye, Renderer::cam->View, Renderer::immediateContext
		, renderTargetLinearDepth.shaderResource.back());
}
void EmittedParticleDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
	Image::Draw(renderTargetParticle.shaderResource.back(), fx);
	fx.blendFlag = BLENDMODE_ADDITIVE;
	Image::Draw(renderTargetAdditiveParticle.shaderResource.back(), fx);
}


HairParticleDemo::HairParticleDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
HairParticleDemo::~HairParticleDemo(){}
void HairParticleDemo::Start(){
	Renderer::LoadModel("HairParticleDemo/", "hair");
	Renderer::FinishLoading();
}
void HairParticleDemo::Update(){
	Renderer::Update();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void HairParticleDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, false
			, nullptr, true);
	}
}
void HairParticleDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}


SoftBodyDemo::SoftBodyDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
SoftBodyDemo::~SoftBodyDemo(){}
void SoftBodyDemo::Start(){
	Renderer::LoadModel("SoftBodyDemo/", "flag");
	Renderer::FinishLoading();
}
void SoftBodyDemo::Update(){
	Renderer::Update();
	Renderer::SychronizeWithPhysicsEngine();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
	Renderer::UpdateSkinnedVB();
}
void SoftBodyDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, false
			, nullptr, true);
	}
}
void SoftBodyDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}

