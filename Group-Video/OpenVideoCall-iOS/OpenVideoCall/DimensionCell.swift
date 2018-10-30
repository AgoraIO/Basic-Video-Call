//
//  ProfileCell.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/26/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

class DimensionCell: UITableViewCell {
    
    @IBOutlet weak var resLabel: UILabel!
    
    func update(with dimension: CGSize, isSelected: Bool) {
        resLabel.text = "\(Int(dimension.width))x\(Int(dimension.height))"
        
        backgroundColor = isSelected ? UIColor(red: 0, green: 0, blue: 0.5, alpha: 0.3) : UIColor.white
    }
}
