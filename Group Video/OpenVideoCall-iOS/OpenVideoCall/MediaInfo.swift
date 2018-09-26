//
//  MediaInfo.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 4/11/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Foundation

struct MediaInfo {
    var resolution = CGSize.zero
    var fps = 0
    
    init() {}
    
    init?(videoProfile: AgoraVideoProfile) {
        if let resolution = videoProfile.resolution() {
            self.resolution = resolution
            self.fps = videoProfile.fps()
        } else {
            return nil
        }
    }
    
    func description() -> String {
        return "\(Int(resolution.width))×\(Int(resolution.height)), \(fps)fps"
    }
}

extension AgoraVideoProfile {
    static func defaultProfile() -> AgoraVideoProfile {
        return .landscape360P
    }
    
    static func validProfileList() -> [AgoraVideoProfile] {
        return [.landscape120P,
                .landscape180P,
                .landscape240P,
                .landscape360P,
                .landscape480P,
                .landscape720P]
    }
    
    func resolution() -> CGSize? {
        switch self {
        case .landscape120P: return CGSize(width:160, height:120)
        case .landscape180P: return CGSize(width:320, height:180)
        case .landscape240P: return CGSize(width:320, height:240)
        case .landscape360P: return CGSize(width:640, height:360)
        case .landscape480P: return CGSize(width:640, height:480)
        case .landscape720P: return CGSize(width:1280, height:720)
        default: return nil
        }
    }
    
    func fps() -> Int {
        return 15
    }
}
