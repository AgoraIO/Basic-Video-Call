//
//  VideoView.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/11/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "VideoView.h"

@interface VideoView ()
@property (strong, nonatomic) UIView *videoView;
@end

@implementation VideoView
- (void)setIsVideoMuted:(BOOL)isVideoMuted {
    _isVideoMuted = isVideoMuted;
    self.videoView.hidden = isVideoMuted;
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        self.translatesAutoresizingMaskIntoConstraints = NO;
        self.backgroundColor = [UIColor whiteColor];
        
        [self addVideoView];
    }
    
    return self;
}

- (void)addVideoView {
    UIView *videoView = [[UIView alloc] init];
    videoView.translatesAutoresizingMaskIntoConstraints = NO;
    videoView.backgroundColor = [UIColor clearColor];
    [self addSubview:videoView];
    
    NSArray *videoViewH = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[video]|" options:0 metrics:nil views:@{@"video": videoView}];
    NSArray *videoViewV = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[video]|" options:0 metrics:nil views:@{@"video": videoView}];
    [NSLayoutConstraint activateConstraints:videoViewH];
    [NSLayoutConstraint activateConstraints:videoViewV];
    
    self.videoView = videoView;
}
@end
