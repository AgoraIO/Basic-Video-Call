//
//  ProfileCell.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "DimensionCell.h"

@interface DimensionCell()
@property (weak, nonatomic) IBOutlet UILabel *resLabel;
@end

@implementation DimensionCell

- (void)updateWithDimension:(CGSize)dimension isSelected:(BOOL)isSelected {
    self.resLabel.text = [NSString stringWithFormat:@"%.0f x %.0f", dimension.width, dimension.height];
    self.backgroundColor = isSelected ? [UIColor colorWithRed:0 green:0 blue:0.5 alpha:0.3] : [UIColor whiteColor];
}

@end
