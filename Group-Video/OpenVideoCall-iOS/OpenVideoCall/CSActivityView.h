//
//  CSActivityView.h
//
//  Created by CavanSu on 17/7/12.
//  Copyright © 2017 CavanSu. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CSActivityView : UIView
@property (nonatomic, assign, readonly) BOOL isAnimating;
@property (nonatomic, strong, nullable) UIColor *dotColor;
@property (nonatomic, assign) CGFloat offsetX;
@property (nonatomic, assign) CGFloat offsetY;

+ (instancetype _Nonnull)activityViewWithSize:(CGFloat)size superview:(UIView * _Nonnull)superview;
- (void)startAnimation;
- (void)stopAnimation;
@end
