//
//  NetTestViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import UIKit

class NetTestViewController: UITableViewController {
    
    @IBOutlet weak var qualityLabel: UILabel!
    @IBOutlet weak var rttLabel: UILabel!
    @IBOutlet weak var uplinkLabel: UILabel!
    @IBOutlet weak var downlinkLabel: UILabel!
    
    @IBOutlet var infoLabels: [UILabel]!
    
    private var isLastmileProbeTesting = false {
        didSet {
            if isLastmileProbeTesting {
                let config = AgoraLastmileProbeConfig()
                config.probeUplink = true
                config.probeDownlink = true
                config.expectedUplinkBitrate = 5000
                config.expectedDownlinkBitrate = 5000
                agoraKit?.startLastmileProbeTest(config)
                self.title = "Testing..."
            } else {
                agoraKit?.stopLastmileProbeTest()
                self.title = "Test result"
            }
            activityVC.isAnimating = isLastmileProbeTesting
        }
    }
    
    lazy var activityVC: ActivityViewController = {
        let story = UIStoryboard(name: "Main", bundle: nil)
        let vc = story.instantiateViewController(withIdentifier: "ActivityViewController") as! ActivityViewController
        return vc
    }()
    
    var agoraKit: AgoraRtcEngineKit? {
        didSet {
            agoraKit?.delegate = self
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        view.addSubview(activityVC.view)
        isLastmileProbeTesting = true
    }
}

extension NetTestViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        qualityLabel.text = quality.description()
        qualityLabel.isHidden = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileProbeTest result: AgoraLastmileProbeResult) {
        rttLabel.text = "\(result.rtt) ms"
        uplinkLabel.text = result.uplinkReport.description()
        downlinkLabel.text = result.downlinkReport.description()
        isLastmileProbeTesting = false
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
        return "\(packetLossRate)"
    }
}
