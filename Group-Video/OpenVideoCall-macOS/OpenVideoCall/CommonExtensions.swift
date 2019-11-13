//
//  CommonExtensions.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

extension NSColor {
    static var AGTextGray: NSColor {
        return NSColor(red: 153.0 / 255.0, green: 153.0 / 255.0, blue: 153.0 / 255.0, alpha: 1)
    }
    
    static var AGTextBlack: NSColor {
        return NSColor(red: 51.0 / 255.0, green: 51.0 / 255.0, blue: 51.0 / 255.0, alpha: 1)
    }
    
    static var AGDarkGray: NSColor {
        return NSColor(red: 102.0 / 255.0, green: 102.0 / 255.0, blue: 102.0 / 255.0, alpha: 1)
    }
    
    static var AGGray: NSColor {
        return NSColor(red: 204.0 / 255.0, green: 204.0 / 255.0, blue: 204.0 / 255.0, alpha: 1)
    }
    
    static var AGLineGray: NSColor {
        return NSColor(red: 234.0 / 255.0, green: 234.0 / 255.0, blue: 234.0 / 255.0, alpha: 1)
    }
    
    static var AGBlue: NSColor {
        return NSColor(red: 0, green: 106.0 / 255.0, blue: 216.0 / 255.0, alpha: 1)
    }
    
    static var AGDarkBlue: NSColor {
        return NSColor(red: 0, green: 106.0 / 255.0, blue: 236.0 / 255.0, alpha: 1)
    }
    
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
