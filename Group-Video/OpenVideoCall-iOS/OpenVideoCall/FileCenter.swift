//
//  FileCenter.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 2019/5/6.
//  Copyright © 2019 Agora. All rights reserved.
//

import Foundation

class FileCenter {
    static func logDirectory() -> String {
        let directory = documentDirectory() + "/AgoraLogs"
        checkAndCreateDirectory(at: directory)
        return directory
    }
    
    static func logFilePath() -> String {
        return logDirectory() + "/agora-rtc.log"
    }
    
    static func audioFilePath() -> String {
        return Bundle.main.path(forResource: "Them", ofType: "mp3")!
    }
}

private extension FileCenter {
    static func documentDirectory() -> String {
        return NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true).first!
    }
    
    static func checkAndCreateDirectory(at path: String) {
        var isDirectory: ObjCBool = false
        let exists = FileManager.default.fileExists(atPath: path, isDirectory: &isDirectory)
        if !exists || !isDirectory.boolValue {
            try? FileManager.default.createDirectory(atPath: path, withIntermediateDirectories: true, attributes: nil)
        }
    }
}
