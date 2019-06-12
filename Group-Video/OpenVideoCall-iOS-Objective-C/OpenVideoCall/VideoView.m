//
//  VideoView.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/11/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "VideoView.h"

@interface VideoView ()
@property (weak, nonatomic) UIView *infoView;
@property (weak, nonatomic) UILabel *infoLabel;
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
        [self addInfoView];
    }
    
    return self;
}

- (void)updateInfo:(MediaInfo *)info {
    self.infoLabel.text = info.description;
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

- (void)addInfoView {
    UIView *infoView = [[UIView alloc] init];
    infoView.translatesAutoresizingMaskIntoConstraints = NO;
    infoView.backgroundColor = UIColor.clearColor;
    
    [self addSubview:infoView];
    self.infoView = infoView;
    

    NSArray *infoViewH = [NSLayoutConstraint constraintsWithVisualFormat: @"H:|[info]|" options: 0 metrics: nil views: @{@"info": infoView}];
    NSArray *infoViewV = [NSLayoutConstraint constraintsWithVisualFormat: @"V:|[info(==135)]" options: 0 metrics: nil views: @{@"info": infoView}];
    
    [NSLayoutConstraint activateConstraints:infoViewH];
    [NSLayoutConstraint activateConstraints:infoViewV];
    

    UILabel *infoLabel = [self createInfoLabel];
    [infoView addSubview:infoLabel];
    self.infoLabel = infoLabel;
    
    NSArray *labelV = [NSLayoutConstraint constraintsWithVisualFormat: @"V:|-(20)-[info]" options: 0 metrics: nil views: @{@"info": infoLabel}];
    NSArray *labelH = [NSLayoutConstraint constraintsWithVisualFormat: @"H:|-(10)-[info]" options: 0 metrics: nil views: @{@"info": infoLabel}];
    [NSLayoutConstraint activateConstraints:labelV];
    [NSLayoutConstraint activateConstraints:labelH];
}

- (UILabel *)createInfoLabel {
    UILabel *label = [[UILabel alloc] init];
    label.translatesAutoresizingMaskIntoConstraints = NO;
    
    label.text = @" ";
    label.shadowOffset = CGSizeMake(0, 1);
    label.shadowColor = UIColor.blackColor;
    label.numberOfLines = 0;
    label.font = [UIFont systemFontOfSize:12];
    label.textColor = UIColor.whiteColor;
    
    return label;
}

@end
