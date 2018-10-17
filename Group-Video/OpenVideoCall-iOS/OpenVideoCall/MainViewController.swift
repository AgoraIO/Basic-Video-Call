//
//  MainViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/17.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit

class MainViewController: UIViewController {
    
    @IBOutlet weak var roomNameTextField: UITextField!
    @IBOutlet weak var encryptionTextField: UITextField!
    @IBOutlet weak var encryptionButton: UIButton!
    
    fileprivate var videoProfile = AgoraVideoProfile.defaultProfile()
    fileprivate var encryptionType = EncryptionType.xts128 {
        didSet {
            encryptionButton?.setTitle(encryptionType.description(), for: UIControlState())
        }
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "mainToSettings":
            let settingsVC = segue.destination as! SettingsViewController
            settingsVC.videoProfile = videoProfile
            settingsVC.delegate = self
        case "mainToRoom":
            let roomVC = segue.destination as! RoomViewController
            roomVC.roomName = (sender as! String)
            roomVC.encryptionSecret = encryptionTextField.text
            roomVC.encryptionType = encryptionType
            roomVC.videoProfile = videoProfile
            roomVC.delegate = self
        default:
            break
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
    func settingsVC(_ settingsVC: SettingsViewController, didSelectProfile profile: AgoraVideoProfile) {
        videoProfile = profile
        dismiss(animated: true, completion: nil)
    }
}

extension MainViewController: RoomVCDelegate {
    func roomVCNeedClose(_ roomVC: RoomViewController) {
        dismiss(animated: true, completion: nil)
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
