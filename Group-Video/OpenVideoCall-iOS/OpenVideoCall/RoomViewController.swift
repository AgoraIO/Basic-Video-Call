//
//  RoomViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/22.
//  Copyright © 2016年 Agora. All rights reserved.
//

import UIKit

protocol RoomVCDelegate: class {
    func roomVCNeedClose(_ roomVC: RoomViewController)
}

class RoomViewController: UIViewController {
    
    //MARK: IBOutlet
    @IBOutlet weak var containerView: UIView!
    @IBOutlet var flowViews: [UIView]!
    @IBOutlet weak var roomNameLabel: UILabel!
    
    @IBOutlet weak var controlView: UIView!
    @IBOutlet weak var messageTableContainerView: UIView!
    
    @IBOutlet weak var messageButton: UIButton!
    
    @IBOutlet weak var muteVideoButton: UIButton!
    @IBOutlet weak var muteAudioButton: UIButton!
    
    @IBOutlet weak var cameraButton: UIButton!
    @IBOutlet weak var speakerButton: UIButton!
    
    @IBOutlet weak var filterButton: UIButton!
    
    @IBOutlet weak var messageInputerView: UIView!
    @IBOutlet weak var messageInputerBottom: NSLayoutConstraint!
    @IBOutlet weak var messageTextField: UITextField!
    
    @IBOutlet var backgroundTap: UITapGestureRecognizer!
    @IBOutlet var backgroundDoubleTap: UITapGestureRecognizer!
    
    //MARK: public var
    var roomName: String!
    var encryptionSecret: String?
    var encryptionType: EncryptionType!
    var videoProfile: AgoraVideoProfile!
    weak var delegate: RoomVCDelegate?
    
    //MARK: hide & show
    fileprivate var shouldHideFlowViews = false {
        didSet {
            if let flowViews = flowViews {
                for view in flowViews {
                    view.isHidden = shouldHideFlowViews
                }
            }
        }
    }
    
    //MARK: engine & session
    var agoraKit: AgoraRtcEngineKit!
    fileprivate var videoSessions = [VideoSession]() {
        didSet {
            updateInterface(with: self.videoSessions, targetSize: containerView.frame.size, animation: true)
        }
    }
    fileprivate var doubleClickFullSession: VideoSession? {
        didSet {
            if videoSessions.count >= 3 && doubleClickFullSession != oldValue {
                updateInterface(with: videoSessions, targetSize: containerView.frame.size, animation: true)
            }
        }
    }
    fileprivate let videoViewLayouter = VideoViewLayouter()
    fileprivate var dataChannelId: Int = -1
    
    //MARK: alert
    fileprivate weak var currentAlert: UIAlertController?
    
    //MARK: mute
    fileprivate var audioMuted = false {
        didSet {
            muteAudioButton?.setImage(UIImage(named: audioMuted ? "btn_mute_blue" : "btn_mute"), for: UIControlState())
            agoraKit.muteLocalAudioStream(audioMuted)
        }
    }
    fileprivate var videoMuted = false {
        didSet {
            muteVideoButton?.setImage(UIImage(named: videoMuted ? "btn_video" : "btn_voice"), for: UIControlState())
            cameraButton?.isHidden = videoMuted
            speakerButton?.isHidden = !videoMuted
            
            agoraKit.muteLocalVideoStream(videoMuted)
            setVideoMuted(videoMuted, forUid: 0)
            
            updateSelfViewVisiable()
        }
    }
    
    //MARK: speaker
    fileprivate var speakerEnabled = true {
        didSet {
            speakerButton?.setImage(UIImage(named: speakerEnabled ? "btn_speaker_blue" : "btn_speaker"), for: UIControlState())
            speakerButton?.setImage(UIImage(named: speakerEnabled ? "btn_speaker" : "btn_speaker_blue"), for: .highlighted)
            
            agoraKit.setEnableSpeakerphone(speakerEnabled)
        }
    }
    
    //MARK: filter
    fileprivate var isFiltering = false {
        didSet {
            guard let agoraKit = agoraKit else {
                return
            }
            
            if isFiltering {
                AGVideoPreProcessing.registerVideoPreprocessing(agoraKit)
                filterButton?.setImage(UIImage(named: "btn_filter_blue"), for: UIControlState())
            } else {
                AGVideoPreProcessing.deregisterVideoPreprocessing(agoraKit)
                filterButton?.setImage(UIImage(named: "btn_filter"), for: UIControlState())
            }
        }
    }
    
    //MARK: text message
    fileprivate var chatMessageVC: ChatMessageViewController?
    fileprivate var isInputing = false {
        didSet {
            if isInputing {
                messageTextField?.becomeFirstResponder()
            } else {
                messageTextField?.resignFirstResponder()
            }
            messageInputerView?.isHidden = !isInputing
            messageButton?.setImage(UIImage(named: isInputing ? "btn_message_blue" : "btn_message"), for: UIControlState())
        }
    }
    
    //MARK: crypto loader
    private let cryptoLoader = AgoraRtcCryptoLoader()
    
