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
    var bitRate = 0
    
    init() {}
    
    init?(videoProfile: AgoraVideoProfile) {
        if let resolution = videoProfile.resolution(), let bitRate = videoProfile.bitRate() {
            self.resolution = resolution
            self.bitRate = bitRate
            self.fps = videoProfile.fps()
        } else {
            return nil
        }
    }
    
    func description() -> String {
        return "\(Int(resolution.width))×\(Int(resolution.height)), \(fps)fps, \(bitRate)k"
    }
}

extension AgoraVideoProfile {
    static func defaultProfile() -> AgoraVideoProfile {
        return .landscape360P
    }
    
    static func validProfileList() -> [AgoraVideoProfile] {
        return [.landscape120P,
                .landscape240P,
                .landscape360P,
                .landscape480P,
                .landscape720P]
    }
    
    func resolution() -> CGSize? {
        switch self {
        case .landscape120P: return CGSize(width: 160, height: 120)
        case .landscape240P: return CGSize(width: 320, height: 240)
        case .landscape360P: return CGSize(width: 640, height: 360)
        case .landscape480P: return CGSize(width: 640, height: 480)
        case .landscape720P: return CGSize(width: 1280, height: 720)
        default: return nil
        }
    }
    
    func fps() -> Int {
        return 15
    }
    
    func bitRate() -> Int? {
        switch self {
        case .landscape120P: return 65
        case .landscape240P: return 200
        case .landscape360P: return 400
        case .landscape480P: return 500
        case .landscape720P: return 1130
        default: return nil
        }
    }
    
    func description() -> String {
        if let resolution = resolution(), let bitRate = bitRate() {
            return "\(Int(resolution.width))×\(Int(resolution.height)), \(fps())fps, \(bitRate)k"
        } else {
            return "profile \(rawValue)"
        }
    }
}
