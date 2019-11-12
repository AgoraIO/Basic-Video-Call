//
//  WindowItem.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/11/11.
//  Copyright © 2019 Agora. All rights reserved.
//

import Cocoa

class WindowItem: NSCollectionViewItem {
    
    @IBOutlet weak var cImageView: NSImageView!
    
    var image: NSImage? {
        didSet {
            cImageView.image = image
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.wantsLayer = true
        self.view.layer?.backgroundColor = NSColor.black.cgColor
    }
}
