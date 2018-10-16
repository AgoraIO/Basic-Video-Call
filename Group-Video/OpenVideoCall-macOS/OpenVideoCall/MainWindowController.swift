//
//  MainWindowController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

class MainWindowController: NSWindowController {

    override func windowDidLoad() {
        super.windowDidLoad()
        
        window?.styleMask.insert(.fullSizeContentView)
        window?.titlebarAppearsTransparent = true
        window?.isMovableByWindowBackground = true
    }
}
