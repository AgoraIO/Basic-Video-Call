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
    
    weak var titleView: UIImageView?
    
    lazy fileprivate var agoraKit: AgoraRtcEngineKit = {
        let engine = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: nil)
        engine.setLogFilter(AgoraLogFilter.info.rawValue)
        engine.setLogFile(FileCenter.logFilePath())
        return engine
    }()
    
    fileprivate var settings = PreSettings() {
        didSet {
            if let encryptionType = settings.encryptionType {
                encryptionButton.setTitle(encryptionType.description(), for: .normal)
            }
        }
    }
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .lightContent
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "mainToSettings":
            let settingsVC = segue.destination as! SettingsViewController
            settingsVC.settings = settings
            settingsVC.delegate = self
        case "mainToRoom":
            let roomVC = segue.destination as! RoomViewController
            roomVC.settings = settings
            roomVC.agoraKit = agoraKit
            roomVC.roomName = sender as? String // roomName use as room vc title and rtc channel id
        case "mainToNetTest":
            let testVC = segue.destination as! NetTestViewController
            testVC.agoraKit = agoraKit
        default:
            break
        }
    }
    
    override func viewWillAppear(_ animated: Bool) {
        self.titleView?.isHidden = false
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        self.titleView?.isHidden = true
    }
    
    override func viewDidLoad() {
        updateViews()
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        view.endEditing(true)
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
                self?.settings.encryptionType = encryptionType
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
        
        let imageView = UIImageView(image: UIImage(named: "logo"))
        let width: CGFloat = 108
        let height: CGFloat = 30
        let screenWidth = UIScreen.main.bounds.width
        let x = (screenWidth - width) * 0.5
        let y: CGFloat = 8
        imageView.frame = CGRect(x: x, y: y, width: width, height: height)
        imageView.contentMode = .scaleAspectFit
        self.titleView = imageView
        self.navigationController?.navigationBar.addSubview(imageView)
    }
    
    func enter(roomName: String?) {
        guard let roomName = roomName , !roomName.isEmpty else {
            return
        }
        performSegue(withIdentifier: "mainToRoom", sender: roomName)
    }
}

extension MainViewController: SettingsVCDelegate {
    func settingsVC(_ settingsVC: SettingsViewController, didSelect settings: PreSettings) {
        self.settings = settings
    }
}

extension MainViewController: UITextFieldDelegate {
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        switch textField {
        case roomNameTextField:     enter(roomName: textField.text)
        case encryptionTextField:   settings.encryptionType?.updateText(roomNameTextField.text); textField.resignFirstResponder()
        default: break
        }
        return true
    }
}

