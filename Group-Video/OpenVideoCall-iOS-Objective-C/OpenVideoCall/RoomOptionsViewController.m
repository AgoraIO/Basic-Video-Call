//
//  RoomOptionsViewController.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/6.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "RoomOptionsViewController.h"

@implementation RoomOptions
@end

@interface RoomOptionsViewController ()
@property (weak, nonatomic) IBOutlet UISwitch *debugSwitch;
@property (weak, nonatomic) RoomOptions *options;
@end

@implementation RoomOptionsViewController
- (RoomOptions *)options {
    return [self.dataSource roomOptionsVCNeedOptions];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.debugSwitch.on = self.options.isDebugMode;
}

- (IBAction)doDebugPressed:(UISwitch *)sender {
    BOOL debugMode = sender.isOn;
    [self.delegate roomOptions:self debugModeDidEnable:debugMode];
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section {
    return 16;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section {
    UIView *view = [[UIView alloc] init];
    view.backgroundColor = [UIColor colorWithRed:229.0 / 255.0 green:229.0 / 255.0 blue:229.0 / 255.0 alpha:1];
    return view;
}

@end
