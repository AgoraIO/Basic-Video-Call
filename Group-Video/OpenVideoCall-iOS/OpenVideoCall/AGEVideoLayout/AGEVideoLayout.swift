//
//  AGEVideoLayout.swift
//  AGE
//
//  Created by CavanSu on 2019/7/23.
//  Copyright © 2019 CavanSu. All rights reserved.
//

#if os(iOS)
import UIKit
#else
import Cocoa
#endif

struct AGEIndex {
    var level: Int
    var item: Int
    
    var description: String {
        return "level: \(level), item: \(item)"
    }
}

struct AGERankRow {
    var ranks: Int
    var rows: Int
}

struct AGEVideoLayout {
    
    enum Direction {
        case vertical, horizontal
    }
    
    enum ScrollType {
        case `static`
        case scroll(Direction)
        
        var isScroll: Bool {
            switch self {
            case .scroll: return true
            case .static:   return false
            }
        }
        
        fileprivate var rawValue: Int {
            switch self {
            case .static:
                return 0
            case .scroll(let direction):
                switch direction {
                case .horizontal: return 1
                case .vertical:   return 2
                }
            }
        }
    }
    
    enum ConstraintsType {
        case scale(CGSize), constant(CGSize)
        
        fileprivate var rawValue: Int {
            switch self {
            case .scale:
                return 0
            case .constant:
                return 1
            }
        }
        
        fileprivate var value: CGSize {
            switch self {
            case .scale(let size):
                return size
            case .constant(let size):
                return size
            }
        }
        
        var width: CGFloat {
            return value.width
        }
        
        var height: CGFloat {
            return value.height
        }
        
        static func ==(left: ConstraintsType, right: ConstraintsType) -> Bool {
            if left.rawValue == right.rawValue && left.value == right.value {
                return true
            } else {
                return false
            }
        }
        
        static func !=(left: ConstraintsType, right: ConstraintsType) -> Bool {
            if left.rawValue == right.rawValue && left.value == right.value {
                return false
            } else {
                return true
            }
        }
    }
    
    var interitemSpacing: CGFloat
    var lineSpacing: CGFloat
    
    var startPoint: CGPoint
    var scrollType: ScrollType
    var itemSize: ConstraintsType
    var size: ConstraintsType
    var level: Int
    
    init(interitemSpacing: CGFloat = 0,
         lineSpacing: CGFloat = 0,
         startPoint: CGPoint = CGPoint.zero,
         size: ConstraintsType = .scale(CGSize(width: 1, height: 1)),
         itemSize: ConstraintsType = .scale(CGSize(width: 1, height: 1)),
         scrollType: ScrollType = .static,
         level: Int) {
        
        self.interitemSpacing = interitemSpacing
        self.lineSpacing = lineSpacing
        self.startPoint = startPoint
        self.scrollType = scrollType
        self.level = level
        
        switch size {
        case .scale(let value):
            let tSize = value.maxNoMoreThanOne()
            self.size = ConstraintsType.scale(tSize)
        case .constant:
            self.size = size
        }
        
        switch itemSize {
        case .scale(let value):
            let tSize = value.maxNoMoreThanOne()
            self.itemSize = ConstraintsType.scale(tSize)
        case .constant:
            self.itemSize = itemSize
        }
    }
    
    func scrollType(_ type: ScrollType) -> AGEVideoLayout {
        var new = self
        new.scrollType = type
        return new
    }
    
    func size(_ size: ConstraintsType) -> AGEVideoLayout {
        var new = self
        
        switch size {
        case .scale(let value):
            let tSize = value.maxNoMoreThanOne()
            new.size = ConstraintsType.scale(tSize)
        case .constant:
            new.size = size
        }
        
        return new
    }
    
    func itemSize(_ itemSize: ConstraintsType) -> AGEVideoLayout {
        var new = self
        
        switch itemSize {
        case .scale(let value):
            let tSize = value.maxNoMoreThanOne()
            new.itemSize = ConstraintsType.scale(tSize)
        case .constant:
            new.itemSize = itemSize
        }
        
        return new
    }
    
