//
//  AGVideoPreProcessing.h
//  OpenVideoCall
//
//  Created by CavanSu on 24/11/2017.
//  Copyright © 2017 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface AGVideoPreProcessing : NSObject
+ (int) registerVideoPreprocessing:(AgoraRtcEngineKit*) kit;
+ (int) deregisterVideoPreprocessing:(AgoraRtcEngineKit*) kit;
@end
