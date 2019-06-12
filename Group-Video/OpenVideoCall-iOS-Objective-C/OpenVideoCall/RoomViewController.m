//
//  RoomViewController.m
//  OpenVideoCall
//
//  Created by GongYuhua on 2016/9/12.
//  Copyright © 2016年 Agora. All rights reserved.
//

#import <AgoraRtcCryptoLoader/AgoraRtcCryptoLoader.h>
#import "RoomOptionsViewController.h"
#import "MessageViewController.h"
#import "RoomViewController.h"
#import "VideoViewLayouter.h"
#import "VideoSession.h"
#import "AppDelegate.h"
#import "FileCenter.h"
#import "KeyCenter.h"

@interface RoomViewController () <AgoraRtcEngineDelegate, RoomOptionsVCDelegate, RoomOptionsVCDataSource>
@property (weak, nonatomic) IBOutlet UIView *containerView;
@property (weak, nonatomic) IBOutlet UIView *messageTableContainerView;

@property (weak, nonatomic) IBOutlet UIButton *cameraButton;
@property (weak, nonatomic) IBOutlet UIButton *audioMixingButton;
@property (weak, nonatomic) IBOutlet UIButton *speakerPhoneButton;
@property (weak, nonatomic) IBOutlet UIButton *beautyButton;
@property (weak, nonatomic) IBOutlet UIButton *muteVideoButton;
@property (weak, nonatomic) IBOutlet UIButton *muteAudioButton;

@property (weak, nonatomic) IBOutlet UITapGestureRecognizer *backgroundDoubleTap;

@property (weak, nonatomic) AgoraRtcEngineKit *agoraKit;
@property (strong, nonatomic) AgoraRtcCryptoLoader *agoraLoader;

@property (assign, nonatomic) BOOL isSwitchCamera;
@property (assign, nonatomic) BOOL isAudioMixing;
@property (assign, nonatomic) BOOL isBeauty;
@property (assign, nonatomic) BOOL isEarPhone;
@property (assign, nonatomic) BOOL isVideoMuted;
@property (assign, nonatomic) BOOL isAudioMuted;
@property (assign, nonatomic) BOOL isDebugMode;

@property (strong, nonatomic) NSMutableArray<VideoSession *> *videoSessions;
@property (strong, nonatomic) VideoSession *doubleClickFullSession;
@property (strong, nonatomic) VideoViewLayouter *videoViewLayouter;

@property (strong, nonatomic) RoomOptions *options;

@property (weak, nonatomic) MessageViewController *messageVC;
@property (weak, nonatomic) RoomOptionsViewController *optionsVC;
@property (weak, nonatomic) Settings *settings;
@end

@implementation RoomViewController
#pragma mark - Setter, Getter
- (void)setIsSwitchCamera:(BOOL)isSwitchCamera {
    [self.agoraKit switchCamera];
}

- (void)setIsAudioMixing:(BOOL)isAudioMixing {
    if (_isAudioMixing == isAudioMixing) {
        return;
    }
    
    _isAudioMixing = isAudioMixing;
    self.audioMixingButton.selected = _isAudioMixing;
    if (_isAudioMixing) {
        // play music file
        [self.agoraKit startAudioMixing:[FileCenter audioFilePath]
                               loopback:false
                                replace:false
                                  cycle:1];
    } else {
        // stop play
        [self.agoraKit stopAudioMixing];
    }
}

- (void)setIsBeauty:(BOOL)isBeauty {
    if (_isBeauty == isBeauty) {
        return;
    }
    
    _isBeauty = isBeauty;
    self.beautyButton.selected = _isBeauty;
    AgoraBeautyOptions *options = nil;
    if (_isBeauty) {
        options = [[AgoraBeautyOptions alloc] init];
        options.lighteningContrastLevel = AgoraLighteningContrastNormal;
        options.lighteningLevel = 0.7;
        options.smoothnessLevel = 0.5;
        options.rednessLevel = 0.1;
    }
    // improve local render view
    [self.agoraKit setBeautyEffectOptions:_isBeauty options:options];
}

- (void)setIsEarPhone:(BOOL)isEarPhone {
    if (_isEarPhone == isEarPhone) {
        return;
    }
    
    _isEarPhone = isEarPhone;
    self.speakerPhoneButton.selected = _isEarPhone;
    // switch playout audio route
    [self.agoraKit setEnableSpeakerphone:!_isEarPhone];
}

- (void)setIsVideoMuted:(BOOL)isVideoMuted {
    if (_isVideoMuted == isVideoMuted) {
        return;
    }
    
    _isVideoMuted = isVideoMuted;
    self.muteVideoButton.selected = _isVideoMuted;
    [self setVideoMuted:_isVideoMuted forUid:0];
    [self updateSelfViewVisiable];
    // mute local video
    [self.agoraKit muteLocalVideoStream:_isVideoMuted];
}

