//
//  RoomViewController.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2/20/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Cocoa
import Quartz.ImageKit

protocol RoomVCDelegate: class {
    func roomVCNeedClose(_ roomVC: RoomViewController)
}

class RoomViewController: NSViewController {
    
    //MARK: IBOutlet
    @IBOutlet weak var roomNameLabel: NSTextField!
    @IBOutlet weak var containerView: NSView!
    @IBOutlet weak var buttonContainerView: NSView!
    @IBOutlet weak var messageTableContainerView: NSView!
    
    @IBOutlet weak var muteVideoButton: NSButton!
    @IBOutlet weak var muteAudioButton: NSButton!
    
    @IBOutlet weak var screenSharingButton: NSButton!
    @IBOutlet weak var windowListView: IKImageBrowserView!
    
    @IBOutlet weak var filterButton: NSButton!
    
    @IBOutlet weak var messageButton: NSButton!
    @IBOutlet weak var messageInputerView: NSView!
    @IBOutlet weak var messageTextField: NSTextField!
    
    //MARK: public var
    var roomName: String!
    var encryptionSecret: String?
    var encryptionType: EncryptionType!
    var videoProfile: AgoraVideoProfile!
    var delegate: RoomVCDelegate?
    
    //MARK: hide & show
    fileprivate var shouldHideFlowViews = false {
        didSet {
            buttonContainerView?.isHidden = shouldHideFlowViews
            messageTableContainerView?.isHidden = shouldHideFlowViews
            roomNameLabel?.isHidden = shouldHideFlowViews
            
            if screenSharingStatus == .list {
                screenSharingStatus = .none
            }
            
            if shouldHideFlowViews {
                messageTextField?.resignFirstResponder()
                messageInputerView?.isHidden = true
            } else {
                buttonContainerView?.isHidden = false
                if isInputing {
                    messageTextField?.becomeFirstResponder()
                    messageInputerView?.isHidden = false
                }
            }
        }
    }
    fileprivate var shouldCompressSelfView = false {
        didSet {
            updateSelfViewVisibility()
        }
    }
    
    //MARK: engine & session
    var agoraKit: AgoraRtcEngineKit!
    fileprivate var videoSessions = [VideoSession]() {
        didSet {
            updateInterface(with: videoSessions)
        }
    }
    fileprivate var doubleClickEnabled = false
    fileprivate var doubleClickFullSession: VideoSession? {
        didSet {
            if videoSessions.count >= 3 && doubleClickFullSession != oldValue {
                updateInterface(with: videoSessions)
            }
        }
    }
    fileprivate let videoViewLayout = VideoViewLayout()
    fileprivate var dataChannelId: Int = -1
    
    //MARK: mute
    fileprivate var audioMuted = false {
        didSet {
            muteAudioButton?.image = NSImage(named: audioMuted ? "btn_mute_blue" : "btn_mute")
            agoraKit.muteLocalAudioStream(audioMuted)
        }
    }
    fileprivate var videoMuted = false {
        didSet {
            muteVideoButton?.image = NSImage(named: videoMuted ? "btn_video" : "btn_voice")
            
            agoraKit.muteLocalVideoStream(videoMuted)
            setVideoMuted(videoMuted, forUid: 0)
            
            updateSelfViewVisibility()
        }
    }
    
    //MARK: screen sharing
    enum ScreenSharingStatus {
        case none, list, sharing
        
        func nextStatus() -> ScreenSharingStatus {
            switch self {
            case .none: return .list
            case .list: return .none
            case .sharing: return .none
            }
        }
    }
    fileprivate var screenSharingStatus = ScreenSharingStatus.none {
        didSet {
            screenSharingButton?.image = NSImage(named: (screenSharingStatus == .sharing) ? "btn_screen_sharing_blue" : "btn_screen_sharing")
            
            if oldValue == .sharing {
                stopShareWindow()
            }
            
            showWindowList(screenSharingStatus == .list)
        }
    }
    fileprivate var windows = WindowList()
    
