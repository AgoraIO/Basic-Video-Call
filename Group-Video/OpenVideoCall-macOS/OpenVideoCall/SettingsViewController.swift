//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa
import AgoraRtcKit

protocol SettingsVCDataSource: NSObjectProtocol {
    func settingsVCNeedSettings() -> Settings
    func settingsVCNeedAgoraKit() -> AgoraRtcEngineKit
}

protocol SettingsVCDelegate: NSObjectProtocol {
    func settingsVC(_ vc: SettingsViewController, didUpdate settings: Settings)
    func settingsVCNeedClose(_ vc: SettingsViewController)
}

class SettingsViewController: NSViewController {

    @IBOutlet weak var resolutionPopUpButton: NSPopUpButton!
    @IBOutlet weak var frameRatePopUpButton: NSPopUpButton!
    
    @IBOutlet weak var recordDevicePopUpButton: NSPopUpButton!
    @IBOutlet weak var speakerDevicePopUpButton: NSPopUpButton!
    @IBOutlet weak var cameraPopUpButton: NSPopUpButton!
    
    @IBOutlet weak var lineView0: NSView!
    @IBOutlet weak var lineView1: NSView!
    @IBOutlet weak var lineView2: NSView!
    
    @IBOutlet weak var lineView3: NSView!
    @IBOutlet weak var lineView4: NSView!
    @IBOutlet weak var lineView5: NSView!
    
    private var recordDevices: [AgoraRtcDeviceInfo]?
    private var speakerDevices: [AgoraRtcDeviceInfo]?
    private var cameraDevices: [AgoraRtcDeviceInfo]?
    
    private lazy var settings: Settings = dataSource!.settingsVCNeedSettings()
    
    private var agoraKit: AgoraRtcEngineKit {
        return dataSource!.settingsVCNeedAgoraKit()
    }
    
    weak var dataSource: SettingsVCDataSource?
    
    var delegate: SettingsVCDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        updateViews()
        loadDevices()
        loadProfileItems()
        loadFrameRateItems()
    }
    
    @IBAction func doBackPressed(_ sender: NSButton) {
        delegate?.settingsVC(self, didUpdate: settings)
        delegate?.settingsVCNeedClose(self)
    }
    
    @IBAction func doResolutionChanged(_ sender: NSPopUpButton) {
        settings.dimension = CGSize.validDimensionList()[sender.indexOfSelectedItem]
    }
    
    @IBAction func doFrameRateChanged(_ sender: NSPopUpButton) {
        settings.frameRate = AgoraVideoFrameRate.validList()[sender.indexOfSelectedItem]
    }
    
    @IBAction func doRecordDeviceChanged(_ sender: NSPopUpButton) {
        if let deviceId = recordDevices?[sender.indexOfSelectedItem].deviceId {
            settings.recordDevice = .record(deviceId)
        }
    }
    
    @IBAction func doSpeakerDeviceChanged(_ sender: NSPopUpButton) {
        if let deviceId = speakerDevices?[sender.indexOfSelectedItem].deviceId {
            settings.speakerDevice = .speaker(deviceId)
        }
    }
    
    @IBAction func doCameraChanged(_ sender: NSPopUpButton) {
        if let deviceId = cameraDevices?[sender.indexOfSelectedItem].deviceId {
            settings.cameraDevice = .camera(deviceId)
        }
    }
}

private extension SettingsViewController {
    func updateViews() {
        view.wantsLayer = true
        view.layer?.backgroundColor = NSColor.white.cgColor
        
        lineView0.wantsLayer = true
        lineView1.wantsLayer = true
        lineView2.wantsLayer = true
        lineView3.wantsLayer = true
        lineView4.wantsLayer = true
        lineView5.wantsLayer = true
        
        lineView0.layer?.backgroundColor = NSColor.AGLineGray.cgColor
        lineView1.layer?.backgroundColor = NSColor.AGLineGray.cgColor
        lineView2.layer?.backgroundColor = NSColor.AGLineGray.cgColor
        lineView3.layer?.backgroundColor = NSColor.AGLineGray.cgColor
        lineView4.layer?.backgroundColor = NSColor.AGLineGray.cgColor
        lineView5.layer?.backgroundColor = NSColor.AGLineGray.cgColor
    }
    
    func updatePopUpButtonColor(_ button: NSPopUpButton) {
        let attributes = [NSAttributedString.Key.foregroundColor: NSColor.AGTextBlack]
        for item in button.itemArray {
            let title = item.title
            item.attributedTitle = NSAttributedString(string: title,
                                                      attributes: attributes)
        }
    }
}

private extension SettingsViewController {
    func loadProfileItems() {
        resolutionPopUpButton.addItems(withTitles: CGSize.validDimensionList().map { (dimension) -> String in
            return dimension.description()
        })
        resolutionPopUpButton.selectItem(withTitle: settings.dimension.description())
        updatePopUpButtonColor(resolutionPopUpButton)
    }
    
    func loadFrameRateItems() {
        frameRatePopUpButton.addItems(withTitles: AgoraVideoFrameRate.validList().map { (rate) -> String in
            return rate.description
        })
        frameRatePopUpButton.selectItem(withTitle: settings.frameRate.description)
        updatePopUpButtonColor(frameRatePopUpButton)
    }
}

// MARK: - Devices
private extension SettingsViewController {
    func loadDevices() {
        loadDevice(of: settings.cameraDevice)
        loadDevice(of: settings.recordDevice)
        loadDevice(of: settings.speakerDevice)
    }
    
    func loadDevice(of type: Settings.DeviceType) {
        var agType: AgoraMediaDeviceType
        var localId: String?
        var localDevices: [AgoraRtcDeviceInfo]?
        var localButton: NSPopUpButton
        
        switch type {
        case .record(let id):
            agType = .audioRecording
            localId = id
            localDevices = agoraKit.enumerateDevices(agType)! as NSArray as? [AgoraRtcDeviceInfo]
            recordDevices = localDevices
            localButton = recordDevicePopUpButton
        case .speaker(let id):
            agType = .audioPlayout
            localId = id
            localDevices = agoraKit.enumerateDevices(agType)! as NSArray as? [AgoraRtcDeviceInfo]
            speakerDevices = localDevices
            localButton = speakerDevicePopUpButton
        case .camera(let id):
            agType = .videoCapture
            localId = id
            localDevices = agoraKit.enumerateDevices(agType)! as NSArray as? [AgoraRtcDeviceInfo]
            cameraDevices = localDevices
            localButton = cameraPopUpButton
        }
        
        if localId == nil {
            localId = localDevices?.first?.deviceId
        }
        
        guard let devices = localDevices,
            let deviceId = localId else {
            return
        }
        
        updatePopUpButton(localButton, withValue: deviceId, inValueList: devices)
        updatePopUpButtonColor(localButton)
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
}
