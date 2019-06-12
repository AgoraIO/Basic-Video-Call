//
//  ProfileCell.h
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface DimensionCell : UICollectionViewCell
- (void)updateWithDimension:(CGSize)dimension isSelected:(BOOL)isSelected;
@end
