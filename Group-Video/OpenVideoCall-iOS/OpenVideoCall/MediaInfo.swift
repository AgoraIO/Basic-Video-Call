//
//  MediaInfo.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 4/11/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Foundation
import AgoraRtcEngineKit

struct MediaInfo {
    var dimension = CGSize.zero
    var fps = 0
    
    init() {}
    
    init(dimension: CGSize, fps: Int) {
        self.dimension = dimension
        self.fps = fps
    }
    
    func description() -> String {
        return "\(Int(dimension.width))×\(Int(dimension.height)), \(fps)fps"
    }
}

extension CGSize {
    static func defaultDimension() -> CGSize {
        return AgoraVideoDimension640x360
    }
    
    static func validDimensionList() -> [CGSize] {
        return [AgoraVideoDimension160x120,
                AgoraVideoDimension240x180,
                AgoraVideoDimension320x240,
                AgoraVideoDimension640x360,
                AgoraVideoDimension640x480,
                AgoraVideoDimension960x720]
    }
}

extension AgoraVideoFrameRate {
    var description: String {
        switch self {
        case .fps1:    return "1 fps"
        case .fps7:    return "7 fps"
        case .fps10:   return "10 fps"
        case .fps15:   return "15 fps"
        case .fps24:   return "24 fps"
        case .fps30:   return "30 fps"
        default:       return "unsported"
        }
    }
    
    var value: Int {
        switch self {
        case .fps1:    return 1
        case .fps7:    return 7
        case .fps10:   return 10
        case .fps15:   return 15
        case .fps24:   return 24
        case .fps30:   return 30
        default:       return -1
        }
    }
    
    static var defaultValue = AgoraVideoFrameRate.fps15
    
    static func validList() -> [AgoraVideoFrameRate] {
        return [.fps1,
                .fps7,
                .fps10,
                .fps15,
                .fps24,
                .fps30]
    }
}
