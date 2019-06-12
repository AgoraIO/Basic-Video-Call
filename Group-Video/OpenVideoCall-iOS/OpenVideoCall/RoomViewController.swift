//
//  RoomViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/22.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit
import AgoraRtcEngineKit

protocol RoomVCDataSource: NSObjectProtocol {
    func roomVCNeedAgoraKit() -> AgoraRtcEngineKit
    func roomVCNeedSettings() -> Settings
}

class RoomViewController: UIViewController {
    
    @IBOutlet weak var containerView: UIView!
    @IBOutlet weak var messageTableContainerView: UIView!
    
    @IBOutlet weak var cameraButton: UIButton!
    @IBOutlet weak var audioMixingButton: UIButton!
    @IBOutlet weak var speakerPhoneButton: UIButton!
    @IBOutlet weak var beautyButton: UIButton!
    @IBOutlet weak var muteVideoButton: UIButton!
    @IBOutlet weak var muteAudioButton: UIButton!
    
    @IBOutlet var backgroundDoubleTap: UITapGestureRecognizer!
    
    private var agoraKit: AgoraRtcEngineKit {
        return dataSource!.roomVCNeedAgoraKit()
    }
    
    private var settings: Settings {
        return dataSource!.roomVCNeedSettings()
    }
    
    private var isSwitchCamera = true {
        didSet {
            agoraKit.switchCamera()
        }
    }
    
    private var isAudioMixing = false {
        didSet {
            guard oldValue != isAudioMixing else {
                return
            }
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
            guard oldValue != isBeauty else {
                return
            }
            beautyButton?.isSelected = isBeauty
            var options: AgoraBeautyOptions? = nil
            if isBeauty {
                options = AgoraBeautyOptions()
                options?.lighteningContrastLevel = .normal
                options?.lighteningLevel = 0.7
                options?.smoothnessLevel = 0.5
                options?.rednessLevel = 0.1
            }
            // improve local render view
            agoraKit.setBeautyEffectOptions(isBeauty, options: options)
        }
    }
    
    private var isSpeakerPhone = true {
        didSet {
            guard oldValue != isSpeakerPhone else {
                return
            }
            speakerPhoneButton.isSelected = !isSpeakerPhone
            // switch playout audio route
            agoraKit.setEnableSpeakerphone(isSpeakerPhone)
        }
    }
    
    private var isVideoMuted = false {
        didSet {
            guard oldValue != isVideoMuted else {
                return
            }
            muteVideoButton?.isSelected = isVideoMuted
            setVideoMuted(isVideoMuted, forUid: 0)
            updateSelfViewVisiable()
            // mute local video
            agoraKit.muteLocalVideoStream(isVideoMuted)
        }
    }
    
    private var isAudioMuted = false {
        didSet {
            guard oldValue != isAudioMuted else {
                return
            }
            muteAudioButton?.isSelected = isAudioMuted
            // mute local audio
            agoraKit.muteLocalAudioStream(isAudioMuted)
        }
    }
    
    private var isDebugMode = false {
        didSet {
            guard oldValue != isDebugMode else {
                return
            }
            options.isDebugMode = isDebugMode
            messageTableContainerView.isHidden = !isDebugMode
        }
    }
    
    private var videoSessions = [VideoSession]() {
        didSet {
            // videoSessions and videoViewLayouter manage all render views
            let animation = videoSessions.count > 0 ? true : false
            updateInterface(with: self.videoSessions, targetSize: containerView.frame.size, animation: animation)
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
    
    private weak var optionsVC: RoomOptionsViewController?
    private lazy var options = RoomOptions(isDebugMode: false)
    private var messageVC: MessageViewController?
   
    weak var dataSource: RoomVCDataSource?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        title = settings.roomName
        loadAgoraKit()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "roomEmbedMessage":
            messageVC = segue.destination as? MessageViewController
        case "roomToOptions":
            let optionsVC = segue.destination as? RoomOptionsViewController
            optionsVC?.delegate = self
            optionsVC?.dataSource = self
            self.optionsVC = optionsVC
        default:
            break
        }
    }
    
