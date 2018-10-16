//
//  ProfileCell.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface ProfileCell : UITableViewCell
- (void)updateWithProfile:(AgoraVideoProfile)profile isSelected:(BOOL)isSelected;
@end
