//
//  DevicesViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/2/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

let DeviceListChangeNotificationKey = "io.agora.deviceListChangeNotification"
let VolumeChangeNotificationKey = "io.agora.volumeChangeNotification"

class DevicesViewController: NSViewController {
    
    @IBOutlet weak var inputDevicePopUpButton: NSPopUpButton!
    @IBOutlet weak var inputDeviceVolSlider: NSSlider!
    @IBOutlet weak var intputDeviceTestButton: NSButton!
    @IBOutlet weak var inputDeviceVolLevelIndicator: NSLevelIndicator!
    
    @IBOutlet weak var outputDevicePopUpButton: NSPopUpButton!
    @IBOutlet weak var outputDeviceVolSlider: NSSlider!
    @IBOutlet weak var outputDeviceTestButton: NSButton!
    
    @IBOutlet weak var cameraPopUpButton: NSPopUpButton!
    @IBOutlet weak var cameraTestButton: NSButton!
    @IBOutlet weak var cameraPreviewView: NSView!
    
    var agoraKit: AgoraRtcEngineKit!
    var couldTest = true
    
    fileprivate var recordingDeviceId: String?
    fileprivate var recordingDevices = [AgoraRtcDeviceInfo]()
    fileprivate var playoutDeviceId: String?
    fileprivate var playoutDevices = [AgoraRtcDeviceInfo]()
    fileprivate var captureDeviceId: String?
    fileprivate var captureDevices = [AgoraRtcDeviceInfo]()
    
    fileprivate var isInputTesting = false {
        didSet {
            config(button: intputDeviceTestButton, isTesting: isInputTesting)
            if isInputTesting {
                agoraKit?.startRecordingDeviceTest(200)
            } else {
                agoraKit?.stopRecordingDeviceTest()
            }
            inputDeviceVolLevelIndicator?.isHidden = !isInputTesting
        }
    }
    fileprivate var isOutputTesting = false {
        didSet {
            config(button: outputDeviceTestButton, isTesting: isOutputTesting)
            if isOutputTesting {
                if let path = Bundle.main.path(forResource: "test", ofType: "wav") {
                    agoraKit?.startPlaybackDeviceTest(path)
                }
            } else {
                agoraKit?.stopPlaybackDeviceTest()
            }
        }
    }
    fileprivate var isCameraputTesting = false {
        didSet {
            config(button: cameraTestButton, isTesting: isCameraputTesting)
            if isCameraputTesting {
                if let view = cameraPreviewView {
                    agoraKit?.startCaptureDeviceTest(view)
                }
            } else {
                agoraKit?.stopCaptureDeviceTest()
            }
        }
    }
    fileprivate var deviceVolume = 0 {
        didSet {
            inputDeviceVolLevelIndicator?.integerValue = deviceVolume
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        cameraPreviewView.wantsLayer = true
        cameraPreviewView.layer?.backgroundColor = NSColor.black.cgColor
        
        configButtonStyle()
        
        loadDevices()
    }
    
    override func viewWillAppear() {
        super.viewWillAppear()
        configStyle(of: view.window!)
    }
    
    override func viewWillDisappear() {
        super.viewWillDisappear()
        
        if couldTest {
            if isInputTesting {
                isInputTesting = false
            }
            if isOutputTesting {
                isOutputTesting = false
            }
            if isCameraputTesting {
                isCameraputTesting = false
            }
        }
    }
    
    @IBAction func doInputDeviceChanged(_ sender: NSPopUpButton) {
        if isInputTesting {
            isInputTesting = false
        }
        if let deviceId = recordingDevices[sender.indexOfSelectedItem].deviceId {
            agoraKit.setDevice(.audioRecording, deviceId: deviceId)
        }
    }
    
    @IBAction func doInputDeviceTestClicked(_ sender: NSButton) {
        isInputTesting = !isInputTesting
    }
    
    @IBAction func doInputVolSliderChanged(_ sender: NSSlider) {
        let vol = sender.intValue
        agoraKit.setDeviceVolume(.audioRecording, volume: vol)
    }
    
    @IBAction func doOutputDeviceChanged(_ sender: NSPopUpButton) {
        if isOutputTesting {
            isOutputTesting = false
        }
        if let deviceId = playoutDevices[sender.indexOfSelectedItem].deviceId {
            agoraKit.setDevice(.audioPlayout, deviceId: deviceId)
        }
    }
    
    @IBAction func doOutputDeviceTestClicked(_ sender: NSButton) {
        isOutputTesting = !isOutputTesting
    }
    
    @IBAction func doOutputVolSliderChanged(_ sender: NSSlider) {
        let vol = sender.intValue
        agoraKit.setDeviceVolume(.audioPlayout, volume: vol)
    }
    
    @IBAction func doCameraChanged(_ sender: NSPopUpButton) {
        if isCameraputTesting {
            isCameraputTesting = false
        }
        if let deviceId = captureDevices[sender.indexOfSelectedItem].deviceId {
            agoraKit.setDevice(.videoCapture, deviceId: deviceId)
        }
    }
    
    @IBAction func doCameraTestClicked(_ sender: NSButton) {
        isCameraputTesting = !isCameraputTesting
    }
}

private extension DevicesViewController {
    func configStyle(of window: NSWindow) {
        window.styleMask.insert(.fullSizeContentView)
        window.titlebarAppearsTransparent = true
        window.isMovableByWindowBackground = true
        
        window.minSize = CGSize(width: 600, height: 600)
        window.maxSize = CGSize(width: 600, height: 600)
    }
    
