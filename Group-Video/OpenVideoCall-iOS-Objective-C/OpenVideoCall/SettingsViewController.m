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

@interface SettingsViewController () <UICollectionViewDelegate, UICollectionViewDataSource, UIPickerViewDelegate, UIPickerViewDataSource>
@property (weak, nonatomic) IBOutlet UICollectionView *dimensionCollectionView;
@property (weak, nonatomic) IBOutlet UIPickerView *frameRatePickerView;
@property (weak, nonatomic) IBOutlet UILabel *frameRateLabel;

@property (assign, nonatomic) AgoraVideoFrameRate selectedFrameRate;
@property (assign, nonatomic) CGSize selectedDimension;
@property (strong, nonatomic) Settings *settings;
@property (strong, nonatomic) NSArray *dimensionList;
@property (strong, nonatomic) NSArray *frameRateList;
@end

@implementation SettingsViewController
#pragma mark - Getter, Setter
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

- (NSArray *)frameRateList {
    if (!_frameRateList) {
        _frameRateList = @[@(AgoraVideoFrameRateFps1),
                           @(AgoraVideoFrameRateFps7),
                           @(AgoraVideoFrameRateFps10),
                           @(AgoraVideoFrameRateFps15),
                           @(AgoraVideoFrameRateFps24),
                           @(AgoraVideoFrameRateFps30)];
    }
    return _frameRateList;
}

- (Settings *)settings {
    return [self.dataSource settingsVCNeedSettings];
}

- (void)setSelectedDimension:(CGSize)selectedDimension {
    _selectedDimension = selectedDimension;
    [self.dimensionCollectionView reloadData];
}

- (void)setSelectedFrameRate:(AgoraVideoFrameRate)selectedFrameRate {
    _selectedFrameRate = selectedFrameRate;
    self.frameRateLabel.text = [self descriptionOfFrameRate:selectedFrameRate];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self updateCollectionViewLayout];
    self.selectedDimension = self.settings.dimension;
    self.selectedFrameRate = self.settings.frameRate;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    NSInteger frameRateChooseIndexSection = 1;
    if (indexPath.section == frameRateChooseIndexSection) {
        [tableView deselectRowAtIndexPath:indexPath animated:YES];
        [self doFrameRatePressed];
    }
}

#pragma mark - Private
- (void)doFrameRatePressed {
    self.frameRatePickerView.hidden = !self.frameRatePickerView.hidden;
    NSUInteger row = [self.frameRateList indexOfObject:@(self.selectedFrameRate)];
    [self.frameRatePickerView selectRow:row inComponent:0 animated:YES];
}

- (void)updateCollectionViewLayout {
    CGFloat itemInteritemSpacing = 8;
    CGFloat collectionViewInteritemSpacing = 19;
    CGFloat screenWidth = [UIScreen mainScreen].bounds.size.width;
    CGFloat width = ((screenWidth - (collectionViewInteritemSpacing * 2) - (itemInteritemSpacing * 2)) / 3);
    CGFloat height = 40;
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc] init];
    layout.minimumLineSpacing = 16;
    layout.minimumInteritemSpacing = itemInteritemSpacing;
    layout.itemSize = CGSizeMake(width, height);
    [self.dimensionCollectionView setCollectionViewLayout:layout animated:YES];
}

#pragma mark - UICollectionViewDelegate, UICollectionViewDataSource
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.dimensionList.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    DimensionCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"DimensionCell" forIndexPath:indexPath];
    CGSize dimension = [self.dimensionList[indexPath.row] CGSizeValue];
    BOOL isSelected = [self sizeIsEqual:dimension right:self.selectedDimension];
    [cell updateWithDimension:dimension isSelected:isSelected];
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    self.selectedDimension = [self.dimensionList[indexPath.row] CGSizeValue];
}

#pragma mark - UIPickerViewDelegate, UIPickerViewDataSource
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return self.frameRateList.count;
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    self.selectedFrameRate = [self.frameRateList[row] integerValue];
    self.frameRatePickerView.hidden = YES;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return [self descriptionOfFrameRate:[self.frameRateList[row] integerValue]];
}

#pragma mark - Others
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 16;
}

- (NSString *)descriptionOfFrameRate:(AgoraVideoFrameRate)frameRate {
    switch (frameRate) {
        case AgoraVideoFrameRateFps1:  return @"1 fps";     break;
        case AgoraVideoFrameRateFps7:  return @"7 fps";     break;
        case AgoraVideoFrameRateFps10: return @"10 fps";    break;
        case AgoraVideoFrameRateFps15: return @"15 fps";    break;
        case AgoraVideoFrameRateFps24: return @"24 fps";    break;
        case AgoraVideoFrameRateFps30: return @"30 fps";    break;
        default:                       return @"unsported"; break;
    }
}

- (BOOL)sizeIsEqual:(CGSize)left right:(CGSize)right {
    if (left.width == right.width && left.height == right.height) {
        return YES;
    }
    return NO;
}
@end
