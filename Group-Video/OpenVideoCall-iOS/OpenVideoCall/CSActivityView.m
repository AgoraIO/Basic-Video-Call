//
//  CSActivityView.m
//
//  Created by CavanSu on 17/7/12.
//  Copyright © 2017 CavanSu. All rights reserved.
//

#import "CSActivityView.h"

@interface CSActivityView()
@property (nonatomic, strong) CAReplicatorLayer *reLayer;
@property (nonatomic, strong) NSLayoutConstraint *centerX;
@property (nonatomic, strong) NSLayoutConstraint *centerY;
@property (nonatomic, strong) UIColor * defaultColor;
@property (nonatomic, assign) BOOL isAnimating;
@property (nonatomic, assign) CGFloat size;
@property (nonatomic, weak) CALayer *dotLayer;
@end

@implementation CSActivityView

+ (instancetype _Nonnull)activityViewWithSize:(CGFloat)size superview:(UIView *)superview; {
    CSActivityView *activityView = [[CSActivityView alloc] init];
    activityView.backgroundColor = [UIColor clearColor];
    activityView.isAnimating = NO;
    activityView.hidden = YES;
    activityView.size = size;
    
    [superview addSubview:activityView];
    [activityView layoutSelf];
    return activityView;
}

- (void)startAnimation {
    self.isAnimating = YES;
}

- (void)stopAnimation {
    self.isAnimating = NO;
}

#pragma mark - Private
- (void)setIsAnimating:(BOOL)isAnimating {
    if (_isAnimating == isAnimating) {
        return;
    }
    
    _isAnimating = isAnimating;
    
    if (isAnimating) {
        [self privateStartAnimation];
    } else {
        [self privatStopAnimation];
    }
}

- (void)setDotColor:(UIColor *)dotColor {
    _dotColor = dotColor;
    _dotLayer.backgroundColor = dotColor.CGColor;
}

- (void)setOffsetX:(CGFloat)offsetX {
    if (!self.centerX) {
        return;
    }
    
    self.centerX.constant = offsetX;
    [self.superview updateConstraints];
}

- (void)setOffsetY:(CGFloat)offsetY {
    if (!self.centerY) {
        return;
    }
    
    self.centerY.constant = offsetY;
    [self.superview updateConstraints];
}

- (void)layoutSelf {
    CGSize size = CGSizeMake(self.size, self.size);
    self.translatesAutoresizingMaskIntoConstraints = NO;
    
    [self addConstraint:[NSLayoutConstraint constraintWithItem:self
                                                     attribute:NSLayoutAttributeWidth
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:nil
                                                     attribute:NSLayoutAttributeNotAnAttribute
                                                    multiplier:0.0
                                                      constant:size.width]];
    
    [self addConstraint:[NSLayoutConstraint constraintWithItem:self
                                                     attribute:NSLayoutAttributeHeight
                                                     relatedBy:NSLayoutRelationEqual
                                                        toItem:nil
                                                     attribute:NSLayoutAttributeNotAnAttribute
                                                    multiplier:0.0
                                                      constant:size.height]];
    
    self.centerX = [NSLayoutConstraint constraintWithItem:self
                                                attribute:NSLayoutAttributeCenterX
                                                relatedBy:NSLayoutRelationEqual
                                                   toItem:self.superview
                                                attribute:NSLayoutAttributeCenterX
                                               multiplier:1.0
                                                 constant:0];
    
    [self.superview addConstraint: self.centerX];
    
    self.centerY = [NSLayoutConstraint constraintWithItem:self
                                                attribute:NSLayoutAttributeCenterY
                                                relatedBy:NSLayoutRelationEqual
                                                   toItem:self.superview
                                                attribute:NSLayoutAttributeCenterY
                                               multiplier:1.0
                                                 constant:0];
    
    [self.superview addConstraint: self.centerY];
}

- (void)privateStartAnimation {
    [self reLayer];
    
    [UIView animateWithDuration:0.2 animations:^{
        self.hidden = NO;
    } completion:^(BOOL finished) {
        [self startOpacityDot: YES];
    }];
}

- (void)privatStopAnimation {
    [UIView animateWithDuration:0.2 animations:^{
        self.hidden = YES;
    } completion:^(BOOL finished) {
        [self startOpacityDot: NO];
    }];
}

- (void)startScaleDot:(BOOL)start {
    if (start) {
        CABasicAnimation *anim = [CABasicAnimation animation];
        anim.keyPath = @"transform.scale";
        anim.fromValue = @1;
        anim.toValue = @0.1;
        anim.duration = 2;
        anim.repeatCount = MAXFLOAT;
        self.dotLayer.transform = CATransform3DMakeScale(0, 0, 0);
        [self.dotLayer addAnimation:anim forKey:nil];
    } else {
        [self.dotLayer removeAllAnimations];
    }
}

- (void)startOpacityDot:(BOOL)start {
    if (start) {
        CABasicAnimation *anim = [CABasicAnimation animation];
        anim.keyPath = @"opacity";
        anim.fromValue = @1.0;
        anim.toValue = @0.0;
        anim.duration = 1;
        anim.repeatCount = MAXFLOAT;
        self.dotLayer.opacity = 0.0;
        [self.dotLayer addAnimation:anim forKey:nil];
    } else {
        [self.dotLayer removeAllAnimations];
    }
}

- (CAReplicatorLayer *)reLayer {
    if (!_reLayer) {
        _reLayer = [CAReplicatorLayer layer];
        _reLayer.frame = CGRectMake(0, 0, self.size, self.size);
        [self.layer addSublayer:_reLayer];
        
        NSInteger dotCount = 10;
        UIColor *dotColor = (self.dotColor ? self.dotColor : self.defaultColor);
        CGFloat dotSize = 8;
        
        CALayer *dotLayer = [CALayer layer];
        dotLayer.bounds = CGRectMake(0, 0, dotSize, dotSize);
        dotLayer.position = CGPointMake(self.size * 0.5, self.size - 10);
        dotLayer.cornerRadius = dotSize * 0.5;
        dotLayer.backgroundColor = dotColor.CGColor;
        [_reLayer addSublayer:dotLayer];
        
        CGFloat angle = M_PI * 2 / dotCount;
        
        _reLayer.instanceCount = dotCount;
        _reLayer.instanceDelay = 0.1;
        _reLayer.instanceTransform = CATransform3DMakeRotation(angle, 0, 0, 1);
        _reLayer.instanceColor = dotColor.CGColor;
        self.dotLayer = dotLayer;
    }
    return _reLayer;
}

- (UIColor *)defaultColor {
    if (!_defaultColor) {
        _defaultColor = [UIColor blueColor];
    }
    return _defaultColor;
}

@end
