//
//  VideoViewLayouter.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VideoSession.h"

@interface VideoViewLayouter : NSObject
@property (strong, nonatomic) UIView *selfView;
@property (assign, nonatomic) CGSize selfSize;
@property (assign, nonatomic) CGSize targetSize;

@property (strong, nonatomic) NSArray<UIView *> *videoViews;
@property (strong, nonatomic) UIView *fullView;
@property (strong, nonatomic) UIView *containerView;

- (void)layoutVideoViews;
- (NSInteger)responseIndexOfLocation:(CGPoint)location;
@end