    //MARK: - life cycle
    override func viewDidLoad() {
        super.viewDidLoad()
        
        roomNameLabel.text = "\(roomName!)"
        backgroundTap.require(toFail: backgroundDoubleTap)
        addKeyboardObserver()
        
        loadAgoraKit()
    }
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier else {
            return
        }
        
        switch segueId {
        case "VideoVCEmbedChatMessageVC":
            chatMessageVC = segue.destination as? ChatMessageViewController
        default:
            break
        }
    }
    
    //MARK: - user action
    @IBAction func doMessagePressed(_ sender: UIButton) {
        isInputing = !isInputing
    }
    
    @IBAction func doCloseMessagePressed(_ sender: UIButton) {
        isInputing = false
    }
    
    @IBAction func doMuteVideoPressed(_ sender: UIButton) {
        videoMuted = !videoMuted
    }
    
    @IBAction func doMuteAudioPressed(_ sender: UIButton) {
        audioMuted = !audioMuted
    }
    
    @IBAction func doCameraPressed(_ sender: UIButton) {
        agoraKit.switchCamera()
    }
    
    @IBAction func doSpeakerPressed(_ sender: UIButton) {
        speakerEnabled = !speakerEnabled
    }
    
    @IBAction func doFilterPressed(_ sender: UIButton) {
        isFiltering = !isFiltering
    }
    
    @IBAction func doClosePressed(_ sender: UIButton) {
        leaveChannel()
    }
    
    @IBAction func doBackTapped(_ sender: UITapGestureRecognizer) {
        if !isInputing {
            shouldHideFlowViews = !shouldHideFlowViews
        }
    }
    
    @IBAction func doBackDoubleTapped(_ sender: UITapGestureRecognizer) {
        if doubleClickFullSession == nil {
            //将双击到的session全屏
            if let tappedIndex = videoViewLayouter.reponseViewIndex(of: sender.location(in: containerView)) {
                doubleClickFullSession = videoSessions[tappedIndex]
            }
        } else {
            doubleClickFullSession = nil
        }
    }
    
    override var supportedInterfaceOrientations : UIInterfaceOrientationMask {
        return .all
    }
}

//MARK: - textFiled
extension RoomViewController: UITextFieldDelegate {
    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        if let text = textField.text , !text.isEmpty {
            send(text: text)
            textField.text = nil
        }
        return true
    }
}

//MARK: - private
private extension RoomViewController {
    func addKeyboardObserver() {
        NotificationCenter.default.addObserver(forName: NSNotification.Name.UIKeyboardWillShow, object: nil, queue: nil) { [weak self] notify in
            guard let strongSelf = self, let userInfo = (notify as NSNotification).userInfo,
                let keyBoardBoundsValue = userInfo[UIKeyboardFrameEndUserInfoKey] as? NSValue,
                let durationValue = userInfo[UIKeyboardAnimationDurationUserInfoKey] as? NSNumber else {
                    return
            }
            
            let keyBoardBounds = keyBoardBoundsValue.cgRectValue
            let duration = durationValue.doubleValue
            let deltaY = keyBoardBounds.size.height
            
            if duration > 0 {
                var optionsInt: UInt = 0
                if let optionsValue = userInfo[UIKeyboardAnimationCurveUserInfoKey] as? NSNumber {
                    optionsInt = optionsValue.uintValue
                }
                let options = UIViewAnimationOptions(rawValue: optionsInt)
                
                UIView.animate(withDuration: duration, delay: 0, options: options, animations: {
                    strongSelf.messageInputerBottom.constant = deltaY
                    strongSelf.view?.layoutIfNeeded()
                    }, completion: nil)
                
            } else {
                strongSelf.messageInputerBottom.constant = deltaY
            }
        }
        
        NotificationCenter.default.addObserver(forName: NSNotification.Name.UIKeyboardWillHide, object: nil, queue: nil) { [weak self] notify in
            guard let strongSelf = self else {
                return
            }
            
            let duration: Double
            if let userInfo = (notify as NSNotification).userInfo, let durationValue = userInfo[UIKeyboardAnimationDurationUserInfoKey] as? NSNumber {
                duration = durationValue.doubleValue
            } else {
                duration = 0
            }
            
            if duration > 0 {
                var optionsInt: UInt = 0
                if let userInfo = (notify as NSNotification).userInfo, let optionsValue = userInfo[UIKeyboardAnimationCurveUserInfoKey] as? NSNumber {
                    optionsInt = optionsValue.uintValue
                }
                let options = UIViewAnimationOptions(rawValue: optionsInt)
                
                UIView.animate(withDuration: duration, delay: 0, options: options, animations: {
                    strongSelf.messageInputerBottom.constant = 0
                    strongSelf.view?.layoutIfNeeded()
                    }, completion: nil)
                
            } else {
                strongSelf.messageInputerBottom.constant = 0
            }
        }
    }
    
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
        
