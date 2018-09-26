//
//  VideoViewLayout.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 3/24/16.
//  Copyright © 2016 Agora. All rights reserved.
//


import Cocoa
import WebKit

class VideoViewLayout {
    
    static let ViewEdgeInset: CGFloat = 1
    
    fileprivate var MaxPeerCount = 4
    fileprivate var layoutConstraints = [NSLayoutConstraint]()
    
    var selfView: VideoView?
    var selfSize: CGSize?
    var targetSize = CGSize.zero
    
    var videoViews = [VideoView]()
    var fullView: VideoView?
    var containerView: NSView?
    fileprivate var scrollView = NSScrollView()
    
    fileprivate let Multiplier: CGFloat = 0.99999
    fileprivate let ViewInset = VideoViewLayout.ViewEdgeInset
    
    fileprivate var allViews: [VideoView] {
        get {
            var allViews = [VideoView]()
            allViews.append(contentsOf: videoViews)
            if let selfView = selfView {
                allViews.append(selfView)
            }
            return allViews
        }
    }
    
    init() {
        scrollView.translatesAutoresizingMaskIntoConstraints = false
        scrollView.hasHorizontalScroller = false
        scrollView.hasVerticalScroller = false
        scrollView.drawsBackground = false
    }
    
    func layoutVideoViews() {
        guard let selfView = selfView, let containerView = containerView else {
            return
        }
        
        selfView.removeFromSuperview()
        for view in videoViews {
            view.removeFromSuperview()
        }
        scrollView.removeFromSuperview()
        
        NSLayoutConstraint.deactivate(layoutConstraints)
        layoutConstraints.removeAll()
        
        switch videoViews.count {
        case 0:
            let fullViewLayouts = layoutFullSessionView(selfView, inContainerView: containerView)
            layoutConstraints.append(contentsOf: fullViewLayouts)
            
        case 1:
            let peerView = videoViews.first!
            let fullViewLayouts = layoutFullSessionView(peerView, inContainerView: containerView)
            layoutConstraints.append(contentsOf: fullViewLayouts)
            
            let cornerViewLayouts = layoutCornerSessionView(selfView, inContainerView: containerView)
            layoutConstraints.append(contentsOf: cornerViewLayouts)
            
        default:
            if let fullView = fullView {
                let fullViewLayouts = layoutFullSessionView(fullView, inContainerView: containerView)
                layoutConstraints.append(contentsOf: fullViewLayouts)
                
                var smallViews = [VideoView]()
                let smallCount = min(videoViews.count, MaxPeerCount)
                var index = 0
                repeat {
                    let view: VideoView
                    if index >= videoViews.count {
                        view = selfView
                    } else {
                        view = videoViews[index]
                    }
                    if view != fullView {
                        smallViews.append(view)
                    }
                    index += 1
                } while smallViews.count < smallCount
                
                let smallViewLayouts = layoutSmallSessionViews(smallViews, inContainerView: containerView)
                layoutConstraints.append(contentsOf: smallViewLayouts)
                
            } else {
                let layouts = layoutEqualSessionViews(allViews, inContainerView: containerView)
                layoutConstraints.append(contentsOf: layouts)
            }
        }
        
        NSLayoutConstraint.activate(layoutConstraints)
    }
    
    func reponseViewIndex(of location: CGPoint) -> Int? {
        guard let selfView = selfView, let containerView = containerView , fullView == nil else {
            return nil
        }
        
        var allViews = [VideoView]()
        allViews.append(selfView)
        allViews.append(contentsOf: videoViews)
        
        for (index, view) in allViews.enumerated() {
            if let superview = view.superview , superview == containerView && view.frame.contains(location) {
                return index
            }
        }
        
        return nil
    }
}

//MARK: - layouts
private extension VideoViewLayout {
    func layoutFullSessionView(_ view: NSView, inContainerView containerView: NSView) -> [NSLayoutConstraint] {
        containerView.addSubview(view)
        var layouts = [NSLayoutConstraint]()
        
        let constraintsH = NSLayoutConstraint.constraints(withVisualFormat: "H:|-(\(-ViewInset))-[view]-(\(-ViewInset))-|", options: [], metrics: nil, views: ["view": view])
        let constraintsV = NSLayoutConstraint.constraints(withVisualFormat: "V:|-(\(-ViewInset))-[view]-(\(-ViewInset))-|", options: [], metrics: nil, views: ["view": view])
        layouts.append(contentsOf: constraintsH)
        layouts.append(contentsOf: constraintsV)
        
        return layouts
    }
    
