LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_frame_shared

LOCAL_MODULE_FILENAME := libgame_frame

LOCAL_SRC_FILES := game_frame.cpp \
                   ../../Classes/AAnimateObject.cpp \
                   ../../Classes/AAudio.cpp \
                   ../../Classes/ACache.cpp \
				   ../../Classes/ADebug.cpp \
                   ../../Classes/AEnvironment.cpp \
                   ../../Classes/AEvent.cpp \
                   ../../Classes/AFixArray.cpp \
                   ../../Classes/ALogger.cpp \
                   ../../Classes/AMachine.cpp \
				   ../../Classes/AMath2d.cpp \
				   ../../Classes/AMath3d.cpp \
                   ../../Classes/AObject.cpp \
                   ../../Classes/ASettings.cpp \
                   ../../Classes/ASprite.cpp \
                   ../../Classes/ASpriteContainer.cpp \
                   ../../Classes/ASpriteModel.cpp \
                   ../../Classes/ASpritesPattern.cpp \
                   ../../Classes/AStage.cpp \
                   ../../Classes/AStageLayer.cpp \
                   ../../Classes/AState.cpp \
                   ../../Classes/AString.cpp \
                   ../../Classes/ATransition.cpp \
                   ../../Classes/AValue.cpp \
				   ../../Classes/AUserData.cpp \
                   ../../Classes/AUtils.cpp \
                   ../../Classes/AWorld.cpp \
                   ../../Classes/AXML.cpp \
                   ../../Classes/TSSpriteBubble.cpp \
                   ../../Classes/TSSpriteButton.cpp \
                   ../../Classes/TSSpriteCommon.cpp \
                   ../../Classes/TSSpriteFish.cpp \
                   ../../Classes/TSSpritePlayer.cpp \
                   ../../Classes/TSStageLogo.cpp \
                   ../../Classes/TSStageLogoLayerMain.cpp \
				   ../../Classes/TSGroupNumber.cpp \
				   ../../Classes/TSTraceLine.cpp \
                   ../../Classes/TSStagePlay.cpp \
                   ../../Classes/TSStagePlayLayerGame.cpp \
                   ../../Classes/TSStagePlayLayerGamePrepare.cpp \
                   ../../Classes/TSStagePlayLayerGameShop.cpp \
                   ../../Classes/TSStagePlayLayerGamePlay.cpp \
                   ../../Classes/TSStagePlayLayerGamePlayPause.cpp \
                   ../../Classes/TSStageTest.cpp \
                   ../../Classes/TSStageTestLayerTest.cpp \
                   ../../Classes/TSStageTestLayerTestBubbles.cpp \
                   ../../Classes/TSWorld.cpp \
                   ../../Classes/AppDelegate.cpp \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../libcacore/Classes                   

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) $(call import-module,cocos2dx)
