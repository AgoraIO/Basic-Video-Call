//
//  DeviceSelectionViewController.swift
//  Agora Mac Tutorial Swift
//
//  Created by James Fang on 8/12/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

import Cocoa
import AgoraRtcEngineKit

class DeviceSelectionViewController: NSViewController {
    @IBOutlet weak var microphoneSelection: NSPopUpButton!
    @IBOutlet weak var speakerSelection: NSPopUpButton!
    @IBOutlet weak var cameraSelection: NSPopUpButton!
    
    var connectedRecordingDevices: [AgoraRtcDeviceInfo]!
    var connectedPlaybackDevices: [AgoraRtcDeviceInfo]!
    var connectedVideoCaptureDevices: [AgoraRtcDeviceInfo]!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.preferredContentSize = NSMakeSize(500, 250)
        loadDevicesInPopUpButtons()
    }
    
    func loadDevicesInPopUpButtons() {
        microphoneSelection.removeAllItems()
        speakerSelection.removeAllItems()
        cameraSelection.removeAllItems()
    
        connectedRecordingDevices = AgoraKit.enumerateDevices(.audioRecording)
        for device in connectedRecordingDevices {
            if let title = device.deviceName {
                microphoneSelection.addItem(withTitle: title)
            }
        }
    
        connectedPlaybackDevices = AgoraKit.enumerateDevices(.audioPlayout)
        for device in connectedPlaybackDevices {
            if let title = device.deviceName {
                speakerSelection.addItem(withTitle: title)
            }
        }
        
        connectedVideoCaptureDevices = AgoraKit.enumerateDevices(.videoCapture)
        for device in connectedVideoCaptureDevices {
            if let title = device.deviceName {
                cameraSelection.addItem(withTitle: title)
            }
        }
        // Populate the NSPopUpButtons with the enumerated device list
    }
    
    @IBAction func didClickConfirmButton(_ sender: NSButton) {
        if let deviceId = connectedRecordingDevices[microphoneSelection.indexOfSelectedItem].deviceId {
            AgoraKit.setDevice(.audioRecording, deviceId: deviceId)
        }
        if let deviceId = connectedPlaybackDevices[speakerSelection.indexOfSelectedItem].deviceId {
            AgoraKit.setDevice(.audioPlayout, deviceId: deviceId)
        }
        if let deviceId = connectedVideoCaptureDevices[cameraSelection.indexOfSelectedItem].deviceId {
            AgoraKit.setDevice(.videoCapture, deviceId: deviceId)
        }
        // Set the devices
        self.dismiss(self)
    }
}

extension DeviceSelectionViewController: AgoraRtcEngineDelegate {
    @objc(rtcEngine:device:type:stateChanged:) func rtcEngine(_ engine: AgoraRtcEngineKit, device deviceId: String, type deviceType: AgoraMediaDeviceType, stateChanged state: Int) {
        loadDevicesInPopUpButtons()
        // Repopulate NSPopUpButtons if a device is plugged in or removed
    }
}
