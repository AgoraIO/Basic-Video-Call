//
//  VideoSession.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 3/28/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa

class VideoSession: NSObject {
    var uid: UInt = 0
    var hostingView: VideoView!
    var canvas: AgoraRtcVideoCanvas!
    var size: CGSize?
    var mediaInfo = MediaInfo() {
        didSet {
            hostingView?.update(with: mediaInfo)
        }
    }
    var isVideoMuted = false {
        didSet {
            hostingView.isVideoMuted = isVideoMuted
        }
    }
    
    init(uid: UInt) {
        self.uid = uid
        
        hostingView = VideoView(frame: CGRect(x: 0, y: 0, width: 100, height: 100))
        canvas = AgoraRtcVideoCanvas()
        canvas.uid = uid
        canvas.view = hostingView.videoView
        canvas.renderMode = .fit
    }
}

extension VideoSession {
    static func localSession() -> VideoSession {
        return VideoSession(uid: 0)
    }
    
    func updateMediaInfo(resolution: CGSize? = nil, bitRate: Int? = nil, fps: Int? = nil) {
        if let resolution = resolution {
            mediaInfo.resolution = resolution
        }
        
        if let bitRate = bitRate {
            mediaInfo.bitRate = bitRate
        }
        
        if let fps = fps {
            mediaInfo.fps = fps
        }
        
        hostingView.update(with: mediaInfo)
    }
}
