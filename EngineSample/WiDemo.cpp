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
	Renderer::objects.back()->transform(XMMatrixRotationRollPitchYaw(0,0.001f*XM_2PI,0));
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void BasicModelDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, Renderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void BasicModelDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}


SkinnedModelDemo::SkinnedModelDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
SkinnedModelDemo::~SkinnedModelDemo(){}
void SkinnedModelDemo::Start(){
	Renderer::LoadModel("SkinnedModelDemo/", "girl");
	Renderer::FinishLoading();
	Renderer::SetToDrawDebugLines(true);
	Renderer::objects.front()->translation_rest.y = 2.2f;

	for (int i = 0; i < Renderer::armatures.back()->actions.size(); ++i){
		if (Renderer::armatures.back()->actions[i].name.find("Idle") != string::npos){
			Renderer::armatures.back()->activeAction = i;
			break;
		}
	}
}
void SkinnedModelDemo::Update(){
	Renderer::Update();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void SkinnedModelDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, Renderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
		Renderer::DrawDebugLines(Renderer::cam->View, Renderer::immediateContext);
	}
}
void SkinnedModelDemo::Compose(){
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
			, false, Renderer::SHADED_NONE
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
	HairParticle::Settings(8, 14, 28);
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
			, false, Renderer::SHADED_NONE
			, nullptr, true);
	}
}
void HairParticleDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}