    //MARK: filter
    fileprivate var isFiltering = false {
        didSet {
            guard let agoraKit = agoraKit else {
                return
            }
            
            if isFiltering {
                AGVideoPreProcessing.registerVideoPreprocessing(agoraKit)
                filterButton?.image = NSImage(named: "btn_filter_blue")
            } else {
                AGVideoPreProcessing.deregisterVideoPreprocessing(agoraKit)
                filterButton?.image = NSImage(named: "btn_filter")
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
            messageButton?.image = NSImage(named: isInputing ? "btn_message_blue" : "btn_message")
        }
    }
    
    //MARK: - life cycle
    override func viewDidLoad() {
        super.viewDidLoad()
        roomNameLabel.stringValue = roomName
        
        messageInputerView.wantsLayer = true
        messageInputerView.layer?.backgroundColor = NSColor(hex: 0x000000, alpha: 0.75).cgColor
        messageInputerView.layer?.cornerRadius = 2
        
        setupWindowListView()
        loadAgoraKit()
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        configStyle(of: view.window!)
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        guard let segueId = segue.identifier , !segueId.isEmpty else {
            return
        }
        
        switch segueId {
        case "videoVCToDevicesVC":
            let devicesVC = segue.destinationController as! DevicesViewController
            devicesVC.agoraKit = agoraKit
            devicesVC.couldTest = false
        case "VideoVCEmbedChatMessageVC":
            chatMessageVC = segue.destinationController as? ChatMessageViewController
        default:
            break
        }
    }
    
    //MARK: - user action
    @IBAction func doMessageClicked(_ sender: NSButton) {
        isInputing = !isInputing
    }
    
    @IBAction func doSettingsClicked(_ sender: NSButton) {
        performSegue(withIdentifier: "videoVCToDevicesVC", sender: nil)
    }
    
    @IBAction func doMuteVideoClicked(_ sender: NSButton) {
        videoMuted = !videoMuted
    }
    
    @IBAction func doMuteAudioClicked(_ sender: NSButton) {
        audioMuted = !audioMuted
    }
    
    @IBAction func doShareScreenClicked(_ sender: NSButton) {
        screenSharingStatus = screenSharingStatus.nextStatus()
    }
    
    @IBAction func doFilterClicked(_ sender: NSButton) {
        isFiltering = !isFiltering
    }
    
    @IBAction func doMessageInput(_ sender: NSTextField) {
        let text = sender.stringValue
        if !text.isEmpty {
            send(text: text)
            sender.stringValue = ""
        }
    }
    
    @IBAction func doCloseClicked(_ sender: NSButton) {
        leaveChannel()
    }
    
    override func mouseUp(with theEvent: NSEvent) {
        if theEvent.clickCount == 1 {
            shouldHideFlowViews = !shouldHideFlowViews
        } else if theEvent.clickCount == 2 && doubleClickEnabled {
            if doubleClickFullSession == nil {
                if let clickedIndex = videoViewLayout.reponseViewIndex(of: theEvent.locationInWindow) {
                    doubleClickFullSession = videoSessions[clickedIndex]
                }
            } else {
                doubleClickFullSession = nil
            }
        }
    }
}

//MARK: - private
private extension RoomViewController {
    func configStyle(of window: NSWindow) {
        window.styleMask.insert([.fullSizeContentView, .miniaturizable])
        window.delegate = self
        window.collectionBehavior = [.fullScreenPrimary]
        
        let minSize = CGSize(width: 960, height: 600)
        window.minSize = minSize
        window.maxSize = CGSize(width: CGFloat.greatestFiniteMagnitude, height: CGFloat.greatestFiniteMagnitude)
        window.setContentSize(minSize)
    }
    
