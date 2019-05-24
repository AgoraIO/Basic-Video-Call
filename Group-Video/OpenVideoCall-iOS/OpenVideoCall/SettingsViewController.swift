//
//  SettingsViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/25/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

protocol SettingsVCDelegate: NSObjectProtocol {
    func settingsVC(_ settingsVC: SettingsViewController, didSelect settings : PreSettings)
}

class SettingsViewController: UITableViewController {
    
    @IBOutlet weak var frameRateButton: UIButton!
    @IBOutlet weak var frameRatePickerView: UIPickerView!
    @IBOutlet weak var dimensionCollectionView: UICollectionView!
    
    var dimension: CGSize?
    var lastFrameRateRow: Int = 3
    
    var settings: PreSettings? {
        didSet {
            if let _ = settings?.dimension {
                dimensionCollectionView?.reloadData()
            }
            
            if let frameRate = settings?.frameRate {
                frameRateButton?.setTitle(frameRate.description, for: .normal)
            }
        }
    }
    
    weak var delegate: SettingsVCDelegate?
    
    fileprivate let dimensionList: [CGSize] = CGSize.validDimensionList()
    fileprivate let frameRateList: [AgoraVideoFrameRate] = AgoraVideoFrameRate.validList()
    
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .lightContent
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        if let settings = settings {
            delegate?.settingsVC(self, didSelect: settings)
        }
    }
    
    override func viewDidLoad() {
        dimension = settings?.dimension
        updateCollectionViewLayout()
    }
    
    @IBAction func doFrameRatePressed(_ sender: UIButton) {
        frameRatePickerView.isHidden.toggle()
        frameRatePickerView.selectRow(lastFrameRateRow, inComponent: 0, animated: true)
    }
    
    func updateCollectionViewLayout() {
        let width = Double((UIScreen.main.bounds.width - (19 * 2) - (8 * 2)) / 3)
        let height = 40.0
        let layout = UICollectionViewFlowLayout()
        layout.minimumLineSpacing = 16
        layout.minimumInteritemSpacing = 8
        layout.itemSize = CGSize(width: width, height: height)
        dimensionCollectionView.setCollectionViewLayout(layout, animated: true)
    }
    
    override func tableView(_ tableView: UITableView, heightForHeaderInSection section: Int) -> CGFloat {
        return 16
    }
}

extension SettingsViewController: UICollectionViewDataSource {
    func collectionView(_ collectionView: UICollectionView, numberOfItemsInSection section: Int) -> Int {
        return dimensionList.count
    }
    
    func collectionView(_ collectionView: UICollectionView, cellForItemAt indexPath: IndexPath) -> UICollectionViewCell {
        let cell = collectionView.dequeueReusableCell(withReuseIdentifier: "DimensionCell", for: indexPath) as! DimensionCell
        let selectedDimension = dimensionList[indexPath.row]
        cell.update(with: selectedDimension, isSelected: (selectedDimension == dimension))
        return cell
    }
}

extension SettingsViewController: UICollectionViewDelegate {
    func collectionView(_ collectionView: UICollectionView, didSelectItemAt indexPath: IndexPath) {
        settings?.dimension = dimensionList[indexPath.row]
        dimension = dimensionList[indexPath.row]
    }
}

extension SettingsViewController: UIPickerViewDataSource {
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return frameRateList.count
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return frameRateList[row].description
    }
}

extension SettingsViewController: UIPickerViewDelegate {
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        settings?.frameRate = frameRateList[row]
        lastFrameRateRow = row
        frameRatePickerView.isHidden.toggle()
    }
}
