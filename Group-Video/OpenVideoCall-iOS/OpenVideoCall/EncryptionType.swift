//
//  EncryptionType.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/7/27.
//  Copyright © 2016年 Agora. All rights reserved.
//

import AgoraRtcKit

enum EncryptionType {
    case xts128(String?), xts256(String?)
    
    static let allValues: [EncryptionType] = [.xts128(nil), .xts256(nil)]
    
    var text: String? {
        switch self {
        case .xts128(let xText): return xText
        case .xts256(let xText): return xText
        }
    }
    
    mutating func updateText(_ text: String?) {
        switch self {
        case .xts128:
            self = EncryptionType.xts128(text)
        case .xts256:
            self = EncryptionType.xts256(text)
        }
    }
    
    func modeString() -> String {
        switch self {
        case .xts128: return "aes-128-xts"
        case .xts256: return "aes-256-xts"
        }
    }
    
    func modeValue() -> AgoraEncryptionMode {
        switch self {
        case .xts128:
            return .AES128XTS
        case .xts256:
            return .AES256XTS
        }
    }
    
    func description() -> String {
        switch self {
        case .xts128: return "AES 128"
        case .xts256: return "AES 256"
        }
    }
}
