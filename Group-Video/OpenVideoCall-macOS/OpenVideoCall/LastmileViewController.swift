//
//  LastmileViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import Cocoa
import AgoraRtcKit

protocol LastmileVCDelegate: NSObjectProtocol {
    func lastmileVCNeedClose(_ lastmileVC: LastmileViewController)
}

protocol LastmileVCDataSource: NSObjectProtocol {
    func lastmileVCNeedAgoraKit() -> AgoraRtcEngineKit
}

class LastmileViewController: NSViewController {
    
    @IBOutlet weak var titleLabel: NSTextField!
    @IBOutlet weak var qualityLabel: NSTextField!
    @IBOutlet weak var rttLabel: NSTextField!
    @IBOutlet weak var uplinkLabel: NSTextField!
    @IBOutlet weak var downlinkLabel: NSTextField!
    @IBOutlet weak var activityView: NSProgressIndicator!
    
    @IBOutlet weak var lineView0: NSView!
    @IBOutlet weak var lineView1: NSView!
    @IBOutlet weak var lineView2: NSView!
    @IBOutlet weak var lineView3: NSView!
    @IBOutlet weak var lineView4: NSView!
    
    private var isLastmileProbeTesting = false {
        didSet {
            if isLastmileProbeTesting {
                let config = AgoraLastmileProbeConfig()
                config.probeUplink = true
                config.probeDownlink = true
                config.expectedUplinkBitrate = 5000
                config.expectedDownlinkBitrate = 5000
                agoraKit.startLastmileProbeTest(config)
                activityView?.isHidden = false
                activityView?.startAnimation(nil)
                self.titleLabel.stringValue = "Testing..."
            } else {
                agoraKit.stopLastmileProbeTest()
                activityView?.stopAnimation(nil)
                activityView?.isHidden = true
                self.titleLabel.stringValue = "Test result"
            }
        }
    }
    
    private var agoraKit: AgoraRtcEngineKit {
        return dataSource!.lastmileVCNeedAgoraKit()
    }
    
    weak var dataSource: LastmileVCDataSource?
    
    var delegate: LastmileVCDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        updateViews()
        agoraKit.delegate = self
        isLastmileProbeTesting = true
    }
    
    deinit {
        isLastmileProbeTesting = false
    }
    
    @IBAction func doBackPressed(_ sender: NSButton) {
        delegate?.lastmileVCNeedClose(self)
    }
    
    private func updateViews() {
        view.layer?.backgroundColor = NSColor.white.cgColor
        
        lineView0.layer?.backgroundColor = NSColor.AGGray.cgColor
        lineView1.layer?.backgroundColor = NSColor.AGGray.cgColor
        lineView2.layer?.backgroundColor = NSColor.AGGray.cgColor
        lineView3.layer?.backgroundColor = NSColor.AGGray.cgColor
        lineView4.layer?.backgroundColor = NSColor.AGGray.cgColor
    }
}

extension LastmileViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        qualityLabel.stringValue = quality.description()
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileProbeTest result: AgoraLastmileProbeResult) {
        rttLabel.stringValue = "\(result.rtt) ms"
        uplinkLabel.stringValue = result.uplinkReport.description()
        downlinkLabel.stringValue = result.downlinkReport.description()
        isLastmileProbeTesting = false
    }
}