    func layoutCornerSessionView(_ view: NSView, inContainerView containerView: NSView) -> [NSLayoutConstraint] {
        containerView.addSubview(view)
        var layouts = [NSLayoutConstraint]()
        
        containerView.addSubview(view)
        let right = NSLayoutConstraint(item: view, attribute: .right, relatedBy: .equal, toItem: containerView, attribute: .right, multiplier: Multiplier, constant: -5)
        let top = NSLayoutConstraint(item: view, attribute: .top, relatedBy: .equal, toItem: containerView, attribute: .top, multiplier: Multiplier, constant: 64)
        let width = NSLayoutConstraint(item: view, attribute: .width, relatedBy: .equal, toItem: containerView, attribute: .width, multiplier: 0.249999, constant: 0)
        
        let ratioValue: CGFloat
        if let selfSize = selfSize , selfSize.width > 0 && selfSize.height > 0 {
            ratioValue = selfSize.width / selfSize.height
        } else if targetSize.width > 0 && targetSize.height > 0 {
            ratioValue = targetSize.width / targetSize.height
        } else {
            ratioValue = containerView.bounds.width / containerView.bounds.height
        }
        let ratio = NSLayoutConstraint(item: view, attribute: .width, relatedBy: .equal, toItem: selfView, attribute: .height, multiplier: ratioValue, constant: 0)
        layouts.append(contentsOf: [right, top, width, ratio])
        
        return layouts
    }
    
    func layoutSmallSessionViews(_ smallViews: [NSView], inContainerView containerView: NSView) -> [NSLayoutConstraint] {
        let ratio: CGFloat
        if targetSize.width > 0 && targetSize.height > 0 {
            ratio = targetSize.width / targetSize.height
        } else {
            ratio = containerView.bounds.width / containerView.bounds.height
        }

        scrollView.verticalScrollElasticity = .none
        scrollView.verticalScrollElasticity = .automatic
        
        var layouts = [NSLayoutConstraint]()
        var lastView: NSView?
        
        let scrollContainerView = NSView()
        scrollContainerView.translatesAutoresizingMaskIntoConstraints = false
        scrollView.documentView = scrollContainerView
        
        let scrollContainerH: [NSLayoutConstraint]
        let scrollContainerV: [NSLayoutConstraint]
        
        scrollContainerH = NSLayoutConstraint.constraints(withVisualFormat: "H:|[view]", options: [], metrics: nil, views: ["view": scrollContainerView])
        scrollContainerV = NSLayoutConstraint.constraints(withVisualFormat: "V:|[view]|", options: [], metrics: nil, views: ["view": scrollContainerView])
        
        layouts.append(contentsOf: scrollContainerH)
        layouts.append(contentsOf: scrollContainerV)
        
        let itemSpace: CGFloat = 12
        
        for view in smallViews {
            if view == fullView {
                continue
            }
            
            scrollContainerView.addSubview(view)
            let viewWidth: NSLayoutConstraint
            
            viewWidth = NSLayoutConstraint(item: view, attribute: .width, relatedBy: .equal, toItem: containerView, attribute: .width, multiplier: 0.249999, constant: 0)
            
            let viewRatio = NSLayoutConstraint(item: view, attribute: .width, relatedBy: .equal, toItem: view, attribute: .height, multiplier: ratio, constant: 0)
            let viewTop: NSLayoutConstraint
            let viewLeft: NSLayoutConstraint
            
            viewTop = NSLayoutConstraint(item: view, attribute: .top, relatedBy: .equal, toItem: scrollContainerView, attribute: .top, multiplier: 1, constant: 0)
            if let lastView = lastView {
                viewLeft = NSLayoutConstraint(item: view, attribute: .left, relatedBy: .equal, toItem: lastView, attribute: .right, multiplier: 1, constant: itemSpace)
            } else {
                viewLeft = NSLayoutConstraint(item: view, attribute: .left, relatedBy: .equal, toItem: scrollContainerView, attribute: .left, multiplier: 1, constant: itemSpace)
            }
            
            layouts.append(contentsOf: [viewWidth, viewRatio, viewLeft, viewTop])
            lastView = view
        }
        
        let lastViewRight = NSLayoutConstraint(item: lastView!, attribute: .right, relatedBy: .equal, toItem: scrollContainerView, attribute: .right, multiplier: 1, constant: 0)
        let lastViewBottom = NSLayoutConstraint(item: lastView!, attribute: .bottom, relatedBy: .equal, toItem: scrollContainerView, attribute: .bottom, multiplier: 1, constant: 0)
        layouts.append(contentsOf: [lastViewRight, lastViewBottom])
        
        containerView.addSubview(scrollView)
        
        let scrollConstraintsH = NSLayoutConstraint.constraints(withVisualFormat: "H:|[view]|", options: [], metrics: nil, views: ["view": scrollView])
        let scrollHeight = NSLayoutConstraint(item: scrollView, attribute: .height, relatedBy: .equal, toItem: lastView!, attribute: .height, multiplier: 1, constant: 0)
        let scrollTop = NSLayoutConstraint(item: scrollView, attribute: .top, relatedBy: .equal, toItem: containerView, attribute: .top, multiplier: 1, constant: 64)
        layouts.append(contentsOf: scrollConstraintsH)
        layouts.append(contentsOf: [scrollHeight, scrollTop])
        
        return layouts
    }
    
