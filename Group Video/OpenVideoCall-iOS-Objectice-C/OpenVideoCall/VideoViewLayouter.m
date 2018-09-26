//
//  VideoViewLayouter.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "VideoViewLayouter.h"

@interface VideoViewLayouter ()
@property (assign, nonatomic) NSInteger MaxPeerCount;
@property (strong, nonatomic) NSMutableArray<NSLayoutConstraint *> *layoutConstraints;
@property (strong, nonatomic) UIScrollView *scrollView;
@property (readonly, nonatomic) NSArray<UIView *> *allViews;
@end

@implementation VideoViewLayouter
- (NSMutableArray<NSLayoutConstraint *> *)layoutConstraints {
    if (!_layoutConstraints) {
        _layoutConstraints = [[NSMutableArray<NSLayoutConstraint *> alloc] init];
    }
    return _layoutConstraints;
}

- (UIScrollView *)scrollView {
    if (!_scrollView) {
        _scrollView = [[UIScrollView alloc] init];
        _scrollView.translatesAutoresizingMaskIntoConstraints = NO;
    }
    return _scrollView;
}

- (NSArray<UIView *> *)allViews {
    NSMutableArray *allViews = [[NSMutableArray alloc] init];
    if (self.videoViews) {
        [allViews addObjectsFromArray:self.videoViews];
    }
    if (self.selfView) {
        [allViews addObject:self.selfView];
    }
    return [allViews copy];
}

- (instancetype)init {
    if (self = [super init]) {
        self.MaxPeerCount = 3;
    }
    return self;
}

- (void)layoutVideoViews {
    if (!self.selfView || !self.containerView ) {
        return;
    }
    
    [self.selfView removeFromSuperview];
    for (UIView *view in self.videoViews) {
        [view removeFromSuperview];
    }
    [self.scrollView removeFromSuperview];
    
    [NSLayoutConstraint deactivateConstraints:self.layoutConstraints];
    [self.layoutConstraints removeAllObjects];
    
    
    NSInteger count = self.videoViews.count;
    if (count == 0) {
        NSArray *fullViewLayouts = [self layoutFullSessionView:self.selfView inContainerView:self.containerView];
        [self.layoutConstraints addObjectsFromArray:fullViewLayouts];
        
    } else if (count == 1) {
        UIView *peerView = self.videoViews.firstObject;
        NSArray *fullViewLayouts = [self layoutFullSessionView:peerView inContainerView:self.containerView];
        [self.layoutConstraints addObjectsFromArray:fullViewLayouts];
        
        NSArray *cornerViewLayouts = [self layoutCornerSessionView:self.selfView inContainerView:self.containerView];
        [self.layoutConstraints addObjectsFromArray:cornerViewLayouts];
        
    } else {
        if (self.fullView) {
            NSArray *fullViewLayouts = [self layoutFullSessionView:self.fullView inContainerView:self.containerView];
            [self.layoutConstraints addObjectsFromArray:fullViewLayouts];
            
            NSMutableArray *smallViews = [[NSMutableArray alloc] init];
            for (UIView *view in self.videoViews) {
                if (view != self.fullView && smallViews.count < self.MaxPeerCount) {
                    [smallViews addObject:view];
                }
            }
            if (smallViews.count < self.MaxPeerCount && self.fullView != self.selfView) {
                [smallViews addObject:self.selfView];
            }
            
            NSArray *smallViewLayouts = [self layoutSmallSessionViews:smallViews inContainerView:self.containerView];
            [self.layoutConstraints addObjectsFromArray:smallViewLayouts];
            
        } else {
            NSArray *layouts = [self layoutEqualSessionViews:self.allViews inContainerView:self.containerView];
            [self.layoutConstraints addObjectsFromArray:layouts];
        }
    }
    
    if (self.layoutConstraints.count) {
        [NSLayoutConstraint activateConstraints:self.layoutConstraints];
    }
}

- (NSInteger)responseIndexOfLocation:(CGPoint)location {
    if (!self.selfView || !self.containerView || self.fullView) {
        return -1;
    }
    
    NSMutableArray *allViews = [[NSMutableArray alloc] init];
    [allViews addObject:self.selfView];
    [allViews addObjectsFromArray:self.videoViews];
    
    for (NSInteger index = 0; index < allViews.count; ++index) {
        UIView *view = allViews[index];
        if (view.superview == self.containerView && CGRectContainsPoint(view.frame, location)) {
            return index;
        }
    }
    
    return -1;
}

- (NSArray<NSLayoutConstraint *> *)layoutFullSessionView:(UIView *)view inContainerView:(UIView *)container {
    NSMutableArray *layouts = [[NSMutableArray alloc] init];
    [container addSubview:view];
    
    NSArray<NSLayoutConstraint *> *constraintsH = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view]|" options:0 metrics:nil views:@{@"view": view}];
    NSArray<NSLayoutConstraint *> *constraintsV = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[view]|" options:0 metrics:nil views:@{@"view": view}];
    [layouts addObjectsFromArray:constraintsH];
    [layouts addObjectsFromArray:constraintsV];
    
    return [layouts copy];
}

- (NSArray<NSLayoutConstraint *> *)layoutCornerSessionView:(UIView *)view inContainerView:(UIView *)container {
    NSMutableArray *layouts = [[NSMutableArray alloc] init];
    [container addSubview:view];
    
    NSLayoutConstraint *right = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeRight relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeRight multiplier:1 constant:-5];
    NSLayoutConstraint *top = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeTop multiplier:1 constant:64];
    NSLayoutConstraint *width = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:120];
    NSLayoutConstraint *height = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:120];
    
    [layouts addObjectsFromArray:@[right, top, width, height]];
    
    return layouts;
}

