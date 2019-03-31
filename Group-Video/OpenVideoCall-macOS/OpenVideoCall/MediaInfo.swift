//
//  MediaInfo.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 4/11/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Foundation

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
        return AgoraVideoDimension640x480
    }
    
    static func validDimensionList() -> [CGSize] {
        return [AgoraVideoDimension160x120,
                AgoraVideoDimension320x240,
                AgoraVideoDimension640x360,
                AgoraVideoDimension640x480,
                AgoraVideoDimension960x720]
    }
    
    func description() -> String {
        return "\(Int(width))×\(Int(height))"
    }
}