    deinit {
        leaveChannel()
    }
    
    @IBAction func doAudioMixingPressed(_ sender: UIButton) {
        isAudioMixing.toggle()
    }
    
    @IBAction func doBeautyPressed(_ sender: UIButton) {
        isBeauty.toggle()
    }
    
    @IBAction func doSpeakerPhonePressed(_ sender: UIButton) {
        isSpeakerPhone.toggle()
    }
    
    @IBAction func doMuteVideoPressed(_ sender: UIButton) {
        isVideoMuted.toggle()
    }
    
    @IBAction func doMuteAudioPressed(_ sender: UIButton) {
        isAudioMuted.toggle()
    }
    
    @IBAction func doCameraPressed(_ sender: UIButton) {
        isSwitchCamera.toggle()
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
                frameRate: settings.frameRate,
                bitrate: AgoraVideoBitrateStandard,
                orientationMode: .adaptative
            )
        )
        // add local render view and start preview
        addLocalSession()
        agoraKit.startPreview()
        
        // Step 4, enable encryption mode
        if let type = settings.encryptionType, let text = type.text, !text.isEmpty {
            agoraKit.setEncryptionMode(type.modeString())
            agoraKit.setEncryptionSecret(text)
        }
        
        // Step 5, join channel and start group chat
        // If join  channel success, agoraKit triggers it's delegate function
        // 'rtcEngine(_ engine: AgoraRtcEngineKit, didJoinChannel channel: String, withUid uid: UInt, elapsed: Int)'
        agoraKit.joinChannel(byToken: KeyCenter.Token, channelId: settings.roomName!, info: nil, uid: 0, joinSuccess: nil)
        setIdleTimerActive(false)
    }
    
    func addLocalSession() {
        let localSession = VideoSession.localSession()
        videoSessions.append(localSession)
        agoraKit.setupLocalVideo(localSession.canvas)
        
        let mediaInfo = MediaInfo(dimension: settings.dimension, fps: settings.frameRate.rawValue)
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
            info(string: "local video dimension: \(size.width) x \(size.height)")
        }
    }
    
    // user offline
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOfflineOfUid uid: UInt, reason: AgoraUserOfflineReason) {
        var indexToDelete: Int?
        for (index, session) in videoSessions.enumerated() where session.uid == uid {
            indexToDelete = index
            break
        }
        
        if let indexToDelete = indexToDelete {
            let deletedSession = videoSessions.remove(at: indexToDelete)
            deletedSession.hostingView.removeFromSuperview()
            if let doubleClickFullSession = doubleClickFullSession , doubleClickFullSession == deletedSession {
                self.doubleClickFullSession = nil
            }
            
            // release canvas's view
            deletedSession.canvas.view = nil
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

// MARK: RoomOptionsVCDelegate
extension RoomViewController: RoomOptionsVCDelegate {
    func roomOptionsVC(_ vc: RoomOptionsViewController, debugModeDid enable: Bool) {
        isDebugMode = enable
    }
}
// MARK: RoomOptionsVCDataSource
extension RoomViewController: RoomOptionsVCDataSource {
    func roomOptionsVCNeedOptions() -> RoomOptions {
        return options
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
            selfView.isHidden = isVideoMuted
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
    func info(string: String) {
        guard !string.isEmpty else {
            return
        }
        messageVC?.append(info: string)
    }
    
    func alert(string: String) {
        guard !string.isEmpty else {
            return
        }
        messageVC?.append(alert: string)
    }
}

extension RoomViewController {
    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        if let appDelegate = UIApplication.shared.delegate as? AppDelegate {
            appDelegate.orientation = .all
        }
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        super.viewWillDisappear(animated)
        if let appDelegate = UIApplication.shared.delegate as? AppDelegate {
            appDelegate.orientation = .portrait
        }
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

