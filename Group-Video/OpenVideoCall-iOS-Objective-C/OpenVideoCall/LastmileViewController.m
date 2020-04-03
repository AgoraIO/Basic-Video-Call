//
//  LastmileViewController.m
//  OpenVideoCall
//
//  Created by CavanSu on 2019/6/4.
//  Copyright © 2019 Agora. All rights reserved.
//

#import "LastmileViewController.h"

/// LastmileViewController implements various last mille related delegate functions for AgoraRtcEngine.
/// "Last mile" refers to the connection between the local device and Agora's edge server.
@interface LastmileViewController () <AgoraRtcEngineDelegate>

@property (weak, nonatomic) IBOutlet UILabel *qualityLabel;
@property (weak, nonatomic) IBOutlet UILabel *rttLabel;
@property (weak, nonatomic) IBOutlet UILabel *uplinkLabel;
@property (weak, nonatomic) IBOutlet UILabel *downlinkLabel;

@property (strong, nonatomic) UIActivityIndicatorView *activityView;
@property (assign, nonatomic) BOOL isLastmileProbeTesting;
@property (weak, nonatomic) AgoraRtcEngineKit *agoraKit;
@end

@implementation LastmileViewController
#pragma mark - Getter, Setter
- (AgoraRtcEngineKit *)agoraKit {    
    return [self.dataSource lastmileVCNeedAgoraKit];
}

- (void)setIsLastmileProbeTesting:(BOOL)isLastmileProbeTesting {
    if (isLastmileProbeTesting) {
        AgoraLastmileProbeConfig *config = [[AgoraLastmileProbeConfig alloc] init];
        config.probeUplink = YES;
        config.probeDownlink = YES;
        config.expectedUplinkBitrate = 5000;
        config.expectedDownlinkBitrate = 5000;
        [self.agoraKit startLastmileProbeTest:config];
        [self.activityView startAnimating];
        self.title = @"Testing...";
    } else {
        [self.activityView stopAnimating];
        [self.agoraKit stopLastmileProbeTest];
        self.title = @"Test result";
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self addActivityView];
    self.agoraKit.delegate = self;
    self.agoraKit.delegate = self;
    self.isLastmileProbeTesting = YES;
    self.view.backgroundColor = [UIColor whiteColor];
}

- (void)dealloc {
    self.isLastmileProbeTesting = NO;
}

#pragma mark - Private
- (void)addActivityView {
    UIActivityIndicatorView *activityView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    UIBarButtonItem *rightItem = [[UIBarButtonItem alloc] initWithCustomView:activityView];
    self.navigationItem.rightBarButtonItem = rightItem;
    self.activityView = activityView;
}

#pragma mark - AgoraRtcEngineDelegate

/// Reports the last mile network quality of the local user once every two seconds before the user joins a channel.
/// @param engine - RTC engine instance
/// @param quality -The last mile network quality based on the uplink and dowlink packet loss rate and jitter. See list at:
///  https://docs.agora.io/en/Video/API%20Reference/oc/Constants/AgoraNetworkQuality.html
- (void)rtcEngine:(AgoraRtcEngineKit *)engine lastmileQuality:(AgoraNetworkQuality)quality {
    NSString *string;
    switch (quality) {
        case AgoraNetworkQualityExcellent:      string = @"excellent";   break;
        case AgoraNetworkQualityGood:           string = @"good";        break;
        case AgoraNetworkQualityPoor:           string = @"poor";        break;
        case AgoraNetworkQualityBad:            string = @"bad";         break;
        case AgoraNetworkQualityVBad:           string = @"very bad";    break;
        case AgoraNetworkQualityDown:           string = @"down";        break;
        case AgoraNetworkQualityUnknown:        string = @"unknown";     break;
        case AgoraNetworkQualityDetecting:      string = @"detecting";   break;
        case AgoraNetworkQualityUnsupported:    string = @"unsupported"; break;
    }
    self.qualityLabel.text = string;
}

/// Reports the last-mile network probe result.
/// @param engine - RTC engine instance
/// @param result - The uplink and downlink last-mile network probe test result, see list here:
///   https://docs.agora.io/en/Video/API%20Reference/oc/Classes/AgoraLastmileProbeResult.html
- (void)rtcEngine:(AgoraRtcEngineKit *)engine lastmileProbeTestResult:(AgoraLastmileProbeResult *)result {
    self.rttLabel.text = [NSString stringWithFormat:@"%lu", (unsigned long)result.rtt];
    self.uplinkLabel.text = [NSString stringWithFormat:@"%lu", (unsigned long)result.uplinkReport.packetLossRate];
    self.downlinkLabel.text = [NSString stringWithFormat:@"%lu", (unsigned long)result.downlinkReport.packetLossRate];
    self.isLastmileProbeTesting = NO;
}
@end
