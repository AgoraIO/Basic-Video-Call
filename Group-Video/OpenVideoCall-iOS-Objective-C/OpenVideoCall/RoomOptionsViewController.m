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
@end
