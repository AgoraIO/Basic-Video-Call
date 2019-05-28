//
//  RoomViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/22.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit

class RoomViewController: UIViewController {
    
    @IBOutlet weak var roomNameLabel: UILabel!
    @IBOutlet weak var containerView: UIView!
    @IBOutlet weak var controlView: UIView!
    @IBOutlet weak var messageTableContainerView: UIView!
    
    @IBOutlet weak var audioMixingButton: UIButton!
    @IBOutlet weak var beautyButton: UIButton!
    @IBOutlet weak var muteVideoButton: UIButton!
    @IBOutlet weak var muteAudioButton: UIButton!
    @IBOutlet weak var cameraButton: UIButton!
    
    @IBOutlet var backgroundDoubleTap: UITapGestureRecognizer!
    
    private var isAudioMixing = false {
        didSet {
            audioMixingButton?.isSelected = isAudioMixing
            if isAudioMixing {
                // play music file
                agoraKit.startAudioMixing(
                    FileCenter.audioFilePath(),
                    loopback: false,
                    replace: false,
                    cycle: 1
                )
            } else {
                // stop play
                agoraKit.stopAudioMixing()
            }
        }
    }
    
    private var isBeauty = false {
        didSet {
            beautyButton?.isSelected = isBeauty
            var options: AgoraBeautyOptions? = nil
            if isBeauty {
                options = AgoraBeautyOptions()
                options?.lighteningContrastLevel = .high
                options?.lighteningLevel = 1.0
                options?.smoothnessLevel = 1.0
                options?.rednessLevel = 1.0
            }
            // imprve local render view
            agoraKit.setBeautyEffectOptions(isBeauty, options: options)
        }
    }
    
    private var videoMuted = false {
        didSet {
            muteVideoButton?.isSelected = videoMuted
            agoraKit.muteLocalVideoStream(videoMuted)
            setVideoMuted(videoMuted, forUid: 0)
        }
    }
    
    private var audioMuted = false {
        didSet {
            muteAudioButton?.isSelected = audioMuted
            agoraKit.muteLocalAudioStream(audioMuted)
        }
    }
    
    private var speakerEnabled = true {
        didSet {
            // switch audio playout route
            agoraKit.setEnableSpeakerphone(speakerEnabled)
        }
    }
    
    private var videoSessions = [VideoSession]() {
        didSet {
            // videoSessions and videoViewLayouter manage all render views
            updateInterface(with: self.videoSessions, targetSize: containerView.frame.size, animation: true)
        }
    }
    
    private var doubleClickFullSession: VideoSession? {
        didSet {
            if videoSessions.count >= 3 && doubleClickFullSession != oldValue {
                updateInterface(with: videoSessions, targetSize: containerView.frame.size, animation: true)
            }
        }
    }
    
    private let videoViewLayouter = VideoViewLayouter()
    private let cryptoLoader = AgoraRtcCryptoLoader()
    
    private var chatMessageVC: MessageViewController?
    
    var settings: PreSettings?
    var agoraKit: AgoraRtcEngineKit!
    var roomName: String?
    
    lazy private var options = Options(debugMode: false, enableSpeakerPhone: true)
    
    override func viewDidLoad() {
        roomNameLabel.text = roomName
        loadAgoraKit()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "roomEmbedMessage":
            chatMessageVC = segue.destination as? MessageViewController
        case "roomToOptions":
            let optionsVC = segue.destination as! OptionsViewController
            optionsVC.options = options
            optionsVC.delegate = self
        default:
            break
        }
    }
    @IBAction func doAudioMixingPressed(_ sender: UIButton) {
        isAudioMixing.toggle()
    }
    
    @IBAction func doBeautyPressed(_ sender: UIButton) {
        isBeauty.toggle()
    }
    
    @IBAction func doMuteVideoPressed(_ sender: UIButton) {
        videoMuted.toggle()
        updateSelfViewVisiable()
    }
    
    @IBAction func doMuteAudioPressed(_ sender: UIButton) {
        audioMuted.toggle()
    }
    
    @IBAction func doCameraPressed(_ sender: UIButton) {
        agoraKit.switchCamera()
    }
    
    @IBAction func doClosePressed(_ sender: UIButton) {
        leaveChannel()
    }
    
    @IBAction func doBackDoubleTapped(_ sender: UITapGestureRecognizer) {
        if doubleClickFullSession == nil {
            // full screen display after be double clicked
            if let tappedIndex = videoViewLayouter.reponseViewIndex(of: sender.location(in: containerView)) {
                doubleClickFullSession = videoSessions[tappedIndex]
            }
        } else {
            doubleClickFullSession = nil
        }
    }
}

