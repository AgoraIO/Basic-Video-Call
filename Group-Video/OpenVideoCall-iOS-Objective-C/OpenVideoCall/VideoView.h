//
//  VideoView.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/11/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MediaInfo.h"

@interface VideoView : UIView
@property (assign, nonatomic) BOOL isVideoMuted;
@property (strong, nonatomic) UIView *videoView;
- (void)updateInfo:(MediaInfo *)info;
@end
