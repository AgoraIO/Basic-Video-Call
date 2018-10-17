//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/25/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

protocol SettingsVCDelegate: NSObjectProtocol {
    func settingsVC(_ settingsVC: SettingsViewController, didSelectProfile profile: AgoraVideoProfile)
}

class SettingsViewController: UIViewController {
    
    @IBOutlet weak var profileTableView: UITableView!

    var videoProfile: AgoraVideoProfile! {
        didSet {
            profileTableView?.reloadData()
        }
    }
    weak var delegate: SettingsVCDelegate?
    
    fileprivate let profiles: [AgoraVideoProfile] = AgoraVideoProfile.list()
    
    @IBAction func doConfirmPressed(_ sender: UIButton) {
        delegate?.settingsVC(self, didSelectProfile: videoProfile)
    }
}

extension SettingsViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return profiles.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "profileCell", for: indexPath) as! ProfileCell
        let selectedProfile = profiles[indexPath.row]
        cell.update(with: selectedProfile, isSelected: (selectedProfile == videoProfile))
        
        return cell
    }
}

extension SettingsViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let selectedProfile = profiles[indexPath.row]
        videoProfile = selectedProfile
    }
}

private extension AgoraVideoProfile {
    static func list() -> [AgoraVideoProfile] {
        return AgoraVideoProfile.validProfileList()
    }
}
