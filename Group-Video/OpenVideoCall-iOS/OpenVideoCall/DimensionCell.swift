//
//  ProfileCell.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/26/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

class DimensionCell: UICollectionViewCell {
    
    @IBOutlet weak var resLabel: UILabel!
    
    func update(with dimension: CGSize, isSelected: Bool) {
        resLabel.text = "\(Int(dimension.width))x\(Int(dimension.height))"
        
        resLabel.textColor = isSelected ? UIColor.white : UIColor.AGTextGray
        resLabel.backgroundColor = isSelected ? UIColor.AGBlue : UIColor.white
        resLabel.layer.borderColor = isSelected ? UIColor.AGBlue.cgColor : UIColor.AGGray.cgColor
    }
}
