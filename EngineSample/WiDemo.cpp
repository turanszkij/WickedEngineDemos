#include "stdafx.h"
#include "WiDemo.h"

int WiDemo::screenW = 0, WiDemo::screenH = 0;

HelloWorldDemo::HelloWorldDemo(){
	image = wiSprite("HelloWorldDemo/HelloWorld.png");
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
	wiImage::BatchBegin();
	image.Draw();
}


BasicModelDemo::BasicModelDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
BasicModelDemo::~BasicModelDemo(){}
void BasicModelDemo::Start(){
	wiRenderer::LoadModel("BasicModelDemo/barrel/", "barrel");
	wiRenderer::FinishLoading();
	wiRenderer::objects.front()->translation_rest.y = 2.2f;
}
void BasicModelDemo::Update(){
	wiRenderer::objects.back()->transform(XMMatrixRotationRollPitchYaw(0,0.001f*XM_2PI,0));
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
}
void BasicModelDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void BasicModelDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
}


SkinnedModelDemo::SkinnedModelDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
SkinnedModelDemo::~SkinnedModelDemo(){}
void SkinnedModelDemo::Start(){
	wiRenderer::LoadModel("SkinnedModelDemo/", "girl");
	wiRenderer::FinishLoading();
	wiRenderer::SetToDrawDebugLines(true);
	wiRenderer::objects.front()->translation_rest.y = 2.2f;

	for (int i = 0; i < wiRenderer::armatures.back()->actions.size(); ++i){
		if (wiRenderer::armatures.back()->actions[i].name.find("Idle") != string::npos){
			wiRenderer::armatures.back()->activeAction = i;
			break;
		}
	}
}
void SkinnedModelDemo::Update(){
	wiRenderer::Update();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
}
void SkinnedModelDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
		wiRenderer::DrawDebugLines(wiRenderer::cam->View, wiRenderer::immediateContext);
	}
}
void SkinnedModelDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
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
	wiRenderer::LoadModel("EmitterParticleDemo/", "emitter");
	wiRenderer::FinishLoading();
	for (Object* o:wiRenderer::objects)
		o->translation_rest.y = 2.2f;
}
void EmittedParticleDemo::Update(){
	wiRenderer::Update();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
}
void EmittedParticleDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerFrameCB(wiRenderer::immediateContext);
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_NONE
			, nullptr, true);
	}

	renderTargetLinearDepth.Activate(wiRenderer::immediateContext); {
		ImageEffects fx;
		fx.blendFlag = BLENDMODE_OPAQUE;
		fx.sampleFlag = SAMPLEMODE_CLAMP;
		fx.quality = QUALITY_NEAREST;
		fx.process.setLinDepth(true);
		wiImage::BatchBegin();
		wiImage::Draw(renderTarget.depth->shaderResource, fx);
	}

	renderTargetParticle.Activate(wiRenderer::immediateContext, 0, 0, 0, 0);
	wiRenderer::DrawSoftParticles(wiRenderer::cam->Eye, wiRenderer::cam->View, wiRenderer::immediateContext
		, renderTargetLinearDepth.shaderResource.back());

	renderTargetAdditiveParticle.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	wiRenderer::DrawSoftPremulParticles(wiRenderer::cam->Eye, wiRenderer::cam->View, wiRenderer::immediateContext
		, renderTargetLinearDepth.shaderResource.back());
}
void EmittedParticleDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
	wiImage::Draw(renderTargetParticle.shaderResource.back(), fx);
	fx.blendFlag = BLENDMODE_ADDITIVE;
	wiImage::Draw(renderTargetAdditiveParticle.shaderResource.back(), fx);
}


HairParticleDemo::HairParticleDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
HairParticleDemo::~HairParticleDemo(){}
void HairParticleDemo::Start(){
	wiRenderer::LoadModel("HairParticleDemo/", "hair");
	wiRenderer::FinishLoading();
	wiHairParticle::Settings(8, 14, 28);
}
void HairParticleDemo::Update(){
	wiRenderer::Update();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
}
void HairParticleDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_NONE
			, nullptr, true);
	}
}
void HairParticleDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
}