    func startPoint(x: CGFloat? = nil, y: CGFloat? = nil) -> AGEVideoLayout {
        var new = self
        if let x = x {
            new.startPoint.x = x
        }
        
        if let y = y {
            new.startPoint.y = y
        }
        
        return new
    }
    
    func interitemSpacing(_ spacing: CGFloat) -> AGEVideoLayout {
        var new = self
        new.interitemSpacing = spacing
        return new
    }
    
    func lineSpacing(_ spacing: CGFloat) -> AGEVideoLayout {
        var new = self
        new.lineSpacing = spacing
        return new
    }
}

extension AGEVideoLayout {
    enum Equal {
        enum Reason {
            case lineSpacing, interitemSpacing, startPoint, itemSize, size, scrollType
            
            var description: String {
                switch self {
                case .lineSpacing:      return "lineSpacing"
                case .interitemSpacing: return "interitemSpacing"
                case .startPoint:       return "startPoint"
                case .itemSize:         return "itemSize"
                case .size:             return "size"
                case .scrollType:       return "scrollType"
                }
            }
        }
        
        case yes, no([Reason])
        
        var boolValue: Bool {
            switch self {
            case .yes: return true
            case .no:  return false
            }
        }
        
        var description: String {
            switch self {
            case .yes:             return "nothing update"
            case .no(let reasons): return reasons.description
            }
        }
    }
    
    static func ==(left: AGEVideoLayout, right: AGEVideoLayout) -> Bool {
        var isEqual = true
        
        if left.lineSpacing != right.lineSpacing {
            isEqual = false
            return isEqual
        }
        
        if left.interitemSpacing != right.interitemSpacing {
            isEqual = false
            return isEqual
        }
        
        if left.startPoint != right.startPoint {
            isEqual = false
            return isEqual
        }
        
        if left.itemSize != right.itemSize {
            isEqual = false
            return isEqual
        }
        
        if left.size != right.size {
            isEqual = false
            return isEqual
        }
        
        if left.scrollType.rawValue != right.scrollType.rawValue {
            isEqual = false
            return isEqual
        }
        
        return isEqual
    }
    
    func isEqual(right: AGEVideoLayout) -> AGEVideoLayout.Equal {
        var reasons = [AGEVideoLayout.Equal.Reason]()
        
        if self.lineSpacing != right.lineSpacing {
            reasons.append(.lineSpacing)
        }
        
        if self.interitemSpacing != right.interitemSpacing {
            reasons.append(.interitemSpacing)
        }
        
        if self.startPoint != right.startPoint {
            reasons.append(.startPoint)
        }
        
        if self.itemSize != right.itemSize {
            reasons.append(.itemSize)
        }
        
        if self.size != right.size {
            reasons.append(.size)
        }
        
        if self.scrollType.rawValue != right.scrollType.rawValue {
            reasons.append(.scrollType)
        }
        
        var result: Equal = .yes
        if reasons.count > 0 {
            result = .no(reasons)
        }
        
        return result
    }
}

extension Array where Element == AGEVideoLayout.Equal.Reason {
    func containsOneOf(_ elements: [AGEVideoLayout.Equal.Reason]) -> Bool {
        var isContains = false
        
        for item in elements {
            if self.contains(item) {
                isContains = true
                break
            }
        }
        return isContains
    }
}

extension CGSize {
    fileprivate func maxNoMoreThanOne() -> CGSize {
        var sizeWidth: CGFloat
        
        if self.width > 1.0 {
            sizeWidth = 1.0
        } else {
            sizeWidth = self.width
        }
        
        var sizeHeight: CGFloat
        
        if self.height > 1.0 {
            sizeHeight = 1.0
        } else {
            sizeHeight = self.height
        }
        
        return CGSize(width: sizeWidth, height: sizeHeight)
    }
    
    static func ==(left: CGSize, right: CGSize) -> Bool {
        if left.width == right.width && left.height == right.height {
            return true
        } else {
            return false
        }
    }
}
