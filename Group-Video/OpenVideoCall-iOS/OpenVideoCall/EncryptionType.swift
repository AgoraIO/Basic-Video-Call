//
//  EncryptionType.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/7/27.
//  Copyright © 2016年 Agora. All rights reserved.
//


enum EncryptionType {
    case xts128, xts256
    
    static let allValue: [EncryptionType] = [.xts128, .xts256]
    
    func modeString() -> String {
        switch self {
        case .xts128: return "aes-128-xts"
        case .xts256: return "aes-256-xts"
        }
    }
    
    func description() -> String {
        switch self {
        case .xts128: return "AES 128"
        case .xts256: return "AES 256"
        }
    }
}
