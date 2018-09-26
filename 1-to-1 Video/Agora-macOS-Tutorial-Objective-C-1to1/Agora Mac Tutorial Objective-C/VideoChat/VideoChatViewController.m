//
//  VideoChatViewController.m
//  Agora Mac Tutorial Objective-C
//
//  Created by James Fang on 8/9/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

#import "VideoChatViewController.h"

@interface VideoChatViewController ()

@property (strong, nonatomic) AgoraRtcEngineKit *agoraKit;          // Tutorial Step 1
@property (weak) IBOutlet NSView *localVideo;                       // Tutorial Step 3
@property (weak) IBOutlet NSView *remoteVideo;                      // Tutorial Step 5
@property (weak) IBOutlet NSView *controlButtons;                   // Tutorial Step 8
@property (weak) IBOutlet NSImageView *remoteVideoMutedIndicator;
@property (weak) IBOutlet NSImageView *localVideoMutedBg;
@property (weak) IBOutlet NSImageView *localVideoMutedIndicator;

@property (nonatomic) BOOL muteAudio;                               // Tutorial Step 9
@property (nonatomic) BOOL muteVideo;                               // Tutorial Step 10
@property (nonatomic) BOOL screenShare;                             // Tutorial Step 12

@end

@implementation VideoChatViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.wantsLayer = YES;
    self.remoteVideo.wantsLayer = YES;
    self.localVideo.wantsLayer = YES;
    
    [self setupButtons];            // Tutorial Step 8
    [self hideVideoMuted];          // Tutorial Step 10
    [self initializeAgoraEngine];   // Tutorial Step 1
    [self setupVideo];              // Tutorial Step 2
    [self setupLocalVideo];         // Tutorial Step 3
    [self joinChannel];             // Tutorial Step 4
}

- (void)viewWillAppear {
    [super viewWillAppear];
    
    self.view.layer.backgroundColor = [NSColor blackColor].CGColor;
    self.remoteVideo.layer.backgroundColor = [NSColor clearColor].CGColor;
    self.localVideo.layer.backgroundColor = [NSColor clearColor].CGColor;
}

// Tutorial Step 1
- (void)initializeAgoraEngine {
    self.agoraKit = [AgoraRtcEngineKit sharedEngineWithAppId:AppID delegate:self];
}

// Tutorial Step 2
- (void)setupVideo {
    [self.agoraKit enableVideo];
    // Default mode is disableVideo
    
    [self.agoraKit setVideoProfile:AgoraVideoProfileLandscape720P swapWidthAndHeight:false];
    // Default video profile is 360P
}

// Tutorial Step 3
- (void)setupLocalVideo {
    AgoraRtcVideoCanvas *videoCanvas = [[AgoraRtcVideoCanvas alloc] init];
    videoCanvas.uid = 0;
    // UID = 0 means we let Agora pick a UID for us
    
    videoCanvas.view = self.localVideo;
    videoCanvas.renderMode = AgoraVideoRenderModeAdaptive;
    [self.agoraKit setupLocalVideo:videoCanvas];
    // Bind local video stream to view
}

// Tutorial Step 4
- (void)joinChannel {
    [self.agoraKit joinChannelByToken:nil channelId:@"demoChannel1" info:nil uid:0 joinSuccess:^(NSString * _Nonnull channel, NSUInteger uid, NSInteger elapsed) {
        // Join channel "demoChannel1"
    }];
    // The UID database is maintained by your app to track which users joined which channels. If not assigned (or set to 0), the SDK will allocate one and returns it in joinSuccessBlock callback. The App needs to record and maintain the returned value as the SDK does not maintain it.
}

// Tutorial Step 5
- (void)rtcEngine:(AgoraRtcEngineKit *)engine firstRemoteVideoDecodedOfUid:(NSUInteger)uid size: (CGSize)size elapsed:(NSInteger)elapsed {
    if (self.remoteVideo.hidden)
        self.remoteVideo.hidden = false;
    AgoraRtcVideoCanvas *videoCanvas = [[AgoraRtcVideoCanvas alloc] init];
    videoCanvas.uid = uid;
    // Since we are making a simple 1:1 video chat app, for simplicity sake, we are not storing the UIDs. You could use a mechanism such as an array to store the UIDs in a channel.
    
    videoCanvas.view = self.remoteVideo;
    videoCanvas.renderMode = AgoraVideoRenderModeAdaptive;
    [self.agoraKit setupRemoteVideo:videoCanvas];
    // Bind remote video stream to view
}

