//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/25/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

protocol SettingsVCDelegate: NSObjectProtocol {
    func settingsVC(_ settingsVC: SettingsViewController, didSelectDimension dimension: CGSize)
}

class SettingsViewController: UIViewController {
    
    @IBOutlet weak var profileTableView: UITableView!

    var dimension: CGSize! {
        didSet {
            profileTableView?.reloadData()
        }
    }
    weak var delegate: SettingsVCDelegate?
    
    fileprivate let dimensionList: [CGSize] = CGSize.validDimensionList()
    
    @IBAction func doConfirmPressed(_ sender: UIButton) {
        delegate?.settingsVC(self, didSelectDimension: dimension)
    }
}

extension SettingsViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return dimensionList.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "dimensionCell", for: indexPath) as! DimensionCell
        let selectedDimension = dimensionList[indexPath.row]
        cell.update(with: selectedDimension, isSelected: (selectedDimension == dimension))
        
        return cell
    }
}

extension SettingsViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        dimension = dimensionList[indexPath.row]
    }
}
