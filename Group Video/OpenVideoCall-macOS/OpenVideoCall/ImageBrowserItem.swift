//
//  ImageBrowserItem.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 6/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa
import Quartz.ImageKit

class ImageBrowserItem: NSObject {
    var window: Window!
    
    fileprivate override init() {
        super.init()
    }
    
    init(window: Window) {
        super.init()
        self.window = window
    }
    
    override func imageUID() -> String! {
        return "\(window.id)"
    }
    
    override func imageTitle() -> String! {
        return window.name
    }
    
    override func imageRepresentation() -> Any! {
        return window.image
    }
    
    override func imageRepresentationType() -> String! {
        return IKImageBrowserNSImageRepresentationType
    }
}
