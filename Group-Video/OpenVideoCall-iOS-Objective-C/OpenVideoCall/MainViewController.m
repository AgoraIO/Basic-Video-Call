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
#import "Encryption.h"
#import "KeyCenter.h"
#import "FileCenter.h"
#import "Settings.h"
#import "MediaCharater.h"
#import "CommonExtension.h"
#import "LastmileViewController.h"

@interface MainViewController () <SettingsVCDelegate, SettingsVCDataSource, LastmileVCDataSource, RoomVCDataSource, UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *roomNameTextField;
@property (weak, nonatomic) IBOutlet UITextField *encryptionTextField;
@property (weak, nonatomic) IBOutlet UIButton *encryptionButton;
@property (weak, nonatomic) IBOutlet UIButton *testNetworkButton;

@property (strong, nonatomic) AgoraRtcEngineKit *agoraKit;
@property (assign, nonatomic) EncryptionType encryptionType;
@property (strong, nonatomic) Settings *settings;
@end

@implementation MainViewController
#pragma mark - Getter, Setter
- (AgoraRtcEngineKit *)agoraKit {
    if (!_agoraKit) {
        _agoraKit = [AgoraRtcEngineKit sharedEngineWithAppId:[KeyCenter AppId] delegate:nil];
        [_agoraKit setLogFilter:AgoraLogFilterInfo];
        [_agoraKit setLogFile:[FileCenter logFilePath]];
    }
    return _agoraKit;
}

- (Settings *)settings {
    if (!_settings) {
        _settings = [[Settings alloc] init];
        _settings.dimension = AgoraVideoDimension640x360;
        _settings.frameRate = AgoraVideoFrameRateFps15;
    }
    return _settings;
}

- (void)setEncryptionType:(EncryptionType)encryptionType {
    _encryptionType = encryptionType;
    NSString *name = [Encryption descriptionWithType:encryptionType];
    [self.encryptionButton setTitle:name forState:UIControlStateNormal];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self agoraKit];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    NSString *segueId = segue.identifier;
    
    if ([segueId isEqualToString:@"mainToSettings"]) {
        SettingsViewController *settingsVC = segue.destinationViewController;
        settingsVC.delegate = self;
        settingsVC.dataSource = self;
    } else if ([segueId isEqualToString:@"mainToLastmile"]) {
        LastmileViewController *testVC = segue.destinationViewController;
        testVC.dataSource = self;
    } else if ([segueId isEqualToString:@"mainToRoom"]) {
        RoomViewController *roomVC = segue.destinationViewController;
        roomVC.dataSource = self;
    }
}

- (IBAction)doRoomNameTextFieldEditing:(UITextField *)sender {
    NSString *text = sender.text;
    if (text && text.length > 0) {
        NSString *legalString = [MediaCharater updateToLegalMediaStringFromString:text];
        sender.text = legalString;
    }
}

- (IBAction)doEncryptionPressed:(UIButton *)sender {
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:nil message:nil preferredStyle:UIAlertControllerStyleActionSheet];
    NSArray *typeArray = [Encryption allTypesArray];
    __weak typeof(self) weakself = self;
    
    for (int i = 0; i < typeArray.count; i++) {
        EncryptionType type = [typeArray[i] intValue];
        UIAlertAction *action = [UIAlertAction actionWithTitle:[Encryption descriptionWithType:type]
                                                         style:UIAlertActionStyleDefault
                                                       handler:^(UIAlertAction * _Nonnull action) {
                                                           weakself.encryptionType = type;
                                                       }];
        [alertController addAction:action];
    }
    UIAlertAction *cancel = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleCancel handler:nil];
    [alertController addAction: cancel];
    [self presentViewController:alertController animated:YES completion:nil];
}

- (IBAction)doJoinPressed:(UIButton *)sender {
    // start join room when join button pressed
    [self enterRoom];
}

#pragma mark - Private
- (void)updateViews {
    // view elements initialization
    self.encryptionButton.layer.borderColor = UIColor.AGGray.CGColor;
    self.testNetworkButton.layer.borderColor = UIColor.AGGray.CGColor;
}

- (void)enterRoom {
    NSString *roomName = self.roomNameTextField.text;
    if (!roomName.length) {
        return;
    }
    
    NSString *secret = self.encryptionTextField.text;
    if (secret.length) {
        self.settings.encryption.secret = secret;
        self.settings.encryption.type = self.encryptionType;
    } else {
        self.settings.encryption.type = EncryptionTypeNone;
    }
    self.settings.roomName = roomName;
    [self performSegueWithIdentifier:@"mainToRoom" sender:nil];
}

#pragma mark - SettingsVCDelegate, SettingsVCDataSource
- (void)settingsVC:(SettingsViewController *)settingsVC didSelectDimension:(CGSize)dimension {
    self.settings.dimension = dimension;
}

- (void)settingsVC:(SettingsViewController *)settingsVC didSelectFrameRate:(AgoraVideoFrameRate)frameRate {
    self.settings.frameRate = frameRate;
}

- (Settings *)settingsVCNeedSettings {
    return self.settings;
}

#pragma mark - LastmileVCDataSource
- (AgoraRtcEngineKit *)lastmileVCNeedAgoraKit {
    return self.agoraKit;
}

#pragma mark - RoomVCDataSource
- (AgoraRtcEngineKit *)roomVCNeedAgoraKit {
    return self.agoraKit;
}

- (Settings *)roomVCNeedSettings {
    return self.settings;
}

#pragma mark - UITextFieldDelegate
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    if (textField == self.roomNameTextField) {
        [self enterRoom];
    } else {
        [textField resignFirstResponder];
    }
    return YES;
}

#pragma mark - Others
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.navigationItem.titleView.hidden = NO;
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    self.navigationItem.titleView.hidden = YES;
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:YES];
}
@end
