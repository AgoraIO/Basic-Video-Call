//
//  VideoChatViewController.swift
//  Agora Mac Tutorial Swift
//
//  Created by James Fang on 8/12/16.
//  Copyright © 2016 Agora.io. All rights reserved.
//

import Cocoa

protocol VideoChatDelegate: NSObjectProtocol {                      // Tutorial Step 1
    func VideoChatNeedClose(_ VideoChat: VideoChatViewController)     // Tutorial Step 6
}

var AgoraKit: AgoraRtcEngineKit!                    // Tutorial Step 1
weak var delegate: VideoChatDelegate?

class VideoChatViewController: NSViewController {
    @IBOutlet weak var localVideo: NSView!                          // Tutorial Step 3
    @IBOutlet weak var remoteVideo: NSView!                         // Tutorial Step 5
    @IBOutlet weak var controlButtons: NSView!                      // Tutorial Step 8
    @IBOutlet weak var remoteVideoMutedIndicator: NSImageView!
    @IBOutlet weak var localVideoMutedBg: NSImageView!
    @IBOutlet weak var localVideoMutedIndicator: NSImageView!
    
    lazy var muteAudio: Bool = false                    // Tutorial Step 9
    lazy var muteVideo: Bool = false                    // Tutorial Step 10
    lazy var screenShare: Bool = false                  // Tutorial Step 12

    override func viewDidLoad() {
        super.viewDidLoad()

        self.view.wantsLayer = true
        self.remoteVideo.wantsLayer = true
        self.localVideo.wantsLayer = true
        
        setupButtons()              // Tutorial Step 8
        hideVideoMuted()            // Tutorial Step 10
        initializeAgoraEngine()     // Tutorial Step 1
        setupVideo()                // Tutorial Step 2
        setupLocalVideo()           // Tutorial Step 3
        joinChannel()               // Tutorial Step 4
    }

    override func viewWillAppear() {
        super.viewWillAppear()
        
        view.layer?.backgroundColor = NSColor.black.cgColor
        remoteVideo.layer?.backgroundColor = NSColor.clear.cgColor
        localVideo.layer?.backgroundColor = NSColor.clear.cgColor
    }
    
