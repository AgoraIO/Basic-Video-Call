//
//  VideoSession.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "VideoSession.h"
#import "VideoView.h"

@implementation VideoSession
- (instancetype)initWithUid:(NSUInteger)uid {
    if (self = [super init]) {
        self.uid = uid;
        
        self.hostingView = [[VideoView alloc] init];
        self.hostingView.translatesAutoresizingMaskIntoConstraints = NO;
        
        self.canvas = [[AgoraRtcVideoCanvas alloc] init];
        self.canvas.uid = uid;
        self.canvas.view = self.hostingView;
        self.canvas.renderMode = AgoraVideoRenderModeFit;
    }
    return self;
}

- (void)setIsVideoMuted:(BOOL)isVideoMuted {
    _isVideoMuted = isVideoMuted;
    ((VideoView *)self.hostingView).isVideoMuted = isVideoMuted;
}

+ (instancetype)localSession {
    return [[VideoSession alloc] initWithUid:0];
}
@end
