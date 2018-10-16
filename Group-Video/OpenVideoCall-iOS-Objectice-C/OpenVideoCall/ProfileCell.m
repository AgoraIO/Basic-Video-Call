//
//  ProfileCell.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "ProfileCell.h"

@interface ProfileCell()
@property (weak, nonatomic) IBOutlet UILabel *resLabel;
@property (weak, nonatomic) IBOutlet UILabel *frameLabel;
@end

@implementation ProfileCell

- (void)updateWithProfile:(AgoraVideoProfile)profile isSelected:(BOOL)isSelected {
    self.resLabel.text = [self resolutionOfProfile:profile];
    self.frameLabel.text = [self fpsOfProfile:profile];
    self.backgroundColor = isSelected ? [UIColor colorWithRed:0 green:0 blue:0.5 alpha:0.3] : [UIColor whiteColor];
}

- (NSString *)resolutionOfProfile:(AgoraVideoProfile)profile {
    switch (profile) {
        case AgoraVideoProfileLandscape120P: return @"160×120"; break;
        case AgoraVideoProfileLandscape180P: return @"320×180"; break;
        case AgoraVideoProfileLandscape240P: return @"320×240"; break;
        case AgoraVideoProfileLandscape360P: return @"640×360"; break;
        case AgoraVideoProfileLandscape480P: return @"640×480"; break;
        case AgoraVideoProfileLandscape720P: return @"1280×720"; break;
        default: return @""; break;
    }
}

- (NSString *)fpsOfProfile:(AgoraVideoProfile)profile {
    return @"15";
}

@end
