//
//  makeTemplate.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-1-15.
//
//

#include "makeTemplate.h"
#include "c3dFileUtils.h"
Cship*makeShipTemplate(int shipID){//必须加static
    //判断文件是否存在
    string clipName="ship";
    clipName+=numberToStr(shipID);
    bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(string("data/global/model/shipModelLibrary/")+clipName);
    if(pathExists==false)return NULL;
    //制作模板
    Cship*pship=new Cship();
    pship->autorelease();
    Cc3dModel*pmodel=new Cc3dModel();
    pmodel->autorelease();
    pship->addModel(pmodel);
    //必须先load config再load model
    string shipClipName="ship";
    shipClipName+=numberToStr(shipID);
    pship->loadConfig("data/global/model/shipModelLibrary/"+shipClipName+"/config.txt");
    loadInfo_model(pmodel, "data/global/model/shipModelLibrary/"+shipClipName,pship->modelScale);
    int nMesh=(int)pmodel->getMeshCount();
    for(int j=0;j<nMesh;j++){
        Cc3dMesh*pmesh=pmodel->getMeshByIndex(j);
        pmesh->getIndexVBO()->genBuffers();
        pmesh->submit(GL_STATIC_DRAW);
    }
    pmodel->setDiffuseRGB(0.8, 0.8, 0.8);
    pmodel->setAmbient(0.5, 0.5, 0.5);
    //alSource
    if(pship->fileName_engineSound!="null"){
        string filePathShort_engineSound="data/global/audio/shipEngineSound/";
        filePathShort_engineSound+=pship->fileName_engineSound;
     //   pship->pALSource_shipEngineSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_engineSound);//模板是永久的
     //   pship->pALSource_shipEngineSound->setIsLooping(true);
        Cc3dALSource*pALSource_shipEngineSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_engineSound,"shipEngineSound");//模板是永久的
        pALSource_shipEngineSound->setIsLooping(true);
        pship->addALSource(pALSource_shipEngineSound);
        
        
    }
    if(pship->fileName_collisionSound!="null"){
        string filePathShort_collisionSound="data/global/audio/shipCollisionSound/";
        filePathShort_collisionSound+=pship->fileName_collisionSound;
     //   pship->pALSource_ship_collision=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_collisionSound);
        Cc3dALSource*source=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(filePathShort_collisionSound,"ship_collision");
        pship->addALSource(source);
    }
    return pship;
}
CbossCell*makeCellTemplate(int cellID){//必须加static
    //判断文件是否存在
    string clipName="cell";
    clipName+=numberToStr(cellID);
    bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist("data/global/model/cellModelLibrary/"+clipName);
    if(pathExists==false)return NULL;
    //制作模板
    CbossCell*pcellTemp=new CbossCell();
    pcellTemp->autorelease();
    //生成pcellTemp->pModelBaseList
    for(int i=0;i<CELLPARTCOUNT;i++){
        Cc3dModel*pmodel=new Cc3dModel();
        pmodel->autorelease();
        pcellTemp->addModel(pmodel);
    }
    
    string modelClipPath="data/global/model/cellModelLibrary/";
    modelClipPath+="cell";
    modelClipPath+=numberToStr(cellID);
    pcellTemp->loadConfigAndMeshes(modelClipPath+"/config.txt",modelClipPath);
    //提交各model数据
    {
        pcellTemp->submitModels(GL_STATIC_DRAW);
    }
    //判断是否飞行，如果飞行，则为其创建一个虚拟ship
    if(pcellTemp->get_isFly()){
        Cship*pship=new Cship();
        pship->autorelease();
        pcellTemp->set_ship(pship);//将ship添加到pcellTemp
        //pship的Rc和Rc_small等参数取与pcellTemp相同
        pship->setRc(pcellTemp->getRc());
        pship->seatPos_local.init(0,0,0,1);
        pship->set_Rc_small(pcellTemp->Rc_small);
        pship->set_H_sinkInWater(pcellTemp->H_sinkInWater);
        pship->initiativeAttack=pcellTemp->initiativeAttack;
        //pship的一些与速度有关的参数要比默认缩小些
        pship->v_pushForward_value/=3;
        pship->v_pushBack_value/=3;
        pship->v_pushUp_value/=3;
        pship->v_pushDown_value/=3;
        pship->a_vyPushDec/=3;
        pship->a_vPushForwardDec/=3;
    }
    //声音
 //   pcellTemp->pALSource_boss_scream=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("data/global/audio/boss_scream.wav");
 //   pcellTemp->pALSource_boss_och=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("data/global/audio/boss_och.wav");
    Cc3dALSource*pALSource_boss_scream=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("data/global/audio/boss_scream.wav","boss_scream");
    Cc3dALSource*pALSource_boss_och=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource("data/global/audio/boss_och.wav","boss_och");
    pcellTemp->addALSource(pALSource_boss_scream);
    pcellTemp->addALSource(pALSource_boss_och);
    return pcellTemp;
}
CshotBall*makeShotBallTemplate(int shotBallID,bool isReuseALSource){//必须加static
    //判断文件是否存在
    string clipName="shotBall";
    clipName+=numberToStr(shotBallID);
    bool pathExists=Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist("data/global/model/shotBallLibrary/"+clipName);
    if(pathExists==false)return NULL;
    //制作模板
    CshotBall*pballTemp=new CshotBall();
    pballTemp->autorelease();
    pballTemp->init();
    {
        string clipPathShort="data/global/model/shotBallLibrary/shotBall";
        clipPathShort+=numberToStr(shotBallID);
        pballTemp->loadConfig(clipPathShort+"/config.txt");//先读config再loadModel
        //加载模型
        loadInfo_model(pballTemp->getModel(), clipPathShort,pballTemp->modelScale);
        //加载音效
        if(pballTemp->fileName_launchSound!="null"){
            string audioClipPath="data/global/audio/shotBallLaunchSound/";
            //pballTemp->pALSource_launchSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioClipPath+pballTemp->fileName_launchSound);//模板是永久的
            Cc3dALSource*pALSource_launchSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioClipPath+pballTemp->fileName_launchSound,"launchSound",isReuseALSource);//模板是永久的
            pballTemp->addALSource(pALSource_launchSound);
        }
        if(pballTemp->fileName_collisionWithSolidSound!="null"){
            string audioClipPath="data/global/audio/shotBallCollisionWithSolidSound/";
            //pballTemp->pALSource_collisionWithSolidSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioClipPath+pballTemp->fileName_collisionWithSolidSound);
            Cc3dALSource*pALSource_collisionWithSolidSound=Cc3dAudioCache::sharedAudioCache()->createBufferAndSource(audioClipPath+pballTemp->fileName_collisionWithSolidSound,"collisionWithSolidSound",isReuseALSource);
            assert(pALSource_collisionWithSolidSound);
            pballTemp->addALSource(pALSource_collisionWithSolidSound);
        }
        //创建indexVBO并提交数据
        int nMesh=(int)pballTemp->getModel()->getMeshCount();
        for(int j=0;j<nMesh;j++){
            Cc3dMesh*pmesh=pballTemp->getModel()->getMeshByIndex(j);
            pmesh->getIndexVBO()->genBuffers();
            pmesh->submit(GL_STATIC_DRAW);
        }
        pballTemp->set_typeID(shotBallID);
    }
    return pballTemp;
}
