//
//  CommonExtensions.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

extension NSColor {
    convenience init(hex: Int, alpha: CGFloat = 1) {
        func transform(_ input: Int, offset: Int = 0) -> CGFloat {
            let value = (input >> offset) & 0xff
            return CGFloat(value) / 255
        }
        
        self.init(red: transform(hex, offset: 16),
            green: transform(hex, offset: 8),
            blue: transform(hex),
            alpha: alpha)
    }
    
    static func randomColor() -> NSColor {
        let randomHex = Int(arc4random_uniform(0xCCCCCC) + 0x555555)
        return NSColor(hex: randomHex)
    }
}

extension CGSize {
    func fixedSize() -> CGSize {
        let width = self.width
        let height = self.height
        if width < height {
            return CGSize(width: height, height: width)
        } else {
            return self
        }
    }
}
