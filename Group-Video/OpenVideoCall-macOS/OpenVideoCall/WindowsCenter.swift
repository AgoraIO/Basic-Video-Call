//
//  WindowsCenter.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa
import CoreGraphics

class Window {
    fileprivate(set) var id: CGWindowID = 0
    fileprivate(set) var name: String!
    fileprivate(set) var image: NSImage!
    
    init?(windowDic: NSDictionary) {
        let windowBounds = windowDic[Window.convert(CFString: kCGWindowBounds)] as! CFDictionary
        let bounds = CGRect(dictionaryRepresentation: windowBounds)
        guard bounds != nil && bounds!.width > 100 && bounds!.height > 100 else {
            return nil
        }
        
        let idNumber = windowDic[Window.convert(CFString: kCGWindowNumber)] as! CFNumber
        let id = Window.convert(CFNumber: idNumber)
        var name: String?
        if let ownerName = windowDic[Window.convert(CFString: kCGWindowOwnerName)] {
            let cfName = ownerName as! CFString
            name = Window.convert(CFString: cfName)
        }
        if let name = name , name == "Dock" {
            return nil
        }
        
        let image = Window.image(of: id)
        
        self.id = id
        self.name = name ?? "Unknown"
        self.image = image
    }
    
    fileprivate init() {}
    
    static func fullScreenWindow() -> Window {
        let window = Window()
        window.name = "Full Screen"
        window.image = imageOfFullScreen()
        return window
    }
    
    fileprivate static func image(of windowId: CGWindowID) -> NSImage {
        if let screenShot = CGWindowListCreateImage(CGRect.null, .optionIncludingWindow, CGWindowID(windowId), CGWindowImageOption()) {
            let bitmapRep = NSBitmapImageRep(cgImage: screenShot)
            let image = NSImage()
            image.addRepresentation(bitmapRep)
            return image
        } else {
            return NSImage()
        }
    }
    
    fileprivate static func imageOfFullScreen() -> NSImage {
        if let screenShot = CGWindowListCreateImage(CGRect.infinite, .optionOnScreenOnly, CGWindowID(0), CGWindowImageOption()) {
            let bitmapRep = NSBitmapImageRep(cgImage: screenShot)
            let image = NSImage()
            image.addRepresentation(bitmapRep)
            return image
        } else {
            return NSImage()
        }
    }
}

class WindowList {
    var items = [ImageBrowserItem]()
    fileprivate var list = [Window]() {
        didSet {
            var items = [ImageBrowserItem]()
            for window in list {
                items.append(ImageBrowserItem(window: window))
            }
            self.items = items
        }
    }
    
    func getList() {
        var list = [Window]()
        list.append(Window.fullScreenWindow())
        
        if let windowDicCFArray = CGWindowListCopyWindowInfo([.optionOnScreenOnly, .excludeDesktopElements], 0) {
            let windowDicList = windowDicCFArray as NSArray
            
            for windowElement in windowDicList {
                let windowDic = windowElement
                if let windowDic = windowDic as? NSDictionary {
                    if let window = Window(windowDic: windowDic) {
                        list.append(window)
                    }
                }
                
            }
        }
        
        self.list = list
    }
}

private extension Window {
    class func convert(CFString cfString: CFString) -> String {
        let string = cfString as NSString
        return string as String
        
    }
    
    class func convert(CFNumber cfNumber: CFNumber) -> CGWindowID {
        let number = cfNumber as NSNumber
        return number.uint32Value
    }
}