- (void)setIsAudioMuted:(BOOL)isAudioMuted {
    if (_isAudioMuted == isAudioMuted) {
        return;
    }
    
    _isAudioMuted = isAudioMuted;
    self.muteAudioButton.selected = _isAudioMuted;
    // mute local audio
    [self.agoraKit muteLocalAudioStream:_isAudioMuted];
}

- (void)setIsDebugMode:(BOOL)isDebugMode {
    if (_isDebugMode == isDebugMode) {
        return;
    }
    
    _isDebugMode = isDebugMode;
    _options.isDebugMode = _isDebugMode;
    self.messageTableContainerView.hidden = !_isDebugMode;
}

- (void)setDoubleClickFullSession:(VideoSession *)doubleClickFullSession {
    _doubleClickFullSession = doubleClickFullSession;
    if (self.videoSessions.count >= 3) {
        [self updateInterfaceWithSessions:self.videoSessions targetSize:self.containerView.frame.size animation:YES];
    }
}

- (AgoraRtcEngineKit *)agoraKit {
    return [self.dataSource roomVCNeedAgoraKit];
}

- (AgoraRtcCryptoLoader *)agoraLoader {
    if (!_agoraLoader) {
        _agoraLoader = [[AgoraRtcCryptoLoader alloc] init];
    }
    return _agoraLoader;
}

// videoViewLayouter and videoSessions manage all render views
- (VideoViewLayouter *)videoViewLayouter {
    if (!_videoViewLayouter) {
        _videoViewLayouter = [[VideoViewLayouter alloc] init];
    }
    return _videoViewLayouter;
}

- (NSMutableArray<VideoSession *> *)videoSessions {
    if (!_videoSessions) {
        _videoSessions = [[NSMutableArray alloc] init];
    }
    return _videoSessions;
}

- (Settings *)settings {
    return [self.dataSource roomVCNeedSettings];
}

- (RoomOptions *)options {
    if (!_options) {
        _options = [[RoomOptions alloc] init];
        _options.isDebugMode = false;
    }
    return _options;
}

#pragma mark - VC Life
- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = self.settings.roomName;
    [self loadAgoraKit];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    NSString *segueId = segue.identifier;
    if (!segueId.length) {
        return;
    }
    
    if ([segueId isEqualToString:@"roomEmbedMessage"]) {
        self.messageVC = segue.destinationViewController;
    } else if ([segueId isEqualToString:@"roomToOptions"]) {
        RoomOptionsViewController *optionsVC = segue.destinationViewController;
        optionsVC.delegate = self;
        optionsVC.dataSource = self;
        self.optionsVC = optionsVC;
    }
}

- (void)dealloc {
    [self leaveChannel];
}

#pragma mark - UI Actions
- (IBAction)doCameraPressed:(UIButton *)sender {
    self.isSwitchCamera = !self.isSwitchCamera;
}

- (IBAction)doBeautyPressed:(UIButton *)sender {
    self.isBeauty = !self.isBeauty;
}

- (IBAction)doAudioMixingPressed:(UIButton *)sender {
    self.isAudioMixing = !self.isAudioMixing;
}

- (IBAction)doSpeakerPhonePressed:(UIButton *)sender {
    self.isEarPhone = !self.isEarPhone;
}

- (IBAction)doMuteVideoPressed:(UIButton *)sender {
    self.isVideoMuted = !self.isVideoMuted;
}

- (IBAction)doMuteAudioPressed:(UIButton *)sender {
    self.isAudioMuted = !self.isAudioMuted;
}

- (IBAction)doBackDoubleTapped:(UITapGestureRecognizer *)sender {
    if (!self.doubleClickFullSession) {
        // full screen display after be double clicked
        NSInteger tappedIndex = [self.videoViewLayouter responseIndexOfLocation:[sender locationInView:self.containerView]];
        if (tappedIndex >= 0 && tappedIndex < self.videoSessions.count) {
            self.doubleClickFullSession = self.videoSessions[tappedIndex];
        }
    } else {
        self.doubleClickFullSession = nil;
    }
}

