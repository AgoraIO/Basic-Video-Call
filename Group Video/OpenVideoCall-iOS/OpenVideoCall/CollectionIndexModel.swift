//
//  CollectionScrollIndexModel.swift
//  OpenVideoCall
//
//  Created by GongYuhua on 4/12/16.
//  Copyright © 2016 Agora. All rights reserved.
//

import Foundation

struct CollectionIndexModel {
    
    //0 1 2   9 10 11
    //3 4 5  12 13 14
    //6 7 8  15 ...
    
    static func rowsPerScreen(with totalCount: Int) -> Int {
        if totalCount <= 0 {
            return 0
        } else if totalCount == 1 {
            return 1
        } else {
            return 2
//            return Int(sqrt(Double(totalCount - 1))) + 1
        }
    }
    
    //Top adjacent object
    static func topIndex(of index: Int, rowsPerScreen: Int) -> Int? {
        let mode = index % (rowsPerScreen * rowsPerScreen)
        if mode >= rowsPerScreen {
            return index - rowsPerScreen
        } else {
            return nil
        }
    }
    
    //Left adjacent object
    static func leftIndex(of index: Int, rowsPerScreen: Int) -> Int? {
        let mode = index % (rowsPerScreen * rowsPerScreen)
        
        let result: Int
        if mode % rowsPerScreen == 0 {
            result = index - (rowsPerScreen * (rowsPerScreen - 1)) - 1
        } else {
            result = index - 1
        }
        
        return result < 0 ? nil : result
    }
}