- (NSArray<NSLayoutConstraint *> *)layoutSmallSessionViews:(NSArray<UIView *> *)smallViews inContainerView:(UIView *)container {
    NSMutableArray *layouts = [[NSMutableArray alloc] init];
    UIView *lastView;
    
    CGFloat itemSpace = 5;
    
    for (UIView *view in smallViews) {
        [container addSubview:view];
        NSLayoutConstraint *viewWidth = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:120];
        NSLayoutConstraint *viewHeight = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1 constant:120];
        
        NSLayoutConstraint *viewTop = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeTop multiplier:1 constant:60 + itemSpace];
        NSLayoutConstraint *viewLeft;
        if (lastView) {
            viewLeft = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:lastView attribute:NSLayoutAttributeRight multiplier:1 constant:itemSpace];
        } else {
            viewLeft = [NSLayoutConstraint constraintWithItem:view attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeLeft multiplier:1 constant:itemSpace];
        }
        
        [layouts addObjectsFromArray:@[viewWidth, viewHeight, viewLeft, viewTop]];
        lastView = view;
    }
    
    return [layouts copy];
}

- (NSArray<NSLayoutConstraint *> *)layoutEqualSessionViews:(NSArray<UIView *> *)allViews inContainerView:(UIView *)container {
    NSMutableArray *layouts = [[NSMutableArray alloc] init];
    NSUInteger viewCount = allViews.count;
    
    if (viewCount == 1) {
        [layouts addObjectsFromArray:[self layoutFullSessionView:allViews.firstObject inContainerView:container]];
    } else if (viewCount == 2) {
        UIView *firstView = allViews.firstObject;
        UIView *lastView = allViews.lastObject;
        [container addSubview:firstView];
        [container addSubview:lastView];
        
        NSArray<NSLayoutConstraint *> *h1 = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view]|" options:0 metrics:nil views:@{@"view": firstView}];
        NSArray<NSLayoutConstraint *> *h2 = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view]|" options:0 metrics:nil views:@{@"view": lastView}];
        NSArray<NSLayoutConstraint *> *v = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": firstView, @"view2": lastView}];
        NSLayoutConstraint *equal = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:lastView attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        
        [layouts addObjectsFromArray:h1];
        [layouts addObjectsFromArray:h2];
        [layouts addObjectsFromArray:v];
        [layouts addObject:equal];
        
    } else if (viewCount == 3) {
        UIView *firstView = allViews.firstObject;
        UIView *secondView = allViews[1];
        UIView *lastView = allViews.lastObject;
        [container addSubview:firstView];
        [container addSubview:secondView];
        [container addSubview:lastView];
        
        NSArray<NSLayoutConstraint *> *h1 = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": firstView, @"view2": secondView}];
        NSArray<NSLayoutConstraint *> *v1 = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": firstView, @"view2": lastView}];
        NSLayoutConstraint *left = [NSLayoutConstraint constraintWithItem:lastView attribute:NSLayoutAttributeLeft relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeLeft multiplier:1 constant:0];
        NSLayoutConstraint *top = [NSLayoutConstraint constraintWithItem:secondView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:container attribute:NSLayoutAttributeTop multiplier:1 constant:0];
        NSLayoutConstraint *equalWidth1 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:secondView attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *equalWidth2 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:lastView attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *equalHeight1 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:secondView attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        NSLayoutConstraint *equalHeight2 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:lastView attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        
        [layouts addObjectsFromArray:h1];
        [layouts addObjectsFromArray:v1];
        [layouts addObjectsFromArray:@[left, top, equalWidth1, equalWidth2, equalHeight1, equalHeight2]];
        
    } else if (viewCount >= 4) {
        UIView *firstView = allViews.firstObject;
        UIView *secondView = allViews[1];
        UIView *thirdView = allViews[2];
        UIView *lastView = allViews.lastObject;
        [container addSubview:firstView];
        [container addSubview:secondView];
        [container addSubview:thirdView];
        [container addSubview:lastView];
        
        NSArray<NSLayoutConstraint *> *h1 = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": firstView, @"view2": secondView}];
        NSArray<NSLayoutConstraint *> *h2 = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": thirdView, @"view2": lastView}];
        NSArray<NSLayoutConstraint *> *v1 = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": firstView, @"view2": thirdView}];
        NSArray<NSLayoutConstraint *> *v2 = [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[view1]-1-[view2]|" options:0 metrics:nil views:@{@"view1": secondView, @"view2": lastView}];
        
        NSLayoutConstraint *equalWidth1 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:secondView attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *equalWidth2 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:thirdView attribute:NSLayoutAttributeWidth multiplier:1 constant:0];
        NSLayoutConstraint *equalHeight1 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:secondView attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        NSLayoutConstraint *equalHeight2 = [NSLayoutConstraint constraintWithItem:firstView attribute:NSLayoutAttributeHeight relatedBy:NSLayoutRelationEqual toItem:thirdView attribute:NSLayoutAttributeHeight multiplier:1 constant:0];
        
        [layouts addObjectsFromArray:h1];
        [layouts addObjectsFromArray:h2];
        [layouts addObjectsFromArray:v1];
        [layouts addObjectsFromArray:v2];
        [layouts addObjectsFromArray:@[equalWidth1, equalWidth2, equalHeight1, equalHeight2]];
    }
    
    return [layouts copy];
}
@end
