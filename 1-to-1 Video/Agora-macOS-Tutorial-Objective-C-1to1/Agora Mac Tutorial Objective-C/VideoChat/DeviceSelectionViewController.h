//
//  DeviceSelectionViewController.h
//  Agora Mac Tutorial Objective-C
//
//  Created by James Fang on 8/9/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface DeviceSelectionViewController : NSViewController <AgoraRtcEngineDelegate>

@property (strong, nonatomic) AgoraRtcEngineKit *agoraKit;

@end
