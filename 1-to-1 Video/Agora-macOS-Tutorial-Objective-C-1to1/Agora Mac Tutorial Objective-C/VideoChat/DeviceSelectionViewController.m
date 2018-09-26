//
//  DeviceSelectionViewController.m
//  Agora Mac Tutorial Objective-C
//
//  Created by James Fang on 8/9/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

#import "DeviceSelectionViewController.h"

@interface DeviceSelectionViewController ()

@property (weak) IBOutlet NSPopUpButton *microphoneSelection;
@property (weak) IBOutlet NSPopUpButton *speakerSelection;
@property (weak) IBOutlet NSPopUpButton *cameraSelection;

@property (strong, nonatomic) NSArray* connectedRecordingDevices;
@property (strong, nonatomic) NSArray* connectedPlaybackDevices;
@property (strong, nonatomic) NSArray* connectedVideoCaptureDevices;

@end

@implementation DeviceSelectionViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Tutorial Step 11
    self.preferredContentSize = NSMakeSize(500, 250);
    [self loadDevicesInPopUpButtons];
}

- (void)loadDevicesInPopUpButtons {
    [self.microphoneSelection removeAllItems];
    [self.speakerSelection removeAllItems];
    [self.cameraSelection removeAllItems];

    self.connectedRecordingDevices = [self.agoraKit enumerateDevices:0];
    for (id device in self.connectedRecordingDevices) {
        [self.microphoneSelection addItemWithTitle:([device deviceName])];
    }
    
    self.connectedPlaybackDevices = [self.agoraKit enumerateDevices:1];
    for (id device in self.connectedPlaybackDevices) {
        [self.speakerSelection addItemWithTitle:([device deviceName])];
    }
    
    self.connectedVideoCaptureDevices = [self.agoraKit enumerateDevices:3];
    for (id device in self.connectedVideoCaptureDevices) {
        [self.cameraSelection addItemWithTitle:([device deviceName])];
    // Populate the NSPopUpButtons with the enumerated device list
    }
}

- (void)rtcEngine:(AgoraRtcEngineKit *)engine device:(NSString*) deviceId type:(AgoraMediaDeviceType) deviceType stateChanged:(NSInteger) state {
    [self loadDevicesInPopUpButtons];
    // Repopulate NSPopUpButtons if a device is plugged in or removed
}

- (IBAction)didClickConfirmButton:(NSButton *)button {
    [self.agoraKit setDevice:0 deviceId:[self.connectedRecordingDevices[self.microphoneSelection.indexOfSelectedItem] deviceId]];
    [self.agoraKit setDevice:1 deviceId:[self.connectedPlaybackDevices[self.speakerSelection.indexOfSelectedItem] deviceId]];
    [self.agoraKit setDevice:3 deviceId:[self.connectedVideoCaptureDevices[self.cameraSelection.indexOfSelectedItem] deviceId]];
    // Set the devices
    [self dismissViewController:self];
}

@end
