//
//  AGEView.swift
//  AGE
//
//  Created by CavanSu on 2019/9/24.
//  Copyright © 2019 CavanSu. All rights reserved.
//

#if os(iOS)
import UIKit
#else
import Cocoa
#endif

#if os(iOS)
typealias AGEView = UIView
typealias AGEColor = UIColor
typealias AGESize = CGSize
#else
typealias AGEView = NSView
typealias AGEColor = NSColor
typealias AGESize = NSSize
#endif

extension AGEView {
    #if os(macOS)
    func layoutIfNeeded() {
        self.layoutSubtreeIfNeeded()
    }
    
    var backgroundColor: AGEColor {
        set {
            if self.layer == nil {
                self.wantsLayer = true
            }
            
            self.layer?.backgroundColor = newValue.cgColor
        }
        
        get {
            var color: AGEColor?
            if let layerColor = self.layer?.backgroundColor {
                color = AGEColor(cgColor: layerColor)
            }
            
            if let value = color {
                return value
            } else {
                return AGEColor.clear
            }
        }
    }
    #endif
}

#if os(iOS)
typealias AGEScrollViewDelegate = UIScrollViewDelegate

class AGEScrollView: UIScrollView {
    override init(frame frameRect: CGRect) {
        super.init(frame: frameRect)
        doInitSettings()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        doInitSettings()
    }
    
    init() {
        super.init(frame: CGRect(x: 0, y: 0, width: 0, height: 0))
        doInitSettings()
    }
}

#else
protocol AGEScrollViewDelegate: NSObjectProtocol {
    func scrollViewDidScroll(_ scrollView: AGEScrollView)
}

class AGEScrollView: NSScrollView {
    
    fileprivate var contentViewBoundsObserver: Any?
    weak var delegate: AGEScrollViewDelegate?
    
    var contentOffset: CGPoint {
        set {
            documentView?.scroll(newValue)
        }
        get {
            return documentVisibleRect.origin
        }
    }
    
    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        doInitSettings()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        doInitSettings()
    }
    
    init() {
        super.init(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
        doInitSettings()
    }
    
    deinit {
        if let contentViewBoundsObserver = contentViewBoundsObserver {
            NotificationCenter.default.removeObserver(contentViewBoundsObserver)
            self.contentViewBoundsObserver = nil
        }
    }
    
    func scrollToTop() {
        guard let documentView = self.documentView else {
            return
        }
        
        if documentView.isFlipped {
            documentView.scroll(.zero)
        } else {
            let maxHeight = max(bounds.height, documentView.bounds.height)
            documentView.scroll(NSPoint(x: 0, y: maxHeight))
        }
    }
}
#endif

extension AGEScrollView {
    func removeAllItemViews() {
        #if os(iOS)
        for subView in self.subviews {
            subView.removeFromSuperview()
        }
        #else
        guard let view = self.documentView else {
            return
        }
        
        for subView in view.subviews {
            subView.removeFromSuperview()
        }
        #endif
    }
}

private extension AGEScrollView {
    func doInitSettings() {
        translatesAutoresizingMaskIntoConstraints = false
        #if os(iOS)
        alwaysBounceHorizontal = false
        showsHorizontalScrollIndicator = false
        alwaysBounceVertical = false
        showsVerticalScrollIndicator = false
        #else
        self.wantsLayer = true
        self.layer?.backgroundColor = NSColor.clear.cgColor
        self.drawsBackground = false
        
        let view = NSView(frame: NSRect(x: 0, y: 0, width: 0, height: 0))
        self.documentView = view
        verticalScrollElasticity = .automatic
        horizontalScrollElasticity = .automatic
        
        contentView.postsBoundsChangedNotifications = true
        guard contentViewBoundsObserver == nil else {
            return
        }
        let center = NotificationCenter.default
        contentViewBoundsObserver = center.addObserver(forName: NSView.boundsDidChangeNotification,
                                                       object: contentView,
                                                       queue: nil) { [weak self] (notify) in
                                                        if let strongSelf = self {
                                                            strongSelf.delegate?.scrollViewDidScroll(strongSelf)
                                                        }
        }
        #endif
    }
}

