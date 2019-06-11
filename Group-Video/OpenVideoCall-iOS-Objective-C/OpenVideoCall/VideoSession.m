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
- (void)setIsVideoMuted:(BOOL)isVideoMuted {
    _isVideoMuted = isVideoMuted;
    ((VideoView *)self.hostingView).isVideoMuted = isVideoMuted;
}

+ (instancetype)localSession {
    return [[VideoSession alloc] initWithUid:0];
}

- (instancetype)initWithUid:(NSUInteger)uid {
    if (self = [super init]) {
        self.uid = uid;
        
        self.hostingView = [[VideoView alloc] init];
        self.hostingView.translatesAutoresizingMaskIntoConstraints = NO;
        
        self.canvas = [[AgoraRtcVideoCanvas alloc] init];
        self.canvas.uid = uid;
        self.canvas.view = ((VideoView *)self.hostingView).videoView;
        self.canvas.renderMode = AgoraVideoRenderModeHidden;
    }
    return self;
}


- (void)updateMediaInfo:(CGSize)resolution fps:(NSInteger)fps {
    MediaInfo *info = [[MediaInfo alloc] initWithDimension:resolution fps:fps];
    [((VideoView *)self.hostingView) updateInfo:info];
}

@end
