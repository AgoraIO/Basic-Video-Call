//
//  SettingsViewController.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "SettingsViewController.h"
#import "DimensionCell.h"
#import <AgoraRtcEngineKit/AgoraRtcEngineKit.h>

@interface SettingsViewController () <UITableViewDataSource, UITableViewDelegate>
@property (weak, nonatomic) IBOutlet UITableView *profileTableView;
@property (strong, nonatomic) NSArray *dimensionList;
@end

@implementation SettingsViewController
- (NSArray *)dimensionList {
    if (!_dimensionList) {
        _dimensionList = @[@(AgoraVideoDimension160x120),
                           @(AgoraVideoDimension240x180),
                           @(AgoraVideoDimension320x240),
                           @(AgoraVideoDimension640x360),
                           @(AgoraVideoDimension640x480),
                           @(AgoraVideoDimension960x720)];
    }
    return _dimensionList;
}

- (void)setDimension:(CGSize)dimension {
    _dimension = dimension;
    [self.profileTableView reloadData];
}

- (IBAction)doConfirmPressed:(UIButton *)sender {
    if ([self.delegate respondsToSelector:@selector(settingsVC:didSelectDimension:)]) {
        [self.delegate settingsVC:self didSelectDimension:self.dimension];
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.dimensionList.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    DimensionCell *cell = [tableView dequeueReusableCellWithIdentifier:@"profileCell" forIndexPath:indexPath];
    CGSize dimension = [self.dimensionList[indexPath.row] CGSizeValue];
    [cell updateWithDimension:dimension isSelected:CGSizeEqualToSize(self.dimension, dimension)];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    CGSize selectedDimension = [self.dimensionList[indexPath.row] CGSizeValue];
    self.dimension = selectedDimension;
}
@end
