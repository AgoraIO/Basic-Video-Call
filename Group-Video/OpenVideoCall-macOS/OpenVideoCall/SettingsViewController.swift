//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

protocol SettingsVCDelegate: NSObjectProtocol {
    func settingsVC(_ settingsVC: SettingsViewController, closeWithDimension dimension: CGSize)
}

class SettingsViewController: NSViewController {

    @IBOutlet weak var profilePopUpButton: NSPopUpButton!
    
    var dimension: CGSize!
    var delegate: SettingsVCDelegate?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        loadProfileItems()
    }
    
    @IBAction func doProfileChanged(_ sender: NSPopUpButton) {
        self.dimension = CGSize.validDimensionList()[sender.indexOfSelectedItem]
    }
    
    @IBAction func doConfirmClicked(_ sender: NSButton) {
        delegate?.settingsVC(self, closeWithDimension: dimension)
    }
}

private extension SettingsViewController {
    func loadProfileItems() {
        profilePopUpButton.addItems(withTitles: CGSize.validDimensionList().map { (dimension) -> String in
            return dimension.description()
        })
        profilePopUpButton.selectItem(withTitle: dimension.description())
    }
}