    func layoutEqualSessionViews(_ allViews: [NSView], inContainerView containerView: NSView) -> [NSLayoutConstraint] {
        
        var layouts = [NSLayoutConstraint]()
        let rowsPerScreen = CollectionIndexModel.rowsPerScreen(with: allViews.count)
        
        for (index, view) in allViews.enumerated() {
            if index > MaxPeerCount {
                break
            }
            containerView.addSubview(view)
            
            let viewTop: NSLayoutConstraint
            if let topIndex = CollectionIndexModel.topIndex(of: index, rowsPerScreen: rowsPerScreen) {
                viewTop = NSLayoutConstraint(item: view, attribute: .top, relatedBy: .equal, toItem: allViews[topIndex], attribute: .bottom, multiplier: Multiplier, constant: 0)
            } else {
                viewTop = NSLayoutConstraint(item: view, attribute: .top, relatedBy: .equal, toItem: containerView, attribute: .top, multiplier: Multiplier, constant: -ViewInset)
            }
            
            let viewLeft: NSLayoutConstraint
            if let leftIndex = CollectionIndexModel.leftIndex(of: index, rowsPerScreen: rowsPerScreen) {
                viewLeft = NSLayoutConstraint(item: view, attribute: .left, relatedBy: .equal, toItem: allViews[leftIndex], attribute: .right, multiplier: Multiplier, constant: 0)
            } else {
                viewLeft = NSLayoutConstraint(item: view, attribute: .left, relatedBy: .equal, toItem: containerView, attribute: .left, multiplier: Multiplier, constant: -ViewInset)
            }
            
            layouts.append(contentsOf: [viewLeft, viewTop])
            
            if index > 0 {
                let viewWidth = NSLayoutConstraint(item: view, attribute: .width, relatedBy: .equal, toItem: allViews[0], attribute: .width, multiplier: Multiplier, constant: 0)
                let viewHeight = NSLayoutConstraint(item: view, attribute: .height, relatedBy: .equal, toItem: allViews[0], attribute: .height, multiplier: Multiplier, constant: 0)
                layouts.append(contentsOf: [viewWidth, viewHeight])
            }
        }
        
        let multiplier = 1 / CGFloat(rowsPerScreen)
        let rightViewIndex = rowsPerScreen - 1
        if allViews.count > rightViewIndex {
            let viewRight = NSLayoutConstraint(item: allViews[rightViewIndex], attribute: .right, relatedBy: .equal, toItem: containerView, attribute: .right, multiplier: Multiplier, constant: ViewInset)
            layouts.append(viewRight)
        } else {
            let viewWidth = NSLayoutConstraint(item: allViews[0], attribute: .width, relatedBy: .equal, toItem: containerView, attribute: .width, multiplier: multiplier, constant: 0)
            layouts.append(viewWidth)
        }
        
        let bottomViewIndex = rowsPerScreen * (rowsPerScreen - 1)
        if allViews.count > bottomViewIndex {
            let viewBottom = NSLayoutConstraint(item: allViews[bottomViewIndex], attribute: .bottom, relatedBy: .equal, toItem: containerView, attribute: .bottom, multiplier: Multiplier, constant: ViewInset)
            layouts.append(viewBottom)
        } else {
            let viewHeight = NSLayoutConstraint(item: allViews[0], attribute: .height, relatedBy: .equal, toItem: containerView, attribute: .height, multiplier: multiplier, constant: 0)
            layouts.append(viewHeight)
        }
        
        return layouts
    }
}
