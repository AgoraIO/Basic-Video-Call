//
//  AGEConvert.swift
//
//  Created by CavanSu on 2019/9/30.
//  Copyright © 2019 Agora. All rights reserved.
//

import Cocoa

class AGEConvert: NSObject {
    static func force<T: Any>(instance: Any, to type: T.Type) throws -> T {
        if let converted = instance as? T {
            return converted
        } else {
            throw AGEError(type: .fail("convert fail"))
        }
    }
}