    func updateInterface(with sessions: [VideoSession]) {
        guard !sessions.isEmpty else {
            return
        }
        
        let selfSession = sessions.first!
        videoViewLayout.selfView = selfSession.hostingView
        videoViewLayout.selfSize = selfSession.size
        var peerVideoViews = [VideoView]()
        for i in 1..<sessions.count {
            peerVideoViews.append(sessions[i].hostingView)
        }
        videoViewLayout.videoViews = peerVideoViews
        videoViewLayout.fullView = doubleClickFullSession?.hostingView
        videoViewLayout.containerView = containerView
        
        videoViewLayout.layoutVideoViews()
        
        updateSelfViewVisibility()
        
        if sessions.count >= 3 {
            doubleClickEnabled = true
        } else {
            doubleClickEnabled = false
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
    
    func setVideoMuted(_ muted: Bool, forUid uid: UInt) {
        fetchSession(of: uid)?.isVideoMuted = muted
    }
    
    func updateSelfViewVisibility() {
        guard let selfView = videoSessions.first?.hostingView else {
            return
        }
        
        if videoSessions.count == 2 {
            selfView.isHidden = (videoMuted || shouldCompressSelfView)
            
        } else {
            selfView.isHidden = false
        }
    }
    
    func setupWindowListView() {
        windowListView.setContentResizingMask(Int(NSAutoresizingMaskOptions.viewWidthSizable.rawValue))
        windowListView.setValue(NSColor(white: 0, alpha: 0.75), forKey:IKImageBrowserBackgroundColorKey)
        
        let oldAttributres = windowListView.value(forKey: IKImageBrowserCellsTitleAttributesKey) as! NSDictionary
        let attributres = oldAttributres.mutableCopy() as! NSMutableDictionary
        attributres.setObject(NSColor.white, forKey: NSForegroundColorAttributeName as NSCopying)
        windowListView.setValue(attributres, forKey:IKImageBrowserCellsTitleAttributesKey)
    }
    
    func showWindowList(_ shouldShow: Bool) {
        if shouldShow {
            windows.getList()
            windowListView?.reloadData()
            windowListView?.isHidden = false
        } else {
            windowListView?.isHidden = true
        }
    }
    
    //MARK: - alert
    func alert(string: String) {
        guard !string.isEmpty else {
            return
        }
        chatMessageVC?.append(alert: string)
    }
}

//MARK: - agora media kit
private extension RoomViewController {
    func loadAgoraKit() {
        agoraKit = AgoraRtcEngineKit.sharedEngine(withAppId: KeyCenter.AppId, delegate: self)
        agoraKit.setChannelProfile(.communication)
        agoraKit.enableVideo()
        agoraKit.setVideoProfile(videoProfile, swapWidthAndHeight: false)
        
        addLocalSession()
        agoraKit.startPreview()
        
        if let encryptionType = encryptionType, let encryptionSecret = encryptionSecret , !encryptionSecret.isEmpty {
            agoraKit.setEncryptionMode(encryptionType.modeString())
            agoraKit.setEncryptionSecret(encryptionSecret)
        }
        let code = agoraKit.joinChannel(byToken: nil, channelId: roomName, info: nil, uid: 0, joinSuccess: nil)
        if code != 0 {
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
        
        delegate?.roomVCNeedClose(self)
    }
    
    //MARK: - screen sharing
    func startShareWindow(_ window: Window) {
        let windowId = window.id
        agoraKit?.startScreenCapture(UInt(windowId), withCaptureFreq: 15, bitRate: 0, andRect: CGRect.zero )
        videoSessions.first?.hostingView.switchToScreenShare(windowId == 0 || window.name == "Agora Video Call" || window.name == "Full Screen")
    }
    
    func stopShareWindow() {
        agoraKit?.stopScreenCapture()
        videoSessions.first?.hostingView.switchToScreenShare(false)
    }
    
    //MARK: - data channel
    func send(text: String) {
        if dataChannelId > 0, let data = text.data(using: String.Encoding.utf8) {
            agoraKit.sendStreamMessage(dataChannelId, data: data)
            chatMessageVC?.append(chat: text, fromUid: 0)
        }
    }
}

//MARK: - agora media kit delegate
extension RoomViewController: AgoraRtcEngineDelegate {
    func rtcEngineConnectionDidInterrupted(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Interrupted")
    }
    
    func rtcEngineConnectionDidLost(_ engine: AgoraRtcEngineKit) {
        alert(string: "Connection Lost")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurError errorCode: AgoraErrorCode) {
        alert(string: "errorCode \(errorCode.rawValue)")
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstRemoteVideoDecodedOfUid uid: UInt, size: CGSize, elapsed: Int) {
        let userSession = videoSession(of: uid)
        let size = size.fixedSize()
        userSession.size = size
        userSession.updateMediaInfo(resolution: size)
        agoraKit.setupRemoteVideo(userSession.canvas)
    }
    
    // first local video frame
    func rtcEngine(_ engine: AgoraRtcEngineKit, firstLocalVideoFrameWith size: CGSize, elapsed: Int) {
        if let selfSession = videoSessions.first {
            selfSession.size = size.fixedSize()
            updateInterface(with: videoSessions)
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
    
    //remote stat
    func rtcEngine(_ engine: AgoraRtcEngineKit, remoteVideoStats stats: AgoraRtcRemoteVideoStats) {
        if let session = fetchSession(of: stats.uid) {
            session.updateMediaInfo(resolution: CGSize(width: CGFloat(stats.width), height: CGFloat(stats.height)), bitRate: Int(stats.receivedBitrate), fps: Int(stats.receivedFrameRate))
        }
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, device deviceId: String, type deviceType: AgoraMediaDeviceType, stateChanged state: Int) {
        NotificationCenter.default.post(name: NSNotification.Name(rawValue: DeviceListChangeNotificationKey), object: NSNumber(value: deviceType.rawValue))
    }
    
    //data channel
    func rtcEngine(_ engine: AgoraRtcEngineKit, receiveStreamMessageFromUid uid: UInt, streamId: Int, data: Data) {
        guard let string = String(data: data, encoding: String.Encoding.utf8) , !string.isEmpty else {
            return
        }
        chatMessageVC?.append(chat: string, fromUid: Int64(uid))
    }
    
    func rtcEngine(_ engine: AgoraRtcEngineKit, didOccurStreamMessageErrorFromUid uid: UInt, streamId: Int, error: Int, missed: Int, cached: Int) {
        chatMessageVC?.append(alert: "Data channel error: \(error)")
    }
}

//MARK: - IKImageView
extension RoomViewController {
    override func numberOfItems(inImageBrowser aBrowser: IKImageBrowserView!) -> Int {
        return windows.items.count
    }
    
    override func imageBrowser(_ aBrowser: IKImageBrowserView!, itemAt index: Int) -> Any! {
        let item = windows.items[index]
        return item
    }
    
    override func imageBrowser(_ aBrowser: IKImageBrowserView!, cellWasDoubleClickedAt index: Int) {
        guard let selected = aBrowser.selectionIndexes() else {
            return
        }
        
        let index = selected.first
        guard index! < windows.items.count else {
            return
        }
        
        let window = windows.items[index!].window
        startShareWindow(window!)
        screenSharingStatus = .sharing
    }
}

//MARK: - window
extension RoomViewController: NSWindowDelegate {
    func windowShouldClose(_ sender: Any) -> Bool {
        leaveChannel()
        return false
    }
}