    func configButtonStyle() {
        config(button: intputDeviceTestButton, isTesting: false)
        config(button: outputDeviceTestButton, isTesting: false)
        config(button: cameraTestButton, isTesting: false)
        
        intputDeviceTestButton.isHidden = !couldTest
        outputDeviceTestButton.isHidden = !couldTest
        cameraTestButton.isHidden = !couldTest
    }
    
    func config(button: NSButton, isTesting: Bool) {
        button.title = isTesting ? "Stop Test" : "Test"
    }
}

//MARK: - device list
private extension DevicesViewController {
    func loadDevices() {
        loadDevice(of: .audioPlayout)
        loadDevice(of: .audioRecording)
        loadDevice(of: .videoCapture)
        
        NotificationCenter.default.addObserver(forName: NSNotification.Name(rawValue: DeviceListChangeNotificationKey), object: nil, queue: nil) { [weak self] (notify) in
            if let obj = notify.object as? NSNumber, let type = AgoraMediaDeviceType(rawValue: obj.intValue) {
                self?.loadDevice(of: type)
            }
        }
        
        if couldTest {
            NotificationCenter.default.addObserver(forName: NSNotification.Name(rawValue: VolumeChangeNotificationKey), object: nil, queue: nil, using: { [weak self] (notify) in
                if let obj = notify.object as? NSNumber {
                    self?.deviceVolume = obj.intValue
                }
            })
        }
    }
    
    func loadDevice(of type: AgoraMediaDeviceType) {
        guard let devices = agoraKit.enumerateDevices(type)! as NSArray as? [AgoraRtcDeviceInfo] else {
            return
        }
        
        let deviceId = agoraKit.getDeviceId(type)
        switch type {
        case .audioRecording:
            recordingDevices = devices
            recordingDeviceId = deviceId
            updatePopUpButton(inputDevicePopUpButton, withValue: deviceId, inValueList: devices)
        case .audioPlayout:
            playoutDevices = devices
            playoutDeviceId = deviceId
            updatePopUpButton(outputDevicePopUpButton, withValue: deviceId, inValueList: devices)
        case .videoCapture:
            captureDevices = devices
            captureDeviceId = deviceId
            updatePopUpButton(cameraPopUpButton, withValue: deviceId, inValueList: devices)
        default:
            break
        }
        
        updateVolume(of: type)
    }
    
    func updatePopUpButton(_ button: NSPopUpButton, withValue value: String?, inValueList list: [AgoraRtcDeviceInfo]) {
        button.removeAllItems()
        button.addItems(withTitles: list.map({ (info) -> String in
            return info.deviceName!
        }))
        
        let deviceIds = list.map { (info) -> String in
            return info.deviceId!
        }
        if let value = value, let index = deviceIds.index(of: value) {
            button.selectItem(at: index)
        }
    }
    
    func updateVolume(of type: AgoraMediaDeviceType) {
        switch type {
        case .audioRecording:
            let vol = agoraKit.getDeviceVolume(type)
            inputDeviceVolSlider.intValue = vol
        case .audioPlayout:
            let vol = agoraKit.getDeviceVolume(type)
            outputDeviceVolSlider.intValue = vol
        default:
            return
        }
    }
}
