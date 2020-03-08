//
//  AGEEventsObserver.swift
//  AGE
//
//  Created by CavanSu on 2019/11/13.
//  Copyright © 2019 CavanSu. All rights reserved.
//

#if os(macOS)
import Cocoa
#else
import UIKit
#endif

protocol AGEEventsObserverDelegate: NSObjectProtocol {
    #if os(macOS)
    func observer(_ observer: AGEEventsObserver, didTriggerInputEvent event: NSEvent)
    #endif
}

protocol AGEEventsObserverDataSource: NSObjectProtocol {
    #if os(macOS)
    func observerNeedWindowNumber(_ observer: AGEEventsObserver) -> Int
    #endif
}

class AGEEventsObserver: NSObject {
    weak var delegate: AGEEventsObserverDelegate?
    weak var dataSource: AGEEventsObserverDataSource?
    
    #if os(macOS)
    deinit {
        NSEvent.removeMonitor(self)
    }
    #endif
}

#if os(macOS)
extension AGEEventsObserver {
    func listenCurrentWindow(inputEvents: NSEvent.EventTypeMask) {
        NSEvent.addLocalMonitorForEvents(matching: inputEvents) { [weak self] (event) -> NSEvent? in
            guard let strongSelf = self,
                let currentWindowNumber = strongSelf.dataSource?.observerNeedWindowNumber(strongSelf),
                event.windowNumber == currentWindowNumber else {
                    return event
            }
            
            strongSelf.delegate?.observer(strongSelf, didTriggerInputEvent: event)
            return event
        }
    }
}
#endif
