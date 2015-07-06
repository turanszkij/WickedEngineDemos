#include "stdafx.h"
#include "WiDemo.h"

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


BasicModelDemo::BasicModelDemo(){
}
BasicModelDemo::~BasicModelDemo(){}
void BasicModelDemo::Start(){
	wiRenderer::LoadModel("BasicModelDemo/barrel/", "barrel");
	wiRenderer::FinishLoading();
	wiRenderer::objects.front()->translation_rest.y = 2.2f;
}


SkinnedModelDemo::SkinnedModelDemo(){
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


EmittedParticleDemo::EmittedParticleDemo(){
}
EmittedParticleDemo::~EmittedParticleDemo(){}
void EmittedParticleDemo::Start(){
	wiRenderer::LoadModel("EmitterParticleDemo/", "emitter");
	wiRenderer::FinishLoading();
	for (Object* o:wiRenderer::objects)
		o->translation_rest.y = 2.2f;
}


HairParticleDemo::HairParticleDemo(){
}
HairParticleDemo::~HairParticleDemo(){}
void HairParticleDemo::Start(){
	wiRenderer::LoadModel("HairParticleDemo/", "hair");
	wiRenderer::FinishLoading();
	wiHairParticle::Settings(8, 14, 28);
}


RigidBodyDemo::RigidBodyDemo(){
}
RigidBodyDemo::~RigidBodyDemo(){}
void RigidBodyDemo::Start(){
	wiRenderer::LoadModel("RigidBodyDemo/", "rigidScene");
	wiRenderer::FinishLoading();
}


SoftBodyDemo::SoftBodyDemo(){
}
SoftBodyDemo::~SoftBodyDemo(){}
void SoftBodyDemo::Start(){
	wiRenderer::LoadModel("SoftBodyDemo/", "flag");
	wiRenderer::FinishLoading();
}


void DeferredLightDemo::Start(){
	ssao = true;
	ssr = false;
	wiRenderer::LoadModel("DeferredSceneDemo/lightBenchmark/", "lightBenchmark");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/lightBenchmark/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void DeferredSceneDemo::Start(){
	ssao = true;
	ssr = false;
	wiRenderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void ForwardSceneDemo::Start(){
	wiRenderer::LoadModel("DeferredSceneDemo/instanceBenchmark2/", "instanceBenchmark2");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void SSRTestDemo::Start(){
	ssao = true;
	ssr = true;
	wiRenderer::LoadModel("DeferredSceneDemo/ssrtest/", "ssrtest");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}

void SoftBodyDeferredDemo::Start(){
	ssao = true;
	ssr = false;
	wiRenderer::LoadModel("SoftBodyDemo/", "flag");
	wiRenderer::FinishLoading();
	wiRenderer::SetEnviromentMap((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/env.dds"));
	wiRenderer::SetColorGrading((wiRenderer::TextureView)wiResourceManager::add("DeferredSceneDemo/instanceBenchmark2/colorGrading.dds"));
	wiHairParticle::Settings(20, 50, 200);
}
