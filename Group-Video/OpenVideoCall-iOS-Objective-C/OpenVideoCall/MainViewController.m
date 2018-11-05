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

@interface MainViewController () <SettingsVCDelegate, RoomVCDelegate, UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *roomNameTextField;
@property (weak, nonatomic) IBOutlet UITextField *encrypTextField;
@property (assign, nonatomic) CGSize dimension;
@property (assign, nonatomic) EncrypType encrypType;
@end

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.dimension = AgoraVideoDimension640x360;
    self.encrypType = [[EncryptionType encrypTypeArray][0] intValue];
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
        roomVC.dimension = self.dimension;
        roomVC.encrypType = self.encrypType;
        roomVC.encrypSecret = self.encrypTextField.text;
        roomVC.delegate = self;
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
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [self enterRoom];
    return YES;
}
@end