        //Only three people or more can switch the layout
        if sessions.count >= 3 {
            backgroundDoubleTap.isEnabled = true
        } else {
            backgroundDoubleTap.isEnabled = false
            doubleClickFullSession = nil
        }
    }
    
    func setIdleTimerActive(_ active: Bool) {
        UIApplication.shared.isIdleTimerDisabled = !active
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
    
    func setVideoMuted(_ muted: Bool, forUid uid: UInt) {
        fetchSession(of: uid)?.isVideoMuted = muted
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
    
    func alert(string: String) {
        guard !string.isEmpty else {
            return
        }
        chatMessageVC?.append(alert: string)
    }
}

//MARK: - engine
private extension RoomViewController {
    func loadAgoraKit() {
        agoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        agoraKit.setChannelProfile(.communication)
        agoraKit.enableVideo()
        agoraKit.setVideoProfile(videoProfile, swapWidthAndHeight: false)
        
        addLocalSession()
        agoraKit.startPreview()
        if let encryptionType = encryptionType, let encryptionSecret = encryptionSecret, !encryptionSecret.isEmpty {
            agoraKit.setEncryptionMode(encryptionType.modeString())
            agoraKit.setEncryptionSecret(encryptionSecret)
        }
        
        let code = agoraKit.joinChannel(byToken: nil, channelId: roomName, info: nil, uid: 0, joinSuccess: nil)
        
        if code == 0 {
            setIdleTimerActive(false)
        } else {
            DispatchQueue.main.async(execute: {
                self.alert(string: "Join channel failed: \(code)")
            })
        }
        
        agoraKit.createDataStream(&dataChannelId, reliable: true, ordered: true)
    }
    
    func addLocalSession() {
        let localSession = VideoSession.localSession()
        videoSessions.append(localSession)
        agoraKit.setupLocalVideo(localSession.canvas)
        
        if let mediaInfo = MediaInfo(videoProfile: videoProfile) {
            localSession.mediaInfo = mediaInfo
        }
    }
    
    func leaveChannel() {
        agoraKit.setupLocalVideo(nil)
        agoraKit.leaveChannel(nil)
        agoraKit.stopPreview()
        isFiltering = false
        
        for session in videoSessions {
            session.hostingView.removeFromSuperview()
        }
        videoSessions.removeAll()
        
        setIdleTimerActive(true)
        delegate?.roomVCNeedClose(self)
    }
    
    func send(text: String) {
        if dataChannelId > 0, let data = text.data(using: String.Encoding.utf8) {
            agoraKit.sendStreamMessage(dataChannelId, data: data)
            chatMessageVC?.append(chat: text, fromUid: 0)
        }
    }
}

//MARK: - engine delegate
extension RoomViewController: AgoraRtcEngineDelegate {
    func rtcEngineConnectionDidInterrupted(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Interrupted")
    }
    
    func rtcEngineConnectionDidLost(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Lost")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurError errorCode: AgoraErrorCode) {
        //
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        let userSession = videoSession(of: uid)
        let sie = size.fixedSize(with: containerView.bounds.size)
        userSession.size = sie
        userSession.updateMediaInfo(resolution: size)
        agoraKit.setupRemoteVideo(userSession.canvas)
    }
    
    //first local video frame
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstLocalVideoFrameWith size: CGSize, elapsed: Int) {
        if let selfSession = videoSessions.first {
            let fixedSize = size.fixedSize(with: containerView.bounds.size)
            selfSession.size = fixedSize
            updateInterface(with: videoSessions, targetSize: containerView.frame.size, animation: false)
        }
    }
    
    //user offline
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
    
    //video muted
    func rtcEngine(_ engine: AgoraRtcEngineKit, didVideoMuted muted: Bool, byUid uid: UInt) {
        setVideoMuted(muted, forUid: uid)
    }
    
    //remote stat
    func rtcEngine(_ engine: AgoraRtcEngineKit, remoteVideoStats stats: AgoraRtcRemoteVideoStats) {
        if let session = fetchSession(of: stats.uid) {
            session.updateMediaInfo(resolution: CGSize(width: CGFloat(stats.width), height: CGFloat(stats.height)), fps: Int(stats.receivedFrameRate))
        }
    }
    
    //data channel
    func rtcEngine(_ engine: AgoraRtcEngineKit, receiveStreamMessageFromUid uid: UInt, streamId: Int, data: Data) {
        guard let string = String(data: data, encoding: String.Encoding.utf8) , !string.isEmpty else {
            return
        }
        chatMessageVC?.append(chat: string, fromUid: Int64(uid))
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurStreamMessageErrorFromUid uid: UInt, streamId: Int, error: Int, missed: Int, cached: Int) {
        print("Data channel error: \(error), missed: \(missed), cached: \(cached)\n")
    }
}

//MARK: - rotation
extension RoomViewController {
    override func viewWillTransition(to size: CGSize, with coordinator: UIViewControllerTransitionCoordinator) {
        for session in videoSessions {
            if let sessionSize = session.size {
                session.size = sessionSize.fixedSize(with: size)
            }
        }
        updateInterface(with: videoSessions, targetSize: size, animation: true)
    }
}
