//
//  VideoChatViewController.m
//  Agora iOS Tutorial Objective-C
//
//  Created by James Fang on 7/15/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

#import "VideoChatViewController.h"

@interface VideoChatViewController ()

@property (strong, nonatomic) AgoraRtcEngineKit *agoraKit;
@property (weak, nonatomic) IBOutlet UIView *localVideo;
@property (weak, nonatomic) IBOutlet UIView *remoteVideo;
@property (weak, nonatomic) IBOutlet UIView *controlButtons;
@property (weak, nonatomic) IBOutlet UIImageView *remoteVideoMutedIndicator;
@property (weak, nonatomic) IBOutlet UIImageView *localVideoMutedBg;
@property (weak, nonatomic) IBOutlet UIImageView *localVideoMutedIndicator;

@end

@implementation VideoChatViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self setupButtons];
    [self hideVideoMuted];
    [self initializeAgoraEngine];
    [self setupVideo];
    [self setupLocalVideo];
    [self joinChannel];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)initializeAgoraEngine {
    self.agoraKit = [AgoraRtcEngineKit sharedEngineWithAppId:appID delegate:self];
}

- (void)setupVideo {
    [self.agoraKit enableVideo];
    // Default mode is disableVideo
    
    AgoraVideoEncoderConfiguration *encoderConfiguration =
    [[AgoraVideoEncoderConfiguration alloc] initWithSize:AgoraVideoDimension640x360
                                               frameRate:AgoraVideoFrameRateFps15
                                                 bitrate:AgoraVideoBitrateStandard
                                         orientationMode:AgoraVideoOutputOrientationModeAdaptative];
    [self.agoraKit setVideoEncoderConfiguration:encoderConfiguration];
}

- (void)setupLocalVideo {
    AgoraRtcVideoCanvas *videoCanvas = [[AgoraRtcVideoCanvas alloc] init];
    videoCanvas.uid = 0;
    // UID = 0 means we let Agora pick a UID for us
    
    videoCanvas.view = self.localVideo;
    videoCanvas.renderMode = AgoraVideoRenderModeHidden;
    [self.agoraKit setupLocalVideo:videoCanvas];
    // Bind local video stream to view
}

- (void)joinChannel {
    [self.agoraKit joinChannelByToken:token channelId:@"demoChannel1" info:nil uid:0 joinSuccess:^(NSString *channel, NSUInteger uid, NSInteger elapsed) {
        // Join channel "demoChannel1"
    }];
    // The UID database is maintained by your app to track which users joined which channels. If not assigned (or set to 0), the SDK will allocate one and returns it in joinSuccessBlock callback. The App needs to record and maintain the returned value as the SDK does not maintain it.
    
    [self.agoraKit setEnableSpeakerphone:YES];
    [UIApplication sharedApplication].idleTimerDisabled = YES;
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine firstRemoteVideoDecodedOfUid:(NSUInteger)uid size: (CGSize)size elapsed:(NSInteger)elapsed {
    if (self.remoteVideo.hidden) {
        self.remoteVideo.hidden = NO;
    }
    
    AgoraRtcVideoCanvas *videoCanvas = [[AgoraRtcVideoCanvas alloc] init];
    videoCanvas.uid = uid;
    // Since we are making a simple 1:1 video chat app, for simplicity sake, we are not storing the UIDs. You could use a mechanism such as an array to store the UIDs in a channel.
    
    videoCanvas.view = self.remoteVideo;
    videoCanvas.renderMode = AgoraVideoRenderModeHidden;
    [self.agoraKit setupRemoteVideo:videoCanvas];
    // Bind remote video stream to view
}

- (IBAction)hangUpButton:(UIButton *)sender {
    [self leaveChannel];
}

- (void)leaveChannel {
    [self.agoraKit leaveChannel:^(AgoraChannelStats *stat) {
        [self hideControlButtons];
        [UIApplication sharedApplication].idleTimerDisabled = NO;
        [self.remoteVideo removeFromSuperview];
        [self.localVideo removeFromSuperview];
    }];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didOfflineOfUid:(NSUInteger)uid reason:(AgoraUserOfflineReason)reason {
    self.remoteVideo.hidden = true;
}

- (void)setupButtons {
    [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(remoteVideoTapped:)];
    [self.view addGestureRecognizer:tapGestureRecognizer];
    self.view.userInteractionEnabled = true;
}

- (void)hideControlButtons {
    self.controlButtons.hidden = true;
}

- (void)remoteVideoTapped:(UITapGestureRecognizer *)recognizer {
    if (self.controlButtons.hidden) {
        self.controlButtons.hidden = false;
        [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
    }
}

- (void)resetHideButtonsTimer {
    [VideoChatViewController cancelPreviousPerformRequestsWithTarget:self];
    [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
}

- (IBAction)didClickMuteButton:(UIButton *)sender {
    sender.selected = !sender.selected;
    [self.agoraKit muteLocalAudioStream:sender.selected];
    [self resetHideButtonsTimer];
}

- (IBAction)didClickVideoMuteButton:(UIButton *)sender {
    sender.selected = !sender.selected;
    [self.agoraKit muteLocalVideoStream:sender.selected];
    self.localVideo.hidden = sender.selected;
    self.localVideoMutedBg.hidden = !sender.selected;
    self.localVideoMutedIndicator.hidden = !sender.selected;
    [self resetHideButtonsTimer];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid {
    self.remoteVideo.hidden = muted;
    self.remoteVideoMutedIndicator.hidden = !muted;
}

- (void) hideVideoMuted {
    self.remoteVideoMutedIndicator.hidden = true;
    self.localVideoMutedBg.hidden = true;
    self.localVideoMutedIndicator.hidden = true;
}

- (IBAction)didClickSwitchCameraButton:(UIButton *)sender {
    sender.selected = !sender.selected;
    [self.agoraKit switchCamera];
    [self resetHideButtonsTimer];
}
@end
