//
//  VideoView.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/14/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import UIKit

class VideoView: UIView {
    
    fileprivate(set) var videoView: UIView!
    fileprivate var screenShareImageView: UIView?
    
    fileprivate var infoView: UIView!
    fileprivate var infoLabel: UILabel!
    
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
        backgroundColor = UIColor.white
        
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
        infoLabel?.text = info.description()
    }
}

private extension VideoView {
    func addVideoView() {
        videoView = UIView()
        videoView.translatesAutoresizingMaskIntoConstraints = false
        videoView.backgroundColor = UIColor.clear
        addSubview(videoView)
        
        let videoViewH = NSLayoutConstraint.constraints(withVisualFormat: "H:|[video]|", options: [], metrics: nil, views: ["video": videoView])
        let videoViewV = NSLayoutConstraint.constraints(withVisualFormat: "V:|[video]|", options: [], metrics: nil, views: ["video": videoView])
        NSLayoutConstraint.activate(videoViewH + videoViewV)
    }
    
    func addInfoView() {
        infoView = UIView()
        infoView.translatesAutoresizingMaskIntoConstraints = false
        infoView.backgroundColor = UIColor.clear
        
        addSubview(infoView)
        let infoViewH = NSLayoutConstraint.constraints(withVisualFormat: "H:|[info]|", options: [], metrics: nil, views: ["info": infoView])
        let infoViewV = NSLayoutConstraint.constraints(withVisualFormat: "V:|[info(==135)]", options: [], metrics: nil, views: ["info": infoView])
        NSLayoutConstraint.activate(infoViewH + infoViewV)
        
        func createInfoLabel() -> UILabel {
            let label = UILabel()
            label.translatesAutoresizingMaskIntoConstraints = false
            
            label.text = " "
            label.shadowOffset = CGSize(width: 0, height: 1)
            label.shadowColor = UIColor.black
            label.numberOfLines = 0
            
            label.font = UIFont.systemFont(ofSize: 12)
            label.textColor = UIColor.white
            
            return label
        }
        
        infoLabel = createInfoLabel()
        infoView.addSubview(infoLabel)
        
        let top: CGFloat = 20
        let left: CGFloat = 10
        
        let labelV = NSLayoutConstraint.constraints(withVisualFormat: "V:|-(\(top))-[info]", options: [], metrics: nil, views: ["info": infoLabel])
        let labelH = NSLayoutConstraint.constraints(withVisualFormat: "H:|-(\(left))-[info]", options: [], metrics: nil, views: ["info": infoLabel])
        NSLayoutConstraint.activate(labelV)
        NSLayoutConstraint.activate(labelH)
    }
    
    //MARK: - screen share
    func addScreenShareImageView() {
        let imageView = UIImageView(frame: CGRect(x: 0, y: 0, width: 144, height: 144))
        imageView.image = UIImage(named: "icon_sharing_desktop")
        imageView.translatesAutoresizingMaskIntoConstraints = false
        addSubview(imageView)
        
        let avatarH = NSLayoutConstraint(item: imageView, attribute: .centerX, relatedBy: .equal, toItem: self, attribute: .centerX, multiplier: 1, constant: 0)
        let avatarV = NSLayoutConstraint(item: imageView, attribute: .centerY, relatedBy: .equal, toItem: self, attribute: .centerY, multiplier: 1, constant: 0)
        let avatarRatio = NSLayoutConstraint(item: imageView, attribute: .width, relatedBy: .equal, toItem: imageView, attribute: .height, multiplier: 1, constant: 0)
        let avatarLeft = NSLayoutConstraint(item: imageView, attribute: .left, relatedBy: .greaterThanOrEqual, toItem: self, attribute: .left, multiplier: 1, constant: 10)
        let avatarTop = NSLayoutConstraint(item: imageView, attribute: .top, relatedBy: .greaterThanOrEqual, toItem: self, attribute: .top, multiplier: 1, constant: 10)
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
