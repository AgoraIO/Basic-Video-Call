//
//  AGEButton.swift
//  AGEButton
//
//  Created by CavanSu on 2019/11/8.
//  Copyright © 2019 Agora. All rights reserved.
//

import Cocoa

typealias AGETextAlignment = NSTextAlignment

class AGEButton: NSButton {
    enum AGETrackingAreaType {
        case `default`, custom(NSTrackingArea)
    }
    
    required init(coder: NSCoder) {
        super.init(coder: coder)!
        self.wantsLayer = true
    }
    
    func setTitle(_ title: String, with color: NSColor, alignment: AGETextAlignment = .center) {
        let paragraph = NSMutableParagraphStyle()
        paragraph.alignment = alignment
        let attributes = [NSAttributedString.Key.paragraphStyle: paragraph,
                          NSAttributedString.Key.foregroundColor: color]
        
        self.attributedTitle = NSAttributedString(string: title,
                                                  attributes: attributes)
    }
    
    func addTrackingArea(_ type: AGETrackingAreaType) {
        switch type {
        case .default:
            let trackingArea = NSTrackingArea(rect: self.bounds,
                                              options: [.mouseEnteredAndExited, .activeAlways],
                                              owner: self,
                                              userInfo: nil)
            self.addTrackingArea(trackingArea)
        case .custom(let area):
            self.addTrackingArea(area)
        }
    }
}
