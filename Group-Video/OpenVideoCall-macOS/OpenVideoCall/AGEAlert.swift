//
//  AGEAlert.swift
//  AGEAlert
//
//  Created by CavanSu on 2019/11/8.
//  Copyright © 2019 Agora. All rights reserved.
//

import Cocoa

typealias AGEAlertStyle = NSAlert.Style
typealias AfterPressed = (() -> Void)?

class AGEAlert: NSObject {
    struct AGEAction {
        var index: Int
        var completion: AfterPressed
    }
    
    private var alert: NSAlert
    private var actionCount: Int = 0
    private var actionCountMax: Int = 3
    private lazy var actionsDic = [String: AGEAction]() // Key: action title
    
    init(title: String, style: AGEAlertStyle) {
        alert = NSAlert()
        alert.messageText = title
        alert.alertStyle = style
    }
    
    func addAction(title: String, afterPressed: AfterPressed = nil) {
        guard actionCount <= actionCountMax else {
            return
        }
        
        if let _ = actionsDic.keys.firstIndex(of: title) {
            return
        }
        
        defer {
            actionCount += 1;
        }
        
        alert.addButton(withTitle: title)
        alert.buttons[actionCount].setAccessibilityIdentifier(title)
        
        actionsDic[title] = AGEAction(index: actionCount,
                                      completion: afterPressed)
    }
    
    func present() {
        let response = alert.runModal()
        
        func callBack(index: Int) {
            let item = actionsDic.first { (key, value) -> Bool in
                return value.index == index
            }
            if let completion = item?.value.completion {
                completion()
            }
        }
        
        switch response {
        case NSApplication.ModalResponse.alertFirstButtonReturn:
            callBack(index: 0)
        case NSApplication.ModalResponse.alertSecondButtonReturn:
            callBack(index: 1)
        case NSApplication.ModalResponse.alertThirdButtonReturn:
            callBack(index: 2)
        default:
            break
        }
    }
}