    // Tutorial Step 1
    func initializeAgoraEngine() {
        AgoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: AppID, delegate: self)
    }
    
    // Tutorial Step 2
    func setupVideo() {
        AgoraKit.enableVideo()  // Default mode is disableVideo
        AgoraKit.setVideoProfile(.landscape720P, swapWidthAndHeight: false)
    }
    
    // Tutorial Step 3
    func setupLocalVideo() {
        let videoCanvas = AgoraRtcVideoCanvas()
        videoCanvas.uid = 0
        videoCanvas.view = localVideo
        videoCanvas.renderMode = .adaptive
        AgoraKit.setupLocalVideo(videoCanvas)
    }
    
    // Tutorial Step 4
    func joinChannel() {
        AgoraKit.joinChannel(byToken: nil, channelId: "demoChannel1", info:nil, uid:0) { (sid, uid, elapsed) -> Void in
            // Join channel "demoChannel1"
        }
    }
    
    // Tutorial Step 6
    @IBAction func didClickHangUpButton(_ sender: NSButton) {
        leaveChannel()
    }
    
    func leaveChannel() {
        AgoraKit.leaveChannel(nil)
        AgoraKit.setupLocalVideo(nil)
        remoteVideo.removeFromSuperview()
        localVideo.removeFromSuperview()
        delegate?.VideoChatNeedClose(self)
        AgoraKit = nil
        view.window!.close()
    }
    
    // Tutorial Step 8
    func setupButtons() {
        perform(#selector(hideControlButtons), with:nil, afterDelay:3)
        
        let remoteVideoTrackingArea = NSTrackingArea(rect: remoteVideo.bounds,
                                                  options: [NSTrackingAreaOptions.mouseMoved, NSTrackingAreaOptions.activeInActiveApp, NSTrackingAreaOptions.inVisibleRect],
                                                    owner: self,
                                                 userInfo: nil)
        remoteVideo.addTrackingArea(remoteVideoTrackingArea)
    
        let controlButtonsTrackingArea = NSTrackingArea(rect: controlButtons.bounds,
                                                     options: [NSTrackingAreaOptions.mouseEnteredAndExited, NSTrackingAreaOptions.activeInActiveApp, NSTrackingAreaOptions.inVisibleRect],
                                                     owner: self,
                                                     userInfo: nil)
        controlButtons.addTrackingArea(controlButtonsTrackingArea)
    }
    
    func hideControlButtons() {
        controlButtons.isHidden = true
    }
    
    override func mouseMoved(with theEvent : NSEvent) {
        if (controlButtons.isHidden) {
            controlButtons.isHidden = false
            perform(#selector(hideControlButtons), with:nil, afterDelay:3)
        }
    }
    
    override func mouseEntered(with theEvent : NSEvent) {
        VideoChatViewController.cancelPreviousPerformRequests(withTarget: self)
    }
    
     override func mouseExited(with theEvent : NSEvent) {
        perform(#selector(hideControlButtons), with:nil, afterDelay:3)
    }
    
    // Tutorial Step 9
    @IBAction func didClickMuteButton(_ sender: NSButton) {
        muteAudio = !muteAudio
        AgoraKit.muteLocalAudioStream(muteAudio)
    
        if (muteAudio) {
            sender.image = NSImage(named:"muteButtonSelected")
        } else {
            sender.image = NSImage(named:"muteButton")
        }
    }
    
    // Tutorial Step 10
    @IBAction func didClickVideoMuteButton(_ sender: NSButton) {
        muteVideo = !muteVideo
        AgoraKit.muteLocalVideoStream(muteVideo)
    
        if (muteVideo) {
            sender.image = NSImage(named:"videoMuteButtonSelected")
        } else {
            sender.image = NSImage(named:"videoMuteButton")
        }
        localVideo.isHidden = muteVideo
        localVideoMutedBg.isHidden = !(muteVideo)
        localVideoMutedIndicator.isHidden = !(muteVideo)
    }
    
    func hideVideoMuted() {
        remoteVideoMutedIndicator.isHidden = true
        localVideoMutedBg.isHidden = true
        localVideoMutedIndicator.isHidden = true
    }
    
    // Tutorial Step 11
    @IBAction func didClickDeviceSelectionButton(_ sender: NSButton) {
        let deviceSelectionViewController: NSViewController = {
            return self.storyboard!.instantiateController(withIdentifier: "DeviceSelectionViewController")
                as! NSViewController
        }()
        self.presentViewControllerAsSheet(deviceSelectionViewController)
        // Segue to sheet view controller DeviceSelectionViewController
    }
    
    // Tutorial Step 12
    @IBAction func didClickScreenShareButton(_ sender: NSButton) {
        screenShare = !screenShare
        if (screenShare) {
            sender.image = NSImage(named:"screenShareButtonSelected")
            AgoraKit.startScreenCapture(0, withCaptureFreq: 15, bitRate: 0, andRect: CGRect.zero)
        } else {
            sender.image = NSImage(named:"screenShareButton")
            AgoraKit.stopScreenCapture()
        }
    }
}

extension VideoChatViewController: AgoraRtcEngineDelegate {
    // Tutorial Step 5
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid:UInt, size:CGSize, elapsed:Int) {
        if (remoteVideo.isHidden) {
            remoteVideo.isHidden = false
        }
        let videoCanvas = AgoraRtcVideoCanvas()
        videoCanvas.uid = uid
        videoCanvas.view = remoteVideo
        videoCanvas.renderMode = .adaptive
        AgoraKit.setupRemoteVideo(videoCanvas)
    }
    
    // Tutorial Step 7
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid:UInt, reason:AgoraUserOfflineReason) {
        self.remoteVideo.isHidden = true
    }
    
    // Tutorial Step 10
    func rtcEngine(_ engine: AgoraRtcEngineKit, didVideoMuted muted:Bool, byUid:UInt) {
        remoteVideo.isHidden = muted
        remoteVideoMutedIndicator.isHidden = !muted
    }
}
