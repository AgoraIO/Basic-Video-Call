//
//  ReplacementSegue.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

class ReplacementSegue: NSStoryboardSegue {
    override func perform() {
        (sourceController as AnyObject).view.window?.contentViewController = destinationController as? NSViewController
    }
}
