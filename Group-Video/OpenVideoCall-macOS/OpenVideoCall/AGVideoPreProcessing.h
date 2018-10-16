//
//  AGVideoPreProcessing.h
//  OpenVideoCall
//
//  Created by Alex Zheng on 7/28/16.
//  Copyright © 2016 Agora.io All rights reserved.
//

@class AgoraRtcEngineKit;

@interface AGVideoPreProcessing : NSObject
+ (int) registerVideoPreprocessing:(AgoraRtcEngineKit*) kit;
+ (int) deregisterVideoPreprocessing:(AgoraRtcEngineKit*) kit;
@end