// MARK: - AgoraRtcEngineKit
private extension RoomViewController {
    func loadAgoraKit() {
        guard let settings = settings,
            let roomName = roomName else {
            return
        }
        // Step 1, set delegate
        agoraKit.delegate = self
        
        // Step 2, set communication mode
        agoraKit.setChannelProfile(.communication)
        
        // Step 3, enable the video module
        agoraKit.enableVideo()
        // set video configuration
        agoraKit.setVideoEncoderConfiguration(
            AgoraVideoEncoderConfiguration(
                size: settings.dimension,
                frameRate: .fps15,
                bitrate: AgoraVideoBitrateStandard,
                orientationMode: .adaptative
            )
        )
        // add local render view and start preview
        addLocalSession()
        agoraKit.startPreview()
        
        // Step 4, enable encryption mode
        if let type = settings.encryptionType {
            agoraKit.setEncryptionMode(type.modeString())
            agoraKit.setEncryptionSecret(type.text)
        }
        
        // Step 5, join channel and start group chat
        // If join  channel success, agoraKit triggers it's delegate function 'rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int)'
        let code = agoraKit.joinChannel(byToken: KeyCenter.token, channelId: roomName, info: nil, uid: 0, joinSuccess: nil)
    
        if code == 0 {
            setIdleTimerActive(false)
        } else {
            DispatchQueue.main.async(execute: {
                self.alert(string: "Join channel failed: \(code)")
            })
        }
    }
    
    func addLocalSession() {
        let localSession = VideoSession.localSession()
        videoSessions.append(localSession)
        agoraKit.setupLocalVideo(localSession.canvas)
        
        let mediaInfo = MediaInfo(dimension: settings!.dimension, fps: settings!.frameRate.rawValue)
        localSession.mediaInfo = mediaInfo
    }
    
    func leaveChannel() {
        // Step 1, release local AgoraRtcVideoCanvas instance
        agoraKit.setupLocalVideo(nil)
        // Step 2, leave channel and end group chat
        agoraKit.leaveChannel(nil)
        // Step 3, please attention, stop preview after leave channel
        agoraKit.stopPreview()
        
        // Step 4, remove all render views
        for session in videoSessions {
            session.hostingView.removeFromSuperview()
        }
        videoSessions.removeAll()
        
        setIdleTimerActive(true)
        navigationController?.popViewController(animated: true)
    }
}

// MARK: - AgoraRtcEngineDelegate
extension RoomViewController: AgoraRtcEngineDelegate {
    func rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int) {
        info(string: "Join channel: \(channel)")
    }
    
    func rtcEngineConnectionDidInterrupted(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Interrupted")
    }
    
    func rtcEngineConnectionDidLost(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Lost")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurError errorCode: AgoraErrorCode) {
        alert(string: "Occur error: \(errorCode.rawValue)")
    }
    
    // first remote video frame
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        let userSession = videoSession(of: uid)
        let sie = size.fixedSize(with: containerView.bounds.size)
        userSession.size = sie
        userSession.updateMediaInfo(resolution: size)
        agoraKit.setupRemoteVideo(userSession.canvas)
    }
    
    // first local video frame
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstLocalVideoFrameWith size: CGSize, elapsed: Int) {
        if let selfSession = videoSessions.first {
            let fixedSize = size.fixedSize(with: containerView.bounds.size)
            selfSession.size = fixedSize
            updateInterface(with: videoSessions, targetSize: containerView.frame.size, animation: false)
            info(string: "local video resolution: \(size.width) x \(size.height)")
        }
    }
    
    // user offline
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        var indexToDelete: Int?
        for (index, session) in videoSessions.enumerated() {
            if session.uid == uid {
                indexToDelete = index
            }
        }
        
        if let indexToDelete = indexToDelete {
            let deletedSession = videoSessions.remove(at: indexToDelete)
            deletedSession.hostingView.removeFromSuperview()
            if let doubleClickFullSession = doubleClickFullSession , doubleClickFullSession == deletedSession {
                self.doubleClickFullSession = nil
            }
        }
    }
    
    // video muted
    func rtcEngine(_ engine: AgoraRtcEngineKit, didVideoMuted muted: Bool, byUid uid: UInt) {
        setVideoMuted(muted, forUid: uid)
    }
    
    // remote stat
    func rtcEngine(_ engine: AgoraRtcEngineKit, remoteVideoStats stats: AgoraRtcRemoteVideoStats) {
        if let session = fetchSession(of: stats.uid) {
            session.updateMediaInfo(resolution: CGSize(width: CGFloat(stats.width), height: CGFloat(stats.height)), fps: Int(stats.rendererOutputFrameRate))
        }
    }
    
    // audio mixing
    func rtcEngineLocalAudioMixingDidFinish(_ engine: AgoraRtcEngineKit) {
        isAudioMixing = false
    }
}