RigidBodyDemo::RigidBodyDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
RigidBodyDemo::~RigidBodyDemo(){}
void RigidBodyDemo::Start(){
	wiRenderer::LoadModel("RigidBodyDemo/", "rigidScene");
	wiRenderer::FinishLoading();
}
void RigidBodyDemo::Update(){
	wiRenderer::Update();
	wiRenderer::SychronizeWithPhysicsEngine();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
}
void RigidBodyDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void RigidBodyDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
}


SoftBodyDemo::SoftBodyDemo(){
	renderTarget.Initialize(screenW, screenH, 1, true);
}
SoftBodyDemo::~SoftBodyDemo(){}
void SoftBodyDemo::Start(){
	wiRenderer::LoadModel("SoftBodyDemo/", "flag");
	wiRenderer::FinishLoading();
}
void SoftBodyDemo::Update(){
	wiRenderer::Update();
	wiRenderer::SychronizeWithPhysicsEngine();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
	wiRenderer::UpdateSkinnedVB();
}
void SoftBodyDemo::Render(){
	renderTarget.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);
	{
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye, XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_FORWARD_SIMPLE
			, nullptr, true);
	}
}
void SoftBodyDemo::Compose(){
	ImageEffects fx = ImageEffects(screenW, screenH);
	wiImage::BatchBegin();
	wiImage::Draw(renderTarget.shaderResource.back(), fx);
}


DeferredDemo::DeferredDemo() :ssao(true), ssr(true){
	static const float lightShaftQuality = .4f;
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

	rtBloom.resize(3);
	rtBloom[0].Initialize(
		screenW
		, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0
		);
	for (int i = 1; i<rtBloom.size(); ++i)
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
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0);
	rtSSR.Initialize(
		screenW/2, screenH/2
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
	rtLinearDepth.Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R32_FLOAT
		);
	rtParticle.Initialize(
		screenW*particleDownSample, screenH*particleDownSample
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	rtParticleAdditive.Initialize(
		screenW*particleDownSample, screenH*particleDownSample
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	rtWater.Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT
		); 
	rtWaterRipple.Initialize(
		screenW
		,screenH
		, 1, false, 1, 0, DXGI_FORMAT_R8G8B8A8_SNORM
		); 
	rtWaterRipple.Activate(wiRenderer::immediateContext, 0, 0, 0, 0);
	rtTransparent.Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT
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
		, 1, true, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT
		);
	rtFinal[0].Initialize(
		screenW, screenH
		, 1, false, 1, 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
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
	rtSSAO.back().Activate(wiRenderer::immediateContext, 1, 1, 1, 1);

}
DeferredDemo::~DeferredDemo(){}
void DeferredDemo::Update(){
	wiRenderer::Update();
	wiRenderer::UpdateLights();
	wiRenderer::SychronizeWithPhysicsEngine();
	wiRenderer::UpdateRenderInfo(wiRenderer::immediateContext);
	wiRenderer::UpdateSkinnedVB();
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

	wiImage::BatchBegin();
	ImageEffects fx = ImageEffects(0, 0, 200, 200);
	fx.blendFlag = BLENDMODE_OPAQUE;
	fx.mipLevel = 4.5f;
	wiImage::Draw(rtBloom.back().shaderResource[0], fx);
}

