//
//  ProfileCell.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "DimensionCell.h"
#import "CommonExtension.h"

@interface DimensionCell()
@property (weak, nonatomic) IBOutlet UILabel *dimensionLabel;
@end

@implementation DimensionCell
- (void)updateWithDimension:(CGSize)dimension isSelected:(BOOL)isSelected {
    self.dimensionLabel.text = [NSString stringWithFormat:@"%.0fx%.0f", dimension.width, dimension.height];
    self.dimensionLabel.textColor = isSelected ? UIColor.whiteColor : UIColor.AGTextGray;
    self.dimensionLabel.backgroundColor = isSelected ? UIColor.AGBlue : UIColor.whiteColor;
    self.dimensionLabel.layer.borderColor = isSelected ? UIColor.AGBlue.CGColor : UIColor.AGGray.CGColor;
}
@end