#pragma mark - AgoraRtcEngineKit
- (void)loadAgoraKit {
    // Step 1, set delegate
    self.agoraKit.delegate = self;
    // Step 2, set communication mode
    [self.agoraKit setChannelProfile:AgoraChannelProfileCommunication];
    
    // Step 3, enable the video module
    [self.agoraKit enableVideo];
    // set video configuration
    AgoraVideoEncoderConfiguration *configuration = [[AgoraVideoEncoderConfiguration alloc] initWithSize:self.settings.dimension
                                                                                               frameRate:self.settings.frameRate
                                                                                                 bitrate:AgoraVideoBitrateStandard
                                                                                         orientationMode:AgoraVideoOutputOrientationModeAdaptative];
    [self.agoraKit setVideoEncoderConfiguration:configuration];
    // add local render view and start preview
    [self addLocalSession];
    [self.agoraKit startPreview];
    
    // Step 4, enable encryption mode
    if (self.settings.encryption.type != EncryptionTypeNone && self.settings.encryption.secret.length) {
        [self.agoraKit setEncryptionMode:self.settings.encryption.modeString];
        [self.agoraKit setEncryptionSecret:self.settings.encryption.secret];
    }
    
    // Step 5, join channel and start group chat
    // If join  channel success, agoraKit triggers it's delegate function
    // 'rtcEngine:(AgoraRtcEngineKit *)engine didJoinChannel:(NSString *)channel withUid:(NSUInteger)uid elapsed:(NSInteger)elapsed'
    [self.agoraKit joinChannelByToken:nil channelId:self.settings.roomName info:nil uid:0 joinSuccess:nil];
    [self setIdleTimerActive:NO];
}

- (void)addLocalSession {
    VideoSession *localSession = [VideoSession localSession];
    [localSession updateMediaInfo:self.settings.dimension fps:self.settings.frameRate];
    [self.videoSessions addObject:localSession];
    [self.agoraKit setupLocalVideo:localSession.canvas];
    [self updateInterfaceWithSessions:self.videoSessions targetSize:self.containerView.frame.size animation:YES];
    [self.agoraKit startPreview];
}

- (void)leaveChannel {
    // Step 1, release local AgoraRtcVideoCanvas instance
    [self.agoraKit setupLocalVideo:nil];
    // Step 2, leave channel and end group chat
    [self.agoraKit leaveChannel:nil];
    // Step 3, please attention, stop preview after leave channel
    [self.agoraKit stopPreview];
    
    // Step 4, remove all render views
    for (VideoSession *session in self.videoSessions) {
        [session.hostingView removeFromSuperview];
    }
    [self.videoSessions removeAllObjects];
    
    [self setIdleTimerActive:YES];
}

#pragma mark - <AgoraRtcEngineDelegate>
- (void)rtcEngine:(AgoraRtcEngineKit *)engine didJoinChannel:(NSString *)channel withUid:(NSUInteger)uid elapsed:(NSInteger)elapsed {
    [self info:[NSString stringWithFormat:@"Join channel: %@", channel]];
}

- (void)rtcEngineConnectionDidInterrupted:(AgoraRtcEngineKit *)engine {
    [self alert:@"Connection Interrupted"];
}

