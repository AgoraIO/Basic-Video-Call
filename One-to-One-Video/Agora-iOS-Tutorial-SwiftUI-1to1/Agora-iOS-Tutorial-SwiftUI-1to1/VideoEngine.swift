//
//  VideoEngine.swift
//  Agora-iOS-Tutorial-SwiftUI-1to1
//
//  Created by GongYuhua on 2019/11/21.
//  Copyright © 2019 Agora. All rights reserved.
//

import AgoraRtcKit

class VideoEngine: NSObject {
    // init AgoraRtcEngineKit
    lazy var agoraEngine = AgoraRtcEngineKit.sharedEngine(withAppId: AppID, delegate: self)
    var contentView: ContentView?
}

extension VideoEngine: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int) {
        contentView?.log(content: "did join channel")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didLeaveChannelWith stats: AgoraChannelStats) {
        contentView?.log(content: "did leave channel")
        contentView?.isLocalAudioMuted = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinedOfUid uid: UInt, elapsed: Int) {
        contentView?.isRemoteInSession = true
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        contentView?.isRemoteInSession = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        // Only one remote video view is available for this
        // tutorial. Here we check if there exists a surface
        // view tagged as this uid.
        let videoCanvas = AgoraRtcVideoCanvas()
        videoCanvas.view = contentView?.remoteCanvas.rendererView
        videoCanvas.renderMode = .hidden
        videoCanvas.uid = uid
        agoraEngine.setupRemoteVideo(videoCanvas)

        contentView?.isRemoteVideoMuted = false
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didVideoMuted muted:Bool, byUid:UInt) {
        contentView?.isRemoteVideoMuted = muted
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurWarning warningCode: AgoraWarningCode) {
        contentView?.log(content: "did occur warning: \(warningCode.rawValue)")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurError errorCode: AgoraErrorCode) {
        contentView?.log(content: "did occur error: \(errorCode.rawValue)")
    }
}
