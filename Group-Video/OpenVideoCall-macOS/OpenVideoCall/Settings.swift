//
//  Settings.swift
//  OpenVideoCall
//
//  Created by CavanSu on 2019/5/14.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation
import AgoraRtcKit

struct Settings {
    enum DeviceType {
        case record(String?), speaker(String?), camera(String?)
        
        var deviceId: String? {
            switch self {
            case .record(let id):  return id
            case .speaker(let id): return id
            case .camera(let id):  return id
            }
        }
    }
    
    var roomName: String?
    var encryptionType: EncryptionType?
    var logViewOpen = false
    var dimension = CGSize.defaultDimension()
    var frameRate = AgoraVideoFrameRate.defaultValue
    
    var recordDevice = DeviceType.record(nil)
    var speakerDevice = DeviceType.speaker(nil)
    var cameraDevice = DeviceType.camera(nil)
}