void DeferredDemo::RenderReflections(){
	static const XMFLOAT4 waterPlane = XMFLOAT4(0, 1, 0, 0);

	rtReflection.Activate(wiRenderer::immediateContext); {
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->refView, wiRenderer::cam->View, wiRenderer::cam->Projection, wiRenderer::cam->refEye, waterPlane);
		wiRenderer::DrawWorld(wiRenderer::cam->refView, false, 0, wiRenderer::immediateContext
			, false, wiRenderer::SHADED_NONE
			, nullptr, false, 1);
		wiRenderer::DrawSky(wiRenderer::cam->refEye, wiRenderer::immediateContext);
	}
}
void DeferredDemo::RenderShadows(){
	wiRenderer::ClearShadowMaps(wiRenderer::immediateContext);
	wiRenderer::DrawForShadowMap(wiRenderer::immediateContext);
}
void DeferredDemo::RenderScene(){
	static const int tessellationQuality = 0;

	wiRenderer::UpdatePerFrameCB(wiRenderer::immediateContext);
	ImageEffects fx(screenW, screenH);

	rtGBuffer.Activate(wiRenderer::immediateContext); {
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, tessellationQuality);
		wiRenderer::UpdatePerViewCB(wiRenderer::immediateContext, wiRenderer::cam->View, wiRenderer::cam->refView, wiRenderer::cam->Projection, wiRenderer::cam->Eye);


		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(0, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawWorld(wiRenderer::cam->View, wiRenderer::DX11, tessellationQuality, wiRenderer::immediateContext, false
			, wiRenderer::SHADED_DEFERRED, rtReflection.shaderResource.front(), true, 2);


	}

	rtLensFlare.Activate(wiRenderer::immediateContext);
	if (!wiRenderer::GetRasterizer())
		wiRenderer::DrawLensFlares(wiRenderer::immediateContext, rtGBuffer.depth->shaderResource, screenW, screenH);

	rtLinearDepth.Activate(wiRenderer::immediateContext); {
		wiImage::BatchBegin(wiRenderer::immediateContext);
		fx.blendFlag = BLENDMODE_OPAQUE;
		fx.sampleFlag = SAMPLEMODE_CLAMP;
		fx.quality = QUALITY_NEAREST;
		fx.process.setLinDepth(true);
		wiImage::Draw(rtGBuffer.depth->shaderResource, fx, wiRenderer::immediateContext);
		fx.process.clear();
	}
	dtDepthCopy.CopyFrom(*rtGBuffer.depth, wiRenderer::immediateContext);

	rtGBuffer.Set(wiRenderer::immediateContext); {
		wiRenderer::DrawDecals(wiRenderer::cam->View, wiRenderer::immediateContext, dtDepthCopy.shaderResource);
	}

	rtLight.Activate(wiRenderer::immediateContext, rtGBuffer.depth); {
		wiRenderer::DrawLights(wiRenderer::cam->View, wiRenderer::immediateContext,
			dtDepthCopy.shaderResource, rtGBuffer.shaderResource[1], rtGBuffer.shaderResource[2]);
	}

	rtVolumeLight.Activate(wiRenderer::immediateContext, rtGBuffer.depth);
	wiRenderer::DrawVolumeLights(wiRenderer::cam->View, wiRenderer::immediateContext);



	if (ssao){
		wiImage::BatchBegin(wiRenderer::immediateContext, STENCILREF_DEFAULT);
		rtSSAO[0].Activate(wiRenderer::immediateContext); {
			fx.process.setSSAO(true);
			fx.setDepthMap(rtLinearDepth.shaderResource.back());
			fx.setNormalMap(rtGBuffer.shaderResource[1]);
			fx.setMaskMap((ID3D11ShaderResourceView*)wiResourceManager::add("images/noise.png"));
			//fx.sampleFlag=SAMPLEMODE_CLAMP;
			fx.quality = QUALITY_BILINEAR;
			fx.sampleFlag = SAMPLEMODE_MIRROR;
			wiImage::Draw(nullptr, fx, wiRenderer::immediateContext);
			//fx.sampleFlag=SAMPLEMODE_CLAMP;
			fx.process.clear();
		}
		static const float ssaoBlur = 2.f;
		rtSSAO[1].Activate(wiRenderer::immediateContext); {
			fx.blur = ssaoBlur;
			fx.blurDir = 0;
			fx.blendFlag = BLENDMODE_OPAQUE;
			wiImage::Draw(rtSSAO[0].shaderResource.back(), fx, wiRenderer::immediateContext);
		}
		rtSSAO[2].Activate(wiRenderer::immediateContext); {
			fx.blur = ssaoBlur;
			fx.blurDir = 1;
			fx.blendFlag = BLENDMODE_OPAQUE;
			wiImage::Draw(rtSSAO[1].shaderResource.back(), fx, wiRenderer::immediateContext);
			fx.blur = 0;
		}
	}


	rtDeferred.Activate(wiRenderer::immediateContext, rtGBuffer.depth); {
		wiImage::DrawDeferred(rtGBuffer.shaderResource[0]
			, rtLinearDepth.shaderResource.back(), rtLight.shaderResource.front(), rtGBuffer.shaderResource[1]
			, rtSSAO.back().shaderResource.back(), wiRenderer::immediateContext, STENCILREF_DEFAULT);
		wiRenderer::DrawSky(wiRenderer::cam->Eye, wiRenderer::immediateContext);
	}


	if (ssr){
		rtSSR.Activate(wiRenderer::immediateContext); {
			wiImage::BatchBegin(wiRenderer::immediateContext);
			wiRenderer::immediateContext->GenerateMips(rtDeferred.shaderResource[0]);
			fx.process.setSSR(true);
			fx.setDepthMap(dtDepthCopy.shaderResource);
			fx.setNormalMap(rtGBuffer.shaderResource[1]);
			fx.setVelocityMap(rtGBuffer.shaderResource[2]);
			fx.setMaskMap(rtLinearDepth.shaderResource.front());
			wiImage::Draw(rtDeferred.shaderResource.front(), fx, wiRenderer::immediateContext);
			fx.process.clear();
		}
	}


	rtParticle.Activate(wiRenderer::immediateContext, 0, 0, 0, 0);  //OFFSCREEN RENDER ALPHAPARTICLES
	wiRenderer::DrawSoftParticles(wiRenderer::cam->Eye, wiRenderer::cam->View, wiRenderer::immediateContext, rtLinearDepth.shaderResource.back());
	rtParticleAdditive.Activate(wiRenderer::immediateContext, 0, 0, 0, 1);  //OFFSCREEN RENDER ADDITIVEPARTICLES
	wiRenderer::DrawSoftPremulParticles(wiRenderer::cam->Eye, wiRenderer::cam->View, wiRenderer::immediateContext, rtLinearDepth.shaderResource.back());
	rtWater.Activate(wiRenderer::immediateContext, rtGBuffer.depth); {
		wiRenderer::DrawWorldWater(wiRenderer::cam->View, rtDeferred.shaderResource.front(), rtReflection.shaderResource.front(), rtLinearDepth.shaderResource.back()
			, rtWaterRipple.shaderResource.back(), wiRenderer::immediateContext, 2);
	}
	rtTransparent.Activate(wiRenderer::immediateContext, rtGBuffer.depth); {
		wiRenderer::DrawWorldTransparent(wiRenderer::cam->View, rtDeferred.shaderResource.front(), rtReflection.shaderResource.front(), rtLinearDepth.shaderResource.back()
			, wiRenderer::immediateContext, 2);
	}
}
void DeferredDemo::RenderBloom(){
	static const float bloomStren = 19.3f, bloomThreshold = 0.99f, bloomSaturation = -3.86f;


	ImageEffects fx(screenW, screenH);

	wiImage::BatchBegin();

	rtBloom[0].Activate(wiRenderer::immediateContext); 
	{
		fx.bloom.separate = true;
		fx.blendFlag = BLENDMODE_OPAQUE;
		fx.sampleFlag = SAMPLEMODE_CLAMP;
		wiImage::Draw(rtFinal[0].shaderResource.front(), fx);
	}


	rtBloom[1].Activate(wiRenderer::immediateContext); //horizontal
	{
		wiRenderer::immediateContext->GenerateMips(rtBloom[0].shaderResource[0]);
		fx.mipLevel = 5.32f;
		fx.blur = bloomStren;
		fx.blurDir = 0;
		fx.blendFlag = BLENDMODE_OPAQUE;
		wiImage::Draw(rtBloom[0].shaderResource.back(), fx);
	}
	rtBloom[2].Activate(wiRenderer::immediateContext); //vertical
	{
		wiRenderer::immediateContext->GenerateMips(rtBloom[0].shaderResource[0]);
		fx.blur = bloomStren;
		fx.blurDir = 1;
		fx.blendFlag = BLENDMODE_OPAQUE;
		wiImage::Draw(rtBloom[1].shaderResource.back(), fx);
	}


	//if (rtBloom.size()>2){
	//	for (int i = 0; i<rtBloom.size() - 1; ++i){
	//		rtBloom[i].Activate(wiRenderer::immediateContext);
	//		if (i == 0){
	//			fx.bloom.separate = true;
	//			fx.bloom.threshold = bloomThreshold;
	//			fx.bloom.saturation = bloomSaturation;
	//			fx.blendFlag = BLENDMODE_OPAQUE;
	//			fx.sampleFlag = SAMPLEMODE_CLAMP;
	//			wiImage::Draw(rtFinal[0].shaderResource.front(), fx);
	//		}
	//		else { //horizontal blurs
	//			if (i == 1)
	//			{
	//				wiRenderer::immediateContext->GenerateMips(rtBloom[0].shaderResource[0]);
	//			}
	//			fx.mipLevel = 4;
	//			fx.blur = bloomStren;
	//			fx.blurDir = 0;
	//			fx.blendFlag = BLENDMODE_OPAQUE;
	//			wiImage::Draw(rtBloom[i - 1].shaderResource.back(), fx);
	//		}
	//	}

	//	rtBloom.back().Activate(wiRenderer::immediateContext);
	//	//vertical blur
	//	fx.blur = bloomStren;
	//	fx.blurDir = 1;
	//	fx.blendFlag = BLENDMODE_OPAQUE;
	//	wiImage::Draw(rtBloom[rtBloom.size() - 2].shaderResource.back(), fx);
	//}
}
void DeferredDemo::RenderLightShafts(){
	ImageEffects fx(screenW, screenH);


	rtSun[0].Activate(wiRenderer::immediateContext, rtGBuffer.depth); {
		wiRenderer::UpdatePerRenderCB(wiRenderer::immediateContext, 0);
		wiRenderer::UpdatePerEffectCB(wiRenderer::immediateContext, XMFLOAT4(1, 0, 0, 0), XMFLOAT4(0, 0, 0, 0));
		wiRenderer::DrawSky(wiRenderer::cam->Eye, wiRenderer::immediateContext);
	}

	wiImage::BatchBegin();
	rtSun[1].Activate(wiRenderer::immediateContext); {
		ImageEffects fxs = fx;
		fxs.blendFlag = BLENDMODE_ADDITIVE;
		XMVECTOR sunPos = XMVector3Project(wiRenderer::GetSunPosition() * 100000, 0, 0, screenW, screenH, 0.1f, 1.0f, wiRenderer::cam->Projection, wiRenderer::cam->View, XMMatrixIdentity());
		{
			XMStoreFloat2(&fxs.sunPos, sunPos);
			wiImage::Draw(rtSun[0].shaderResource.back(), fxs, wiRenderer::immediateContext);
		}
	}
}
void DeferredDemo::RenderComposition1(){
	ImageEffects fx(screenW, screenH);
	wiImage::BatchBegin();

	rtFinal[0].Activate(wiRenderer::immediateContext);

	fx.blendFlag = BLENDMODE_OPAQUE;
	wiImage::Draw(rtDeferred.shaderResource.back(), fx);

	fx.blendFlag = BLENDMODE_ALPHA;
	if (ssr){
		wiImage::Draw(rtSSR.shaderResource.back(), fx);
	}
	wiImage::Draw(rtWater.shaderResource.back(), fx);
	wiImage::Draw(rtTransparent.shaderResource.back(), fx);
	wiImage::Draw(rtParticle.shaderResource.back(), fx);

	fx.blendFlag = BLENDMODE_ADDITIVE;
	wiImage::Draw(rtVolumeLight.shaderResource.back(), fx);
	wiImage::Draw(rtParticleAdditive.shaderResource.back(), fx);
	wiImage::Draw(rtLensFlare.shaderResource.back(), fx);
}
void DeferredDemo::RenderComposition2(){
	ImageEffects fx(screenW, screenH);
	wiImage::BatchBegin();

	rtFinal[1].Activate(wiRenderer::immediateContext);

	fx.blendFlag = BLENDMODE_OPAQUE;
	fx.process.setFXAA(true);
	wiImage::Draw(rtFinal[0].shaderResource.back(), fx);
	fx.process.clear();

	fx.blendFlag = BLENDMODE_ADDITIVE;
	wiImage::Draw(rtBloom.back().shaderResource.back(), fx);
	wiImage::Draw(rtSun.back().shaderResource.back(), fx);
}
void DeferredDemo::RenderColorGradedComposition(){

	ImageEffects fx(screenW, screenH);
	wiImage::BatchBegin();

	if (wiRenderer::GetColorGrading()){
		fx.process.setColorGrade(true);
		fx.setMaskMap(wiRenderer::GetColorGrading());
	}
	wiImage::Draw(rtFinal[1].shaderResource.back(), fx);
}


void DeferredLightDemo::Start(){
	wiRenderer::LoadModel("DeferredSceneDemo/lightBenchmark/", "lightBenchmark");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void DeferredSceneDemo::Start(){
	wiRenderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void SSRTestDemo::Start(){
	wiRenderer::LoadModel("DeferredSceneDemo/ssrtest/", "ssrtest");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void SoftBodyDeferredDemo::Start(){
	wiRenderer::LoadModel("SoftBodyDemo/", "flag");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}
