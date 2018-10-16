//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

protocol SettingsVCDelegate: class {
    func settingsVC(_ settingsVC: SettingsViewController, closeWithProfile videoProfile: AgoraVideoProfile)
}

class SettingsViewController: NSViewController {

    @IBOutlet weak var profilePopUpButton: NSPopUpButton!
    
    var videoProfile: AgoraVideoProfile!
    var delegate: SettingsVCDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        loadProfileItems()
    }
    
    @IBAction func doProfileChanged(_ sender: NSPopUpButton) {
        let profile = AgoraVideoProfile.validProfileList()[sender.indexOfSelectedItem]
        videoProfile = profile
    }
    
    @IBAction func doConfirmClicked(_ sender: NSButton) {
        delegate?.settingsVC(self, closeWithProfile: videoProfile)
    }
}

private extension SettingsViewController {
    func loadProfileItems() {
        profilePopUpButton.addItems(withTitles: AgoraVideoProfile.validProfileList().map { (res) -> String in
            return res.description()
        })
        profilePopUpButton.selectItem(withTitle: videoProfile.description())
    }
}
