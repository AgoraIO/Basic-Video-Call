//
//  LastmileViewController.h
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/4.
//  Copyright © 2019 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@class LastmileViewController;
@protocol LastmileVCDataSource <NSObject>
- (AgoraRtcEngineKit *)lastmileVCNeedAgoraKit;
@end

@interface LastmileViewController : UITableViewController
@property (weak, nonatomic) id<LastmileVCDataSource> dataSource;
@end
