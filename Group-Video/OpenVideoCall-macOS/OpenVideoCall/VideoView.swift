//
//  VideoView.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

class VideoView: NSView {
    
    fileprivate(set) var videoView: NSView!
    fileprivate var screenShareImageView: NSView?
    
    fileprivate var infoView: NSView!
    fileprivate var infoLabel: NSTextField!
    
    var isVideoMuted = false {
        didSet {
            videoView?.isHidden = isVideoMuted || isScreenSharing
        }
    }
    fileprivate var isScreenSharing = false {
        didSet {
            removeScreenShareImageView()
            
            if isScreenSharing {
                addScreenShareImageView()
            }
            
            videoView.isHidden = isVideoMuted || isScreenSharing
        }
    }
    
    override init(frame frameRect: CGRect) {
        super.init(frame: frameRect)
        translatesAutoresizingMaskIntoConstraints = false
        wantsLayer = true
        layer?.backgroundColor = NSColor.white.cgColor
        
        addVideoView()
        addInfoView()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
}

extension VideoView {
    func switchToScreenShare(_ isScreenShare: Bool) {
        isScreenSharing = isScreenShare
    }
}

extension VideoView {
    func update(with info: MediaInfo) {
        infoLabel?.stringValue = info.description()
    }
}

private extension VideoView {
    func addVideoView() {
        videoView = NSView()
        videoView.translatesAutoresizingMaskIntoConstraints = false
        
        addSubview(videoView)
        
        let inset = VideoViewLayout.ViewEdgeInset
        let videoViewH = NSLayoutConstraint.constraints(withVisualFormat: "H:|-(\(inset))-[video]-(\(inset))-|", options: [], metrics: nil, views: ["video": videoView])
        let videoViewV = NSLayoutConstraint.constraints(withVisualFormat: "V:|-(\(inset))-[video]-(\(inset))-|", options: [], metrics: nil, views: ["video": videoView])
        NSLayoutConstraint.activate(videoViewH + videoViewV)
    }
    
    func addInfoView() {
        infoView = NSView()
        infoView.translatesAutoresizingMaskIntoConstraints = false
        
        addSubview(infoView)
        let infoViewH = NSLayoutConstraint.constraints(withVisualFormat: "H:|[info]|", options: [], metrics: nil, views: ["info": infoView])
        let infoViewV = NSLayoutConstraint.constraints(withVisualFormat: "V:|[info(==135)]", options: [], metrics: nil, views: ["info": infoView])
        infoView.lowerContentCompressionResistancePriority()
        NSLayoutConstraint.activate(infoViewH + infoViewV)
        
        func createInfoLabel() -> NSTextField {
            let label = NSTextField()
            label.translatesAutoresizingMaskIntoConstraints = false
            
            label.stringValue = " "
            let shadow = NSShadow()
            shadow.shadowOffset = NSSize(width: 0, height: 1)
            shadow.shadowColor = NSColor.black
            label.shadow = shadow
            label.maximumNumberOfLines = 0
            
            label.isEditable = false
            label.isBezeled = false
            label.drawsBackground = false
            
            label.font = NSFont.systemFont(ofSize: 12)
            label.textColor = NSColor.white
            
            return label
        }
        
        infoLabel = createInfoLabel()
        infoView.addSubview(infoLabel)
        
        let top: CGFloat = 16
        let left: CGFloat = 8
        
        let labelV = NSLayoutConstraint.constraints(withVisualFormat: "V:|-(\(top))-[info]", options: [], metrics: nil, views: ["info": infoLabel])
        let labelH = NSLayoutConstraint.constraints(withVisualFormat: "H:|-(\(left))-[info]", options: [], metrics: nil, views: ["info": infoLabel])
        infoLabel.lowerContentCompressionResistancePriority()
        NSLayoutConstraint.activate(labelV)
        NSLayoutConstraint.activate(labelH)
    }
    
    //MARK: - screen share
    func addScreenShareImageView() {
        let imageView = NSImageView(frame: CGRect(x: 0, y: 0, width: 144, height: 144))
        imageView.image = NSImage(named: "icon_sharing_desktop")
        imageView.translatesAutoresizingMaskIntoConstraints = false
        addSubview(imageView)
        
        let avatarH = NSLayoutConstraint(item: imageView, attribute: .centerX, relatedBy: .equal, toItem: self, attribute: .centerX, multiplier: 1, constant: 0)
        let avatarV = NSLayoutConstraint(item: imageView, attribute: .centerY, relatedBy: .equal, toItem: self, attribute: .centerY, multiplier: 1, constant: 0)
        let avatarRatio = NSLayoutConstraint(item: imageView, attribute: .width, relatedBy: .equal, toItem: imageView, attribute: .height, multiplier: 1, constant: 0)
        let avatarLeft = NSLayoutConstraint(item: imageView, attribute: .left, relatedBy: .greaterThanOrEqual, toItem: self, attribute: .left, multiplier: 1, constant: 10)
        let avatarTop = NSLayoutConstraint(item: imageView, attribute: .top, relatedBy: .greaterThanOrEqual, toItem: self, attribute: .top, multiplier: 1, constant: 10)
        imageView.lowerContentCompressionResistancePriority()
        NSLayoutConstraint.activate([avatarH, avatarV, avatarRatio, avatarLeft, avatarTop])
        
        screenShareImageView = imageView
    }
    
    func removeScreenShareImageView() {
        if let imageView = screenShareImageView {
            imageView.removeFromSuperview()
            self.screenShareImageView = nil
        }
    }
}

private extension NSView {
    func lowerContentCompressionResistancePriority() {
        setContentCompressionResistancePriority(NSLayoutConstraint.Priority(rawValue: 200), for: .horizontal)
        setContentCompressionResistancePriority(NSLayoutConstraint.Priority(rawValue: 200), for: .vertical)
    }
}
