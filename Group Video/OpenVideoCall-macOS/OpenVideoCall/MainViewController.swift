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
    
    var videoProfile = AgoraVideoProfile.defaultProfile()
    fileprivate var agoraKit: AgoraRtcEngineKit!
    fileprivate var encryptionType = EncryptionType.xts128
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        loadAgoraKit()
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
            settingsVC.videoProfile = videoProfile
            settingsVC.delegate = self
        } else if segueId == "roomNameVCToVideoVC" {
            let videoVC = segue.destinationController as! RoomViewController
            if let sender = sender as? String {
                videoVC.roomName = sender
            }
            videoVC.encryptionSecret = encryptionTextField.stringValue
            videoVC.encryptionType = encryptionType
            videoVC.videoProfile = videoProfile
            videoVC.delegate = self
        } else if segueId == "roomVCToDevicesVC" {
            let devicesVC = segue.destinationController as! DevicesViewController
            devicesVC.agoraKit = agoraKit
            devicesVC.couldTest = true
        }
    }
    
    //MARK: - user actions
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
    func loadAgoraKit() {
        agoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        agoraKit.enableVideo()
    }
    
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
    func settingsVC(_ settingsVC: SettingsViewController, closeWithProfile profile: AgoraVideoProfile) {
        videoProfile = profile
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
        window.collectionBehavior = NSWindowCollectionBehavior()

        window.contentViewController = self
        
        let size = CGSize(width: 720, height: 600)
        window.minSize = size
        window.setContentSize(size)
        window.maxSize = size
    }
}

extension MainViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, reportAudioVolumeIndicationOfSpeakers speakers: [AgoraRtcAudioVolumeInfo], totalVolume: Int) {
        NotificationCenter.default.post(name: Notification.Name(rawValue: VolumeChangeNotificationKey), object: NSNumber(value: totalVolume as Int))
    }
//    func rtcEngine(_ engine: AgoraRtcEngineKit, reportAudioVolumeIndicationOfSpeakers speakers: [Any]!, totalVolume: Int) {
//        NotificationCenter.default.post(name: Notification.Name(rawValue: VolumeChangeNotificationKey), object: NSNumber(value: totalVolume as Int))
//    }

    func rtcEngine(_ engine: AgoraRtcEngineKit, device deviceId: String, type deviceType: AgoraMediaDeviceType, stateChanged state: Int) {
        NotificationCenter.default.post(name: NSNotification.Name(rawValue: DeviceListChangeNotificationKey), object: NSNumber(value: deviceType.rawValue))
    }
}

//MARK: - text field
extension MainViewController: NSControlTextEditingDelegate {
    override func controlTextDidChange(_ obj: Notification) {
        guard let field = obj.object as? NSTextField else {
            return
        }
        
        let legalString = MediaCharacter.updateToLegalMediaString(from: field.stringValue)
        field.stringValue = legalString
    }
}
