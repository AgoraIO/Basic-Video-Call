//
//  RoomViewController.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>
#import "Encryption.h"
#import "Settings.h"

@class RoomViewController;
@protocol RoomVCDataSource <NSObject>
- (AgoraRtcEngineKit *)roomVCNeedAgoraKit;
- (Settings *)roomVCNeedSettings;
@end

@interface RoomViewController : UIViewController
@property (weak, nonatomic) id<RoomVCDataSource> dataSource;
@end
