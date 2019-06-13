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
    @IBOutlet weak var testNetworkButton: UIButton!
    
    private lazy var agoraKit: AgoraRtcEngineKit = {
        let engine = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: nil)
        engine.setLogFilter(AgoraLogFilter.info.rawValue)
        engine.setLogFile(FileCenter.logFilePath())
        return engine
    }()
    
    private var encryptionType = EncryptionType.xts128(nil) {
        didSet {
            encryptionButton.setTitle(encryptionType.description(), for: .normal)
        }
    }
    
    private var settings = Settings()
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "mainToSettings":
            let settingsVC = segue.destination as? SettingsViewController
            settingsVC?.delegate = self
            settingsVC?.dataSource = self
        case "mainToLastmile":
            let testVC = segue.destination as? LastmileViewController
            testVC?.dataSource = self
        case "mainToRoom":
            let roomVC = segue.destination as? RoomViewController
            roomVC?.dataSource = self
        default:
            break
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        updateViews()
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
        for encryptionType in EncryptionType.allValues {
            let action = UIAlertAction(title: encryptionType.description(), style: .default) { [unowned self] _ in
                self.encryptionType = encryptionType
            }
            sheet.addAction(action)
        }
        
        let cancel = UIAlertAction(title: "Cancel", style: .cancel, handler: nil)
        sheet.addAction(cancel)
        present(sheet, animated: true, completion: nil)
    }
    
    @IBAction func doJoinPressed(_ sender: UIButton) {
        // start join room when join button pressed
        enter(roomName: roomNameTextField.text)
    }
}

private extension MainViewController {
    func updateViews() {
        // view elements initialization
        self.encryptionButton.layer.borderColor = UIColor.AGGray.cgColor    
        self.testNetworkButton.layer.borderColor = UIColor.AGGray.cgColor
    }
    
    func enter(roomName: String?) {
        guard let roomName = roomName , !roomName.isEmpty else {
            return
        }
        
        if let encryptionText = encryptionTextField.text {
            settings.encryptionType = encryptionType
            settings.encryptionType?.updateText(encryptionText)
        } else {
            settings.encryptionType = nil
        }
        
        settings.roomName = roomName
        performSegue(withIdentifier: "mainToRoom", sender: nil)
    }
}

extension MainViewController: SettingsVCDelegate {
    func settingsVC(_ vc: SettingsViewController, didSelect dimension: CGSize) {
        settings.dimension = dimension
    }
    
    func settingsVC(_ vc: SettingsViewController, didSelect frameRate: AgoraVideoFrameRate) {
        settings.frameRate = frameRate
    }
}

extension MainViewController: SettingsVCDataSource {
    func settingsVCNeedSettings() -> Settings {
        return settings
    }
}

extension MainViewController: LastmileVCDataSource {
    func lastmileVCNeedAgoraKit() -> AgoraRtcEngineKit {
        return agoraKit
    }
}

extension MainViewController: RoomVCDataSource {
    func roomVCNeedSettings() -> Settings {
        return settings
    }
    
    func roomVCNeedAgoraKit() -> AgoraRtcEngineKit {
        return agoraKit
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

extension MainViewController {
    override func viewWillAppear(_ animated: Bool) {
        navigationItem.titleView?.isHidden = false
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        navigationItem.titleView?.isHidden = true
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        view.endEditing(true)
    }
}
