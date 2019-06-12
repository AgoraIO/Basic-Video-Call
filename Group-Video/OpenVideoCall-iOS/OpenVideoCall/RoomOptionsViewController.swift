//
//  RoomOptionsViewController.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/22.
//  Copyright © 2019 Agora. All rights reserved.
//

import UIKit

struct RoomOptions {
    var isDebugMode: Bool = false
}

protocol RoomOptionsVCDataSource: NSObjectProtocol {
    func roomOptionsVCNeedOptions() -> RoomOptions
}

protocol RoomOptionsVCDelegate: NSObjectProtocol {
    func roomOptionsVC(_ vc: RoomOptionsViewController, debugModeDid enable: Bool)
}

class RoomOptionsViewController: UITableViewController {
    
    @IBOutlet weak var debugSwitch: UISwitch!
    
    weak var delegate: RoomOptionsVCDelegate?
    weak var dataSource: RoomOptionsVCDataSource?
    
    var options: RoomOptions {
        return dataSource!.roomOptionsVCNeedOptions()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        debugSwitch.isOn = options.isDebugMode
    }
    
    @IBAction func doDebugPressed(_ sender: UISwitch) {
        let debugMode = sender.isOn
        delegate?.roomOptionsVC(self, debugModeDid: debugMode)
    }
}

extension RoomOptionsViewController {
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        return 16
    }
}
