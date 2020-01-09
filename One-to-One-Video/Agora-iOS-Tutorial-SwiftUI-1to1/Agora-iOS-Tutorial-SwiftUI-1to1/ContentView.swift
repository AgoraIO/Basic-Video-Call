//
//  ContentView.swift
//  Agora-iOS-Tutorial-SwiftUI-1to1
//
//  Created by GongYuhua on 2019/6/17.
//  Copyright © 2019 Agora. All rights reserved.
//

import SwiftUI
import AgoraRtcKit

struct ContentView : View {
    @State var isLocalInSession = false
    @State var isLocalAudioMuted = false
    
    @State var isRemoteInSession = false
    @State var isRemoteVideoMuted = true
    
    let localCanvas = VideoCanvas()
    let remoteCanvas = VideoCanvas()
    
    private let videoEngine = VideoEngine()
    private var rtcEngine: AgoraRtcEngineKit {
        get {
            return videoEngine.agoraEngine
        }
    }
    
    var body: some View {
        ZStack() {
            VideoSessionView(
                backColor: Color("remoteBackColor"),
                backImage: Image("big_logo"),
                hideCanvas: isRemoteVideoMuted || !isRemoteInSession || !isLocalInSession,
                canvas: remoteCanvas
            ).edgesIgnoringSafeArea(.all)
            VStack {
                HStack {
                    Spacer()
                    VideoSessionView(
                        backColor: Color("localBackColor"),
                        backImage: Image("logo"),
                        hideCanvas: !isLocalInSession,
                        canvas: localCanvas
                    ).frame(width: 84, height: 112)
                }.padding()
                Spacer()
                HStack {
                    Button(action: toggleLocalAudio) {
                        Image(isLocalAudioMuted ? "mute" : "mic")
                            .resizable()
                    }.frame(width: 55, height: 55)
                    Button(action: toggleLocalSession) {
                        Image(isLocalInSession ? "end" : "call")
                            .resizable()
                    }.frame(width: 70, height: 70)
                    Button(action: switchCamera) {
                        Image("switch").resizable()
                    }.frame(width: 55, height: 55)
                }.padding()
            }
        }.onAppear {
            // This is our usual steps for joining
            // a channel and starting a call.
            self.initializeAgoraEngine()
            self.setupVideo()
            self.setupLocalVideo()
            self.toggleLocalSession()
        }
    }
}

extension ContentView {
    func log(content: String) {
        print(content)
    }
}

fileprivate extension ContentView {
    func initializeAgoraEngine() {
        // init AgoraRtcEngineKit
        videoEngine.contentView = self
    }
    
    func setupVideo() {
        // In simple use cases, we only need to enable video capturing
        // and rendering once at the initialization step.
        // Note: audio recording and playing is enabled by default.
        rtcEngine.enableVideo()
        
        // Set video configuration
        // Please go to this page for detailed explanation
        // https://docs.agora.io/en/Voice/API%20Reference/oc/Classes/AgoraRtcEngineKit.html#//api/name/setVideoEncoderConfiguration:
        rtcEngine.setVideoEncoderConfiguration(
            AgoraVideoEncoderConfiguration(
                size: AgoraVideoDimension640x360,
                frameRate: .fps15,
                bitrate: AgoraVideoBitrateStandard,
                orientationMode: .adaptative
        ))
    }
    
    func setupLocalVideo() {
        // This is used to set a local preview.
        // The steps setting local and remote view are very similar.
        // But note that if the local user do not have a uid or do
        // not care what the uid is, he can set his uid as ZERO.
        // Our server will assign one and return the uid via the block
        // callback (joinSuccessBlock) after
        // joining the channel successfully.
        let videoCanvas = AgoraRtcVideoCanvas()
        videoCanvas.view = localCanvas.rendererView
        videoCanvas.renderMode = .hidden
        rtcEngine.setupLocalVideo(videoCanvas)
    }
    
    func joinChannel() {
        // Set audio route to speaker
        rtcEngine.setDefaultAudioRouteToSpeakerphone(true)
        
        // 1. Users can only see each other after they join the
        // same channel successfully using the same app id.
        // 2. One token is only valid for the channel name that
        // you use to generate this token.
        rtcEngine.joinChannel(byToken: Token, channelId: "demoChannel1", info: nil, uid: 0, joinSuccess: nil)
    }

    func leaveChannel() {
        // leave channel and end chat
        rtcEngine.leaveChannel(nil)
    }
}

fileprivate extension ContentView {
    func toggleLocalSession() {
        isLocalInSession.toggle()
        if isLocalInSession {
            joinChannel()
        } else {
            leaveChannel()
        }
    }
    
    func switchCamera() {
        rtcEngine.switchCamera()
    }
    
    func toggleLocalAudio() {
        isLocalAudioMuted.toggle()
        // mute/unmute local audio
        rtcEngine.muteLocalAudioStream(isLocalAudioMuted)
    }
}

struct ContentView_Previews : PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