- (void)rtcEngineConnectionDidLost:(AgoraRtcEngineKit *)engine {
    [self alert:@"Connection Lost"];
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine didOccurError:(AgoraErrorCode)errorCode {
    [self alert:[NSString stringWithFormat:@"Occur error: %ld", errorCode]];
}

// first remote video frame
- (void)rtcEngine:(AgoraRtcEngineKit *)engine firstRemoteVideoDecodedOfUid:(NSUInteger)uid size:(CGSize)size elapsed:(NSInteger)elapsed {
    VideoSession *userSession = [self videoSessionOfUid:uid];
    userSession.size = size;
    [self.agoraKit setupRemoteVideo:userSession.canvas];
}

// first local video frame
- (void)rtcEngine:(AgoraRtcEngineKit *)engine firstLocalVideoFrameWithSize:(CGSize)size elapsed:(NSInteger)elapsed {
    if (self.videoSessions.count) {
        VideoSession *selfSession = self.videoSessions.firstObject;
        selfSession.size = size;
        [self updateInterfaceWithSessions:self.videoSessions targetSize:self.containerView.frame.size animation:NO];
        [self info:[NSString stringWithFormat:@"local video dimension: %f x %f", size.width, size.height]];
    }
}

// user offline
- (void)rtcEngine:(AgoraRtcEngineKit *)engine didOfflineOfUid:(NSUInteger)uid reason:(AgoraUserOfflineReason)reason {
    VideoSession *deleteSession;
    for (VideoSession *session in self.videoSessions) {
        if (session.uid == uid) {
            deleteSession = session;
            break;
        }
    }
    
    if (deleteSession) {
        [self.videoSessions removeObject:deleteSession];
        [deleteSession.hostingView removeFromSuperview];
        [self updateInterfaceWithSessions:self.videoSessions targetSize:self.containerView.frame.size animation:YES];
        
        if (deleteSession == self.doubleClickFullSession) {
            self.doubleClickFullSession = nil;
        }
        
        // release canvas's view
        deleteSession.canvas.view = nil;
    }
}

// video muted
- (void)rtcEngine:(AgoraRtcEngineKit *)engine didVideoMuted:(BOOL)muted byUid:(NSUInteger)uid {
    [self setVideoMuted:muted forUid:uid];
}

// remote stat
- (void)rtcEngine:(AgoraRtcEngineKit *)engine remoteVideoStats:(AgoraRtcRemoteVideoStats *)stats {
    VideoSession *session = [self fetchSessionOfUid:stats.uid];
    [session updateMediaInfo:CGSizeMake(stats.width, stats.height) fps:stats.rendererOutputFrameRate];
}

// audio mixing
- (void)rtcEngineLocalAudioMixingDidFinish:(AgoraRtcEngineKit *)engine {
    self.isAudioMixing = NO;
}

#pragma mark - RoomOptionsVCDelegate, RoomOptionsVCDataSource
- (void)roomOptions:(RoomOptionsViewController *)vc debugModeDidEnable:(BOOL)enable {
    self.isDebugMode = enable;
}

- (RoomOptions *)roomOptionsVCNeedOptions {
    return self.options;
}

#pragma mark - Private
- (void)updateInterfaceWithSessions:(NSArray *)sessions targetSize:(CGSize)targetSize animation:(BOOL)animation {
    if (animation) {
        [UIView animateWithDuration:0.3 animations:^{
            [self updateInterfaceWithSessions:sessions targetSize:targetSize];
            [self.view layoutIfNeeded];
        }];
    } else {
        [self updateInterfaceWithSessions:sessions targetSize:targetSize];
    }
}

- (void)updateInterfaceWithSessions:(NSArray *)sessions targetSize:(CGSize)targetSize {
    if (!sessions.count) {
        return;
    }
    
    VideoSession *selfSession = sessions.firstObject;
    self.videoViewLayouter.selfView = selfSession.hostingView;
    self.videoViewLayouter.selfSize = selfSession.size;
    self.videoViewLayouter.targetSize = targetSize;
    
    NSMutableArray *peerVideoViews = [[NSMutableArray alloc] init];
    for (NSInteger i = 1; i < sessions.count; ++i) {
        VideoSession *session = sessions[i];
        [peerVideoViews addObject:session.hostingView];
    }
    self.videoViewLayouter.videoViews = peerVideoViews;
    self.videoViewLayouter.fullView = self.doubleClickFullSession.hostingView;
    self.videoViewLayouter.containerView = self.containerView;
    
    [self.videoViewLayouter layoutVideoViews];
    [self updateSelfViewVisiable];
    
    // Only three people or more can switch the layout
    if (sessions.count >= 3) {
        self.backgroundDoubleTap.enabled = YES;
    } else {
        self.backgroundDoubleTap.enabled = NO;
        self.doubleClickFullSession = nil;
    }
}

- (void)setIdleTimerActive:(BOOL)active {
    [UIApplication sharedApplication].idleTimerDisabled = !active;
}

- (VideoSession *)fetchSessionOfUid:(NSUInteger)uid {
    for (VideoSession *session in self.videoSessions) {
        if (session.uid == uid) {
            return session;
        }
    }
    return nil;
}

- (VideoSession *)videoSessionOfUid:(NSUInteger)uid {
    VideoSession *fetchedSession = [self fetchSessionOfUid:uid];
    if (fetchedSession) {
        return fetchedSession;
    } else {
        VideoSession *newSession = [[VideoSession alloc] initWithUid:uid];
        [self.videoSessions addObject:newSession];
        [self updateInterfaceWithSessions:self.videoSessions targetSize:self.containerView.frame.size animation:YES];
        return newSession;
    }
}

- (void)setVideoMuted:(BOOL)muted forUid:(NSUInteger)uid {
    VideoSession *fetchedSession = [self fetchSessionOfUid:uid];
    fetchedSession.isVideoMuted = muted;
}

- (void)updateSelfViewVisiable {
    UIView *selfView = self.videoSessions.firstObject.hostingView;
    if (self.videoSessions.count == 2) {
        selfView.hidden = self.isVideoMuted;
    } else {
        selfView.hidden = false;
    }
}

// Log
- (void)info:(NSString *)text {
    if (!text.length) {
        return;
    }
    
    [self.messageVC appendInfo:text];
}

- (void)alert:(NSString *)text {
    if (!text.length) {
        return;
    }
    
    [self.messageVC appendError:text];
}

#pragma mark - Others
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    appDelegate.orientation = UIInterfaceOrientationMaskAll;
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    appDelegate.orientation = UIInterfaceOrientationMaskPortrait;
}
@end