RigidBodyDemo::RigidBodyDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
RigidBodyDemo::~RigidBodyDemo(){}
void RigidBodyDemo::Start(){
	Renderer::LoadModel("RigidBodyDemo/", "rigidScene");
	Renderer::FinishLoading();
}
void RigidBodyDemo::Update(){
	Renderer::Update();
	Renderer::SychronizeWithPhysicsEngine();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
}
void RigidBodyDemo::Render(){
	renderTarget.Activate(Renderer::immediateContext, 0, 0, 0, 1);
	{
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, false, 0, Renderer::immediateContext
			, false, Renderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void RigidBodyDemo::Compose(){
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
			, false, Renderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void SoftBodyDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	Image::BatchBegin();
	Image::Draw(renderTarget.shaderResource.back(), fx);
}


DeferredDemo::DeferredDemo(){
	static const float lightShaftQuality = .4f;
	static const int bloomPassCount = 4;
	static const float bloomDownSample = 4.f;
	static const float particleDownSample = 1.0f;
	static const float reflectionDownSample = 0.5f;
	static const float ssaoQuality = 0.3f;

	rtSun.resize(2);
	rtSun[0].Initialize(
		screenW
		, screenH
		, 1, true
		);
	rtSun[1].Initialize(
		screenW*lightShaftQuality
		, screenH*lightShaftQuality
		, 1, false
		);
	rtLensFlare.Initialize(screenW, screenH, 1, false);

	rtBloom.resize(bloomPassCount + 1);
	for (int i = 0; i<rtBloom.size(); ++i)
		rtBloom[i].Initialize(
		screenW / bloomDownSample
		, screenH / bloomDownSample
		, 1, false
		);

	rtGBuffer.Initialize(
		screenW, screenH
		, 3, true, 1, 0
		,DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	rtDeferred.Initialize(
		screenW, screenH
		, 1, false); 
	rtLinearDepth.Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R32_FLOAT
		);
	rtParticle.Initialize(
		screenW*particleDownSample, screenH*particleDownSample
		, 1, false
		);
	rtParticleAdditive.Initialize(
		screenW*particleDownSample, screenH*particleDownSample
		, 1, false
		);
	rtWater.Initialize(
		screenW, screenH
		, 1, false
		); 
	rtWaterRipple.Initialize(
		screenW
		,screenH
		, 1, false, 1, 0, DXGI_FORMAT_R8G8B8A8_SNORM
		); 
	rtWaterRipple.Activate(Renderer::immediateContext, 0, 0, 0, 0);
	rtTransparent.Initialize(
		screenW, screenH
		, 1, false
		);
	rtLight.Initialize(
		screenW, screenH
		, 1, false, 1, 0
		, DXGI_FORMAT_R11G11B10_FLOAT
		);
	rtVolumeLight.Initialize(
		screenW, screenH
		, 1, false
		);
	rtReflection.Initialize(
		screenW * reflectionDownSample
		, screenH * reflectionDownSample
		, 1, true
		);
	rtFinal[0].Initialize(
		screenW, screenH
		, 1, false);
	rtFinal[1].Initialize(
		screenW, screenH
		, 1, false);

	dtDepthCopy.Initialize(screenW, screenH
		, 1, 0
		);

	rtSSAO.resize(3);
	for (int i = 0; i<rtSSAO.size(); i++)
		rtSSAO[i].Initialize(
		screenW*ssaoQuality,screenH*ssaoQuality
		, 1, false, 1, 0, DXGI_FORMAT_R8_UNORM
		);
	rtSSAO.back().Activate(Renderer::immediateContext, 1, 1, 1, 1);

}
DeferredDemo::~DeferredDemo(){}
void DeferredDemo::Update(){
	Renderer::Update();
	Renderer::UpdateLights();
	Renderer::SychronizeWithPhysicsEngine();
	Renderer::UpdateRenderInfo(Renderer::immediateContext);
	Renderer::UpdateSkinnedVB();
}
void DeferredDemo::Render(){
	RenderReflections();
	RenderShadows();
	RenderScene();
	RenderComposition1();
	RenderBloom();
	RenderLightShafts();
	RenderComposition2();
}
void DeferredDemo::Compose(){

	RenderColorGradedComposition();

}

void DeferredDemo::RenderReflections(){
	static const XMFLOAT4 waterPlane = XMFLOAT4(0, 1, 0, 0);

	rtReflection.Activate(Renderer::immediateContext); {
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->refView, Renderer::cam->View, Renderer::cam->Projection, Renderer::cam->refEye, waterPlane);
		Renderer::DrawWorld(Renderer::cam->refView, false, 0, Renderer::immediateContext
			, false, Renderer::SHADED_NONE
			, nullptr, false, 1);
		Renderer::DrawSky(Renderer::cam->refEye, Renderer::immediateContext);
	}
}
void DeferredDemo::RenderShadows(){
	Renderer::ClearShadowMaps(Renderer::immediateContext);
	Renderer::DrawForShadowMap(Renderer::immediateContext);
}
void DeferredDemo::RenderScene(){
	static const int tessellationQuality = 0;

	Renderer::UpdatePerFrameCB(Renderer::immediateContext);
	ImageEffects fx(screenW, screenH);

	rtGBuffer.Activate(Renderer::immediateContext); {
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, tessellationQuality);
		Renderer::UpdatePerViewCB(Renderer::immediateContext, Renderer::cam->View, Renderer::cam->refView, Renderer::cam->Projection, Renderer::cam->Eye);


		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawWorld(Renderer::cam->View, Renderer::DX11, tessellationQuality, Renderer::immediateContext, false
			, Renderer::SHADED_DEFERRED, rtReflection.shaderResource.front(), true, 2);


	}

	rtLensFlare.Activate(Renderer::immediateContext);
	if (!Renderer::GetRasterizer())
		Renderer::DrawLensFlares(Renderer::immediateContext, rtGBuffer.depth->shaderResource, screenW, screenH);

	rtLinearDepth.Activate(Renderer::immediateContext); {
		Image::BatchBegin(Renderer::immediateContext);
		fx.blendFlag = BLENDMODE_OPAQUE;
		fx.sampleFlag = SAMPLEMODE_CLAMP;
		fx.quality = QUALITY_NEAREST;
		fx.process.setLinDepth(true);
		Image::Draw(rtGBuffer.depth->shaderResource, fx, Renderer::immediateContext);
		fx.process.clear();
	}
	dtDepthCopy.CopyFrom(*rtGBuffer.depth, Renderer::immediateContext);

	rtGBuffer.Set(Renderer::immediateContext); {
		Renderer::DrawDecals(Renderer::cam->View, Renderer::immediateContext, dtDepthCopy.shaderResource);
	}

	rtLight.Activate(Renderer::immediateContext, rtGBuffer.depth); {
		Renderer::DrawLights(Renderer::cam->View, Renderer::immediateContext,
			dtDepthCopy.shaderResource, rtGBuffer.shaderResource[1], rtGBuffer.shaderResource[2]);
	}

	rtVolumeLight.Activate(Renderer::immediateContext, rtGBuffer.depth);
	Renderer::DrawVolumeLights(Renderer::cam->View, Renderer::immediateContext);


	rtDeferred.Activate(Renderer::immediateContext, rtGBuffer.depth); { //DEFERRED+COMPOSITION
		Image::DrawDeferred(rtGBuffer.shaderResource[0]
			, rtLinearDepth.shaderResource.back(), rtLight.shaderResource.front(), rtGBuffer.shaderResource[1]
			, rtSSAO.back().shaderResource.back(), Renderer::immediateContext, STENCILREF_DEFAULT);
		Renderer::DrawSky(Renderer::cam->Eye, Renderer::immediateContext);
	}


	rtParticle.Activate(Renderer::immediateContext, 0, 0, 0, 0);  //OFFSCREEN RENDER ALPHAPARTICLES
	Renderer::DrawSoftParticles(Renderer::cam->Eye, Renderer::cam->View, Renderer::immediateContext, rtLinearDepth.shaderResource.back());
	rtParticleAdditive.Activate(Renderer::immediateContext, 0, 0, 0, 1);  //OFFSCREEN RENDER ADDITIVEPARTICLES
	Renderer::DrawSoftPremulParticles(Renderer::cam->Eye, Renderer::cam->View, Renderer::immediateContext, rtLinearDepth.shaderResource.back());
	rtWater.Activate(Renderer::immediateContext, rtGBuffer.depth); {
		Renderer::DrawWorldWater(Renderer::cam->View, rtDeferred.shaderResource.front(), rtReflection.shaderResource.front(), rtLinearDepth.shaderResource.back()
			, rtWaterRipple.shaderResource.back(), Renderer::immediateContext, 2);
	}
	rtTransparent.Activate(Renderer::immediateContext, rtGBuffer.depth); {
		Renderer::DrawWorldTransparent(Renderer::cam->View, rtDeferred.shaderResource.front(), rtReflection.shaderResource.front(), rtLinearDepth.shaderResource.back()
			, Renderer::immediateContext, 2);
	}
}
void DeferredDemo::RenderBloom(){
	static const float bloomStren = 6.f, bloomThreshold = 0.99f, bloomSaturation = -3.86f;


	ImageEffects fx(screenW, screenH);

	Image::BatchBegin();
	if (rtBloom.size()>2){
		for (int i = 0; i<rtBloom.size() - 1; ++i){
			rtBloom[i].Activate(Renderer::immediateContext);
			if (i == 0){
				fx.bloom.separate = true;
				fx.bloom.threshold = bloomThreshold;
				fx.bloom.saturation = bloomSaturation;
				fx.blendFlag = BLENDMODE_OPAQUE;
				fx.sampleFlag = SAMPLEMODE_CLAMP;
				Image::Draw(rtFinal[0].shaderResource.front(), fx);
			}
			else { //horizontal blurs
				fx.blur = bloomStren;
				fx.blurDir = 0;
				fx.blendFlag = BLENDMODE_OPAQUE;
				Image::Draw(rtBloom[i - 1].shaderResource.back(), fx);
			}
		}

		rtBloom.back().Activate(Renderer::immediateContext);
		//vertical blur
		fx.blur = bloomStren;
		fx.blurDir = 1;
		fx.blendFlag = BLENDMODE_OPAQUE;
		Image::Draw(rtBloom[rtBloom.size() - 2].shaderResource.back(), fx);
	}
}
void DeferredDemo::RenderLightShafts(){
	ImageEffects fx(screenW, screenH);


	rtSun[0].Activate(Renderer::immediateContext, rtGBuffer.depth); {
		Renderer::UpdatePerRenderCB(Renderer::immediateContext, 0);
		Renderer::UpdatePerEffectCB(Renderer::immediateContext, XMFLOAT4(1, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		Renderer::DrawSky(Renderer::cam->Eye, Renderer::immediateContext);
	}

	Image::BatchBegin();
	rtSun[1].Activate(Renderer::immediateContext); {
		ImageEffects fxs = fx;
		fxs.blendFlag = BLENDMODE_ADDITIVE;
		XMVECTOR sunPos = XMVector3Project(Renderer::GetSunPosition() * 100000, 0, 0, screenW, screenH, 0.1f, 1.0f, Renderer::cam->Projection, Renderer::cam->View, XMMatrixIdentity());
		{
			XMStoreFloat2(&fxs.sunPos, sunPos);
			Image::Draw(rtSun[0].shaderResource.back(), fxs, Renderer::immediateContext);
		}
	}
}
void DeferredDemo::RenderComposition1(){
	ImageEffects fx(screenW, screenH);
	Image::BatchBegin();

	rtFinal[0].Activate(Renderer::immediateContext);

	fx.blendFlag = BLENDMODE_OPAQUE;
	Image::Draw(rtDeferred.shaderResource.back(), fx);

	fx.blendFlag = BLENDMODE_ALPHA;
	Image::Draw(rtWater.shaderResource.back(), fx);
	Image::Draw(rtTransparent.shaderResource.back(), fx);
	Image::Draw(rtParticle.shaderResource.back(), fx);
	Image::Draw(rtVolumeLight.shaderResource.back(), fx);

	fx.blendFlag = BLENDMODE_ADDITIVE;
	Image::Draw(rtParticleAdditive.shaderResource.back(), fx);
	Image::Draw(rtLensFlare.shaderResource.back(), fx);
}
void DeferredDemo::RenderComposition2(){
	ImageEffects fx(screenW, screenH);
	Image::BatchBegin();

	rtFinal[1].Activate(Renderer::immediateContext);

	fx.blendFlag = BLENDMODE_OPAQUE;
	fx.process.setFXAA(true);
	Image::Draw(rtFinal[0].shaderResource.back(), fx);
	fx.process.clear();

	fx.blendFlag = BLENDMODE_ADDITIVE;
	Image::Draw(rtBloom.back().shaderResource.back(), fx);
	Image::Draw(rtSun.back().shaderResource.back(), fx);
}
void DeferredDemo::RenderColorGradedComposition(){

	ImageEffects fx(screenW, screenH);
	Image::BatchBegin();

	if (Renderer::GetColorGrading()){
		fx.process.setColorGrade(true);
		fx.setMaskMap(Renderer::GetColorGrading());
	}
	Image::Draw(rtFinal[1].shaderResource.back(), fx);
}


void DeferredLightDemo::Start(){
	Renderer::LoadModel("DeferredSceneDemo/lightBenchmark/", "lightBenchmark");
	Renderer::FinishLoading();
	Renderer::SetEnviromentMap((Renderer::TextureView)ResourceManager::add("DeferredSceneDemo/lightBenchmark/env.dds"));
	Renderer::SetColorGrading((Renderer::TextureView)ResourceManager::add("DeferredSceneDemo/lightBenchmark/colorGrading.dds"));
	HairParticle::Settings(20, 50, 200);
}

void DeferredSceneDemo::Start(){
	Renderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	Renderer::FinishLoading();
	Renderer::SetEnviromentMap((Renderer::TextureView)ResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	Renderer::SetColorGrading((Renderer::TextureView)ResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	HairParticle::Settings(20, 50, 200);
}
