//
//  RoomViewController.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>
#import "EncryptionType.h"

@class RoomViewController;
@protocol RoomVCDelegate <NSObject>
- (void)roomVCNeedClose:(RoomViewController *)roomVC;
@end

@interface RoomViewController : UIViewController
@property (copy, nonatomic) NSString *roomName;
@property (assign, nonatomic) AgoraVideoProfile videoProfile;
@property (assign, nonatomic) EncrypType encrypType;
@property (copy, nonatomic) NSString *encrypSecret;
@property (weak, nonatomic) id<RoomVCDelegate> delegate;
@end
