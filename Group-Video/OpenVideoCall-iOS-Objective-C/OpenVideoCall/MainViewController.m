//
//  MainViewController.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import "MainViewController.h"
#import "SettingsViewController.h"
#import "RoomViewController.h"
#import "EncryptionType.h"
#import "KeyCenter.h"
#import "FileCenter.h"

@interface MainViewController () <SettingsVCDelegate, RoomVCDelegate, AgoraRtcEngineDelegate, UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *roomNameTextField;
@property (weak, nonatomic) IBOutlet UITextField *encrypTextField;

@property (weak, nonatomic) IBOutlet UIButton *lastmileTestButton;
@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *lastmileTestIndicator;
@property (weak, nonatomic) IBOutlet UILabel *qualityLabel;
@property (weak, nonatomic) IBOutlet UILabel *rttLabel;
@property (weak, nonatomic) IBOutlet UILabel *uplinkLabel;
@property (weak, nonatomic) IBOutlet UILabel *downlinkLabel;
@property (strong, nonatomic) IBOutletCollection(UILabel) NSArray *infoLabels;

@property (strong, nonatomic) AgoraRtcEngineKit *agoraKit;
@property (assign, nonatomic) CGSize dimension;
@property (assign, nonatomic) EncrypType encrypType;
@property (assign, nonatomic) BOOL isLastmileProbeTesting;
@end

@implementation MainViewController
- (void)setIsLastmileProbeTesting:(BOOL)isLastmileProbeTesting {
    _isLastmileProbeTesting = isLastmileProbeTesting;
    self.lastmileTestButton.hidden = isLastmileProbeTesting;
    if (isLastmileProbeTesting) {
        [self.lastmileTestIndicator startAnimating];
    } else {
        [self.lastmileTestIndicator stopAnimating];
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.dimension = AgoraVideoDimension640x360;
    self.encrypType = [[EncryptionType encrypTypeArray][0] intValue];
    self.agoraKit = [AgoraRtcEngineKit sharedEngineWithAppId:[KeyCenter AppId] delegate:self];
    [self.agoraKit setLogFilter:AgoraLogFilterInfo];
    [self.agoraKit setLogFile:[FileCenter logFilePath]];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    NSString *segueId = segue.identifier;
    
    if ([segueId isEqualToString:@"mainToSettings"]) {
        SettingsViewController *settingsVC = segue.destinationViewController;
        settingsVC.dimension = self.dimension;
        settingsVC.delegate = self;
    } else if ([segueId isEqualToString:@"mainToRoom"]) {
        RoomViewController *roomVC = segue.destinationViewController;
        roomVC.roomName = sender;
        roomVC.agoraKit = self.agoraKit;
        roomVC.dimension = self.dimension;
        roomVC.encrypType = self.encrypType;
        roomVC.encrypSecret = self.encrypTextField.text;
        roomVC.delegate = self;
    }
}

- (IBAction)doLastmileProbeTestPressed:(UIButton *)sender {
    AgoraLastmileProbeConfig *config = [[AgoraLastmileProbeConfig alloc] init];
    config.probeUplink = YES;
    config.probeDownlink = YES;
    config.expectedUplinkBitrate = 5000;
    config.expectedDownlinkBitrate = 5000;
    
    [self.agoraKit startLastmileProbeTest:config];
    
    self.isLastmileProbeTesting = YES;
    
    for (UILabel *label in self.infoLabels) {
        label.hidden = YES;
    }
}

- (IBAction)doJoinPressed:(UIButton *)sender {
    [self enterRoom];
}

- (void)enterRoom {
    if (!self.roomNameTextField.text.length) {
        return;
    }
    
    [self performSegueWithIdentifier:@"mainToRoom" sender:self.roomNameTextField.text];
}

- (IBAction)doEncrypPressed:(UIButton *)sender {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    NSArray *encrypTypeArray = [EncryptionType encrypTypeArray];
    __weak typeof(self) weakself = self;
    
    for (int i = 0; i < encrypTypeArray.count; i++) {
        EncrypType type = [encrypTypeArray[i] intValue];
        UIAlertAction *action = [UIAlertAction actionWithTitle:[EncryptionType descriptionWithEncrypType:type] style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            weakself.encrypType = type;
            [sender setTitle:[EncryptionType descriptionWithEncrypType:type] forState:UIControlStateNormal];
        }];
        [alertController addAction:action];
    }
    
    [alertController addAction:[UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil]];
    alertController.popoverPresentationController.sourceView = sender;
    [self presentViewController:alertController animated:YES completion:nil];
}

//MARK: - delegates
- (void)settingsVC:(SettingsViewController *)settingsVC didSelectDimension:(CGSize)dimension {
    self.dimension = dimension;
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)roomVCNeedClose:(RoomViewController *)roomVC {
    self.agoraKit.delegate = self;
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine lastmileQuality:(AgoraNetworkQuality)quality {
    NSString *string;
    switch (quality) {
        case AgoraNetworkQualityExcellent:  string = @"excellent"; break;
        case AgoraNetworkQualityGood:       string = @"good"; break;
        case AgoraNetworkQualityPoor:       string = @"poor"; break;
        case AgoraNetworkQualityBad:        string = @"bad"; break;
        case AgoraNetworkQualityVBad:       string = @"very bad"; break;
        case AgoraNetworkQualityDown:       string = @"down"; break;
        case AgoraNetworkQualityUnknown:    string = @"unknown"; break;
    }
    self.qualityLabel.text = [NSString stringWithFormat:@"quality: %@", string];
    self.qualityLabel.hidden = NO;
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine lastmileProbeTestResult:(AgoraLastmileProbeResult *)result {
    self.rttLabel.text = [NSString stringWithFormat:@"rtt: %lu", (unsigned long)result.rtt];
    self.rttLabel.hidden = NO;
    self.uplinkLabel.text = [self descriptionOfProbeOneWayResult:result.uplinkReport];
    self.uplinkLabel.hidden = NO;
    self.downlinkLabel.text = [self descriptionOfProbeOneWayResult:result.downlinkReport];
    self.downlinkLabel.hidden = NO;
    
    [self.agoraKit stopLastmileProbeTest];
    self.isLastmileProbeTesting = NO;
}

- (NSString *)descriptionOfProbeOneWayResult:(AgoraLastmileProbeOneWayResult *)result {
    return [NSString stringWithFormat:@"packetLoss: %lu, jitter: %lu, availableBandwidth: %lu", (unsigned long)result.packetLossRate, result.jitter, result.availableBandwidth];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [self enterRoom];
    return YES;
}
@end