// Tutorial Step 6
- (IBAction)didClickHangUpButton:(NSButton *)sender {
    [self leaveChannel];
}

- (void)leaveChannel {
    [self.agoraKit leaveChannel:nil];
    [self.agoraKit setupLocalVideo:nil];
    [self.remoteVideo removeFromSuperview];
    [self.localVideo removeFromSuperview];
    self.agoraKit = nil;
    [self.view.window close];
}

// Tutorial Step 7
- (void)rtcEngine:(AgoraRtcEngineKit *)engine didOfflineOfUid:(NSUInteger)uid reason:(AgoraUserOfflineReason)reason {
    self.remoteVideo.hidden = true;
}

// Tutorial Step 8
- (void)setupButtons {
    [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
    
    [self.remoteVideo addTrackingArea:[[NSTrackingArea alloc] initWithRect:self.remoteVideo.bounds
                                                                            options:NSTrackingMouseMoved | NSTrackingActiveInActiveApp | NSTrackingInVisibleRect
                                                                              owner:self
                                                                           userInfo:nil]];
    
    [self.controlButtons addTrackingArea:[[NSTrackingArea alloc] initWithRect:self.controlButtons.bounds
                                                                    options:NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp | NSTrackingInVisibleRect
                                                                      owner:self
                                                                   userInfo:nil]];
}

- (void)hideControlButtons {
    self.controlButtons.hidden = YES;
}

- (void)mouseMoved:(NSEvent *)theEvent {
    if (self.controlButtons.hidden) {
        self.controlButtons.hidden = NO;
        [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
    }
}

- (void)mouseEntered:(NSEvent *)theEvent {
    [VideoChatViewController cancelPreviousPerformRequestsWithTarget:self];
}

- (void)mouseExited:(NSEvent *)theEvent {
    [self performSelector:@selector(hideControlButtons) withObject:nil afterDelay:3];
}

// Tutorial Step 9
- (IBAction)didClickMuteButton:(NSButton *)sender {
    self.muteAudio = !self.muteAudio;
    [self.agoraKit muteLocalAudioStream:self.muteAudio];
    
    if (self.muteAudio) {
        [sender setImage:[NSImage imageNamed:@"muteButtonSelected"]];
    } else {
        [sender setImage:[NSImage imageNamed:@"muteButton"]];
    }
}

// Tutorial Step 10
- (IBAction)didClickVideoMuteButton:(NSButton *)sender {
    self.muteVideo = !self.muteVideo;
    [self.agoraKit muteLocalVideoStream:self.muteVideo];
    
    if (self.muteVideo) {
        [sender setImage:[NSImage imageNamed:@"videoMuteButtonSelected"]];
    } else {
        [sender setImage:[NSImage imageNamed:@"videoMuteButton"]];
    }
    self.localVideo.hidden = self.muteVideo;
    self.localVideoMutedBg.hidden = !(self.muteVideo);
    self.localVideoMutedIndicator.hidden = !(self.muteVideo);
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid {
    self.remoteVideo.hidden = muted;
    self.remoteVideoMutedIndicator.hidden = !muted;
}

- (void)hideVideoMuted {
    self.remoteVideoMutedIndicator.hidden = true;
    self.localVideoMutedBg.hidden = true;
    self.localVideoMutedIndicator.hidden = true;
}

// Tutorial Step 11
- (IBAction)didClickDeviceSelectionButton:(NSButton *)sender {
    DeviceSelectionViewController *deviceSelectionViewController = [self.storyboard instantiateControllerWithIdentifier:@"DeviceSelectionViewController"];
    deviceSelectionViewController.agoraKit = self.agoraKit;
    // Pass in AgoraRtcEngineKit
    [self presentViewControllerAsSheet:deviceSelectionViewController];
    // Segue to sheet view controller DeviceSelectionViewController
}

// Tutorial Step 12
- (IBAction)didClickScreenShareButton:(NSButton *)sender {
    self.screenShare = !self.screenShare;
    if (self.screenShare) {
        [sender setImage:[NSImage imageNamed:@"screenShareButtonSelected"]];
        [self.agoraKit startScreenCapture:0 withCaptureFreq:15 bitRate:0 andRect:CGRectZero];
    } else {
        [sender setImage:[NSImage imageNamed:@"screenShareButton"]];
        [self.agoraKit stopScreenCapture];
    }
}

@end
