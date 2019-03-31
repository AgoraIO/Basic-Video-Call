//
//  MainViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit
import AgoraRtcEngineKit

class MainViewController: UIViewController {
    
    @IBOutlet weak var roomNameTextField: UITextField!
    @IBOutlet weak var encryptionTextField: UITextField!
    @IBOutlet weak var encryptionButton: UIButton!
    
    @IBOutlet weak var lastmileTestButton: UIButton!
    @IBOutlet weak var lastmileTestIndicator: UIActivityIndicatorView!
    @IBOutlet weak var qualityLabel: UILabel!
    @IBOutlet weak var rttLabel: UILabel!
    @IBOutlet weak var uplinkLabel: UILabel!
    @IBOutlet weak var downlinkLabel: UILabel!
    @IBOutlet var infoLabels: [UILabel]!
    
    lazy fileprivate var agoraKit: AgoraRtcEngineKit = {
        let engine = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        return engine
    }()
    fileprivate var dimension = CGSize.defaultDimension()
    fileprivate var encryptionType = EncryptionType.xts128 {
        didSet {
            encryptionButton?.setTitle(encryptionType.description(), for: .normal)
        }
    }
    
    private var isLastmileProbeTesting = false {
        didSet {
            lastmileTestButton?.isHidden = isLastmileProbeTesting
            if isLastmileProbeTesting {
                lastmileTestIndicator?.startAnimating()
            } else {
                lastmileTestIndicator?.stopAnimating()
            }
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "mainToSettings":
            let settingsVC = segue.destination as! SettingsViewController
            settingsVC.dimension = dimension
            settingsVC.delegate = self
        case "mainToRoom":
            let roomVC = segue.destination as! RoomViewController
            roomVC.roomName = (sender as! String)
            roomVC.agoraKit = agoraKit
            roomVC.encryptionSecret = encryptionTextField.text
            roomVC.encryptionType = encryptionType
            roomVC.dimension = dimension
            roomVC.delegate = self
        default:
            break
        }
    }
    
    @IBAction func doLastmileProbeTestPressed(_ sender: UIButton) {
        let config = AgoraLastmileProbeConfig()
        config.probeUplink = true
        config.probeDownlink = true
        config.expectedUplinkBitrate = 5000
        config.expectedDownlinkBitrate = 5000
        
        agoraKit.startLastmileProbeTest(config)
        
        isLastmileProbeTesting = true
        
        for label in infoLabels {
            label.isHidden = true
        }
    }
    
    @IBAction func doRoomNameTextFieldEditing(_ sender: UITextField) {
        if let text = sender.text , !text.isEmpty {
            let legalString = MediaCharacter.updateToLegalMediaString(from: text)
            sender.text = legalString
        }
    }
    
    @IBAction func doEncryptionTextFieldEditing(_ sender: UITextField) {
        if let text = sender.text , !text.isEmpty {
            let legalString = MediaCharacter.updateToLegalMediaString(from: text)
            sender.text = legalString
        }
    }
    
    @IBAction func doEncryptionTypePressed(_ sender: UIButton) {
        let sheet = UIAlertController(title: nil, message: nil, preferredStyle: .actionSheet)
        
        for encryptionType in EncryptionType.allValue {
            let action = UIAlertAction(title: encryptionType.description(), style: .default) { [weak self] _ in
                self?.encryptionType = encryptionType
            }
            sheet.addAction(action)
        }
        
        let cancel = UIAlertAction(title: "Cancel", style: .cancel, handler: nil)
        sheet.addAction(cancel)
        sheet.popoverPresentationController?.sourceView = encryptionButton
        sheet.popoverPresentationController?.permittedArrowDirections = .up
        present(sheet, animated: true, completion: nil)
    }
    
    @IBAction func doJoinPressed(_ sender: UIButton) {
        enter(roomName: roomNameTextField.text)
    }
}

private extension MainViewController {
    func enter(roomName: String?) {
        guard let roomName = roomName , !roomName.isEmpty else {
            return
        }
        performSegue(withIdentifier: "mainToRoom", sender: roomName)
    }
}

extension MainViewController: SettingsVCDelegate {
    func settingsVC(_ settingsVC: SettingsViewController, didSelectDimension dimension: CGSize) {
        self.dimension = dimension
        dismiss(animated: true, completion: nil)
    }
}

extension MainViewController: RoomVCDelegate {
    func roomVCNeedClose(_ roomVC: RoomViewController) {
        agoraKit.delegate = self
        dismiss(animated: true, completion: nil)
    }
}

extension MainViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileQuality quality: AgoraNetworkQuality) {
        qualityLabel.text = "quality: " + quality.description()
        qualityLabel.isHidden = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, lastmileProbeTest result: AgoraLastmileProbeResult) {
        rttLabel.text = "rtt: \(result.rtt)"
        rttLabel.isHidden = false
        uplinkLabel.text = "up: \(result.uplinkReport.description())"
        uplinkLabel.isHidden = false
        downlinkLabel.text = "down: \(result.downlinkReport.description())"
        downlinkLabel.isHidden = false
        
        agoraKit.stopLastmileProbeTest()
        isLastmileProbeTesting = false
    }
}

extension MainViewController: UITextFieldDelegate {
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        switch textField {
        case roomNameTextField:     enter(roomName: textField.text)
        case encryptionTextField:   textField.resignFirstResponder()
        default: break
        }
        
        return true
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
