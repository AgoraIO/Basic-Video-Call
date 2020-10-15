//
//  LastmileViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/23.
//  Copyright © 2019 Agora. All rights reserved.
//

import UIKit
import AgoraRtcKit

protocol LastmileVCDataSource: NSObjectProtocol {
    func lastmileVCNeedAgoraKit() -> AgoraRtcEngineKit
}

class LastmileViewController: UITableViewController {
    
    @IBOutlet weak var qualityLabel: UILabel!
    @IBOutlet weak var rttLabel: UILabel!
    @IBOutlet weak var uplinkLabel: UILabel!
    @IBOutlet weak var downlinkLabel: UILabel!
    
    private var agoraKit: AgoraRtcEngineKit {
        return dataSource!.lastmileVCNeedAgoraKit()
    }
    
    private var isLastmileProbeTesting = false {
        didSet {
            if isLastmileProbeTesting {
                let config = AgoraLastmileProbeConfig()
                config.probeUplink = true
                config.probeDownlink = true
                config.expectedUplinkBitrate = 5000
                config.expectedDownlinkBitrate = 5000
                agoraKit.startLastmileProbeTest(config)
                activityView?.startAnimating()
                self.title = "Testing..."
            } else {
                agoraKit.stopLastmileProbeTest()
                activityView?.stopAnimating()
                self.title = "Test result"
            }
        }
    }
    
    private var activityView: UIActivityIndicatorView?
    
    weak var dataSource: LastmileVCDataSource?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        addActivityView()
        agoraKit.delegate = self
        isLastmileProbeTesting = true
        view.backgroundColor = UIColor.white
    }
       
    deinit {
        isLastmileProbeTesting = false
    }
}

extension LastmileViewController: AgoraRtcEngineDelegate {
    /// Reports the last mile network quality of the local user once every two seconds before the user joins a channel.
    /// - Parameters:
    ///   - engine: the Agora engine
    ///   - quality: An enum describing the network quality. Possible values are: Unknown = 0, Excellent = 1, Good = 2, Poor = 3, Bad = 4, VBad = 5, Down = 6, Unsupported = 7, Detecting = 8.
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        qualityLabel.text = quality.description()
    }
    
    /// Reports the last-mile network probe result.
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileProbeTest result: AgoraLastmileProbeResult) {
        rttLabel.text = "\(result.rtt) ms"
        uplinkLabel.text = result.uplinkReport.description()
        downlinkLabel.text = result.downlinkReport.description()
        isLastmileProbeTesting = false
    }
}

extension LastmileViewController {
    func addActivityView() {
        let activityView = UIActivityIndicatorView(style: .white)
        let rightItem = UIBarButtonItem(customView: activityView)
        navigationItem.rightBarButtonItem = rightItem
        self.activityView = activityView
    }
}

extension AgoraLastmileProbeOneWayResult {
    func description() -> String {
        return "\(packetLossRate)"
    }
}
