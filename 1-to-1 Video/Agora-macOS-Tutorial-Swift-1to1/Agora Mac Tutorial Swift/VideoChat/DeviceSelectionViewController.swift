//
//  DeviceSelectionViewController.swift
//  Agora Mac Tutorial Swift
//
//  Created by James Fang on 8/12/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

import Cocoa

protocol DeviceSelectionDelegate: NSObjectProtocol {
}

class DeviceSelectionViewController: NSViewController {
    @IBOutlet weak var microphoneSelection: NSPopUpButton!
    @IBOutlet weak var speakerSelection: NSPopUpButton!
    @IBOutlet weak var cameraSelection: NSPopUpButton!
    
    var connectedRecordingDevices: NSArray!
    var connectedPlaybackDevices: NSArray!
    var connectedVideoCaptureDevices: NSArray!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.preferredContentSize = NSMakeSize(500, 250)
        loadDevicesInPopUpButtons()
    }
    
    func loadDevicesInPopUpButtons() {
        microphoneSelection.removeAllItems()
        speakerSelection.removeAllItems()
        cameraSelection.removeAllItems()
    
        connectedRecordingDevices = AgoraKit.enumerateDevices(.audioRecording) as NSArray!
        for device in connectedRecordingDevices {
            if let title = (device as AnyObject).deviceName as? String {
                microphoneSelection.addItem(withTitle: title)
            }
        }
    
        connectedPlaybackDevices = AgoraKit.enumerateDevices(.audioPlayout) as NSArray!
        for device in connectedPlaybackDevices {
            if let title = (device as AnyObject).deviceName as? String {
                speakerSelection.addItem(withTitle: title)
            }
        }
        
        connectedVideoCaptureDevices = AgoraKit.enumerateDevices(.videoCapture) as NSArray!
        for device in connectedVideoCaptureDevices {
            if let title = (device as AnyObject).deviceName as? String {
                cameraSelection.addItem(withTitle: title)
            }
        }
        // Populate the NSPopUpButtons with the enumerated device list
    }
    
    @IBAction func didClickConfirmButton(_ sender: NSButton) {
        if let deviceId = (connectedRecordingDevices[microphoneSelection.indexOfSelectedItem] as AnyObject).deviceId as? String {
            AgoraKit.setDevice(.audioRecording, deviceId: deviceId)
        }
        if let deviceId = (connectedPlaybackDevices[speakerSelection.indexOfSelectedItem] as AnyObject).deviceId as? String {
            AgoraKit.setDevice(.audioPlayout, deviceId: deviceId)
        }
        if let deviceId = (connectedVideoCaptureDevices[cameraSelection.indexOfSelectedItem] as AnyObject).deviceId as? String {
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