// MARK: - Private
private extension RoomViewController {
    // Update views
    func updateInterface(with sessions: [VideoSession], targetSize: CGSize, animation: Bool) {
        if animation {
            UIView.animate(withDuration: 0.3, delay: 0, options: .beginFromCurrentState, animations: {[weak self] () -> Void in
                self?.updateInterface(with: sessions, targetSize: targetSize)
                self?.view.layoutIfNeeded()
                }, completion: nil)
        } else {
            updateInterface(with: sessions, targetSize: targetSize)
        }
    }
    
    func updateInterface(with sessions: [VideoSession], targetSize: CGSize) {
        guard !sessions.isEmpty else {
            return
        }
        
        let selfSession = sessions.first!
        videoViewLayouter.selfView = selfSession.hostingView
        videoViewLayouter.selfSize = selfSession.size
        videoViewLayouter.targetSize = targetSize
        var peerVideoViews = [VideoView]()
        for i in 1..<sessions.count {
            peerVideoViews.append(sessions[i].hostingView)
        }
        videoViewLayouter.videoViews = peerVideoViews
        videoViewLayouter.fullView = doubleClickFullSession?.hostingView
        videoViewLayouter.containerView = containerView
        
        videoViewLayouter.layoutVideoViews()
        
        updateSelfViewVisiable()
        
        // Only three people or more can switch the layout
        if sessions.count >= 3 {
            backgroundDoubleTap.isEnabled = true
        } else {
            backgroundDoubleTap.isEnabled = false
            doubleClickFullSession = nil
        }
    }
    
    func fetchSession(of uid: UInt) -> VideoSession? {
        for session in videoSessions {
            if session.uid == uid {
                return session
            }
        }
        
        return nil
    }
    
    func videoSession(of uid: UInt) -> VideoSession {
        if let fetchedSession = fetchSession(of: uid) {
            return fetchedSession
        } else {
            let newSession = VideoSession(uid: uid)
            videoSessions.append(newSession)
            return newSession
        }
    }
    
    func updateSelfViewVisiable() {
        guard let selfView = videoSessions.first?.hostingView else {
            return
        }
        
        if videoSessions.count == 2 {
            selfView.isHidden = videoMuted
        } else {
            selfView.isHidden = false
        }
    }
    
    func setVideoMuted(_ muted: Bool, forUid uid: UInt) {
        fetchSession(of: uid)?.isVideoMuted = muted
    }
    
    func setIdleTimerActive(_ active: Bool) {
        UIApplication.shared.isIdleTimerDisabled = !active
    }
    
    // Log
    func alert(string: String) {
        guard !string.isEmpty else {
            return
        }
        chatMessageVC?.append(alert: string)
    }
    
    func info(string: String) {
        guard !string.isEmpty else {
            return
        }
        chatMessageVC?.append(info: string)
    }
}

extension RoomViewController {
    override var preferredStatusBarStyle: UIStatusBarStyle {
        return .default
    }
    
    override var supportedInterfaceOrientations : UIInterfaceOrientationMask {
        return .all
    }
    
    override func viewWillAppear(_ animated: Bool) {
        navigationController?.navigationBar.barStyle = .default
        navigationController?.navigationBar.isHidden = true
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        navigationController?.navigationBar.barStyle = .blackOpaque
        navigationController?.navigationBar.isHidden = false
    }
    
    override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        for session in videoSessions {
            if let sessionSize = session.size {
                session.size = sessionSize.fixedSize(with: size)
            }
        }
        updateInterface(with: videoSessions, targetSize: size, animation: true)
    }
}

extension RoomViewController: OptionsVCDelegate {
    func optionsVC(_ vc: OptionsViewController, didUpdate options: Options) {
        self.options = options
        messageTableContainerView.isHidden = !options.debugMode
        speakerEnabled = options.enableSpeakerPhone
    }
}
