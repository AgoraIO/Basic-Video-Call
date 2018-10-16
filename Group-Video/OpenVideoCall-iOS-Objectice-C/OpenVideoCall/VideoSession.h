//
//  VideoSession.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface VideoSession : NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (strong, nonatomic) UIView *hostingView;
@property (strong, nonatomic) AgoraRtcVideoCanvas *canvas;
@property (assign, nonatomic) CGSize size;
@property (assign, nonatomic) BOOL isVideoMuted;

- (instancetype)initWithUid:(NSUInteger)uid;
+ (instancetype)localSession;
@end
