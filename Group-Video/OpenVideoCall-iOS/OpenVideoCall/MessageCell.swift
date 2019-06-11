//
//  ChatMessageCell.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/15.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit

class MessageCell: UITableViewCell {
    
    @IBOutlet weak var colorView: UIView!
    @IBOutlet weak var messageLabel: UILabel!
    
    func set(with message: Message) {
        colorView.backgroundColor = message.type.color()
        messageLabel.text = message.text
    }
}
