//
//  MediaCharacter.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 16/8/1.
//  Copyright © 2016年 Agora. All rights reserved.
//

struct MediaCharacter {
    
    fileprivate static let legalMediaCharacterSet: NSCharacterSet = {
        return NSCharacterSet(charactersIn: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&()+,-:;<=.>?@[]^_`{|}~")
    }()
    
    static func updateToLegalMediaString(from string: String) -> String {
        let legalSet = MediaCharacter.legalMediaCharacterSet
        let separatedArray = string.components(separatedBy: legalSet.inverted)
        let legalString = separatedArray.joined(separator: "")
        return legalString
    }
}
