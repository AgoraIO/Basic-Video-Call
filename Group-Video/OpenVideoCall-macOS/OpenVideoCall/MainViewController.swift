//
//  MainViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

class MainViewController: NSViewController {
    
    @IBOutlet weak var roomInputTextField: NSTextField!
    @IBOutlet weak var encryptionTextField: NSTextField!
    @IBOutlet weak var encryptionPopUpButton: NSPopUpButton!
    @IBOutlet weak var testButton: NSButton!
    @IBOutlet weak var joinButton: NSButton!
    @IBOutlet weak var settingsButton: NSButton!
    
    @IBOutlet weak var lastmileTestButton: NSButton!
    @IBOutlet weak var lastmileTestIndicator: NSProgressIndicator!
    @IBOutlet weak var qualityLabel: NSTextField!
    @IBOutlet weak var rttLabel: NSTextField!
    @IBOutlet weak var uplinkLabel: NSTextField!
    @IBOutlet weak var downlinkLabel: NSTextField!
    
    lazy fileprivate var agoraKit: AgoraRtcEngineKit = {
        let engine = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        engine.setLogFilter(AgoraLogFilter.info.rawValue)
        engine.setLogFile(FileCenter.logFilePath())
        engine.enableVideo()
        return engine
    }()
    var dimension = CGSize.defaultDimension()
    fileprivate var encryptionType = EncryptionType.xts128
    
    private var isLastmileProbeTesting = false {
        didSet {
            lastmileTestButton?.isHidden = isLastmileProbeTesting
            if isLastmileProbeTesting {
                lastmileTestIndicator?.startAnimation(nil)
            } else {
                lastmileTestIndicator?.stopAnimation(nil)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadEncryptionItems()
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        roomInputTextField.becomeFirstResponder()
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier , !segueId.isEmpty else {
            return
        }
        
        if segueId == "roomVCToSettingsVC" {
            let settingsVC = segue.destinationController as! SettingsViewController
            settingsVC.dimension = dimension
            settingsVC.delegate = self
        } else if segueId == "roomNameVCToVideoVC" {
            let videoVC = segue.destinationController as! RoomViewController
            if let sender = sender as? String {
                videoVC.roomName = sender
            }
            videoVC.agoraKit = agoraKit
            videoVC.encryptionSecret = encryptionTextField.stringValue
            videoVC.encryptionType = encryptionType
            videoVC.dimension = dimension
            videoVC.delegate = self
        } else if segueId == "roomVCToDevicesVC" {
            let devicesVC = segue.destinationController as! DevicesViewController
            devicesVC.agoraKit = agoraKit
            devicesVC.couldTest = true
        }
    }
    
    //MARK: - user actions
    @IBAction func doLastmileProbeTestPressed(_ sender: NSButton) {
        let config = AgoraLastmileProbeConfig()
        config.probeUplink = true
        config.probeDownlink = true
        config.expectedUplinkBitrate = 5000
        config.expectedDownlinkBitrate = 5000
        
        agoraKit.startLastmileProbeTest(config)
        
        isLastmileProbeTesting = true
        
        qualityLabel.isHidden = true
        rttLabel.isHidden = true
        uplinkLabel.isHidden = true
        downlinkLabel.isHidden = true
    }
    
    @IBAction func doEncryptionChanged(_ sender: NSPopUpButton) {
        encryptionType = EncryptionType.allValue[sender.indexOfSelectedItem]
    }
    
    @IBAction func doTestClicked(_ sender: NSButton) {
        performSegue(withIdentifier: "roomVCToDevicesVC", sender: nil)
    }
    
    @IBAction func doJoinClicked(_ sender: NSButton) {
        enter(roomName: roomInputTextField.stringValue)
    }
    
    @IBAction func doSettingsClicked(_ sender: NSButton) {
        performSegue(withIdentifier: "roomVCToSettingsVC", sender: nil)
    }
}

private extension MainViewController {
    func loadEncryptionItems() {
        encryptionPopUpButton.addItems(withTitles: EncryptionType.allValue.map { type -> String in
            return type.description()
        })
        encryptionPopUpButton.selectItem(withTitle: encryptionType.description())
    }
    
    func enter(roomName: String?) {
        guard let roomName = roomName , !roomName.isEmpty else {
            return
        }
        
        performSegue(withIdentifier: "roomNameVCToVideoVC", sender: roomName)
    }
}

extension MainViewController: SettingsVCDelegate {
    func settingsVC(_ settingsVC: SettingsViewController, closeWithDimension dimension: CGSize) {
        self.dimension = dimension
        settingsVC.view.window?.contentViewController = self
    }
}

extension MainViewController: RoomVCDelegate {
    func roomVCNeedClose(_ roomVC: RoomViewController) {
        guard let window = roomVC.view.window else {
            return
        }
        
        if window.styleMask.contains(.fullScreen) {
            window.toggleFullScreen(nil)
        }
        
        window.styleMask.insert([.fullSizeContentView, .miniaturizable])
        window.delegate = nil
        window.collectionBehavior = NSWindow.CollectionBehavior()

        window.contentViewController = self
        
        let size = CGSize(width: 720, height: 600)
        window.minSize = size
        window.setContentSize(size)
        window.maxSize = size
        
        agoraKit.delegate = self
    }
}

extension MainViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, reportAudioVolumeIndicationOfSpeakers speakers: [AgoraRtcAudioVolumeInfo], totalVolume: Int) {
        NotificationCenter.default.post(name: Notification.Name(rawValue: VolumeChangeNotificationKey), object: NSNumber(value: totalVolume as Int))
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, device deviceId: String, type deviceType: AgoraMediaDeviceType, stateChanged state: Int) {
        NotificationCenter.default.post(name: NSNotification.Name(rawValue: DeviceListChangeNotificationKey), object: NSNumber(value: deviceType.rawValue))
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        qualityLabel.stringValue = "quality: " + quality.description()
        qualityLabel.isHidden = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileProbeTest result: AgoraLastmileProbeResult) {
        rttLabel.stringValue = "rtt: \(result.rtt)"
        rttLabel.isHidden = false
        uplinkLabel.stringValue = "up: \(result.uplinkReport.description())"
        uplinkLabel.isHidden = false
        downlinkLabel.stringValue = "down: \(result.downlinkReport.description())"
        downlinkLabel.isHidden = false
        
        agoraKit.stopLastmileProbeTest()
        isLastmileProbeTesting = false
    }
}

//MARK: - text field
extension MainViewController: NSControlTextEditingDelegate {
    func controlTextDidChange(_ obj: Notification) {
        guard let field = obj.object as? NSTextField else {
            return
        }
        
        let legalString = MediaCharacter.updateToLegalMediaString(from: field.stringValue)
        field.stringValue = legalString
    }
}

extension AgoraNetworkQuality {
    func description() -> String {
        switch self {
        case .excellent: return "excellent"
        case .good:      return "good"
        case .poor:      return "poor"
        case .bad:       return "bad"
        case .vBad:      return "very bad"
        case .down:      return "down"
        case .unknown:   return "unknown"
        }
    }
}

extension AgoraLastmileProbeOneWayResult {
    func description() -> String {
        return "packetLoss: \(packetLossRate), jitter: \(jitter), availableBandwidth: \(availableBandwidth)"
    }
}
