//
//  SettingsViewController.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@class SettingsViewController;
@protocol SettingsVCDelegate <NSObject>
- (void)settingsVC:(SettingsViewController *)settingsVC didSelectDimension:(CGSize)dimension;
@end

@interface SettingsViewController : UIViewController
@property (assign, nonatomic) CGSize dimension;
@property (weak, nonatomic) id<SettingsVCDelegate> delegate;
@end
