//
//  OptionsViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/22.
//  Copyright © 2019 Agora. All rights reserved.
//

import UIKit

struct Options {
    var debugMode: Bool = false
    var enableSpeakerPhone: Bool = false
}

protocol OptionsVCDelegate: NSObjectProtocol {
    func optionsVC(_ vc: OptionsViewController, didUpdate options: Options)
}

class OptionsViewController: UITableViewController {
    
    @IBOutlet weak var debugSwitch: UISwitch!
    @IBOutlet weak var enableSpeakerSwitch: UISwitch!
    
    weak var delegate: OptionsVCDelegate?
    
    var options: Options? {
        didSet {
            guard let _ = oldValue else {
                return
            }
            
            if let options = options {
                delegate?.optionsVC(self, didUpdate: options)
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        if let options = options {
            debugSwitch.isOn = options.debugMode
            enableSpeakerSwitch.isOn = options.enableSpeakerPhone
        }
    }
    
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        return 16
    }
    
    @IBAction func doDebugPressed(_ sender: UISwitch) {
        options?.debugMode = sender.isOn
    }
    
    @IBAction func doEnableSpeakerPhoneChanged(_ sender: UISwitch) {
        options?.enableSpeakerPhone = sender.isOn
    }
}
